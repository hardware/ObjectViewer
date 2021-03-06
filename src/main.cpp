#include "ui/mainwindow.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/resources/images/splash.png");
    QSplashScreen splash(pixmap);

    splash.show();
    splash.showMessage("LOADING MESHES...", Qt::AlignHCenter | Qt::AlignBottom, Qt::white);

    qApp->processEvents();

    MainWindow window;
    window.show();

    splash.finish(&window);

    return a.exec();
}
