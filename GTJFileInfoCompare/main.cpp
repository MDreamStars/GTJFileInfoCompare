#include "gtjfileinfocompare.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GTJFileInfoCompare w;
    w.show();
    return a.exec();
}
