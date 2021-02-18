#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSharedPointer>

class User;
class QNetworkAccessManager;
class QNetworkReply;

typedef QVector<QSharedPointer<User>> QUserVector;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QSharedPointer<QNetworkAccessManager>  manager, QWidget *parent = nullptr);
    ~MainWindow();

    QSharedPointer<QNetworkAccessManager> manager = nullptr;
private:
    void addNewUserWidget();
    void addUser();
    void addUserWidget();
    void setupMainArea();
    void setupMainLayout();
    void setupMainMenu();
    void setupSystemTray();

    void logoutUser(QString sUserName);

private slots:
    void onUserLoggout(QSharedPointer<User> pUser);
    void onUserLoggedIn(QSharedPointer<User> pUser);

private:
    QUserVector m_vUsers;
};
#endif // MAINWINDOW_H
