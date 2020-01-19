#include "serial.h"

Serial::Serial(QObject *parent) : QObject(parent)
{
    init();
}

Serial::~Serial(){
    if(_serial){
        delete _serial;
        _serial = nullptr;
    }
}

/*
*   功能：
*       初始化
*   参数：
*       无
*   返回值：
*       无
*/
void Serial::init(){
    _status = false;
    _serial = nullptr;
}

/*
*   功能：
*       获取可见串口
*   参数：
*       无
*   返回值：
*       串口
*/
QVector<QString> Serial::getPort(){
    QVector<QString> port;
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        QSerialPort serial;
        serial.setPort(info);
        port.append(serial.portName());
    }
    return port;
}

/*
*   功能：
*       打开串口
*   参数：
*       QString &port_name              I   串口名
*       QSerialPort::BaudRate &baud     I   波特率
*       QSerialPort::DataBits &bit      I   数据位数
*       QSerialPort::Parity &parity     I   校验位
*       QSerialPort::StopBits &stop     I   停止位
*       QSerialPort::FlowControl &flow  I   流控制
*   返回值：
*       无
*/
void Serial::openSerialSlot(QString port_name, QSerialPort::BaudRate baud, QSerialPort::DataBits bit,
                            QSerialPort::Parity parity, QSerialPort::StopBits stop, QSerialPort::FlowControl flow){
    if(_status){
        closeSerialSlot();
    }
    _serial = new QSerialPort;

    // 串口名
    _serial->setPortName(port_name);
    // 打开串口
    _serial->open(QIODevice::ReadWrite);
    // 波特率
    _serial->setBaudRate(baud);
    // 数据位数
    _serial->setDataBits(bit);
    // 校验位
    _serial->setParity(parity);
    // 停止位
    _serial->setStopBits(stop);
    // 流控制
    _serial->setFlowControl(flow);

    // 连接信号槽
    QObject::connect(_serial, &QSerialPort::readyRead, this, &Serial::readDataSlot);

}

/*
*   功能：
*       关闭串口
*   参数：
*       无
*   返回值：
*       无
*/
void Serial::closeSerialSlot(){
    // 断开信号
    disconnect(_serial, nullptr, nullptr, nullptr);
    _serial->clear();
    _serial->close();
    _serial->deleteLater();
    _serial = nullptr;
}

/*
*   功能：
*       读串口
*   参数：
*       无
*   返回值：
*       无
*/
void Serial::readDataSlot(){
    QByteArray buf;
    buf = _serial->readAll();
    if(!buf.isEmpty()){
        emit readDataSignal(buf);
        buf.clear();
    }
}

/*
*   功能：
*       写串口
*   参数：
*       无
*   返回值：
*       无
*/
void Serial::writeDataSlot(QByteArray& data){
    _serial->write(data, data.length());
}
