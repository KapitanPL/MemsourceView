#ifndef LISTPROJECTSCALL_H
#define LISTPROJECTSCALL_H

#include "base/getcall.h"

class ListProjectsCall : public GetCall
{
public:
    ListProjectsCall(const QString & server,const QString & token, QSharedPointer<QNetworkAccessManager> manager = nullptr);
};

#endif // LISTPROJECTSCALL_H
