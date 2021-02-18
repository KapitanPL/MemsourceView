#include "logoutcall.h"

#include "base/memsourceurlgetter.hpp"

LogoutCall::LogoutCall(const QString & server, QSharedPointer<QNetworkAccessManager> manager)
    : PostCall(MemsourceUrlGetter::logoutUrl(server), manager)
{
}
