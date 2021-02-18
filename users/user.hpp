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
    Q_OBJECT
public:
    User(const QString & sUser, const QString & sPassword, const QString & server, QObject * parent = nullptr);
    virtual ~User() override;

    //QAbstractItemModel
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;



    bool operator==(const User & other);
    bool operator!=(const User & other);

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

signals:
    void userLoggedOut();
    void userLoggedIn();

public slots:
    virtual void UpdateModel();
    virtual void Logout();

private:
    QString     GetProjectName(int iProjectIndex) const;
    QString     GetSourceLanguage(int iProjectIndex) const;
    QString GetTargetLanguages(int iProjectIndex) const;
    QDateTime   GetCreationTime(int iProjectIndex) const;

    virtual void LoginReply(QNetworkReply *reply);
    virtual void UpdateReply(QNetworkReply *reply);

    virtual bool TokenValid();

    QTimer m_timer;

    QString m_sUser = "";
    QString m_sServer = QString();
    QString m_sToken = "";
    QDateTime m_TokenValidity;
    int64_t m_id = 0;
    QString m_uid = "";

    QVector<St_project> m_vProjects_cache;

    QSharedPointer<QNetworkAccessManager> m_manager = nullptr;
};

#endif // User_HPP
