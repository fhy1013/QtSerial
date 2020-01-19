#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QVector>

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial(QObject *parent = nullptr);
    ~Serial();

    void init();
    QVector<QString> getPort();
signals:
    void readDataSignal(QByteArray);

public slots:
    void openSerialSlot(QString,
                        QSerialPort::BaudRate baud = QSerialPort::BaudRate::Baud115200,
                        QSerialPort::DataBits bit = QSerialPort::DataBits::Data8,
                        QSerialPort::Parity parity = QSerialPort::Parity::NoParity,
                        QSerialPort::StopBits stop = QSerialPort::StopBits::OneStop,
                        QSerialPort::FlowControl flow = QSerialPort::NoFlowControl);
    void closeSerialSlot();

    void readDataSlot();
    void writeDataSlot(QByteArray&);
private:
    QSerialPort *_serial;
    bool _status;
};

#endif // SERIAL_H
