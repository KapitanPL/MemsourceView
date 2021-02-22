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

void AddNewUserWidget::fillUserSettings(St_userSettings &settings) const
{
    QLineEdit * userEdit = findChild<QLineEdit*>("EditUsername");
    QLineEdit * userPassword = findChild<QLineEdit*>("EditPassword");
    QLineEdit * editServer = findChild<QLineEdit*>("EditServer");
    QCheckBox * checkRemember = findChild<QCheckBox*>("rememberUserCheckBox");

    if (checkRemember)
        settings.m_bRemberMe = checkRemember->isChecked();

    if (userEdit)
        settings.m_sUerName = userEdit->text();

    if (userPassword)
        settings.m_sPassword = userPassword->text();

    if (editServer)
        settings.m_sServer = editServer->text();
}

void AddNewUserWidget::reset()
{
    QLineEdit * userEdit = findChild<QLineEdit*>("EditUsername");
    QLineEdit * userPassword = findChild<QLineEdit*>("EditPassword");
    QLineEdit * editServer = findChild<QLineEdit*>("EditServer");
    QCheckBox * checkRemember = findChild<QCheckBox*>("rememberUserCheckBox");
    if (userEdit != nullptr)
        userEdit->setText("");
    if (userPassword != nullptr)
        userPassword->setText("");
    if (editServer != nullptr)
        editServer->setText("");
    if (checkRemember != nullptr)
        checkRemember->setChecked(true);
}
