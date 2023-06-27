#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QObject>
#include <QKeyEvent>
#include <QPushButton>
#include <QString>


class Lobby : public QWidget {
    Q_OBJECT
public:
    explicit Lobby(QWidget *parent = 0);
    ~Lobby();
public slots:
    void deactivate();
signals:
    void inputPlayerInfoEntered(const QString& text, int player_type);
    void windowClosed();
private:
    void sendNewPlayerInfo();
    void connectEvents();
    int getPlayerTypeNumber(const QString& playerType);
    void selectPlayerOption();
    void closeEvent(QCloseEvent *event);
    bool is_active {true};
    QLineEdit* inputName;
    QPushButton* activeButton;
    QString initial_stylesheet;
};

#endif // LOBBY_H
