#ifndef ADDNEWUSERWIDGET_H
#define ADDNEWUSERWIDGET_H

#include <QWidget>

struct St_userSettings
{
    QString m_sUerName = QString();
    QString m_sPassword = QString();
    QString m_sServer = QString();
    bool m_bRemberMe = false;
};

class AddNewUserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddNewUserWidget(QWidget *parent = nullptr);

    void fillUserSettings(St_userSettings & settings) const;

    void reset();

signals:
    void addUserClicked();

};

#endif // ADDNEWUSERWIDGET_H
