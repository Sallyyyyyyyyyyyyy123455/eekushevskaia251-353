#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onLogin();
    void onRegister();
    void onEncrypt();
    void onDecrypt();
    void onLogout();
    void onConnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError);
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString currentToken;
    void sendCommand(const QString& cmd);
    void setAuthMode(bool auth);
    void setCipherMode(bool cipher);
};

#endif
