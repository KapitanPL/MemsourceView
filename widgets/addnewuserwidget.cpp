#include "addnewuserwidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include "./../simplecrypt/simplecrypt.hpp"
#include "./../users/Requests/base/memsourceurlgetter.hpp"
#include "./../users/user.hpp"

static SimpleCrypt crypto(Q_UINT64_C(0x0c254a8ab9f52c));

const char * AddNewUserWidget::USERS = "users";

AddNewUserWidget::AddNewUserWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(1);
    setLayout(mainLayout);

    QCheckBox * checkVisible = new QCheckBox(QStringLiteral("Add new User"));
    checkVisible->setChecked(true);
    mainLayout->addWidget(checkVisible);
    QWidget * userWidget = new QWidget();
    QVBoxLayout * userLayout = new QVBoxLayout(userWidget);
    userLayout->setSpacing(1);
    mainLayout->addWidget(userWidget);
    checkVisible->connect(checkVisible, &QCheckBox::stateChanged, this, [userWidget](int state) { userWidget->setVisible(!!state);});

    userLayout->addWidget(new QLabel(tr("User")));

    QComboBox * userCombo = new QComboBox();
    userCombo->setObjectName("userCombo");
    userCombo->setMaximumWidth(AddNewUserWidget::MAX_ELEMENT_WIDTH);
    userLayout->addWidget(userCombo);
    userCombo->setVisible(false);
    connect(userCombo, qOverload<int>(&QComboBox::activated), this, &AddNewUserWidget::userSelected);

    QLineEdit * userEdit = new QLineEdit();
    userEdit->setMaximumWidth(AddNewUserWidget::MAX_ELEMENT_WIDTH);
    userEdit->setObjectName("EditUsername");
    userLayout->addWidget(userEdit);

    userLayout->addWidget(new QLabel(tr("Password")));
    QLineEdit * passwordEdit = new QLineEdit();
    passwordEdit->setMaximumWidth(AddNewUserWidget::MAX_ELEMENT_WIDTH);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setObjectName("EditPassword");
    userLayout->addWidget(passwordEdit);

    QWidget *advancedW = new QWidget();
    advancedW->setObjectName(QStringLiteral("advancedOptions"));
    QVBoxLayout * advancedL = new QVBoxLayout(advancedW);
    advancedL->addWidget(new QLabel(tr("Server")));
    QLineEdit * serverEdit = new QLineEdit();
    serverEdit->setMaximumWidth(AddNewUserWidget::MAX_ELEMENT_WIDTH);
    serverEdit->setObjectName("EditServer");
    advancedL->addWidget(serverEdit);
    QCheckBox * rememberCheck = new QCheckBox(tr("remember Me"));
    rememberCheck->setObjectName("rememberUserCheckBox");
    advancedL->addWidget(rememberCheck);

    QCheckBox * advanced = new QCheckBox(tr("Advanced"));
    userLayout->addWidget(advanced);
    advanced->connect(advanced, &QCheckBox::stateChanged, this, &AddNewUserWidget::advancedChanged);

    userLayout->addWidget(advancedW);

    QPushButton * pushAddUser = new QPushButton(tr("Add"));
    pushAddUser->setMaximumWidth(AddNewUserWidget::MAX_ELEMENT_WIDTH);
    connect(pushAddUser, &QPushButton::clicked, this, [this](bool){emit addUserClicked();});
    userLayout->addWidget(pushAddUser);

    advancedW->setVisible(false);
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
        editServer->setText("https://qa.memsource.com/web");
    if (checkRemember != nullptr)
        checkRemember->setChecked(true);
}

void AddNewUserWidget::updateUserCombo(QVector<QSharedPointer<User>> & vUsers)
{
    QComboBox * userCombo = findChild<QComboBox*>("userCombo");
    QSettings settings;
    auto users = settings.value(AddNewUserWidget::USERS).toList();
    bool bVisible = false;
    for (auto & itUser : users)
    {
        St_userSettings userSetting;
        userSetting.fromVariant(itUser);
        auto itLoggedUser = std::find_if(vUsers.begin(), vUsers.end(),[userSetting] (QSharedPointer<User> const &itUser)
        {return userSetting.m_sUerName == itUser->UserName() && userSetting.m_sServer == itUser->Server();} );
        if (itLoggedUser == vUsers.end())
        {
            bVisible = true;
            userCombo->addItem(QStringLiteral("%1 (%2)").arg(userSetting.m_sUerName, userSetting.m_sServer), itUser);
        }
    }
    userCombo->setVisible(bVisible);
}

void AddNewUserWidget::userSelected(int index)
{
    QComboBox * userCombo = qobject_cast<QComboBox*>(sender());
    auto user = userCombo->itemData(index).toMap();

    QLineEdit * userEdit = findChild<QLineEdit*>("EditUsername");
    QLineEdit * userPassword = findChild<QLineEdit*>("EditPassword");
    QLineEdit * editServer = findChild<QLineEdit*>("EditServer");

    if (userEdit)
        userEdit->setText(user["user"].toString());

    if (userPassword)
        userPassword->setText(crypto.decryptToString(user["password"].toString()));

    if (editServer)
        editServer->setText(user["server"].toString());

    emit addUserClicked();

}

void AddNewUserWidget::advancedChanged(int state)
{
    QWidget * advancedW = findChild<QWidget*>("advancedOptions");
    Q_ASSERT(advancedW);
    advancedW->setVisible(!!state);
    QSize retSize = minimumSizeHint();
    if (state == 0)
    {
        retSize.setHeight(retSize.height() - advancedW->height());
    }
    setMaximumSize(retSize);
}

QVariant St_userSettings::toVariant()
{
    QMap<QString, QVariant> vals;
    vals["user"] = m_sUerName;
    vals["server"] = m_sServer;
    vals["password"] = crypto.encryptToString(m_sPassword);
    return QVariant(vals);
}

void St_userSettings::fromVariant(QVariant const & var)
{
    auto vals = var.toMap();
    if (vals.contains("user") && vals.contains("server") && vals.contains("password"))
    {
        m_sUerName = vals["user"].toString();
        m_sServer = vals["server"].toString();
        m_sPassword = crypto.decryptToString(vals["password"].toString());
    }
}
