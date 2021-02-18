#ifndef ADDNEWUSERWIDGET_H
#define ADDNEWUSERWIDGET_H

#include <QWidget>

class AddNewUserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddNewUserWidget(QWidget *parent = nullptr);

    const QString getUserName();
    const QString getPassword();
    const QString getServer();

    void reset();

signals:
    void addUserClicked();

};

#endif // ADDNEWUSERWIDGET_H
