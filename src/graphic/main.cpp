#include "widget.h"
#include <QApplication>
#include <QDir>


int main(int argc, char *argv[])
{
    QApplication::addLibraryPath(QDir::currentPath());

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
