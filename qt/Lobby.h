#ifndef LOBBY_H
#define LOBBY_H
#define PLAYERTYPE1 "idf"
#define PLAYERTYPE2 "p90"
#define PLAYERTYPE3 "scout"

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
    QLineEdit* inputName{nullptr};
    QPushButton* activeButton{nullptr};
    QString initial_stylesheet;
};

#endif // LOBBY_H
