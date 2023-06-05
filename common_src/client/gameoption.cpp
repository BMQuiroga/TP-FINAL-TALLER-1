#include "GameOption.h"

#include "ui_GameOption.h"
#include "joinGame.h"
#include "numberPlayers.h"

GameOption::GameOption(QWidget *parent) : QWidget(parent) {
    Ui::GameOption GameOption;
    GameOption.setupUi(this);
    connectEvents();
}

void GameOption::createGame() {
    this->close();
    NumberPlayers* numberPlayers = new NumberPlayers();
    numberPlayers->show();
    delete this;
}

void GameOption::joinGame() {
    this->close();
    JoinGame* joinGame = new JoinGame();
    joinGame->show();
    delete this;
}

void GameOption::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonCreate = findChild<QPushButton*>("createGameButton");
    QPushButton* buttonJoin = findChild<QPushButton*>("joinGameButton");
    QObject::connect(buttonCreate, &QPushButton::clicked,
                     this, &GameOption::createGame);
    QObject::connect(buttonCreate, &QPushButton::clicked,
    this, &GameOption::createGame);
}
