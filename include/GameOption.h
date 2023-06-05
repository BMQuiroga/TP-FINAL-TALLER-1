#ifndef GAMEOPTION_H
#define GAMEOPTION_H

#include <QWidget>

class GameOption : public QWidget {
public:
    explicit GameOption(QWidget *parent = 0);
private:
    void createGame();
    void joinGame();
    void connectEvents();
};

#endif // GAMEOPTION_H
