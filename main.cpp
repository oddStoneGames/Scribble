#include "scribble.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Scribble w;
    w.show();
    return a.exec();
}
