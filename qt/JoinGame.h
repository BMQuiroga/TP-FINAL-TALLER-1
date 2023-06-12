#ifndef JOIN_GAME_H
#define JOIN_GAME_H

#include <QWidget>

class JoinGame : public QWidget {
    Q_OBJECT
public:
    explicit JoinGame(QWidget *parent = 0);

signals:
    void sendGameCodeEntered(const QString& text);

private:
    void connectToGame();
    void connectEvents();
};

#endif // JOIN_GAME_H
