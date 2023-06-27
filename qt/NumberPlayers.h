#ifndef NUMBER_PLAYERS_H
#define NUMBER_PLAYERS_H

#include <QWidget>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
#include <QPushButton>
#include <QString>

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
    void selectGameModeOption();
    void connectEvents();
    int getGameModeNumber(const QString& gameMode);
    void closeEvent(QCloseEvent *event);
    bool is_active {true};
    QLineEdit* inputGameName;
    QPushButton* activeButton{nullptr};
    QString initial_stylesheet;
};

#endif // NUMBER_PLAYERS_H
