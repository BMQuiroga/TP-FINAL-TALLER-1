#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <string>
#include "../common_src/queue.h"
#include "../common_src/protocol_types.h"
#include "lobby_command.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Queue<LobbyCommand>& q, 
        Queue<LobbyGameStateResponse>& q_responses, QWidget *parent = nullptr);
    void showLobbyWidget();
    void showJoinGame();
    void showCreateGame();
    void showGameOptionsWidget();
    void showWaitingScreen();
    void waitForGameToStart();
    void delayTimeForUpdates(int seconds);
    std::string get_player_name();
    bool game_started();
    int get_game_code();
    ~MainWindow();

public slots:
    void receivePlayerInfo(const QString& text, int number);
    void startJoinGameOption();
    void startCreateGameOption();
    void receiveInputGame(const QString& text, int number);
    void receiveGameCode(const QString& text);
    void receiveClosedSignal();

signals:
    void joinedSuccessfully();
    void failedToJoin();
    void createdGameWithCode(int code);
    void readyToStartGame();
    void sendWaitingInfo(int number_players_connected, int max_number_players);
    void deactivateWidget();

private:
    Queue<LobbyCommand>& q;
    Queue<LobbyGameStateResponse>& q_responses;
    QWidget* currentWidget {nullptr};
    Ui::MainWindow *ui;
    std::string player_name {""};
    int game_code {-1};
    bool is_game_started {false};
    bool is_closed {false};
};
#endif // MAINWINDOW_H
