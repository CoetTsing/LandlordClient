#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QTcpSocket>
#include <QLabel>

namespace Ui {
class GamePage;
}

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);
    ~GamePage();
    static bool cmp(int a, int b);
    void showCardsLord();
    void showCards();
    void hide();
    void ask();
    void whoIsLord();
    void heIsLord();

private:
    Ui::GamePage *ui;
    QTcpSocket* clientSocket = nullptr;
    QString lord = "";
    int lordplayer;
    int playerId;
    int nowPlayer;
    QVector<int> cardsLord;
    QVector<int> cards;
    QVector<QLabel*> labelsLord = QVector<QLabel*>(3);
    QVector<QLabel*> labels = QVector<QLabel*>(20);

private slots:
    void readInfo();
    void on_ask_clicked();
    void on_noask_clicked();
};

#endif // GAMEPAGE_H
