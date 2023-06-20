#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QObject>
#include <QKeyEvent>


class Lobby : public QWidget {
    Q_OBJECT
public:
    explicit Lobby(QWidget *parent = 0);
    ~Lobby();
public slots:
    void deactivate();
signals:
    void inputPlayerInfoEntered(const QString& text, int number);
    void windowClosed();
private:
    void sendNewPlayerInfo();
    void connectEvents();
    void closeEvent(QCloseEvent *event);
    bool is_active {true};
    QLineEdit* inputName;
    QSpinBox* playerType;
};

#endif // LOBBY_H
