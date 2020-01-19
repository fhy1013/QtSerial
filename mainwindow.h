#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "serial.h"
#include "win_qextserialport.h"

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void initWinSerial();

private slots:
    void on_button_open_serial_clicked();

    void on_button_send_clicked();

    void readData(QByteArray);
    void readDataSlot();
private:
    Ui::MainWindow *ui;

//    QSerialPort *serial;
    Serial serial;
    Win_QextSerialPort *_serial;
};

#endif // MAINWINDOW_H
