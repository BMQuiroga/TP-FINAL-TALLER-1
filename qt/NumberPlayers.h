#ifndef NUMBER_PLAYERS_H
#define NUMBER_PLAYERS_H

#include <QWidget>
#include <QLineEdit>
#include <QObject>

class NumberPlayers : public QWidget {
    Q_OBJECT
public:
    explicit NumberPlayers(QWidget *parent = 0);
public slots:
    void receiveNewGameCreatedCode(int code);
signals:
    void inputNumberEntered(const QString& text, int number);
private:
    void setNumberOfPlayers();
    void connectEvents();
    QLineEdit* inputGameName;
};

#endif // NUMBER_PLAYERS_H
