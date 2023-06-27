#include "Lobby.h"
#include "ui_Lobby.h"
#include <iostream>
#include <QKeyEvent>

Lobby::Lobby(QWidget *parent) : QWidget(parent) {
    Ui::Lobby lobby;
    lobby.setupUi(this);
    connectEvents();
}

int Lobby::getPlayerTypeNumber(const QString& playerType) {
    std::string type = playerType.toStdString();
    if (type == PLAYERTYPE1) {
        return 1;
    } else if (type == PLAYERTYPE2) {
        return 2;
    }
    return 3;
}

void Lobby::sendNewPlayerInfo() {
    inputName = findChild<QLineEdit*>("inputName");
    if (!activeButton) {
        QLabel* errorLabel = findChild<QLabel*>("error");
        errorLabel->setText("Elija una de las opciones antes de continuar");
        return;
    } 
    QString playerType = activeButton->objectName();
    QString name = inputName->text();
    int player_type = getPlayerTypeNumber(playerType);
    emit inputPlayerInfoEntered(name, player_type);
    inputName->clear();
}

void Lobby::deactivate() {
    is_active = false;
}

void Lobby::selectPlayerOption() {
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    QString new_style = buttonSender->styleSheet().append(
        QString("QPushButton { background-color: #89d8d3; }"));
    buttonSender->setStyleSheet(new_style);
    if (activeButton != nullptr) {
        activeButton->setStyleSheet(initial_stylesheet);
    }
    activeButton = buttonSender;
}

void Lobby::closeEvent(QCloseEvent *event)
{
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
    QPushButton* idf_button = findChild<QPushButton*>(PLAYERTYPE1);
    QObject::connect(idf_button, &QPushButton::clicked,
                     this, &Lobby::selectPlayerOption);
    QPushButton* p90_button = findChild<QPushButton*>(PLAYERTYPE2);
    QObject::connect(p90_button, &QPushButton::clicked,
                     this, &Lobby::selectPlayerOption);
    QPushButton* scout_button = findChild<QPushButton*>(PLAYERTYPE3);
    QObject::connect(scout_button, &QPushButton::clicked,
                     this, &Lobby::selectPlayerOption);
    initial_stylesheet = p90_button->styleSheet();
}

Lobby::~Lobby() {
    delete inputName;
    delete this;
}
