#include <QApplication>
#include "PlayerWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Rangers::QuestPlayer::PlayerWindow w;
    w.show();

    return a.exec();
}
