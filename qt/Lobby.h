#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QObject>


class Lobby : public QWidget {
    Q_OBJECT
public:
    explicit Lobby(QWidget *parent = 0);
    ~Lobby();
signals:
    void inputPlayerInfoEntered(const QString& text, int number);
private:
    void sendNewPlayerInfo();
    void connectEvents();
    QLineEdit* inputName;
    QSpinBox* playerType;
};

#endif // LOBBY_H
