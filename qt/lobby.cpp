#include "Lobby.h"
#include "ui_Lobby.h"
#include <iostream>

Lobby::Lobby(QWidget *parent) : QWidget(parent) {
    Ui::Lobby lobby;
    lobby.setupUi(this);
    connectEvents();
}

void Lobby::sendNewPlayerInfo() {
    inputName = findChild<QLineEdit*>("inputName");
    playerType = findChild<QSpinBox*>("playerType");
    QString name = inputName->text();
    QString player_type = playerType->text();
    emit inputPlayerInfoEntered(name, std::stoi(player_type.toStdString()));
    inputName->clear();
    playerType->clear();
}

void Lobby::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonName = findChild<QPushButton*>("buttonName");
    QObject::connect(buttonName, &QPushButton::clicked,
                     this, &Lobby::sendNewPlayerInfo);
}

Lobby::~Lobby() {
    delete inputName;
    delete this;
}
