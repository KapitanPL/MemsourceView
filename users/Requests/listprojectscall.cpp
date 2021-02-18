#include "listprojectscall.h"

#include "base/memsourceurlgetter.hpp"

ListProjectsCall::ListProjectsCall(const QString & server, const QString & token, QSharedPointer<QNetworkAccessManager> manager)
    : GetCall(MemsourceUrlGetter::listProjectsUrl(server, token), manager)
{

}
