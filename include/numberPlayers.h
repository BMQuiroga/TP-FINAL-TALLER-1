#ifndef NUMBER_PLAYERS_H
#define NUMBER_PLAYERS_H

#include <QWidget>

class NumberPlayers : public QWidget {
public:
    explicit NumberPlayers(QWidget *parent = 0);
private:
    void setNumberOfPlayers();
    void connectEvents();
};

#endif // NUMBER_PLAYERS_H
