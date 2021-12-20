#include "AREsimulationWindow.h"
#include <QApplication>

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);

    AREsimulationWindow myWindow;
    myWindow.show();

    return app.exec();
}
