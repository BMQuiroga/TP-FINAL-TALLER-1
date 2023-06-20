#ifndef GAMEOPTION_H
#define GAMEOPTION_H
#include <string>
#include <QWidget>
#include <QKeyEvent>

class GameOption : public QWidget {
    Q_OBJECT
public:
    explicit GameOption(QWidget *parent = 0);
signals:
    void joinGameOptionPicked();
    void createGameOptionPicked();
    void windowClosed();
public slots:
    void deactivate();
private:
    void createGame();
    void joinGame();
    void connectEvents();
    void closeEvent(QCloseEvent *event);
    bool is_active {true};
};

#endif // GAMEOPTION_H
