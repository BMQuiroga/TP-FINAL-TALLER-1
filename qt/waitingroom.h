#ifndef WAITING_ROOM_H
#define WAITING_ROOM_H

#include <QWidget>
#include <QLabel>
#include <QObject>
#include <QKeyEvent>
#include <QTimer>

class WaitingRoom : public QWidget {
    Q_OBJECT
public:
    explicit WaitingRoom(int game_code, QWidget *parent = 0);
signals:
    void windowClosed();
public slots:
    void receiveWaitingInfo(int number_players_connected, int max_number_players);
    void receiveStartSignal();
    void deactivate();
    void handleTimer();
private:
    void connectEvents();
    void closeEvent(QCloseEvent *event);
    bool is_active {true};
    QLabel* counterLabel;
    QLabel* numberPlayersLabel;
    QLabel* gameCodeLabel;
    QTimer* timer;
    int game_code;
    int counter{10};
};

#endif // WAITING_ROOM_H
