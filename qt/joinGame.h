#ifndef JOIN_GAME_H
#define JOIN_GAME_H

#include <QWidget>

class JoinGame : public QWidget {
public:
    explicit JoinGame(QWidget *parent = 0);
private:
    void connectToGame();
    void connectEvents();
};

#endif // JOIN_GAME_H
