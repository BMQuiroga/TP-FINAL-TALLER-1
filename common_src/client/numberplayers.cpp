#include "numberPlayers.h"
#include "GameOption.h"
#include "ui_NumberPlayers.h"
#include <iostream>

NumberPlayers::NumberPlayers(QWidget *parent) : QWidget(parent) {
    Ui::numberPlayers lobby;
    lobby.setupUi(this);
    connectEvents();
}

void NumberPlayers::setNumberOfPlayers() {
    QSpinBox* inputNumber = findChild<QSpinBox*>("inputNumber");
    QLabel* labelOut = findChild<QLabel*>("message");
    QString number = inputNumber->text();
    QString created_message = QString("Partida creada para %1 jugadores").arg(number);
    labelOut->setText(created_message);
    std::cout << "clicked!!" << std::endl;
}

void NumberPlayers::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonGreet = findChild<QPushButton*>("sendNumber");
    QObject::connect(buttonGreet, &QPushButton::clicked,
                     this, &NumberPlayers::setNumberOfPlayers);
}
