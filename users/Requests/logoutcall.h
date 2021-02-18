#ifndef LOGOUTCALL_H
#define LOGOUTCALL_H

#include "base/postcall.h"

class LogoutCall : public PostCall
{
public:
    LogoutCall(const QString & server,QSharedPointer<QNetworkAccessManager> manager = nullptr);
};

#endif // LOGOUTCALL_H
