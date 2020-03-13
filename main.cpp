#include "MainWindow.h"
#include <QApplication>
#include "Utils.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QCoreApplication::setOrganizationName(Utils::CompanyName);
    QCoreApplication::setApplicationName(Utils::AppName);
    QCoreApplication::setApplicationVersion(Utils::GetAppVersion());
    a.setStyle(QStyleFactory::create("fusion"));
    
    MainWindow w;
    w.readSettings();
    w.show();

    return a.exec();
}
