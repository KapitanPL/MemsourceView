#include <QAction>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QPushButton>
#include <QScrollArea>
#include <QTableView>
#include <QToolBar>
#include <QVBoxLayout>

#include "mainwindow.hpp"
#include "users/user.hpp" //backend

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
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
        QWidget * userWidget = new QWidget();
        userWidget->setObjectName("addNewUserWidget");
        QGridLayout * userLayout = new QGridLayout();
        userWidget->setLayout(userLayout);
        centralLayout->addWidget(userWidget);

        userLayout->addWidget(new QLabel(tr("User")),0,0);
        userLayout->addWidget(new QLabel(tr("Password")),1,0);

        QLineEdit * userEdit = new QLineEdit();
        userEdit->setObjectName("EditUsername");
        userLayout->addWidget(userEdit, 0,1);

        QLineEdit * passwordEdit = new QLineEdit();
        passwordEdit->setEchoMode(QLineEdit::Password);
        passwordEdit->setObjectName("EditPassword");
        userLayout->addWidget(passwordEdit, 1, 1);

        QPushButton * pushAddUser = new QPushButton(tr("Add"));
        pushAddUser->connect(pushAddUser, &QPushButton::clicked, this, &MainWindow::addUserWidget);
        userLayout->addWidget(pushAddUser,0,2);

        userWidget->setFixedHeight(userWidget->sizeHint().height());
    }
}

void MainWindow::addUserWidget()
{
    QLineEdit * userEdit = findChild<QLineEdit*>("EditUsername");
    QLineEdit * userPassword = findChild<QLineEdit*>("EditPassword");
    if (userEdit != nullptr && userPassword != nullptr)
    {
        QString sUserName = userEdit->text();
        auto itUser = std::find_if(m_vUsers.begin(), m_vUsers.end(),
                                   [sUserName](QSharedPointer<User> const &itUser){return itUser->UserName() == sUserName;});

        if (itUser == m_vUsers.end())
        {
            m_vUsers.append(QSharedPointer<User>(new User(sUserName, userPassword->text())));
            itUser = &m_vUsers.back();
        }

        QWidget * userTableExist = findChild<QWidget*>(QString("UserTabel_%1").arg(sUserName));
        QVBoxLayout * centralLayout = findChild<QVBoxLayout*>("centralLayout");
        if (centralLayout != nullptr && userTableExist == nullptr)
        {
            QWidget * userWidget = new QWidget();
            userWidget->setObjectName(QString("UserTabel_%1").arg(sUserName));
            QVBoxLayout * userLayout = new QVBoxLayout(userWidget);

            QWidget * userHead = new QWidget();
            QHBoxLayout * headLayout = new QHBoxLayout(userHead);
            headLayout->addWidget(new QLabel(sUserName));
            headLayout->addStretch(1);
            userLayout->addWidget(userHead);

            QToolBar * userToolBar = new QToolBar();
            userToolBar->setObjectName(QString("User_%1_toolbar"));
            QAction * actionUpdate = new QAction(tr("Update"));
            actionUpdate->connect(actionUpdate, &QAction::triggered, itUser->data(), &User::UpdateModel);
            userToolBar->addAction(actionUpdate);
            QAction * actionLogout = new QAction(tr("Logout"));
            actionLogout->connect(actionLogout, &QAction::triggered, this, [sUserName, this](){this->logoutUser(sUserName);});
            userToolBar->addAction(actionLogout);
            headLayout->addWidget(userToolBar);

            QTableView * userTable = new QTableView();
            userTable->setModel(itUser->data());
            userLayout->addWidget(userTable);

            QWidget * addNewUserWidget = findChild<QWidget*>("addNewUserWidget");
            centralLayout->removeWidget(addNewUserWidget);
            centralLayout->addWidget(userWidget);
            centralLayout->addWidget(addNewUserWidget);
        }
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

void MainWindow::logoutUser(QString sUserName)
{
    QWidget * userTable = findChild<QWidget*>(QString("UserTabel_%1").arg(sUserName));
    QVBoxLayout * centralLayout = findChild<QVBoxLayout*>("centralLayout");

    if (centralLayout != nullptr && userTable != nullptr)
    {
        QWidget * addNewUserWidget = findChild<QWidget*>("addNewUserWidget");
        centralLayout->removeWidget(addNewUserWidget);
        centralLayout->removeWidget(userTable);
        centralLayout->addWidget(addNewUserWidget);
        userTable->deleteLater();
        centralLayout->update();
    }

    auto itUser = std::find_if(m_vUsers.begin(), m_vUsers.end(),
                               [sUserName](QSharedPointer<User> const &itUser){return itUser->UserName() == sUserName;});

    if (itUser != m_vUsers.end())
    {
        m_vUsers.remove(static_cast<int>(itUser - m_vUsers.begin()));
    }
}

