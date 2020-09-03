#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class GamePage;
}

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);
    ~GamePage();

private:
    Ui::GamePage *ui;
    QTcpSocket* clientSocket = nullptr;

private slots:
    void readInfo();
};

#endif // GAMEPAGE_H
