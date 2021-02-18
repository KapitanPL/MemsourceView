#include "networkcall.h"

#include <QNetworkReply>

#include "memsourceurlgetter.hpp"


NetworkCall::NetworkCall(const QUrl & url, QSharedPointer<QNetworkAccessManager> manager)
    : QObject()
    , manager(manager)
    , m_request(QNetworkRequest(url))
{
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    connect(manager.get(), &QNetworkAccessManager::finished, this, &NetworkCall::onFinished);
    connect(this, &NetworkCall::callEnded, this, &NetworkCall::deleteLater);
    //manager->get(QNetworkRequest(MemsourceUrlGetter::loginUrl()));
}

NetworkCall::~NetworkCall()
{
#ifdef QT_DEBUG
    qDebug() << m_sID << ": destroyed";
#endif
}

void NetworkCall::execute()
{
    performCall();
}

void NetworkCall::onFinished(QNetworkReply *reply)
{
    if (reply)
    {
        reply->deleteLater();
        if (reply->error())
        {
            emit callError(reply);
        }
        else
        {
            emit callSuccess(reply);
        }
    }
    emit callEnded();
}

void NetworkCall::onError(QNetworkReply::NetworkError code)
{
    qDebug() << code;
    emit callEnded();
}

void NetworkCall::onSslError(const QList<QSslError> &errors)
{
    for (auto & error : errors)
    {
        qDebug() << error.errorString();
    }
    emit callEnded();
}
