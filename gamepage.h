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
    void showCards();

private:
    Ui::GamePage *ui;
    QTcpSocket* clientSocket = nullptr;
    int playerId;
    QVector<int> cardsLord;
    QVector<int> cards;
    QVector<QLabel*> labelsLord = QVector<QLabel*>(3);
    QVector<QLabel*> labels = QVector<QLabel*>(20);

private slots:
    void readInfo();
};

#endif // GAMEPAGE_H
