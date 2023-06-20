#include "waitingroom.h"
#include "ui_WaitingRoom.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <QKeyEvent>

WaitingRoom::WaitingRoom(QWidget *parent) : QWidget(parent)  {
    Ui::WaitingRoom waitingRoom;
    waitingRoom.setupUi(this);
    NumberPlayersLabel = findChild<QLabel*>("playersInfoLabel");
    counterLabel = findChild<QLabel*>("counterLabel");
    connectEvents();
}

void WaitingRoom::receiveWaitingInfo(int number_players_connected, int max_number_players) {
    QString number_connected = QString::number(number_players_connected);
    QString total_number = QString::number(max_number_players);
    QString created_message = QString("%1/%2 jugadores conectados").arg(number_connected, total_number);
    NumberPlayersLabel->setText(created_message);
}

void WaitingRoom::receiveStartSignal() {
    NumberPlayersLabel->setText(QString("La partida empezará en:"));
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
