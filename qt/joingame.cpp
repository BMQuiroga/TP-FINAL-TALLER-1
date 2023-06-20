#include "JoinGame.h"
#include "GameOption.h"
#include "ui_JoinGame.h"
#include <thread>
#include <chrono>
#include <QThread>
#include <QKeyEvent>
#include <iostream>

JoinGame::JoinGame(QWidget *parent) : QWidget(parent)  {
    Ui::JoinGame lobby;
    lobby.setupUi(this);
    connectEvents();
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
    // QThread::sleep(3);
    // std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(3));
}
