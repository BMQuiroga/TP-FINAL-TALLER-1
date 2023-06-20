#include "GameOption.h"

#include "ui_GameOption.h"
#include "NumberPlayers.h"
#include <QKeyEvent>

GameOption::GameOption(QWidget *parent) : QWidget(parent) {
    Ui::GameOption gameOption;
    gameOption.setupUi(this);
    connectEvents();
}

void GameOption::createGame() {
    emit createGameOptionPicked();
}

void GameOption::joinGame() {
    emit joinGameOptionPicked();
}

void GameOption::deactivate() {
    is_active = false;
}

void GameOption::closeEvent(QCloseEvent *event) {
    if (is_active) {
        emit windowClosed();
    }
    event->accept();
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
