#include "NumberPlayers.h"
#include "ui_NumberPlayers.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <QThread>
#include <QKeyEvent>
#include <string>

NumberPlayers::NumberPlayers(QWidget *parent) : QWidget(parent)  {
    Ui::numberPlayers numberPlayers;
    numberPlayers.setupUi(this);
    connectEvents();
}

void NumberPlayers::setNumberOfPlayers() {
    QSpinBox* inputNumber = findChild<QSpinBox*>("inputNumber");
    inputGameName = findChild<QLineEdit*>("inputGameName");
    gameMode = findChild<QSpinBox*>("gameMode");
    QString name = inputGameName->text();
    QLabel* labelOut = findChild<QLabel*>("gameNumberLabel");
    QString number = inputNumber->text();
    QString game_mode = gameMode->text();
    QString created_message = QString("Partida creada para %1 jugadores").arg(number);
    labelOut->setText(created_message);
    emit inputNumberEntered(name, 
        std::stoi(number.toStdString()), std::stoi(game_mode.toStdString()));    
}

void NumberPlayers::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonGreet = findChild<QPushButton*>("sendNumber");
    QObject::connect(buttonGreet, &QPushButton::clicked,
                     this, &NumberPlayers::setNumberOfPlayers);
}

void NumberPlayers::deactivate() {
    is_active = false;
}

void NumberPlayers::closeEvent(QCloseEvent *event) {
    if (is_active) {
        emit windowClosed();
    }
    event->accept();
}

void NumberPlayers::receiveNewGameCreatedCode(int code) {
    QLabel* gameCodeLabel = findChild<QLabel*>("gameCodeLabel");
    QString code_message = QString::number(code);
    QString output_message = QString("Codigo de la partida: %1").arg(code_message);
    gameCodeLabel->setText(output_message);
}
