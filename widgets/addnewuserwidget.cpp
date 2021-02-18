#include "addnewuserwidget.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "./../users/Requests/base/memsourceurlgetter.hpp"

AddNewUserWidget::AddNewUserWidget(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout * userLayout = new QVBoxLayout();
    setLayout(userLayout);

    userLayout->addWidget(new QLabel(tr("User")));
    QLineEdit * userEdit = new QLineEdit();
    userEdit->setObjectName("EditUsername");
    userLayout->addWidget(userEdit);

    userLayout->addWidget(new QLabel(tr("Password")));
    QLineEdit * passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setObjectName("EditPassword");
    userLayout->addWidget(passwordEdit);

    QWidget *advancedW = new QWidget();
    QVBoxLayout * advancedL = new QVBoxLayout(advancedW);
    advancedL->addWidget(new QLabel(tr("Server")));
    QLineEdit * serverEdit = new QLineEdit();
    serverEdit->setObjectName("EditServer");
    advancedL->addWidget(serverEdit);
    QCheckBox * rememberCheck = new QCheckBox(tr("remember Me"));
    rememberCheck->setObjectName("rememberUserCheckBox");
    advancedL->addWidget(rememberCheck);

    QCheckBox * advanced = new QCheckBox(tr("Advanced"));
    userLayout->addWidget(advanced);
    advanced->connect(advanced, &QCheckBox::stateChanged, this, [advancedW](int iState){advancedW->setVisible(!!iState);});

    userLayout->addWidget(advancedW);

    QPushButton * pushAddUser = new QPushButton(tr("Add"));
    connect(pushAddUser, &QPushButton::clicked, this, [this](bool){emit addUserClicked();});
    userLayout->addWidget(pushAddUser);

    setMaximumHeight(sizeHint().height());
    advancedW->setVisible(false);
    userLayout->addStretch(1);
}

const QString AddNewUserWidget::getUserName()
{
    QLineEdit * userEdit = findChild<QLineEdit*>("EditUsername");
    if (userEdit != nullptr)
    {
        return userEdit->text();
    }
    return QString();
}

const QString AddNewUserWidget::getPassword()
{
    QLineEdit * passwordEdit = findChild<QLineEdit*>("EditPassword");
    if (passwordEdit != nullptr)
    {
        return passwordEdit->text();
    }
    return QString();
}

const QString AddNewUserWidget::getServer()
{
    QLineEdit * serverEdit = findChild<QLineEdit*>("EditServer");
    if (serverEdit != nullptr)
    {
        return serverEdit->text();
    }
    return QString();
}

void AddNewUserWidget::reset()
{
    QLineEdit * userEdit = findChild<QLineEdit*>("EditUsername");
    QLineEdit * userPassword = findChild<QLineEdit*>("EditPassword");
    QLineEdit * editServer = findChild<QLineEdit*>("EditServer");
    QCheckBox * checkRemember = findChild<QCheckBox*>("rememberUserCheckBox");
    if (userEdit != nullptr)
        userEdit->setText("stepan.svoboda@memsource.com");
    if (userPassword != nullptr)
        userPassword->setText("Ionofiss_21");
    if (editServer != nullptr)
        editServer->setText("https://qa.memsource.com/web");
    if (checkRemember != nullptr)
        checkRemember->setChecked(true);
}
