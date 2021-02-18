#include "getcall.h"

GetCall::GetCall(const QUrl & url, QSharedPointer<QNetworkAccessManager> mngr)
    : NetworkCall(url, mngr)
{
}

void GetCall::performCall()
{
    if (manager)
    {
        QNetworkReply * rep = manager->get(this->m_request);
        connect(rep, &QNetworkReply::errorOccurred, this, &GetCall::onError);
        connect(rep, &QNetworkReply::sslErrors, this, &GetCall::onSslError);
    }
}
