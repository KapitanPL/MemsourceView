#ifndef User_HPP
#define User_HPP

#include <QAbstractTableModel>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QString>
#include <QTimer>

#include <memory>
#include <mutex>

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
    QStringList GetTargetLanguages(int iProjectIndex) const;
    QDateTime   GetCreationTime(int iProjectIndex) const;

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

    QVector<QString> m_vProjecNames_Cache;
    QVector<QString> m_vSourceLanguage_Cache;
    QVector<QStringList> m_vTargetLanguage_Cache;
    QVector<QDateTime> m_vCreationTime_Cache;
};

#endif // User_HPP
