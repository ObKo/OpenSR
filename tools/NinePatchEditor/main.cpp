#include <QApplication>
#include "EditorWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("QNinePatchEditor");
    QApplication::setApplicationName("QNinePatchEditor");
    QNPE::EditorWindow w;
    w.show();

    return a.exec();
}
