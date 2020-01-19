#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->button_serial, &QPushButton::clicked, this, &MainWindow::on_button_open_serial_clicked);
//    connect(&serial, &Serial::readDataSignal, this, &MainWindow::readData);
    init();

}

void MainWindow::init(){
    //查找可用的串口
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        QSerialPort serial;
        serial.setPort(info);
        ui->box_port->addItem(serial.portName());
//        if(serial.open(QIODevice::ReadWrite)){
//            ui->box_port->addItem(serial.portName());
//            serial.close();
//        }
    }
    // 设置波特率下拉菜单默认显示第0项
    ui->box_baud->addItem(QString::number(QSerialPort::Baud1200));
    ui->box_baud->addItem(QString::number(QSerialPort::Baud2400));
    ui->box_baud->addItem(QString::number(QSerialPort::Baud4800));
    ui->box_baud->addItem(QString::number(QSerialPort::Baud9600));
    ui->box_baud->addItem(QString::number(QSerialPort::Baud19200));
    ui->box_baud->addItem(QString::number(QSerialPort::Baud38400));
    ui->box_baud->addItem(QString::number(QSerialPort::Baud57600));
    ui->box_baud->addItem(QString::number(QSerialPort::Baud115200));
    ui->box_baud->setCurrentIndex(0);

    // 设置数据位
    ui->box_bit->addItem(QString::number(QSerialPort::DataBits::Data8));
    ui->box_bit->addItem(QString::number(QSerialPort::DataBits::Data7));
    ui->box_bit->addItem(QString::number(QSerialPort::DataBits::Data6));
    ui->box_bit->addItem(QString::number(QSerialPort::DataBits::Data5));

    // 设置校验位
    ui->box_parity->addItem(QString::number(QSerialPort::Parity::NoParity));
    ui->box_parity->addItem(QString::number(QSerialPort::Parity::EvenParity));
    ui->box_parity->addItem(QString::number(QSerialPort::Parity::OddParity));
    ui->box_parity->addItem(QString::number(QSerialPort::Parity::SpaceParity));
    ui->box_parity->addItem(QString::number(QSerialPort::Parity::MarkParity));

    // 设置停止位
    ui->box_stop->addItem(QString::number(QSerialPort::StopBits::OneStop));
    ui->box_stop->addItem(QString::number(QSerialPort::StopBits::OneAndHalfStop));
    ui->box_stop->addItem(QString::number(QSerialPort::StopBits::TwoStop));

    // 设置流控
    ui->box_flow_control->addItem(QString::number(QSerialPort::FlowControl::NoFlowControl));
    ui->box_flow_control->addItem(QString::number(QSerialPort::FlowControl::HardwareControl));
    ui->box_flow_control->addItem(QString::number(QSerialPort::FlowControl::SoftwareControl));
}
void ::MainWindow::initWinSerial(){
//    struct PortSettings portSet = {BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,500};

    _serial = new Win_QextSerialPort("\\\\.\\COM3", QextSerialBase::EventDriven);
    auto ret = _serial->open(QIODevice::ReadWrite);
        qDebug() << "initWinSerial: "<< ret;
    if(ret){
        _serial->setBaudRate(BAUD115200);
        _serial->setDataBits(DATA_8);
        _serial->setParity(PAR_NONE);
        _serial->setStopBits(STOP_1);
        _serial->setFlowControl(FLOW_OFF);

        auto ret = connect(_serial, SIGNAL(readyRead()), this, SLOT(readDataSlot()));
        qDebug() << ret;
    }


}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button_open_serial_clicked()
{

    if(ui->button_serial->text() == tr("打开串口")){
#if 0
#else
        initWinSerial();
        ui->button_serial->setText(tr("关闭串口"));
#endif
    }else{
#if 0
#else
        _serial->close();
#endif
        ui->button_serial->setText(tr("打开串口"));
    }
}

void MainWindow::on_button_send_clicked()
{

}


void MainWindow::readData(QByteArray buf){
//    QByteArray buf;
//    buf = serial->readAll();
    static int count = 0;
    static QByteArray str;

    if(!buf.isEmpty()){
        qDebug() << count++ << "buf len: " << buf.length() ;

        QString str = ui->textEdit_read->toPlainText();
        if(count > 200){
            count = 0;
            str.clear();
        }
        str += buf;
//        auto offset = str.indexOf('*');
//        if(-1 != offset){
//            ui->textEdit_read->setText(str.mid(0, offset + 3));
//            str.remove(0, offset + 3);
//        }

//        str += tr(buf);
        ui->textEdit_read->clear();
        ui->textEdit_read->append(str);
    }
//    buf.clear();
}


void MainWindow::readDataSlot(){
    static int count = 0;
    static QByteArray str;
    QByteArray buf = _serial->readAll();
    qDebug() << buf;

    if(!buf.isEmpty()){
        qDebug() << count++ << "buf len: " << buf.length() ;

        QString str = ui->textEdit_read->toPlainText();
        if(count > 200){
            count = 0;
            str.clear();
        }
        str += buf;
//        auto offset = str.indexOf('*');
//        if(-1 != offset){
//            ui->textEdit_read->setText(str.mid(0, offset + 3));
//            str.remove(0, offset + 3);
//        }

//        str += tr(buf);
        ui->textEdit_read->clear();
        ui->textEdit_read->append(str);
    }
}
