#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>
#include <QLineEdit>
#include <QObject>


class Lobby : public QWidget {
    Q_OBJECT
public:
    explicit Lobby(QWidget *parent = 0);
    ~Lobby();
signals:
    void inputTextEntered(const QString& text);
private:
    void sendInputName();
    void connectEvents();
    QLineEdit* inputName;
};

#endif // LOBBY_H
