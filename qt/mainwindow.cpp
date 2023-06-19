#include "MainWindow.h"
#include "Lobby.h"
#include "GameOption.h"
#include "NumberPlayers.h"
#include "JoinGame.h"
#include <iostream>
#include <ostream>
#include <string>
#include <QDebug>
#include "../common_src/protocol_types.h"
#include "./ui_MainWindow.h"
#include "lobby_command.h"

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
        currentWidget->close();
    }

    // Create and show the new widget (e.g., GameOptionsWidget)
    currentWidget = new JoinGame();
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(sendGameCodeEntered(QString)), this, SLOT(receiveGameCode(QString)));
    QObject::connect(this, SIGNAL(joinedSuccessfully()), currentWidget, SLOT(receiveSuccessfulJoin()));
    QObject::connect(this, SIGNAL(failedToJoin()), currentWidget, SLOT(receiveUnsuccessfulJoin()));
}

void MainWindow::showCreateGame()
{
    // Close the current widget if it exists
    if (currentWidget) {
        currentWidget->close();
    }

    // Create and show the new widget (e.g., GameOptionsWidget)
    currentWidget = new NumberPlayers();
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(inputNumberEntered(QString, int)), this, SLOT(receiveInputGame(QString, int)));
    QObject::connect(this, SIGNAL(createdGameWithCode(int)), currentWidget, SLOT(receiveNewGameCreatedCode(int)));
}

void MainWindow::showLobbyWidget() {
    // Close the current widget if it exists
    if (currentWidget) {
        currentWidget->close();
    }

    // Create and show the new widget (e.g., LobbyWidget)
    currentWidget = new Lobby();
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(inputPlayerInfoEntered(QString, int)), this, SLOT(receivePlayerInfo(QString, int)));
}

void MainWindow::showGameOptionsWidget() {
    // Close the current widget if it exists
    if (currentWidget) {
        currentWidget->close();
    }

    // Create and show the new widget (e.g., GameOptionsWidget)
    currentWidget = new GameOption();
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(joinGameOptionPicked()), this, SLOT(startJoinGameOption()));
    QObject::connect(currentWidget, SIGNAL(createGameOptionPicked()), this, SLOT(startCreateGameOption()));
}

void MainWindow::receivePlayerInfo(const QString& text, int number) {
    // Handle the received input text
    qDebug() << "Received input text: " << text;
    showGameOptionsWidget();
    player_name = text.toStdString();
    LobbyCommand command(INPUTNAME, player_name, number);
    q.push(command);
}

void MainWindow::receiveInputGame(const QString& text, int number) {
    // Handle the received input text
    qDebug() << "Received input number: " << text;
    LobbyCommand command(CREATEGAME, text.toStdString(), number);
    q.push(command);
    LobbyGameStateResponse result = q_responses.pop();
    emit createdGameWithCode(result.game_code);
    LobbyCommand end_command(ENDLOBBY, "");
    q.push(end_command);
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
        LobbyCommand end_command(ENDLOBBY, "");
        q.push(end_command);
    } else {
        emit failedToJoin();
    }
}

std::string MainWindow::get_player_name()
{
    return player_name;
}

MainWindow::~MainWindow()
{
    delete ui;
}

