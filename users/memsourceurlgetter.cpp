#include "memsourceurlgetter.hpp"

#include <QString>

static constexpr const char * BASE = "https://cloud.memsource.com/web";

static constexpr const char * LOGIN = "/api2/v1/auth/login";
static constexpr const char * LOGOUT = "/api2/v1/auth/logout";
static constexpr const char * LIST_PROJECTS = "/api2/v1/projects";

MemsourceUrlGetter::MemsourceUrlGetter()
{

}

MemsourceUrlGetter::~MemsourceUrlGetter()
{

}

QUrl MemsourceUrlGetter::loginUrl()
{
    return QUrl(QString(BASE) + QString(LOGIN));
}

QUrl MemsourceUrlGetter::logoutUrl()
{
    return QUrl(QString(BASE) + QString(LOGOUT));
}

QUrl MemsourceUrlGetter::listProjectsUrl(QString token)
{
     QUrl url = QUrl(QString(BASE) + QString(LIST_PROJECTS));
     url.setQuery(QString("token=%1").arg(token));
     return url;
}
