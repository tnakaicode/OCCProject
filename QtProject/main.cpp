#include "ViewerWidget.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    ViewerWidget viewer;
    viewer.resize(800, 600);
    viewer.show();

    return app.exec();
}