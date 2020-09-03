#include "gamepage.h"
#include "ui_gamepage.h"
#include <QDebug>

GamePage::GamePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamePage)
{
    ui->setupUi(this);
    clientSocket = new QTcpSocket(this);
    qint16 port = 8888;
    clientSocket->connectToHost("127.0.0.1", port);
    connect(clientSocket, &QTcpSocket::readyRead, this, &GamePage::readInfo);
}

GamePage::~GamePage()
{
    delete ui;
}

void GamePage::readInfo() {
    QByteArray tmp = clientSocket->readAll();
    qDebug() << tmp;
    while (tmp.size() != 0) {
        if (*tmp.begin() == '0') {
            if (*(tmp.begin() + 1) == '2') {
                playerId = 2;
                ui->label_2->setText("P3");
                ui->label_3->setText("P1");
            } else {
                playerId = 3;
                ui->label_1->setText("P3");
                ui->label_2->setText("P1");
                ui->label_3->setText("P2");
            }
            for (int i = 0; i < 3; i++) {
                delete labelsLord[i];
                labelsLord[i] = new QLabel(this);
                QString path = ":/cards/" + QString::number(1316) + ".png";
                QPixmap pic(path);
                pic.scaled(72, 97, Qt::KeepAspectRatio);
                labelsLord[i]->setScaledContents(true);
                labelsLord[i]->setPixmap(pic);
                labelsLord[i]->resize(72, 97);
                labelsLord[i]->move(464 + i * 100, 30);
                labelsLord[i]->show();
            }
            tmp = tmp.right(tmp.size() - 2);
        } else if (*tmp.begin() == '1') {
            for (int i = 1; i <= 68; i += 4) {
                int a = 1000 * (tmp[i] - '0') + 100 * (tmp[i + 1] - '0') + 10 * (tmp[i + 2] - '0') + (tmp[i + 3] - '0');
                cards.push_back(a);
            }
//            qDebug() << cards;
//            qDebug() << tmp.size();
            showCards();
            tmp = tmp.right(tmp.size() - 69);
        }
    }
}

void GamePage::showCards() {
    for (int i = 0; i < 20; i++) {
        if (labels[i] != nullptr)
            delete labels[i];
        labels[i] = nullptr;
    }
    for (int i = 0; i < cards.size(); i++) {
        labels[i] = new QLabel(this);
        QString path = ":/cards/" + QString::number(cards[i]) + ".png";
        QPixmap pic(path);
        pic.scaled(143, 193, Qt::KeepAspectRatio);
        labels[i]->setScaledContents(true);
        labels[i]->setText(QString::number(i));
        labels[i]->setPixmap(pic);
        labels[i]->resize(143, 193);
        labels[i]->move(200 + i * 40, 550);
        labels[i]->show();
    }
}
