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
    for (auto x: tmp)
        qDebug() << x;
}
