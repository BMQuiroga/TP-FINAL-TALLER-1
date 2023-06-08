#ifndef GAMEOPTION_H
#define GAMEOPTION_H
#include <string>
#include <QWidget>

class GameOption : public QWidget {
    Q_OBJECT
public:
    explicit GameOption(QWidget *parent = 0);
signals:
    void gameOptionPicked(const std::string& text);
private:
    void createGame();
    void joinGame();
    void connectEvents();
};

#endif // GAMEOPTION_H
