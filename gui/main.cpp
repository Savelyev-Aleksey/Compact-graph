#include <fstream>
#include <iostream>
#include <ctime>

#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include <QSettings>

#include "MainWindow.h"

namespace FileLog
{
    std::ofstream logStream("graph.log");
}

namespace Translation
{
    QTranslator program;
    QTranslator qtBase;
    QTranslator qtHelp;
}



const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}


void loadTranslations(QApplication& app)
{
    // load current locale
    QSettings settings("Compact Graph");
    QString loc = QLocale::system().name();
    QString c = settings.value("locale", loc).toString();

    Translation::program.load(QString(":/translations/graph_%1.qm").arg(c));
    app.installTranslator(&Translation::program);

    Translation::qtBase.load(QString(":/translations/qtbase_%1.qm").arg(c));
    app.installTranslator(&Translation::qtBase);

    Translation::qtHelp.load(QString(":/translations/qthelp_%1.qm").arg(c));
    app.installTranslator(&Translation::qtHelp);
}



int main(int argc, char *argv[])
{
    std::clog.rdbuf(FileLog::logStream.rdbuf());
    std::clog << "Program started " << currentDateTime() << std::endl
              << std::endl;


    QApplication app(argc, argv);
    loadTranslations(app);

    MainWindow w;
    w.move(QApplication::desktop()->screen()->rect().center() -
           w.rect().center());
    w.show();

    return app.exec();
}
