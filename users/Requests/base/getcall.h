#ifndef GETCALL_H
#define GETCALL_H

#include "networkcall.h"

class GetCall : public NetworkCall
{
public:
    GetCall(const QUrl & url, QSharedPointer<QNetworkAccessManager> mngr);

protected:
    virtual void performCall() override;
};

#endif // GETCALL_H
