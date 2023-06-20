#ifndef JOIN_GAME_H
#define JOIN_GAME_H
#define SUCCESS_MESSAGE "Te uniste a la partida correctamente"
#define FAILURE_MESSAGE "El código de partida no era valida. Intenta nuevamente."

#include <QWidget>
#include <QKeyEvent>
class JoinGame : public QWidget {
    Q_OBJECT
public:
    explicit JoinGame(QWidget *parent = 0);

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
};

#endif // JOIN_GAME_H
