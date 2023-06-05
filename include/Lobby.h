#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>

class Lobby : public QWidget {
public:
    explicit Lobby(QWidget *parent = 0);
private:
    void showGameOptions();
    void connectEvents();
};

#endif // LOBBY_H
