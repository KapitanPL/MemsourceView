#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

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
}

User::~User()
{
    if (TokenValid())
    {
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
    if (iProjectIndex >= 0 && iProjectIndex < static_cast<int>(m_vProjects_cache.size()))
        return m_vProjects_cache[iProjectIndex].m_SourceLanguage;
    return QString();
}

QString User::GetTargetLanguages(int iProjectIndex) const
{
    QString sRet;
    if (iProjectIndex >= 0 && iProjectIndex < static_cast<int>(m_vProjects_cache.size()))
        for (auto itLang : m_vProjects_cache[iProjectIndex].m_TargetLanguage)
        {
            if (sRet.length() > 0)
                sRet += ", ";
            sRet += itLang;
        }
    return sRet;
}

QDateTime User::GetCreationTime(int iProjectIndex) const
{
    if (iProjectIndex >= 0 && iProjectIndex < static_cast<int>(m_vProjects_cache.size()))
        return m_vProjects_cache[iProjectIndex].m_CreationTime;
    return QDateTime();
}

void User::GetRequest(const QUrl &url)
{
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_manager.get(request);
}

void User::UpdateModel()
{
    GetRequest(MemsourceUrlGetter::listProjectsUrl(m_sToken));
}

int User::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(m_vProjects_cache.size());
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

QDateTime User::parseDateTimeString(QString sDateTime)
{
    QStringRef year(&sDateTime, 0,4);
    QStringRef month(&sDateTime, 5,2);
    QStringRef day(&sDateTime, 8,2);
    QStringRef hour(&sDateTime, 11,2);
    QStringRef minute(&sDateTime, 14,2);
    QStringRef second(&sDateTime, 17,2);
    return QDateTime(QDate(year.toInt(), month.toInt(), day.toInt()),
                     QTime(hour.toInt(), minute.toInt(), second.toInt()),Qt::UTC);
}

QString User::GetProjectName(int iProjectIndex) const
{
    if (iProjectIndex >= 0 && iProjectIndex < static_cast<int>(m_vProjects_cache.size()))
        return m_vProjects_cache[iProjectIndex].m_ProjecName;
    return QString();
}

void User::ReplyFinished(QNetworkReply *reply)
{
    if (reply)
    {
        if (reply->request().url() == MemsourceUrlGetter::loginUrl())
        {
            LoginReply(reply);
            UpdateModel();
        }
        if (reply->request().url() == MemsourceUrlGetter::listProjectsUrl(m_sToken))
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
        m_id = document["id"].toInt();
        m_uid = document["uid"].toString();
        m_sToken = document["token"].toString();
        QString sTokenExpires = document["expires"].toString();
        m_TokenValidity = parseDateTimeString(sTokenExpires);

        if (!m_timer.isActive())
        {
            m_timer.start();
        }
    }
}

void User::UpdateReply(QNetworkReply *reply)
{
    if (reply)
    {
        beginResetModel();
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray jsonArray = document["content"].toArray();
        for (auto itJson : jsonArray)
        {
            QJsonObject json = itJson.toObject();
            St_project parsedProject;
            parsedProject.m_ProjecName = json["name"].toString();
            parsedProject.m_vUId = json["uid"].toString();
            parsedProject.m_SourceLanguage = json["sourceLang"].toString();
            QJsonArray targetLanguagesArray = json["targetLangs"].toArray();
            for (auto itLang : targetLanguagesArray)
            {
                parsedProject.m_TargetLanguage.append(itLang.toString());
            }
            parsedProject.m_CreationTime = parseDateTimeString(json["dateCreated"].toString());

            auto itReplace = std::find_if(m_vProjects_cache.begin(),
                                          m_vProjects_cache.end(),
                                          [parsedProject](St_project & itProject)
                                          {
                                             return parsedProject.m_vUId == itProject.m_vUId;
                                          }
                                          );
            if (itReplace != m_vProjects_cache.end())
            {
                int iProjectIndex = static_cast<int>(itReplace - m_vProjects_cache.begin());
                m_vProjects_cache[iProjectIndex] = parsedProject;
            }
            else
            {
                m_vProjects_cache.append(parsedProject);
            }
        }
        endResetModel();
    }
}
