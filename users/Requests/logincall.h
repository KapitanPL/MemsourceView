#ifndef LOGINCALL_H
#define LOGINCALL_H

#include "base/postcall.h"

class LoginCall : public PostCall
{
public:
    LoginCall(const QString & sUser, const QString & sPassword,const QString & sServer,QSharedPointer<QNetworkAccessManager> manager = nullptr);
};

#endif // LOGINCALL_H
