#ifndef User_HPP
#define User_HPP

#include <QAbstractTableModel>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QString>
#include <QTimer>

#include <memory>
#include <mutex>

struct St_project
{
    QString m_ProjecName;
    QString m_SourceLanguage;
    QStringList m_TargetLanguage;
    QDateTime m_CreationTime;
    QString m_vUId;
};

class User : public QAbstractTableModel
{
public:
    User(QString sUser, QString sPassword, QObject * parent = nullptr);
    virtual ~User() override;

    //QAbstractItemModel
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //Other
    QString     UserName() const;

    static QDateTime parseDateTimeString(QString sDateTime);

    enum eColumn
    {
        eName       = 0,
        eSourceLang = 1,
        eTargetLang = 2,
        eTime       = 3
    };

public slots:
    virtual void UpdateModel();

private:
    QString     GetProjectName(int iProjectIndex) const;
    QString     GetSourceLanguage(int iProjectIndex) const;
    QString GetTargetLanguages(int iProjectIndex) const;
    QDateTime   GetCreationTime(int iProjectIndex) const;

    virtual void GetRequest(const QUrl & url);
    virtual void LoginReply(QNetworkReply *reply);
    virtual void PostRequest(const QUrl & url, QJsonObject & jsonObject, bool bIncludeToken = true);
    virtual void ReplyFinished(QNetworkReply *reply);
    virtual void UpdateReply(QNetworkReply *reply);

    virtual bool TokenValid();

    static QNetworkAccessManager m_manager;
    QTimer m_timer;

    QString m_sUser = "";
    QString m_sToken = "";
    QDateTime m_TokenValidity;
    int64_t m_id = 0;
    QString m_uid = "";

    QVector<St_project> m_vProjects_cache;
};

#endif // User_HPP
