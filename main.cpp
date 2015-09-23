#include <QFile>
#include "polluxusmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Polluxus");
    QCoreApplication::setOrganizationDomain("polluxuscapital.com");
    QCoreApplication::setApplicationName("ptrader");

    PolluxusMain polluxusMain;

    QFile qssFile(":/stylesheets/darkorange.qss");
    qssFile.open(QFile::ReadOnly);
    polluxusMain.setStyleSheet(qssFile.readAll());

    polluxusMain.show();

    return a.exec();
}
