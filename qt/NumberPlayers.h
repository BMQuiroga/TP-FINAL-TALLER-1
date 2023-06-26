#ifndef NUMBER_PLAYERS_H
#define NUMBER_PLAYERS_H

#include <QWidget>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>

class NumberPlayers : public QWidget {
    Q_OBJECT
public:
    explicit NumberPlayers(QWidget *parent = 0);
public slots:
    void receiveNewGameCreatedCode(int code);
    void deactivate();
signals:
    void inputNumberEntered(const QString& text, int number, int game_mode);
    void windowClosed();
private:
    void setNumberOfPlayers();
    void connectEvents();
    void closeEvent(QCloseEvent *event);
    bool is_active {true};
    QLineEdit* inputGameName;
    QSpinBox* gameMode;
};

#endif // NUMBER_PLAYERS_H
