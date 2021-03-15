#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QScrollArea>
#include <QSettings>
#include <QVBoxLayout>

#include "mainwindow.hpp"
#include "users/user.hpp" //backend
#include "users/Requests/base/memsourceurlgetter.hpp"
#include "widgets/userwidget.h"

MainWindow::MainWindow(QSharedPointer<QNetworkAccessManager> manager, QWidget *parent)
    : QWidget(parent)
    , manager(manager)
{
    setGeometry(100,100,600,350);
    setupMainLayout();
    setupMainMenu();
    setupMainArea();
    setupSystemTray();
}

MainWindow::~MainWindow()
{
}

void MainWindow::addNewUserWidget()
{
    QVBoxLayout * centralLayout = findChild<QVBoxLayout*>("centralLayout");
    if (centralLayout != nullptr)
    {
        AddNewUserWidget * userWidget = new AddNewUserWidget(this);
        userWidget->setObjectName("addNewUserWidget");
        connect(userWidget, &AddNewUserWidget::addUserClicked, this, &MainWindow::addUser);
        centralLayout->addWidget(userWidget);
        userWidget->updateUserCombo(m_vUsers);
        userWidget->reset();
        centralLayout->addStretch(1);
    }
}

void MainWindow::addUser()
{
    AddNewUserWidget * userWidget = findChild<AddNewUserWidget*>("addNewUserWidget");
    if (userWidget)
    {
        St_userSettings settings;
        userWidget->fillUserSettings(settings);

        if (settings.m_bRemberMe)
            m_vUserSettings.push_back(settings);


        QSharedPointer<User> pUser = QSharedPointer<User>(new User(settings.m_sUerName,
                                                           settings.m_sPassword,
                                                           settings.m_sServer,
                                                                   this));
        auto itUser = std::find_if(m_vUsers.begin(), m_vUsers.end(),
                                   [pUser](QSharedPointer<User> const &itUser){return (*itUser) == (*pUser); });

        if (itUser != m_vUsers.end())
        {
            return;
        }
        pUser->connect(pUser.get(), &User::userLoggedOut, this, [this, pUser](){onUserLoggout(pUser);} );
        pUser->connect(pUser.get(), &User::userLoggedIn, this, [this, pUser](){onUserLoggedIn(pUser);});
        userWidget->reset();
    }
}

void MainWindow::setupMainArea()
{
    QVBoxLayout * systemLayout = findChild<QVBoxLayout*>("mainLayout");
    if (systemLayout != nullptr)
    {
        QScrollArea * main = new QScrollArea();
        QVBoxLayout* mainH = new QVBoxLayout();
        mainH->setSpacing(1);
        mainH->setContentsMargins(1, 1, 1, 1);
        mainH->setObjectName("centralLayout");
        main->setLayout(mainH);
        main->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        main->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        systemLayout->addWidget(main,1);

        addNewUserWidget();
    }
}

void MainWindow::setupMainLayout()
{
    QVBoxLayout * systemLayout = new QVBoxLayout();
    systemLayout->setObjectName("mainLayout");
    this->setLayout(systemLayout);
    systemLayout->setSpacing(0);
    systemLayout->setContentsMargins(0, 0, 0, 0);
}

void MainWindow::setupMainMenu()
{
    QVBoxLayout * systemLayout = findChild<QVBoxLayout*>("mainLayout");
    if (systemLayout != nullptr)
    {
        QMenuBar * menuBar = new QMenuBar();
        menuBar->setObjectName("systemMenu");
        systemLayout->addWidget(menuBar, 0);

        QAction * close = new QAction("Close");
        close->connect(close, &QAction::triggered, this, &QWidget::close);
        menuBar->addAction(close);
    }
}

void MainWindow::setupSystemTray()
{
    QVBoxLayout * systemLayout = findChild<QVBoxLayout*>("mainLayout");
    if (systemLayout != nullptr)
    {
        QWidget * systemTray = new QWidget();
        systemTray->setObjectName("systemTray");
        systemLayout->addWidget(systemTray,0);

        QHBoxLayout * trayLayout = new QHBoxLayout();
        systemTray->setLayout(trayLayout);

        QLabel * statusLabel = new QLabel("ok");
        trayLayout->addWidget(statusLabel);

    }
}

void MainWindow::onUserLoggout(QSharedPointer<User> pUser)
{
    QWidget * userTable = findChild<QWidget*>(QString("UserTable_%1").arg(pUser->UserName()));
    QVBoxLayout * centralLayout = findChild<QVBoxLayout*>("centralLayout");

    auto itUser = std::find_if(m_vUsers.begin(), m_vUsers.end(),
                               [pUser](QSharedPointer<User> const &itUser){return (*itUser) == (*pUser);});

    if (itUser != m_vUsers.end())
    {
        m_vUsers.remove(static_cast<int>(itUser - m_vUsers.begin()));
    }

    if (centralLayout != nullptr && userTable != nullptr)
    {
        AddNewUserWidget * addNewUserWidget = findChild<AddNewUserWidget*>("addNewUserWidget");
        centralLayout->removeWidget(userTable);
        userTable->deleteLater();
        addNewUserWidget->updateUserCombo(m_vUsers);
        if (m_vUsers.size() == 0)
            centralLayout->addStretch(1);
        centralLayout->update();
    }
}

void MainWindow::onUserLoggedIn(QSharedPointer<User> pUser)
{
    m_vUsers.push_back(pUser);

    auto saveUser = std::find_if(m_vUserSettings.begin(), m_vUserSettings.end(), [pUser](St_userSettings const & userSettings)
                                                                                 {return (pUser->UserName() == userSettings.m_sUerName) && (pUser->Server() == userSettings.m_sServer); });
    if (saveUser != m_vUserSettings.end())
    {
        QSettings settings;
        auto users = settings.value(AddNewUserWidget::USERS).toList();
        auto itUser = std::find_if(users.begin(), users.end(), [saveUser](QVariant const & userVar)
                                                          { St_userSettings userSettings; userSettings.fromVariant(userVar); return userSettings.m_sUerName == saveUser->m_sUerName && userSettings.m_sServer == saveUser->m_sServer; });
        if (itUser == users.end())
        {
            users.append(saveUser->toVariant());
        }
        settings.setValue(AddNewUserWidget::USERS, users);
    }

    QWidget * userTableExist = findChild<QWidget*>(QString("UserTable_%1").arg(pUser->UserName()));
    Q_ASSERT(userTableExist == nullptr);

    QVBoxLayout * centralLayout = findChild<QVBoxLayout*>("centralLayout");
    if (centralLayout != nullptr)
    {
        UserWidget * userWidget = new UserWidget(pUser);
        userWidget->setObjectName(QStringLiteral("UserTable_%1").arg(pUser->UserName()));

        AddNewUserWidget * addNewUserWidget = findChild<AddNewUserWidget*>("addNewUserWidget");
        addNewUserWidget->updateUserCombo(m_vUsers);
        if (m_vUsers.size() == 1)
        {
            delete centralLayout->takeAt(centralLayout->count() - 1);
        }
        centralLayout->addWidget(userWidget);
    }
    update();
}

