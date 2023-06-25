#include "MainWindow.h"
#include "Lobby.h"
#include "GameOption.h"
#include "NumberPlayers.h"
#include "waitingroom.h"
#include "JoinGame.h"
#include <iostream>
#include <ostream>
#include <string>
#include <QDebug>
#include <QKeyEvent>
#include <QTime>
#include <thread>
#include <chrono>
#include "../common_src/protocol_types.h"
#include "./ui_MainWindow.h"
#include "lobby_command.h"
//#include "lobbywidget.h"

MainWindow::MainWindow(Queue<LobbyCommand>& q, 
    Queue<LobbyGameStateResponse>& q_responses, QWidget *parent) :
    q(q),
    q_responses(q_responses),
    QMainWindow(parent), 
    ui(new Ui::MainWindow)
{
    showLobbyWidget();
}

void MainWindow::showJoinGame()
{
    // Close the current widget if it exists
    if (currentWidget) {
        emit deactivateWidget();
        currentWidget->close();
    }

    // Create and show the new widget (e.g., GameOptionsWidget)
    currentWidget = new JoinGame();
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(windowClosed()), this, SLOT(receiveClosedSignal()));
    QObject::connect(this, SIGNAL(deactivateWidget()), currentWidget, SLOT(deactivate()));
    QObject::connect(currentWidget, SIGNAL(sendGameCodeEntered(QString)), this, SLOT(receiveGameCode(QString)));
    QObject::connect(this, SIGNAL(joinedSuccessfully()), currentWidget, SLOT(receiveSuccessfulJoin()));
    QObject::connect(this, SIGNAL(failedToJoin()), currentWidget, SLOT(receiveUnsuccessfulJoin()));
}

void MainWindow::showCreateGame()
{
    // Close the current widget if it exists
    if (currentWidget) {
        emit deactivateWidget();
        currentWidget->close();
    }

    // Create and show the new widget (e.g., GameOptionsWidget)
    currentWidget = new NumberPlayers();
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(windowClosed()), this, SLOT(receiveClosedSignal()));
    QObject::connect(this, SIGNAL(deactivateWidget()), currentWidget, SLOT(deactivate()));
    QObject::connect(currentWidget, SIGNAL(inputNumberEntered(QString, int)), this, SLOT(receiveInputGame(QString, int)));
    QObject::connect(this, SIGNAL(createdGameWithCode(int)), currentWidget, SLOT(receiveNewGameCreatedCode(int)));
}

void MainWindow::showLobbyWidget() {
    // Close the current widget if it exists
    if (currentWidget) {
        emit deactivateWidget();
        currentWidget->close();
    }

    // Create and show the new widget (e.g., LobbyWidget)
    currentWidget = new Lobby();
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(windowClosed()), this, SLOT(receiveClosedSignal()));
    QObject::connect(this, SIGNAL(deactivateWidget()), currentWidget, SLOT(deactivate()));
    QObject::connect(currentWidget, SIGNAL(inputPlayerInfoEntered(QString, int, int)), this, SLOT(receivePlayerInfo(QString, int, int)));
}

void MainWindow::showGameOptionsWidget() {
    // Close the current widget if it exists
    if (currentWidget) {
        emit deactivateWidget();
        currentWidget->close();
    }

    // Create and show the new widget (e.g., GameOptionsWidget)
    currentWidget = new GameOption();
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(windowClosed()), this, SLOT(receiveClosedSignal()));
    QObject::connect(this, SIGNAL(deactivateWidget()), currentWidget, SLOT(deactivate()));
    QObject::connect(currentWidget, SIGNAL(joinGameOptionPicked()), this, SLOT(startJoinGameOption()));
    QObject::connect(currentWidget, SIGNAL(createGameOptionPicked()), this, SLOT(startCreateGameOption()));
}

void MainWindow::receivePlayerInfo(const QString& text, int number, int game_mode) {
    // Handle the received input text
    qDebug() << "Received input text: " << text;
    showGameOptionsWidget();
    player_name = text.toStdString();
    LobbyCommand command(INPUTNAME, player_name, number, game_mode);
    q.push(command);
}

void MainWindow::receiveInputGame(const QString& text, int number) {
    // Handle the received input text
    qDebug() << "Received input number: " << text;
    LobbyCommand command(CREATEGAME, text.toStdString(), number);
    q.push(command);
    LobbyGameStateResponse result = q_responses.pop();
    emit createdGameWithCode(result.game_code);
    game_code = result.game_code;
    waitForGameToStart();
}

void MainWindow::startJoinGameOption() {
    showJoinGame();
}

void MainWindow::startCreateGameOption() {
    showCreateGame();
}

void MainWindow::receiveGameCode(const QString& text) {
    // Handle the received input text
    LobbyCommand command(JOINGAME, text.toStdString());
    q.push(command);
    LobbyGameStateResponse result = q_responses.pop();
    if (result.succeeded == 0) {
        emit joinedSuccessfully();
        game_code = result.game_code;
        waitForGameToStart();
    } else {
        emit failedToJoin();
    }
}

void MainWindow::showWaitingScreen() {
    // Close the current widget if it exists
    if (currentWidget) {
        emit deactivateWidget();
        currentWidget->close();
    }

    // Create and show the new widget (e.g., GameOptionsWidget)
    currentWidget = new WaitingRoom(game_code);
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(windowClosed()), this, SLOT(receiveClosedSignal()));
    QObject::connect(this, SIGNAL(deactivateWidget()), currentWidget, SLOT(deactivate()));
    QObject::connect(this, SIGNAL(sendWaitingInfo(int, int)), currentWidget, SLOT(receiveWaitingInfo(int, int)));
    QObject::connect(this, SIGNAL(readyToStartGame()), currentWidget, SLOT(receiveStartSignal()));
}

void MainWindow::delayTimeForUpdates(int seconds) {
    QTime waitTime= QTime::currentTime().addSecs(seconds);
    while (QTime::currentTime() < waitTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::waitForGameToStart() {
    delayTimeForUpdates(1);
    showWaitingScreen();
    delayTimeForUpdates(5);
    while (!is_game_started && !is_closed) {
        LobbyGameStateResponse result;
        if (q_responses.try_pop(result)) {
            emit sendWaitingInfo(result.number_players_connected, result.max_number_players);
            if (result.ready == 0) {
                emit readyToStartGame();
                is_game_started = true;
            }
        }
        delayTimeForUpdates(5);
    }
}

std::string MainWindow::get_player_name()
{
    return player_name;
}

bool MainWindow::game_started()
{
    return is_game_started;
}

int MainWindow::get_game_code() {
    return game_code;
}

void MainWindow::receiveClosedSignal() {
    LobbyCommand end_command(ENDLOBBY, "");
    is_closed = true;
    q.push(end_command);
}


MainWindow::~MainWindow()
{
    delete ui;
}

