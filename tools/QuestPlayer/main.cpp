#include <QApplication>
#include "PlayerWindow.h"
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(0));

    QApplication a(argc, argv);
    Rangers::QuestPlayer::PlayerWindow w;
    w.show();

    return a.exec();
}
