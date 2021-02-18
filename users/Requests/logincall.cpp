#include "logincall.h"

#include <QJsonObject>

#include "base/memsourceurlgetter.hpp"

LoginCall::LoginCall(QString sUser, QString sPassword, QString sServer, QSharedPointer<QNetworkAccessManager> manager)
    : PostCall(MemsourceUrlGetter::loginUrl(sServer), manager)
{
    QJsonObject loginData;
    loginData[QStringLiteral("password")] = sPassword;
    loginData[QStringLiteral("userName")] = sUser;
    setData(loginData, QString());
#ifdef QT_DEBUG
    m_sID = QStringLiteral("LoginCall");
#endif
}
