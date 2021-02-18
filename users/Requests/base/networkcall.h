#ifndef NETWORKCALL_H
#define NETWORKCALL_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

class NetworkCall : public QObject
{
    Q_OBJECT
public:
    NetworkCall(const QUrl & url, QSharedPointer<QNetworkAccessManager> manager = nullptr);
    ~NetworkCall();
    virtual void execute();

    QSharedPointer<QNetworkAccessManager> manager = nullptr;
signals:
    void callSuccess(QNetworkReply * reply);
    void callError(QNetworkReply * reply);

    void callEnded();

protected:
    virtual void onFinished(QNetworkReply * reply);
    virtual void onError(QNetworkReply::NetworkError code);
    virtual void onSslError(const QList<QSslError> &errors);
    virtual void performCall() = 0;

    QNetworkRequest m_request;
#ifdef QT_DEBUG
    QString m_sID;
#endif
};

#endif // NETWORKCALL_H
