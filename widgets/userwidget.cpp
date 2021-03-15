#include "userwidget.h"

#include <QCheckBox>
#include <QLabel>
#include <QTableView>
#include <QToolBar>
#include <QVBoxLayout>

UserWidget::UserWidget(QSharedPointer<User> pUser, QWidget *parent)
    : QWidget(parent)
{
    auto pal = palette();
    pal.setColor(QPalette::Background, Qt::green);
    setAutoFillBackground(true);
    setPalette(pal);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QVBoxLayout * mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    QCheckBox * checkVisible = new QCheckBox(QStringLiteral("%1 (%2)").arg(pUser->UserName(), pUser->Server()));

    mainLayout->addWidget(checkVisible);
    checkVisible->setChecked(true);
    QWidget * userWidget = new QWidget();
    userWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    mainLayout->addWidget(userWidget);
    checkVisible->connect(checkVisible, &QCheckBox::stateChanged, this, [userWidget](int state){ userWidget->setVisible(!!state); });

    QVBoxLayout * userLayout = new QVBoxLayout(userWidget);

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
    userTable->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    userTable->setModel(pUser.get());
    userLayout->addWidget(userTable);
}
