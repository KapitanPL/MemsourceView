#include "userwidget.h"

#include <QLabel>
#include <QTableView>
#include <QToolBar>
#include <QVBoxLayout>

UserWidget::UserWidget(QSharedPointer<User> pUser, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout * userLayout = new QVBoxLayout();
    setLayout(userLayout);

    QWidget * userHead = new QWidget();
    QHBoxLayout * headLayout = new QHBoxLayout(userHead);
    headLayout->addWidget(new QLabel(pUser->UserName()));
    headLayout->addStretch(1);
    userLayout->addWidget(userHead);

    QToolBar * userToolBar = new QToolBar();
    userToolBar->setObjectName(QString("User_%1_toolbar").arg(pUser->UserName()));
    QAction * actionUpdate = new QAction(tr("Update"));
    actionUpdate->connect(actionUpdate, &QAction::triggered, pUser.get(), &User::UpdateModel);
    userToolBar->addAction(actionUpdate);
    QAction * actionLogout = new QAction(tr("Logout"));
    actionLogout->connect(actionLogout, &QAction::triggered, pUser.get(), &User::Logout);
    userToolBar->addAction(actionLogout);
    headLayout->addWidget(userToolBar);

    QTableView * userTable = new QTableView();
    userTable->setModel(pUser.get());
    userLayout->addWidget(userTable);
}