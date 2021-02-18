#include <QApplication>

#include "mainwindow.hpp" //GUI

#include <QNetworkAccessManager>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSharedPointer<QNetworkAccessManager> mgr = QSharedPointer<QNetworkAccessManager>(new QNetworkAccessManager());
    MainWindow w(mgr);
    w.show();
    return a.exec();
}
