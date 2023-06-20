#include "waitingroom.h"
#include "ui_WaitingRoom.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <QKeyEvent>

WaitingRoom::WaitingRoom(int game_code, QWidget *parent) : 
    QWidget(parent), game_code(game_code) {
    Ui::WaitingRoom waitingRoom;
    waitingRoom.setupUi(this);
    numberPlayersLabel = findChild<QLabel*>("playersInfoLabel");
    counterLabel = findChild<QLabel*>("counterLabel");
    gameCodeLabel = findChild<QLabel*>("gameCodeLabel");
    QString code_message = QString("El código de la partida es: %1").arg(QString::number(game_code));
    gameCodeLabel->setText(code_message);
    connectEvents();
}

void WaitingRoom::receiveWaitingInfo(int number_players_connected, int max_number_players) {
    QString number_connected = QString::number(number_players_connected);
    QString total_number = QString::number(max_number_players);
    QString created_message = QString("%1/%2 jugadores conectados").arg(number_connected, total_number);
    numberPlayersLabel->setText(created_message);
}

void WaitingRoom::receiveStartSignal() {
    numberPlayersLabel->setText(QString("La partida empezará en:"));
    timer->start(1000);
}

void WaitingRoom::handleTimer() {
    counterLabel->setText(QString::number(counter));
    counter--;
    if (counter == 0) {
        is_active = false;
        this->close();
    }
}

void WaitingRoom::deactivate() {
    is_active = false;
}

void WaitingRoom::closeEvent(QCloseEvent *event) {
    if (is_active) {
        emit windowClosed();
    }
    event->accept();
}

void WaitingRoom::connectEvents() {
   timer = new QTimer(this);
   QObject::connect(timer, SIGNAL(timeout()), this, SLOT(handleTimer()));
}
