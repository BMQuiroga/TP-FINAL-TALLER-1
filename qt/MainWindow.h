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
    void pickGameOption(const std::string& text);
    void receiveInputGame(const QString& text, int number);

private:
    Queue<LobbyCommand>& q;
    QWidget* currentWidget {nullptr};
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
