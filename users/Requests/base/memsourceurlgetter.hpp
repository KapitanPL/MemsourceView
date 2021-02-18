#ifndef MEMSOURCEURLGETTER_HPP
#define MEMSOURCEURLGETTER_HPP

#include <QUrl>

class MemsourceUrlGetter final
{
public:
    MemsourceUrlGetter() = delete;
    MemsourceUrlGetter(const MemsourceUrlGetter&& other) = delete;
    MemsourceUrlGetter(const MemsourceUrlGetter& other) = delete;
    virtual ~MemsourceUrlGetter() = delete;
    MemsourceUrlGetter& operator=(const MemsourceUrlGetter& other) = delete;
    MemsourceUrlGetter& operator=(const MemsourceUrlGetter&& other) = delete;

    static QUrl loginUrl();
    static QUrl logoutUrl();
    static QUrl listProjectsUrl(const QString & token);

    static QUrl loginUrl(const QString & base);
    static QUrl logoutUrl(const QString & base);
    static QUrl listProjectsUrl(const QString & base, const QString & token);

    static const char * BASE;
};
#endif // MEMSOURCEURLGETTER_HPP
