#include "MainWindow.h"
#include "Lobby.h"
#include "GameOption.h"
#include "NumberPlayers.h"
#include "JoinGame.h"
#include <iostream>
#include <ostream>
#include <string>
#include <QDebug>
#include "./ui_MainWindow.h"
#include "lobby_command.h"

MainWindow::MainWindow(Queue<LobbyCommand>& q, QWidget *parent) :
    q(q),
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
}

void MainWindow::showLobbyWidget() {
    // Close the current widget if it exists
    if (currentWidget) {
        currentWidget->close();
    }

    // Create and show the new widget (e.g., LobbyWidget)
    currentWidget = new Lobby();
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(inputTextEntered(QString)), this, SLOT(receiveInputText(QString)));
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

void MainWindow::receiveInputText(const QString& text) {
    // Handle the received input text
    qDebug() << "Received input text: " << text;
    showGameOptionsWidget();
    LobbyCommand command(INPUTNAME, text.toStdString());
    q.push(command);
}

void MainWindow::receiveInputGame(const QString& text, int number) {
    // Handle the received input text
    qDebug() << "Received input number: " << text;
    LobbyCommand command(CREATEGAME, text.toStdString(), number);
    q.push(command);
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
    qDebug() << "Received input number to join game: " << text;
    LobbyCommand command(JOINGAME, text.toStdString());
    q.push(command);
    LobbyCommand end_command(ENDLOBBY, "");
    q.push(end_command);
}

MainWindow::~MainWindow()
{
    delete ui;
}

