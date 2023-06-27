#include "NumberPlayers.h"
#include "ui_NumberPlayers.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <QThread>
#include <QKeyEvent>
#include <QPushButton>
#include <string>

NumberPlayers::NumberPlayers(QWidget *parent) : QWidget(parent)  {
    Ui::numberPlayers numberPlayers;
    numberPlayers.setupUi(this);
    connectEvents();
}

int NumberPlayers::getGameModeNumber(const QString& gameMode) {
    std::string mode = gameMode.toStdString();
    if (mode == "clear_zone") {
        return 1;
    }
    return 2;
}

void NumberPlayers::setNumberOfPlayers() {
    QSpinBox* inputNumber = findChild<QSpinBox*>("inputNumber");
    if (!activeButton) {
        QLabel* errorLabel = findChild<QLabel*>("error_game_mode");
        errorLabel->setText("Elija una de las opciones antes de continuar");
        return;
    } 
    inputGameName = findChild<QLineEdit*>("inputGameName");
    QString name = inputGameName->text();
    QLabel* labelOut = findChild<QLabel*>("gameNumberLabel");
    QString number = inputNumber->text();
    QString modeName = activeButton->objectName();
    int game_mode = getGameModeNumber(modeName);
    QString created_message = QString("Partida creada para %1 jugadores").arg(number);
    labelOut->setText(created_message);
    emit inputNumberEntered(name, 
        std::stoi(number.toStdString()), game_mode);    
}

void NumberPlayers::selectGameModeOption() {
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    QString new_style = buttonSender->styleSheet().append(
        QString("QPushButton { background-color: #89d8d3; }"));
    buttonSender->setStyleSheet(new_style);
    if (activeButton != nullptr) {
        activeButton->setStyleSheet(initial_stylesheet);
    }
    activeButton = buttonSender;
}

void NumberPlayers::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonSend = findChild<QPushButton*>("sendNumber");
    QObject::connect(buttonSend, &QPushButton::clicked,
                     this, &NumberPlayers::setNumberOfPlayers);
    QPushButton* clear_zone_button = findChild<QPushButton*>("clear_zone");
    QObject::connect(clear_zone_button, &QPushButton::clicked,
                     this, &NumberPlayers::selectGameModeOption);
    QPushButton* survival_button = findChild<QPushButton*>("survival");
    QObject::connect(survival_button, &QPushButton::clicked,
                     this, &NumberPlayers::selectGameModeOption);
    initial_stylesheet = survival_button->styleSheet();
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
