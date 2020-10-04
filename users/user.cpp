#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <QDebug>

#include "memsourceurlgetter.hpp"
#include "user.hpp"

static constexpr int COLUMN_COUNT = 4;

QNetworkAccessManager User::m_manager;

User::User(QString sUser, QString sPassword, QObject * parent):
    QAbstractTableModel(parent)
{
    m_sUser = sUser;
    m_manager.connect(&m_manager, &QNetworkAccessManager::finished,
                      this, &User::ReplyFinished);

    m_timer.setInterval(30000);
    m_timer.connect(&m_timer, &QTimer::timeout, this, &User::UpdateModel);

    QJsonObject loginData;
    loginData["password"] = sPassword;
    loginData["userName"] = sUser;

    PostRequest(MemsourceUrlGetter::loginUrl(), loginData, false);

    m_vProjecNames_Cache.append("Test1");
    m_vProjecNames_Cache.append("Test2");

    m_vSourceLanguage_Cache.append("En");
    m_vSourceLanguage_Cache.append("Cz");

    m_vTargetLanguage_Cache.append(QStringList({"De, Fr"}));
    m_vTargetLanguage_Cache.append(QStringList({"De, Fr, It, Han"}));

    m_vCreationTime_Cache.append(QDateTime::currentDateTime().addDays(2));
    m_vCreationTime_Cache.append(QDateTime::currentDateTime().addDays(1));
}

User::~User()
{
    if (TokenValid())
    {
        qDebug() << "destroying";
        if (m_timer.isActive())
        {
            m_timer.stop();
        }
        QJsonObject json;
        PostRequest(MemsourceUrlGetter::logoutUrl(),json);
    }
}

QString User::GetSourceLanguage(int iProjectIndex) const
{
    if (iProjectIndex >= 0 && iProjectIndex < static_cast<int>(m_vSourceLanguage_Cache.size()))
        return m_vSourceLanguage_Cache[iProjectIndex];
    return QString();
}

QStringList User::GetTargetLanguages(int iProjectIndex) const
{
    if (iProjectIndex >= 0 && iProjectIndex < static_cast<int>(m_vTargetLanguage_Cache.size()))
        return m_vTargetLanguage_Cache[iProjectIndex];
    return QStringList();
}

QDateTime User::GetCreationTime(int iProjectIndex) const
{
    if (iProjectIndex >= 0 && iProjectIndex < static_cast<int>(m_vCreationTime_Cache.size()))
        return m_vCreationTime_Cache[iProjectIndex];
    return QDateTime();
}

void User::UpdateModel()
{
    QJsonObject updateData;
    updateData["clientId"] = m_id;

    PostRequest(MemsourceUrlGetter::listProjectsUrl(), updateData, false);
}

int User::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(m_vProjecNames_Cache.size());
}

int User::columnCount(const QModelIndex &/*parent*/) const
{
    return COLUMN_COUNT;
}

QVariant User::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    if (index.isValid())
    {
        if (role == Qt::DisplayRole)
        {
            switch (index.column())
            {
            case eName:
            {
                ret = GetProjectName(index.row());
                break;
            }
            case eSourceLang:
            {
                ret = GetSourceLanguage(index.row());
                break;
            }
            case eTargetLang:
            {
                ret = GetTargetLanguages(index.row());
                break;
            }
            case eTime:
            {
                ret = GetCreationTime(index.row());
                break;
            }
            default:
            {
                ret = QVariant();
                break;
            }
            }
        }
    }
    return ret;
}

QVariant User::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant ret;
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Vertical)
        {
            ret = section;
        }
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case eName:
            {
                ret = tr("Project Name");
                break;
            }
            case eSourceLang:
            {
                ret = tr("Source Language");
                break;
            }
            case eTargetLang:
            {
                ret = tr("Target Languages");
                break;
            }
            case eTime:
            {
                ret = tr("Created");
                break;
            }
            default:
            {
                ret = QVariant();
                break;
            }
            }
        }
    }
    return ret;
}

QString User::UserName() const
{
    return m_sUser;
}

QString User::GetProjectName(int iProjectIndex) const
{
    if (iProjectIndex >= 0 && iProjectIndex < static_cast<int>(m_vProjecNames_Cache.size()))
        return m_vProjecNames_Cache[iProjectIndex];
    return QString();
}

void User::ReplyFinished(QNetworkReply *reply)
{
    if (reply)
    {
        if (reply->request().url() == MemsourceUrlGetter::loginUrl())
        {
            LoginReply(reply);
        }
        if (reply->request().url() ==MemsourceUrlGetter::listProjectsUrl())
        {
            UpdateReply(reply);
        }
        reply->deleteLater();
    }
}

bool User::TokenValid()
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    return (m_TokenValidity > now);
}

void User::PostRequest(const QUrl & url, QJsonObject & jsonObject, bool bIncludeToken)
{
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if (bIncludeToken)
    {
        jsonObject["token"] = m_sToken;
    }
    QJsonDocument jsonDoc(jsonObject);
    QByteArray jsonData= jsonDoc.toJson();
    request.setHeader(QNetworkRequest::ContentLengthHeader,
                      QByteArray::number(jsonData.size()));

    m_manager.post(request, jsonData);
}

void User::LoginReply(QNetworkReply *reply)
{
    if (reply)
    {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        qDebug() << document;
        m_id = document["id"].toInt();
        m_uid = document["uid"].toString();
        m_sToken = document["token"].toString();
        QString sTokenExpires = document["expires"].toString();
        QStringRef year(&sTokenExpires, 0,4);
        QStringRef month(&sTokenExpires, 5,2);
        QStringRef day(&sTokenExpires, 8,2);
        QStringRef hour(&sTokenExpires, 11,2);
        QStringRef minute(&sTokenExpires, 14,2);
        QStringRef second(&sTokenExpires, 17,2);
        m_TokenValidity = QDateTime(QDate(year.toInt(), month.toInt(), day.toInt()),
                                    QTime(hour.toInt(), minute.toInt(), second.toInt()),Qt::UTC);

        if (!m_timer.isActive())
        {
            qDebug() << "timer start";
            m_timer.start();
        }
    }
}

void User::UpdateReply(QNetworkReply *reply)
{
    if (reply)
    {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        qDebug() << "update received";
        qDebug() << document;
    }
}
