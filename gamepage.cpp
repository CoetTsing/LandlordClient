#include "gamepage.h"
#include "ui_gamepage.h"
#include <QDebug>
#include <algorithm>
#include <QMouseEvent>

GamePage::GamePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamePage)
{
    ui->setupUi(this);
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->ask->hide();
    ui->noask->hide();
    ui->go->hide();
    ui->nogo->hide();
    setMouseTracking(true);
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
            showCards();
            tmp = tmp.right(tmp.size() - 69);
        } else if (*tmp.begin() == '2') {
            nowPlayer = *(tmp.begin() + 1) - '0';
            hide();
            lord = "";
            for (auto i = tmp.begin() + 2; i != tmp.end(); i++)
                lord += *i;
            ask();
            tmp = tmp.right(tmp.size() - 2 - lord.size());
        } else if (*tmp.begin() == '3') {
            lordplayer = *(tmp.begin() + 1) - '0';
            tmp = tmp.right(tmp.size() - 2);
        } else if (*tmp.begin() == '4') {
            for (int i = 1; i <= 12; i += 4) {
                int a = 1000 * (tmp[i] - '0') + 100 * (tmp[i + 1] - '0') + 10 * (tmp[i + 2] - '0') + (tmp[i + 3] - '0');
                cardsLord.push_back(a);
            }
            showCardsLord();
            if (playerId == lordplayer) {
                for (int i = 0; i < 3; i++)
                    cards.push_back(cardsLord[i]);
                std::sort(cards.begin(), cards.end(), cmp);
                showCards();
            }
            tmp = tmp.right(tmp.size() - 13);
        } else if (*tmp.begin() == '5') {
            nowPlayer = *(tmp.begin() + 1) - '0';
            hide();
            previousPlayer = *(tmp.begin() + 2) - '0';
            typeRecived = *(tmp.begin() + 3) - '0';
            if (typeRecived == 0)
                num = 0;
            else {
                num = *(tmp.begin() + 4) - '0';
                if (num == 0)
                    num = 10;
            }
            go();
            tmp = tmp.right(tmp.size() - 5 - num * 4);
        }
    }
}

bool GamePage::cmp(int a, int b) {
    return a > b;
}

void GamePage::showCardsLord() {
    for (int i = 0; i < 3; i++) {
        delete labelsLord[i];
        labelsLord[i] = new QLabel(this);
        QString path = ":/cards/" + QString::number(cardsLord[i]) + ".png";
        QPixmap pic(path);
        pic.scaled(72, 97, Qt::KeepAspectRatio);
        labelsLord[i]->setScaledContents(true);
        labelsLord[i]->setPixmap(pic);
        labelsLord[i]->resize(72, 97);
        labelsLord[i]->move(464 + i * 100, 30);
        labelsLord[i]->show();
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

void GamePage::showCardsCenter() {
    for (int i = 0; i < 10; i++) {
        if (labelsCenter[i] != nullptr)
            delete labelsCenter[i];
        labelsCenter[i] = nullptr;
    }
    for (int i = 0; i < cardsCenter.size(); i++) {
        labelsCenter[i] = new QLabel(this);
        QString path = ":/cards/" + QString::number(cardsCenter[i]) + ".png";
        QPixmap pic(path);
        pic.scaled(143, 193, Qt::KeepAspectRatio);
        labelsCenter[i]->setScaledContents(true);
        labelsCenter[i]->setText(QString::number(i));
        labelsCenter[i]->setPixmap(pic);
        labelsCenter[i]->resize(143, 193);
        labelsCenter[i]->move(400 + i * 40, 250);
        labelsCenter[i]->show();
    }
}

void GamePage::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    int n = 21;
    if (x >= 200 && x <= 200 + 40 * cards.size() && y <= 743 && y >= 550)
        n = (x - 200) / 40;
    cardsChosen[n] = !cardsChosen[n];
    for (int i = 0; i < cards.size(); i++) {
        if (cardsChosen[i])
            labels[i]->move(200 + i * 40, 530);
        else
           labels[i]->move(200 + i * 40, 550);
    }
}

void GamePage::hide() {
    if (playerId == 2) {
        if (nowPlayer == 1) {
            ui->label_6->show();
            ui->label_5->hide();
            ui->label_4->hide();
        } else if (nowPlayer == 2) {
            ui->label_4->show();
            ui->label_5->hide();
            ui->label_6->hide();
        } else {
            ui->label_5->show();
            ui->label_4->hide();
            ui->label_6->hide();
        }
    } else {
        if (nowPlayer == 1) {
            ui->label_5->show();
            ui->label_6->hide();
            ui->label_4->hide();
        } else if (nowPlayer == 2) {
            ui->label_6->show();
            ui->label_5->hide();
            ui->label_4->hide();
        } else {
            ui->label_4->show();
            ui->label_5->hide();
            ui->label_6->hide();
        }
    }
}

void GamePage::ask() {
    if (nowPlayer == playerId) {
        ui->ask->show();
        ui->noask->show();
    }
}

void GamePage::whoIsLord() {
    if (playerId == 2) {
        if (lord[2] == '2')
            lordplayer = 2;
        else {
            if (lord[1] == '1')
                lordplayer = 1;
            else
                lordplayer = 3;
        }
    }
    else {
        if (lord[2] == '3')
            lordplayer = 3;
        else {
            if (lord[1] == '2')
                lordplayer = 2;
            else
                lordplayer = 1;
        }
    }
    heIsLord();
}

void GamePage::heIsLord() {
    QString iAmLord = "3" + QString::number(lordplayer);
    clientSocket->write(iAmLord.toUtf8().data());
}

void GamePage::go() {
    if (nowPlayer == playerId) {
        ui->go->show();
        ui->nogo->setEnabled(true);
        ui->nogo->show();
    }
    if (previousPlayer == playerId) {
        ui->nogo->setEnabled(false);
    }
}

void GamePage::check() {
    QVector<int> c;
    for (auto x: cardsToGo)
        c.push_back(x / 10);
    qDebug() << c;
    if (c.size() == 1)
        type = 1;
    else if (c.size() == 2) {
        if (c[0]== c[1])
            type = 2;
        else if (c[0] == 122 && c[1] == 121)
            type = 9;
        else
            type = 0;
    }
    else if (c.size() == 3) {
        if (c[0] == c[1] && c[1] == c[2])
            type = 3;
        else
            type = 0;
    }
    else if (c.size() == 4) {
        if (c[0] == c[1] && c[1] == c[2] && c[2] == c[3])
            type = 8;
        else if ((c[0] == c[1] && c[1] == c[2] && c[2] != c[3])
                 || (c[1] == c[2] && c[2] == c[3] && c[0] != c[1]))
            type = 3;
        else
            type = 0;
    }
    else if (c.size() == 5) {
        if ((c[0] == c[1] && c[1] == c[2] && c[2] != c[3] && c[3] == c[4])
                || (c[1] != c[2] && c[2] == c[3] && c[0] == c[1] && c[3] == c[4]))
            type = 3;
        else if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1)
            type = 4;
        else
            type = 0;
    }
    else if (c.size() == 6) {
        if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1 && c[4] - c[5] == 1)
            type = 4;
        else if (c[0] == c[1] && c[1] - c[2] == 1 && c[2] == c[3] && c[3] - c[4] == 1 && c[4] == c[5])
            type = 5;
        else if ((c[0] == c[1] && c[1] == c[2] && c[2] == c[3] && c[4] == c[5])
                 || (c[0] == c[1] && c[2] == c[3] && c[3] == c[4] && c[4] == c[5]))
            type = 6;
        else if (c[0] == c[1] && c[1] == c[2] && c[2] - c[3] == 1 && c[3] == c[4] && c[4] == c[5])
            type = 7;
        else
            type = 0;
    }
    else if (c.size() == 7) {
        if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1 && c[4] - c[5] == 1 && c[5] - c[6] == 1)
            type = 4;
        else
            type = 0;
    }
    else if (c.size() == 8) {
        if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1 && c[4] - c[5] == 1 && c[5] - c[6] == 1 && c[6] - c[7] == 1)
            type = 4;
        else if (c[0] == c[1] && c[1] - c[2] == 1 && c[2] == c[3] && c[3] - c[4] == 1 && c[4] == c[5] && c[5] - c[6] == 1 && c[6] == c[7])
            type = 5;
        else if ((c[2] == c[3] && c[3] == c[4] && c[4] - c[5] == 1 && c[5] == c[6] && c[6] == c[7])
                 || (c[1] == c[2] && c[2] == c[3] && c[3] - c[4] == 1 && c[4] == c[5] && c[5] == c[6])
                 || (c[0] == c[1] && c[1] == c[2] && c[2] - c[3] == 1 && c[3] == c[4] && c[4] == c[5]))
            type = 7;
        else
            type = 0;
    }
    else if (c.size() == 9) {
        if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1 && c[4] - c[5] == 1 && c[5] - c[6] == 1 && c[6] - c[7] == 1 && c[7] == c[8])
            type = 4;
        else if (c[0] == c[1] && c[1] == c[2] && c[2] - c[3] == 1 && c[3] == c[4] && c[4] == c[5] && c[5] - c[6] == 1 && c[6] == c[7] && c[7] == c[8])
            type = 7;
        else
            type = 0;
    }
    else if (c.size() == 10) {
        if (c[0] - c[1] == 1 && c[1] - c[2] == 1 && c[2] - c[3] == 1 && c[3] - c[4] == 1 && c[4] - c[5] == 1 && c[5] - c[6] == 1 && c[6] - c[7] == 1 && c[7] - c[8] == 1 && c[8] - c[9] == 1)
            type = 4;
        else if (c[0] == c[1] && c[1] - c[2] == 1 && c[2] == c[3] && c[3] - c[4] == 1 && c[4] == c[5] && c[5] - c[6] == 1 && c[6] == c[7] && c[7] - c[8] == 1 && c[8] == c[9])
            type = 5;
        else if ((c[0] == c[1] && c[2] == c[3] && c[4] == c[5] && c[5] == c[6] && c[6] - c[7] == 1 && c[7] == c[8] && c[8] == c[9])
                 || (c[0] == c[1] && c[2] == c[3] && c[3] == c[4] && c[4] - c[5] == 1 && c[5] == c[6] && c[6] == c[7] && c[8] == c[9])
                 || (c[0] == c[1] && c[1] == c[2] && c[2] - c[3] == 1 && c[3] == c[4] && c[4] == c[5] && c[6] == c[7] && c[8] == c[9]))
            type = 7;
        else
            type = 0;
    }
}

bool GamePage::test() {
    return true;
}

void GamePage::on_ask_clicked()
{
    lord += QString::number(playerId);
    if (lord.size() == 3)
        whoIsLord();
    else {
        if (playerId == 2)
            nowPlayer = 3;
        else
            nowPlayer = 1;
        hide();
        QString now = "2" + QString::number(nowPlayer) + lord;
        clientSocket->write(now.toUtf8().data());
    }
    ui->ask->hide();
    ui->noask->hide();
}

void GamePage::on_noask_clicked()
{
    lord += '0';
    if (lord.size() == 3)
        whoIsLord();
    else {
        if (playerId == 2)
            nowPlayer = 3;
        else
            nowPlayer = 1;
        hide();
        QString now = "2" + QString::number(nowPlayer) + lord;
        clientSocket->write(now.toUtf8().data());
    }
    ui->ask->hide();
    ui->noask->hide();
}

void GamePage::on_go_clicked()
{
    cardsToGo.clear();
    for (int i = 0; i < cards.size(); i++)
        if (cardsChosen[i])
            cardsToGo.push_back(cards[i]);
    check();
    qDebug() << type;
    if (type != 0) {
        if (test()) {

            for (auto x: cardsToGo)
                cardsCenter.push_back(x);
            showCardsCenter();
            cardsCenter.clear();
            QVector<int> tmp;
            for (int i = 0; i < cards.size(); i++) {
                if (!cardsChosen[i])
                    tmp.push_back(cards[i]);
                else
                    cardsChosen[i] = false;
            }
            cards.clear();
            for (auto x: tmp)
                cards.push_back(x);
            showCards();
        }
    }
}

void GamePage::on_nogo_clicked()
{

}
