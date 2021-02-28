#ifndef ADDNEWUSERWIDGET_H
#define ADDNEWUSERWIDGET_H

#include <QAbstractItemModel>
#include <QWidget>

class User;

struct St_userSettings
{
    QString m_sUerName = QString();
    QString m_sPassword = QString();
    QString m_sServer = QString();
    bool m_bRemberMe = false;

    QVariant toVariant();
    void fromVariant(QVariant const & var);
};

class AddNewUserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddNewUserWidget(QWidget *parent = nullptr);

    void fillUserSettings(St_userSettings & settings) const;

    void reset();

    void updateUserCombo(QVector<QSharedPointer<User>> & vUsers);

    static const char * USERS;

signals:
    void addUserClicked();

private slots:
    void userSelected(int index);
};

#endif // ADDNEWUSERWIDGET_H
