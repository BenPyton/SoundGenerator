#include "mainwindow.h"
#include <QApplication>
#include "utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QCoreApplication::setOrganizationName(Utils::CompanyName);
    QCoreApplication::setApplicationName(Utils::AppName);
    QCoreApplication::setApplicationVersion(Utils::GetAppVersion());
    
    MainWindow w;
    w.readSettings();
    w.show();

    return a.exec();
}
