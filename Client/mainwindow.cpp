#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::onError);
    socket->connectToHost("127.0.0.1", 8080);
    setAuthMode(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onConnected() {
    QMessageBox::information(this, "Connected", "Connected to server");
}

void MainWindow::sendCommand(const QString& cmd) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(cmd.toUtf8());
        socket->flush();
    }
}

void MainWindow::onLogin() {
    QString user = ui->lineEditUser->text();
    QString pass = ui->lineEditPass->text();
    sendCommand(QString("LOGIN|%1|%2").arg(user).arg(pass));
}

void MainWindow::onRegister() {
    QString user = ui->lineEditUser->text();
    QString pass = ui->lineEditPass->text();
    sendCommand(QString("REGISTER|%1|%2").arg(user).arg(pass));
}

void MainWindow::onEncrypt() {
    QString text = ui->plainTextEditInput->toPlainText();
    QString key = ui->lineEditKey->text();
    sendCommand(QString("ENCRYPT|%1|%2").arg(text).arg(key));
}

void MainWindow::onDecrypt() {
    QString text = ui->plainTextEditInput->toPlainText();
    QString key = ui->lineEditKey->text();
    sendCommand(QString("DECRYPT|%1|%2").arg(text).arg(key));
}

void MainWindow::onReadyRead() {
    QByteArray data = socket->readAll();
    QString response = QString::fromUtf8(data);
    QStringList parts = response.split('|');
    
    if (parts[0] == "OK") {
        if (parts.size() > 1 && parts[1] == "Registered") {
            QMessageBox::information(this, "Success", "Registration OK. Now login.");
        } else if (parts[0] == "OK" && parts.size() == 2 && parts[1] != "Registered") {
            // Login или Encrypt/Decrypt результат
            if (parts[1].toInt() > 0) { // Login success
                currentToken = parts[1];
                setAuthMode(false);
                setCipherMode(true);
                QMessageBox::information(this, "Login OK", "Welcome!");
            } else {
                // Encrypt/Decrypt result
                ui->plainTextEditOutput->setPlainText(parts[1]);
            }
        }
    } else if (parts[0] == "ERROR") {
        QMessageBox::warning(this, "Error", parts[1]);
    }
}

void MainWindow::setAuthMode(bool auth) {
    ui->groupBoxAuth->setVisible(auth);
    ui->groupBoxCipher->setVisible(!auth);
}

void MainWindow::onLogout() {
    currentToken.clear();
    setAuthMode(true);
    setCipherMode(false);
    ui->plainTextEditInput->clear();
    ui->plainTextEditOutput->clear();
    ui->lineEditKey->clear();
}

void MainWindow::setCipherMode(bool cipher) {
    // Включаем кнопки шифрования
    ui->pushButtonEncrypt->setEnabled(cipher);
    ui->pushButtonDecrypt->setEnabled(cipher);
}

void MainWindow::onError(QAbstractSocket::SocketError) {
    QMessageBox::critical(this, "Error", socket->errorString());
}
