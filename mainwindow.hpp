#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSharedPointer>

class User;

typedef QVector<QSharedPointer<User>> QUserVector;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void addNewUserWidget();
    void addUserWidget();
    void setupMainArea();
    void setupMainLayout();
    void setupMainMenu();
    void setupSystemTray();

    void logoutUser(QString sUserName);

    QUserVector m_vUsers;
};
#endif // MAINWINDOW_H
