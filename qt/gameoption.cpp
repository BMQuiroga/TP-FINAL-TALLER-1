#include "GameOption.h"

#include "ui_GameOption.h"
#include "JoinGame.h"
#include "NumberPlayers.h"

GameOption::GameOption(QWidget *parent) : QWidget(parent) {
    Ui::GameOption GameOption;
    GameOption.setupUi(this);
    connectEvents();
}

void GameOption::createGame() {
    emit createGameOptionPicked();
}

void GameOption::joinGame() {
    emit joinGameOptionPicked();
}

void GameOption::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonCreate = findChild<QPushButton*>("createGameButton");
    QPushButton* buttonJoin = findChild<QPushButton*>("joinGameButton");
    QObject::connect(buttonCreate, &QPushButton::clicked,
                     this, &GameOption::createGame);
    QObject::connect(buttonJoin, &QPushButton::clicked,
    this, &GameOption::joinGame);
}
