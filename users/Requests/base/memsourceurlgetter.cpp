#include "memsourceurlgetter.hpp"

#include <QString>

const char * MemsourceUrlGetter::BASE = "https://cloud.memsource.com/web";

static const char * LOGIN = "/api2/v1/auth/login";
static const char * LOGOUT = "/api2/v1/auth/logout";
static const char * LIST_PROJECTS = "/api2/v1/projects";

QUrl MemsourceUrlGetter::loginUrl()
{
    return loginUrl(QString(BASE));
}

QUrl MemsourceUrlGetter::logoutUrl()
{
    return logoutUrl(QString(BASE));
}

QUrl MemsourceUrlGetter::listProjectsUrl(const QString & token)
{
     return listProjectsUrl(QString(BASE), token);
}

QUrl MemsourceUrlGetter::loginUrl(const QString & base)
{
    return QUrl(base + QString(LOGIN));
}

QUrl MemsourceUrlGetter::logoutUrl(const QString & base)
{
    return QUrl(base + QString(LOGOUT));
}

QUrl MemsourceUrlGetter::listProjectsUrl(const QString & base, const QString & token)
{
     QUrl url = QUrl(base + QString(LIST_PROJECTS));
     url.setQuery(QStringLiteral("token=%1").arg(token));
     return url;
}
