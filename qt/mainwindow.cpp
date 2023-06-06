#include "mainwindow.h"
#include "lobby.h"
#include "gameoption.h"
#include "numberplayers.h"
#include "joingame.h"
#include <iostream>
#include <ostream>
#include <string>
#include <QDebug>
#include "./ui_mainwindow.h"
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
    // Close the current widget if it exists uwu
    if (currentWidget) {
        currentWidget->close();
    }

    // Create and show the new widget (e.g., GameOptionsWidget)
    currentWidget = new JoinGame();
    currentWidget->show();
}

void MainWindow::showCreateGame()
{
    // Close the current widget if it exists uwu
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
    // Close the current widget if it exists uwu
    if (currentWidget) {
        currentWidget->close();
    }

    // Create and show the new widget (e.g., GameOptionsWidget)
    currentWidget = new GameOption();
    currentWidget->show();
    QObject::connect(currentWidget, SIGNAL(gameOptionPicked(std::string)), this, SLOT(pickGameOption(std::string)));
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
}

void MainWindow::pickGameOption(const std::string& text) {
    if (text == JOINGAME) {
        showJoinGame();
    } else {
        showCreateGame();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

