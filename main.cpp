#include "mainwindow.h"

#include <QApplication>
#include <QSplashScreen>

/*#include <chrono>
#include <thread>
#include <windows.h>*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen();
    splash->setPixmap(QPixmap(":/splashScreen/splashScreen/splash.jpg"));
    splash->show();

    splash->showMessage(QObject::tr("Setting up the main window..."), Qt::AlignTop | Qt::AlignRight, Qt::white);
    MainWindow *mainWin = new MainWindow();
    /*std::this_thread::sleep_for(std::chrono::microseconds(5000));
    Sleep(1000);*/

    splash->showMessage(QObject::tr("Loading modules..."), Qt::AlignTop | Qt::AlignRight, Qt::white);
    //loadModules();
    /*std::this_thread::sleep_for(std::chrono::microseconds(5000));
    Sleep(1000);*/

    splash->showMessage(QObject::tr("Establishsing connections..."), Qt::AlignTop | Qt::AlignRight, Qt::white);
    //establishConnections();
    /*std::this_thread::sleep_for(std::chrono::microseconds(5000));
    Sleep(1000);*/

    mainWin->show();
    splash->finish(mainWin);

    delete splash;
    return a.exec();
}
