#include <QFile>
#include "polluxusmain.h"
#include <QApplication>
#include "runguard.h"


int main(int argc, char *argv[])
{

    RunGuard guard( "polluxus_key" );
    if (!guard.tryToRun())  return 0;

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Polluxus");
    QCoreApplication::setOrganizationDomain("polluxuscapital.com");
    QCoreApplication::setApplicationName("ptrader");

    PolluxusMain polluxusMain;

    QFile qssFile(":/stylesheets/darkorange.qss");
    qssFile.open(QFile::ReadOnly);
    polluxusMain.setStyleSheet(qssFile.readAll());

    polluxusMain.show();
    return app.exec();
}
