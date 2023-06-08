#include "Lobby.h"
#include "ui_Lobby.h"
#include <iostream>

Lobby::Lobby(QWidget *parent) : QWidget(parent) {
    Ui::Lobby lobby;
    lobby.setupUi(this);
    connectEvents();
}

void Lobby::sendInputName() {
    std::cout << "clicked!!" << std::endl;
    inputName = findChild<QLineEdit*>("inputName");
    QString name = inputName->text();
    emit inputTextEntered(name);
    inputName->clear();
}

void Lobby::connectEvents() {
    // Conecto el evento del boton
    QPushButton* buttonName = findChild<QPushButton*>("buttonName");
    QObject::connect(buttonName, &QPushButton::clicked,
                     this, &Lobby::sendInputName);
}

Lobby::~Lobby() {
    delete inputName;
    delete this;
}
