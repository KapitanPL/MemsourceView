#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>

#include "users/user.hpp"

class UserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidget(QSharedPointer<User> pUser, QWidget *parent = nullptr);
};

#endif // USERWIDGET_H
