#include "list.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    List w;
    w.show();

    return a.exec();
}
