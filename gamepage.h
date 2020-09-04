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
    void showCardsCenter();
    void mousePressEvent(QMouseEvent *);
    void hide();
    void ask();
    void whoIsLord();
    void heIsLord();
    void go();
    void check();
    bool test();

private:
    Ui::GamePage *ui;
    QTcpSocket* clientSocket = nullptr;
    QString lord = "";
    int lordplayer;
    int playerId;
    int nowPlayer;
    int previousPlayer;
    QVector<int> cardsLord;
    QVector<int> cards;
    QVector<int> cardsCenter;
    QVector<QLabel*> labelsLord = QVector<QLabel*>(3);
    QVector<QLabel*> labels = QVector<QLabel*>(20);
    QVector<QLabel*> labelsCenter = QVector<QLabel*>(10);
    bool cardsChosen[21] = {0};
    QVector<int> cardsToGo;
    int type;
    int typeRecived;
    int num;

private slots:
    void readInfo();
    void on_ask_clicked();
    void on_noask_clicked();
    void on_go_clicked();
    void on_nogo_clicked();
};

#endif // GAMEPAGE_H
