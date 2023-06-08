#include "JoinGame.h"
#include "GameOption.h"
#include "ui_JoinGame.h"
#include <iostream>

JoinGame::JoinGame(QWidget *parent) : QWidget(parent) {
    Ui::JoinGame lobby;
    lobby.setupUi(this);
    connectEvents();
}

void JoinGame::connectToGame() {
    QLineEdit* inputCode = findChild<QLineEdit*>("inputCode");
    QLabel* labelOut = findChild<QLabel*>("message");
    QString code = inputCode->text();
    QString message = QString("Te uniste a la partida con código %1").arg(code);
    labelOut->setText(message);
    std::cout << "clicked!!" << std::endl;
}

void JoinGame::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonCode = findChild<QPushButton*>("buttonCode");
    QObject::connect(buttonCode, &QPushButton::clicked,
                     this, &JoinGame::connectToGame);
}
