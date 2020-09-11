#include "lightwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LightWidget w;
    w.show();
    return a.exec();
}
