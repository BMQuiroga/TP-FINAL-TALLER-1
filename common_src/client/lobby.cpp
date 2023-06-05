#include "Lobby.h"
#include "GameOption.h"
#include "ui_Lobby.h"
#include <iostream>

Lobby::Lobby(QWidget *parent) : QWidget(parent) {
    Ui::Lobby lobby;
    lobby.setupUi(this);
    connectEvents();
}

void Lobby::showGameOptions() {
    std::cout << "clicked!!" << std::endl;
    GameOption* gameOptions = new GameOption();
    gameOptions->show();
}

void Lobby::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonGreet = findChild<QPushButton*>("buttonName");
    QObject::connect(buttonGreet, &QPushButton::clicked,
                     this, &Lobby::showGameOptions);
}
