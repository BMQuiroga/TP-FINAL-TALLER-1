#include "Lobby.h"
#include "ui_Lobby.h"
#include <iostream>
#include <QKeyEvent>

Lobby::Lobby(QWidget *parent) : QWidget(parent) {
    Ui::Lobby lobby;
    lobby.setupUi(this);
    connectEvents();
}

void Lobby::sendNewPlayerInfo() {
    inputName = findChild<QLineEdit*>("inputName");
    playerType = findChild<QSpinBox*>("playerType");
    gameMode = findChild<QSpinBox*>("gameMode");
    QString name = inputName->text();
    QString player_type = playerType->text();
    QString game_mode = gameMode->text();
    emit inputPlayerInfoEntered(name, 
        std::stoi(player_type.toStdString()), std::stoi(game_mode.toStdString()));
    inputName->clear();
    playerType->clear();
    gameMode->clear();
}

void Lobby::deactivate() {
    is_active = false;
}

void Lobby::closeEvent(QCloseEvent *event) {
    if (is_active) {
        emit windowClosed();
    }
    event->accept();
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
