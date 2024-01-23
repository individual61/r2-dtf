#include "dialog.h"
#include "ui_dialog.h"
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>

// VID:  9025
// PID:  32858

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    arduino_port_name = "";
    arduino_is_available = false;
    arduino = new QSerialPort;
    serialBuffer = "";

   /* qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Has VID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier())
        {
            qDebug() << "VID: " << serialPortInfo.vendorIdentifier();
        }

        qDebug() << "Has PID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier())
        {
            qDebug() << "PID: " << serialPortInfo.productIdentifier();
        }
    }
*/

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())
        {
            if(serialPortInfo.vendorIdentifier() == arduino_nano_33_vid)
            {
                 if(serialPortInfo.productIdentifier() == arduino_nano_33_pid)
                {
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }

    if(arduino_is_available)
    {
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));

    }
    else
    {
        QMessageBox::warning(this, "Port error", "Couldn't find the Arduino");
    }


}

Dialog::~Dialog()
{
    if(arduino->isOpen())
    {
        arduino->close();
    }
    delete ui;
}

void Dialog::readSerial()
{
    // qDebug() << "in readSerial()";
    serialData = arduino->readAll();
    serialBuffer += QString::fromStdString(serialData.toStdString());
    qDebug() << serialBuffer;

}

void Dialog::handle_button_clicked(QString data)
{
    qDebug() << "Button " << data << " clicked";
    if(arduino->isWritable())
    {
        arduino->write(data.toStdString().c_str());
    }
    else
    {
        qDebug() << "Couldn't write to serial";
    }
}

void Dialog::on_buttonA_clicked()
{
    handle_button_clicked("A");
}


void Dialog::on_buttonB_clicked()
{
    handle_button_clicked("B");
}


void Dialog::on_buttonC_clicked()
{
    handle_button_clicked("C");
}

