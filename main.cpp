/**************************************************************************
**   Created by TemetNosce 2020/5/13
**************************************************************************/
#include "Widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
