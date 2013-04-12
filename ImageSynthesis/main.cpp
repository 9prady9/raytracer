#include "myviewer.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc,argv);
    MyViewer viewer;
    viewer.show();
    return app.exec();
}
