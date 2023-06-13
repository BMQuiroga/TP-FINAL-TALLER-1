#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <string>
#include "../common_src/queue.h"
#include "lobby_command.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Queue<LobbyCommand>& q, QWidget *parent = nullptr);
    void showLobbyWidget();
    void showJoinGame();
    void showCreateGame();
    void showGameOptionsWidget();
    ~MainWindow();

public slots:
    void receiveInputText(const QString& text);
    void startJoinGameOption();
    void startCreateGameOption();
    void receiveInputGame(const QString& text, int number);
    void receiveGameCode(const QString& text);
    std::string get_player_name();

private:
    Queue<LobbyCommand>& q;
    QWidget* currentWidget {nullptr};
    Ui::MainWindow *ui;
    std::string player_name;
};
#endif // MAINWINDOW_H
