#ifndef JOIN_GAME_H
#define JOIN_GAME_H
#define SUCCESS_MESSAGE "Te uniste a la partida correctamente"
#define FAILURE_MESSAGE "El código ingresado no es válido. Intente nuevamente."

#include "../common_src/protocol_types.h"
#include <QWidget>
#include <QListWidget>
#include <QKeyEvent>
#include <string>
class JoinGame : public QWidget {
    Q_OBJECT
public:
    explicit JoinGame(std::vector<GameReference> &games_list, QWidget *parent = 0);

public slots:
    void receiveSuccessfulJoin();
    void receiveUnsuccessfulJoin();
    void deactivate();

signals:
    void sendGameCodeEntered(const QString& text);
    void windowClosed();

private:
    void connectToGame();
    void connectEvents();
    void editOutputMessage(const std::string& message);
    void closeEvent(QCloseEvent *event);
    bool is_active {true};
    QListWidget *listWidget;
};

#endif // JOIN_GAME_H
