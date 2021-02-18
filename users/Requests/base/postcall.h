#ifndef POSTCALL_H
#define POSTCALL_H

#include "networkcall.h"

class PostCall : public NetworkCall
{
public:
    PostCall(const QUrl & url,QSharedPointer<QNetworkAccessManager> mngr);

    void setData(QJsonObject & data,const QString & token);

protected:
    virtual void performCall() override;

private:
    QByteArray m_data;
};

#endif // POSTCALL_H
