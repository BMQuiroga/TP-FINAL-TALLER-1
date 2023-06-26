#include "JoinGame.h"
#include "GameOption.h"
#include "ui_JoinGame.h"
#include <thread>
#include <chrono>
#include <QThread>
#include "../common_src/protocol_types.h"
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QVector>
#include <QVectorIterator>
#include <QListWidget>
#include <iostream>

JoinGame::JoinGame(std::vector<GameReference> &games_list, 
    QWidget *parent) : QWidget(parent)  {
    Ui::JoinGame join_game;
    join_game.setupUi(this);
    QVector<GameReference> games = QVector<GameReference>(games_list.begin(), games_list.end());
    connectEvents();
    listWidget = findChild<QListWidget*>("listWidget");
    QVectorIterator<GameReference> i(games);
    int row = 0;
    while (i.hasNext()) {
        GameReference g = i.next();
        QListWidgetItem *newItem = new QListWidgetItem;
        QString id = QString::number(g.id);
        QString name = QString::fromStdString(g.name);
        QString game_mode = g.game_mode == 1 ? QString("Clear the zone") : QString("Survival");
        QString text =  QString("%1: \"%2\" partida para %3 jugadores. Modo de juego: %4").arg(
            id, name, QString::number(g.players), game_mode);
        newItem->setText(text);
        listWidget->insertItem(row, newItem);
        row++;
    }
}

void JoinGame::connectToGame() {
    QLineEdit* inputCode = findChild<QLineEdit*>("inputCode");
    QString code = inputCode->text();
    emit sendGameCodeEntered(code);
}

void JoinGame::closeEvent(QCloseEvent *event) {
    if (is_active) {
        emit windowClosed();
    }
    event->accept();
}

void JoinGame::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonCode = findChild<QPushButton*>("buttonCode");
    QObject::connect(buttonCode, &QPushButton::clicked,
                     this, &JoinGame::connectToGame);
}

void JoinGame::deactivate() {
    is_active = false;
}

void JoinGame::editOutputMessage(const std::string& message) {
    QLabel* labelOut = findChild<QLabel*>("outputLabel");
    QString created_message = QString::fromStdString(message);
    labelOut->setText(created_message);
}

void JoinGame::receiveUnsuccessfulJoin() {
    editOutputMessage(FAILURE_MESSAGE);
}

void JoinGame::receiveSuccessfulJoin() {
    editOutputMessage(SUCCESS_MESSAGE);
}
