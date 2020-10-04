#ifndef MEMSOURCEURLGETTER_HPP
#define MEMSOURCEURLGETTER_HPP

#include <QUrl>

class MemsourceUrlGetter
{
public:
    MemsourceUrlGetter();
    virtual ~MemsourceUrlGetter();

    static QUrl loginUrl();
    static QUrl logoutUrl();
    static QUrl listProjectsUrl();
};
#endif // MEMSOURCEURLGETTER_HPP
