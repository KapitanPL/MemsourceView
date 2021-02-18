#include "postcall.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "memsourceurlgetter.hpp"

PostCall::PostCall(const QUrl &url, QSharedPointer<QNetworkAccessManager> mngr)
    : NetworkCall(url, mngr)
{
}

void PostCall::setData(QJsonObject & data, const QString &token)
{
    if (!token.isEmpty())
    {
        data[QStringLiteral("token")] = token;
    }
    m_data = QJsonDocument(data).toJson();
    m_request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(m_data.size()));
}

void PostCall::performCall()
{
    if (manager)
    {
        QNetworkReply * rep = manager->post(this->m_request, m_data);

        connect(rep, &QNetworkReply::errorOccurred, this, &PostCall::onError);
        connect(rep, &QNetworkReply::sslErrors, this, &PostCall::onSslError);
    }
}
