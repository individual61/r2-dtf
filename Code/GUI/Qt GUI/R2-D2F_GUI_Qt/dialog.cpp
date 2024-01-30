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

    connect(ui->m1a_setbutton, SIGNAL(clicked()), this, SLOT(m1a_setbutton_clicked()));
    connect(ui->m1b_setbutton, SIGNAL(clicked()), this, SLOT(m1b_setbutton_clicked()));
    connect(ui->m2a_setbutton, SIGNAL(clicked()), this, SLOT(m2a_setbutton_clicked()));
    connect(ui->m2b_setbutton, SIGNAL(clicked()), this, SLOT(m2b_setbutton_clicked()));

    connect(ui->m1pwm_setbutton, SIGNAL(clicked()), this, SLOT(m1pwm_setbutton_clicked()));
    connect(ui->m2pwm_setbutton, SIGNAL(clicked()), this, SLOT(m2pwm_setbutton_clicked()));

    connect(ui->m1drive_setbutton, SIGNAL(clicked()), this, SLOT(m1drive_setbutton_clicked()));
    connect(ui->m2drive_setbutton, SIGNAL(clicked()), this, SLOT(m2drive_setbutton_clicked()));

    connect(ui->m1brake_setbutton, SIGNAL(clicked()), this, SLOT(m1brake_setbutton_clicked()));
    connect(ui->m2brake_setbutton, SIGNAL(clicked()), this, SLOT(m2brake_setbutton_clicked()));

    connect(ui->stopall_button, SIGNAL(clicked()), this, SLOT(stopall_setbutton_clicked()));


    // Connect the signals from the buttons to the slots
    connect(ui->buttonA, SIGNAL(clicked()), this, SLOT(buttonA_clicked()));
    connect(ui->buttonB, SIGNAL(clicked()), this, SLOT(buttonB_clicked()));
    connect(ui->buttonC, SIGNAL(clicked()), this, SLOT(buttonC_clicked()));

    // Unknown Command
    connect(ui->unkcommand_setbutton, SIGNAL(clicked()), this, SLOT(unknowncommand_setbutton_clicked()));

    // RGB LED
    connect(ui->redled_setbutton, SIGNAL(clicked()), this, SLOT(redled_setbutton_clicked()));
    connect(ui->greenled_setbutton, SIGNAL(clicked()), this, SLOT(greenled_setbutton_clicked()));
    connect(ui->blueled_setbutton, SIGNAL(clicked()), this, SLOT(blueled_setbutton_clicked()));






    arduino_port_name = "";
    arduino_is_available = false;
    arduino = new QSerialPort;
    serialBuffer = "";

    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        //qDebug() << "Has VID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier())
        {
            qDebug() << "VID: " << serialPortInfo.vendorIdentifier();
        }

        //qDebug() << "Has PID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier())
        {
            qDebug() << "PID: " << serialPortInfo.productIdentifier();
        }
    }


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

        qDebug() << "Arduino connected on port " << arduino_port_name;


        ui->arduinoconnected_connected->setText("True");
        ui->arduinoconnected_comport->setText(arduino_port_name);


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
        qDebug() << "Closed Arduino serial connection.";
    }
    delete ui;
}

/*void Dialog::readSerial()
{
    // qDebug() << "in readSerial()";
    serialData = arduino->readAll();
    serialBuffer += QString::fromStdString(serialData.toStdString());
    qDebug() << serialBuffer;
}*/

//QByteArray serialBuffer; // Use a QByteArray for binary data accumulation


void Dialog::readSerial() {
    qDebug() << "In readSerial()";

    const int SERIAL_MESSAGE_BUFFER_SIZE = 16; // this should not be used here
    const byte SERIAL_HEADER = 0x3E; // decimal 62 '>'
    const byte SERIAL_PAD = 0x5F;    // decimal 95 '_'
    const int SERIAL_SEND_BUFFER_SIZE = (2 * SERIAL_MESSAGE_BUFFER_SIZE + 2);

    serialBuffer = arduino->readAll();
    serialBuffer += serialBuffer; // Accumulate binary data in serialBuffer
    qDebug() << "serialbuffer size: " << serialBuffer.length();
    qDebug() << "serialbuffer: " << serialBuffer.toHex(':');



    bool serial_buffer_long_enough = false;

    int while_loop_count = 0;

    // Make sure it is even worth looking at the serialBuffer
    while (serialBuffer.length() >= SERIAL_SEND_BUFFER_SIZE)
    {
        while_loop_count++;
        //qDebug() << "Entering while loop to process serialBuffer. Iteration number " << while_loop_count;
        //qDebug() << "serialbuffer is long enough.";
        int newlineIndex = serialBuffer.indexOf((char)SERIAL_HEADER);
        if ( (newlineIndex >= 0) && (serialBuffer[newlineIndex+1]== (char)SERIAL_HEADER) )
        {
            //qDebug() << "First two header chars found, first at newlineIndex " << newlineIndex << ". They are " << (char) serialBuffer[newlineIndex] << " and " << (char) serialBuffer[newlineIndex+1] << " and match " << (char) SERIAL_HEADER;

            if (newlineIndex + SERIAL_SEND_BUFFER_SIZE <= serialBuffer.size())
            {
                //qDebug() << "serialBuffer is at least long enough to hold a message starting at newline index.";
                //qDebug() << "Removing message from serialBuffer. Old length: " << serialBuffer.size();
                QByteArray message = serialBuffer.mid(newlineIndex, SERIAL_SEND_BUFFER_SIZE); // Extract message as QByteArray
                //qDebug() << "Message length: " << message.length();
                //qDebug() << "Message: " << message.toHex(':');

                serialBuffer.remove(0, newlineIndex + SERIAL_SEND_BUFFER_SIZE); // Remove processed message from buffer
                //qDebug() << "New serialBuffer length: " << serialBuffer.size();

                // Ensure that the message starts with the header
                if (message[0] == (char)SERIAL_HEADER && message[1] == (char)SERIAL_HEADER)
                {
                    //qDebug() << "Again we have confirmed that the extracted message starts with the header.";
                    // Unpack the boolean values
                    byte boolsPacked1 = message[2];
                    bool status_motor1_ina = boolsPacked1 & 1;
                    bool status_motor1_inb = boolsPacked1 & 2;
                    bool status_motor1_endiag = boolsPacked1 & 4;
                    bool status_motor2_ina = boolsPacked1 & 8;
                    bool status_motor2_inb = boolsPacked1 & 16;
                    bool status_motor2_endiag = boolsPacked1 & 32;
                    bool status_flag_1 = boolsPacked1 & 64;
                    bool status_flag_2 = boolsPacked1 & 128;

                    byte boolsPacked2 = message[4];
                    bool status_txu_oe = boolsPacked2 & 1;
                    bool status_d3 = boolsPacked2 & 2;
                    bool status_d11 = boolsPacked2 & 4;
                    bool status_d13 = boolsPacked2 & 8;
                    bool status_rgb_led_r = boolsPacked2 & 16;
                    bool status_rgb_led_g = boolsPacked2 & 32;
                    bool status_rgb_led_b = boolsPacked2 & 64;
                    bool status_flag_3 = boolsPacked2 & 128;

                    // Unpack the uint8_t values
                    uint8_t status_motor1_pwm, status_motor1_cs;
                    uint8_t status_motor2_pwm, status_motor2_cs;
                    uint8_t status_m1_enc_count, status_m1_rot_rate;
                    uint8_t status_m2_enc_count, status_m2_rot_rate;
                    int8_t status_acc_x, status_acc_y, status_acc_z;
                    int8_t status_ang_rate_x, status_ang_rate_y, status_ang_rate_z;

                    memcpy(&status_motor1_pwm, message.constData() + 6, sizeof(uint8_t));
                    memcpy(&status_motor1_cs, message.constData() + 8, sizeof(uint8_t));
                    memcpy(&status_motor2_pwm, message.constData() + 10, sizeof(uint8_t));
                    memcpy(&status_motor2_cs, message.constData() + 12, sizeof(uint8_t));
                    memcpy(&status_m1_enc_count, message.constData() + 14, sizeof(uint8_t));
                    memcpy(&status_m1_rot_rate, message.constData() + 16, sizeof(uint8_t));
                    memcpy(&status_m2_enc_count, message.constData() + 18, sizeof(uint8_t));
                    memcpy(&status_m2_rot_rate, message.constData() + 20, sizeof(uint8_t));
                    memcpy(&status_acc_x, message.constData() + 22, sizeof(int8_t));
                    memcpy(&status_acc_y, message.constData() + 24, sizeof(int8_t));
                    memcpy(&status_acc_z, message.constData() + 26, sizeof(int8_t));
                    memcpy(&status_ang_rate_x, message.constData() + 28, sizeof(int8_t));
                    memcpy(&status_ang_rate_y, message.constData() + 30, sizeof(int8_t));
                    memcpy(&status_ang_rate_z, message.constData() + 32, sizeof(int8_t));

                    // Print values for debugging (remove in the final code)
                    qDebug() << "-----------------------------";
                    qDebug() << "status_motor1_ina:\t\t\t" << status_motor1_ina;
                    qDebug() << "status_motor1_inb:\t\t\t" << status_motor1_inb;
                    qDebug() << "status_motor1_endiag:\t\t" << status_motor1_endiag;
                    qDebug() << "status_motor2_ina:\t\t\t" << status_motor2_ina;
                    qDebug() << "status_motor2_inb:\t\t\t" << status_motor2_inb;
                    qDebug() << "status_motor2_endiag:\t\t" << status_motor2_endiag;
                    qDebug() << "status_flag_1:\t\t\t" << status_flag_1;
                    qDebug() << "status_flag_2:\t\t\t" << status_flag_2;
                    qDebug() << "\t";
                    qDebug() << "status_txu_oe:\t\t\t" << status_txu_oe;
                    qDebug() << "status_d3:\t\t\t" << status_d3;
                    qDebug() << "status_d11:\t\t\t" << status_d11;
                    qDebug() << "status_d13:\t\t\t" << status_d13;
                    qDebug() << "status_rgb_led_r:\t\t\t" << status_rgb_led_r;
                    qDebug() << "status_rgb_led_g:\t\t\t" << status_rgb_led_g;
                    qDebug() << "status_rgb_led_b:\t\t\t" << status_rgb_led_b;
                    qDebug() << "status_flag_3:\t\t\t" << status_flag_3;
                    qDebug() << "\t";
                    qDebug() << "status_motor1_pwm:\t\t\t" << status_motor1_pwm;
                    qDebug() << "status_motor1_cs:\t\t\t" << status_motor1_cs;
                    qDebug() << "status_motor2_pwm:\t\t\t" << status_motor2_pwm;
                    qDebug() << "status_motor2_cs:\t\t\t" << status_motor2_cs;
                    qDebug() << "status_m1_enc_count:\t\t" << status_m1_enc_count;
                    qDebug() << "status_m1_rot_rate:\t\t\t" << status_m1_rot_rate;
                    qDebug() << "status_m2_enc_count:\t\t" << status_m2_enc_count;
                    qDebug() << "status_m2_rot_rate:\t\t\t" << status_m2_rot_rate;
                    qDebug() << "status_acc_x:\t\t\t" << status_acc_x;
                    qDebug() << "status_acc_y:\t\t\t" << status_acc_y;
                    qDebug() << "status_acc_z:\t\t\t" << status_acc_z;
                    qDebug() << "status_ang_rate_x:\t\t\t" << status_ang_rate_x;
                    qDebug() << "status_ang_rate_y:\t\t\t" << status_ang_rate_y;
                    qDebug() << "status_ang_rate_z:\t\t\t" << status_ang_rate_z;
                    qDebug() << "-----------------------------";
                    qDebug() << "-----------------------------";


                    QString statusText;

                    // Unknown Command
                    ui->unkcommand_indicator->setChecked(status_flag_3);
                    statusText = status_flag_3 ? "Unknown Command" : " ";
                    ui->unkcommand_readvalue->setText(statusText.toUtf8().constData());

                    // Red LED
                    ui->redled_indicator->setChecked(status_rgb_led_r);
                    statusText = status_rgb_led_r ? "On" : "Off";
                    ui->redled_readvalue->setText(statusText.toUtf8().constData());

                    // Green LED
                    ui->greenled_indicator->setChecked(status_rgb_led_g);
                    statusText = status_rgb_led_g ? "On" : "Off";
                    ui->greenled_readvalue->setText(statusText.toUtf8().constData());

                    // Blue LED
                    ui->blueled_indicator->setChecked(status_rgb_led_b);
                    statusText = status_rgb_led_b ? "On" : "Off";
                    ui->blueled_readvalue->setText(statusText.toUtf8().constData());

                    // TXU
                    statusText = status_txu_oe ? "On" : "Off";
                    ui->txu_readvalue->setText(statusText.toUtf8().constData());


                    // M1 INA
                    ui->m1a_indicator->setChecked(status_motor1_ina);
                    statusText = status_motor1_ina ? "1" : "0";
                    ui->m1a_readvalue->setText(statusText.toUtf8().constData());

                    // M1 INB
                    ui->m1b_indicator->setChecked(status_motor1_inb);
                    statusText = status_motor1_inb ? "1" : "0";
                    ui->m1b_readvalue->setText(statusText.toUtf8().constData());

                    // M2 INA
                    ui->m2a_indicator->setChecked(status_motor2_ina);
                    statusText = status_motor2_ina ? "1" : "0";
                    ui->m2a_readvalue->setText(statusText.toUtf8().constData());

                    // M2 INB
                    ui->m2b_indicator->setChecked(status_motor2_inb);
                    statusText = status_motor2_inb ? "1" : "0";
                    ui->m2b_readvalue->setText(statusText.toUtf8().constData());

                    // M1 PWM
                    statusText = QString::number(status_motor1_pwm);
                    ui->m1pwm_readvalue->setText(statusText.toUtf8().constData());

                    // M2 PWM
                    statusText = QString::number(status_motor2_pwm);
                    ui->m2pwm_readvalue->setText(statusText.toUtf8().constData());

                    // M1 EN/DIAG
                    ui->m1diag_indicator->setChecked(status_motor1_endiag);
                    statusText = status_motor1_endiag ? "1" : "0";
                    ui->m1diag_readvalue->setText(statusText.toUtf8().constData());

                    // M2 EN/DIAG
                    ui->m2diag_indicator->setChecked(status_motor2_endiag);
                    statusText = status_motor2_endiag ? "1" : "0";
                    ui->m2diag_readvalue->setText(statusText.toUtf8().constData());

                    // M1 Current
                    statusText = QString::number(status_motor1_cs);
                    ui->m1current_readvalue->setText(statusText.toUtf8().constData());

                    // M2 Current
                    statusText = QString::number(status_motor2_cs);
                    ui->m2current_readvalue->setText(statusText.toUtf8().constData());

                    // M1 Enc Count
                    statusText = QString::number(status_m1_enc_count);
                    ui->m1encct_readvalue->setText(statusText.toUtf8().constData());

                    // M2 Enc Count
                    statusText = QString::number(status_m2_enc_count);
                    ui->m2encct_readvalue->setText(statusText.toUtf8().constData());

                    // M1 Enc Rate
                    statusText = QString::number(status_m1_rot_rate);
                    ui->m1encrate_readvalue->setText(statusText.toUtf8().constData());

                    // M2 Enc Rate
                    statusText = QString::number(status_m2_rot_rate);
                    ui->m2encrate_readvalue->setText(statusText.toUtf8().constData());

                    // M1 Drive
                    statusText = QString::number(status_motor1_pwm);
                    ui->m1drive_readvalue->setText(statusText.toUtf8().constData());

                    // M2 Drive
                    statusText = QString::number(status_motor2_pwm);
                    ui->m2drive_readvalue->setText(statusText.toUtf8().constData());

                    // M1 Brake
                    statusText = QString::number(status_motor1_pwm);
                    ui->m1brake_readvalue->setText(statusText.toUtf8().constData());

                    // M2 Brake
                    statusText = QString::number(status_motor2_pwm);
                    ui->m2brake_readvalue->setText(statusText.toUtf8().constData());

                    // M1 Drive Indicator
                    if( status_motor1_ina && status_motor1_inb)
                    {
                        // Brake
                        ui->m1brake_indicator->setChecked(true);
                        ui->m1drive_indicator->setChecked(false);
                        ui->m1coast_indicator->setChecked(false);
                    }
                    // Drive
                    else if( (status_motor1_ina && (!status_motor1_inb)) || ((!status_motor1_ina) && status_motor1_inb) )
                    {
                        ui->m1brake_indicator->setChecked(false);
                        ui->m1drive_indicator->setChecked(true);
                        ui->m1coast_indicator->setChecked(false);
                    }
                    // Coast
                    else if( (!status_motor1_ina) && (!status_motor1_inb)  )
                    {
                        ui->m1brake_indicator->setChecked(false);
                        ui->m1drive_indicator->setChecked(false);
                        ui->m1coast_indicator->setChecked(true);
                    }

                    // M2 Drive Indicator
                    if( status_motor2_ina && status_motor2_inb)
                    {
                        // Brake
                        ui->m2brake_indicator->setChecked(true);
                        ui->m2drive_indicator->setChecked(false);
                        ui->m2coast_indicator->setChecked(false);
                    }
                    // Drive
                    else if( (status_motor2_ina && (!status_motor2_inb)) || ((!status_motor2_ina) && status_motor2_inb) )
                    {
                        ui->m2brake_indicator->setChecked(false);
                        ui->m2drive_indicator->setChecked(true);
                        ui->m2coast_indicator->setChecked(false);
                    }
                    // Coast
                    else if( (!status_motor2_ina) && (!status_motor2_inb)  )
                    {
                        ui->m2brake_indicator->setChecked(false);
                        ui->m2drive_indicator->setChecked(false);
                        ui->m2coast_indicator->setChecked(true);
                    }

                }
            }
            else
            {
                qDebug() << "serialBuffer was not long enough to hold a full message. Message size : " << SERIAL_SEND_BUFFER_SIZE << ", serialBuffer size: " << serialBuffer.size();
                break;
            }

        }
        else
        {
            qDebug() << "First two header chars not found.";
        }
    }
    qDebug() << "---------------------End of serialRead()------------------";
    qDebug() << "----------------------------------------------------------";
}



/*
void Dialog::readSerial() {
    QByteArray serialData = arduino->readAll();
    serialBuffer.append(serialData); // Accumulate binary data in serialBuffer

    // Process complete messages separated by '\n'
    int newlineIndex;
    while ((newlineIndex = serialBuffer.indexOf('\n')) >= 0) {
        QByteArray message = serialBuffer.left(newlineIndex); // Extract message as QByteArray
        qDebug() << message.toHex(); // Print the hexadecimal representation for debugging
        serialBuffer.remove(0, newlineIndex + 1); // Remove processed message from buffer

        if (message.size() >= 11) { // 1 byte for packed booleans + 4 bytes * 2 for ints + 4 bytes * 2 for floats
            // Extract packed booleans
            byte boolsPacked = message[0];
            bool bool1 = boolsPacked & 1;  // Least significant bit
            bool bool2 = boolsPacked & 2;  // Second least significant bit

            // Extract ints and floats
            int int1, int2;
            float float1, float2;

            memcpy(&int1, message.constData() + 1, sizeof(int));

            // Reverse the byte order for the second int
            memcpy(&int2, message.constData() + 5, sizeof(int));
            int2 = qFromLittleEndian<int>(&int2);

            // Reverse the byte order for the floats
            memcpy(&float1, message.constData() + 9, sizeof(float));
            float1 = qFromLittleEndian<float>(&float1);
            memcpy(&float2, message.constData() + 13, sizeof(float));
            float2 = qFromLittleEndian<float>(&float2);

            // Print values
            qDebug() << "Bool1:" << bool1 << "\tBool2:" << bool2
                     << "\tInt1:" << int1 << "\tInt2:" << int2
                     << "\tFloat1:" << float1 << "\tFloat2:" << float2;
        }
    }
}
*/



void Dialog::m1a_setbutton_clicked()
{
    QString setvalue = ui->m1a_setvalue->text();
    QString command = "<M1A," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}
void Dialog::m1b_setbutton_clicked()
{
    QString setvalue = ui->m1b_setvalue->text();
    QString command = "<M1B," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::m2a_setbutton_clicked()
{
    QString setvalue = ui->m2a_setvalue->text();
    QString command = "<M2A," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}
void Dialog::m2b_setbutton_clicked()
{
    QString setvalue = ui->m2b_setvalue->text();
    QString command = "<M2B," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::m1pwm_setbutton_clicked()
{
    QString setvalue = ui->m1pwm_setvalue->text();
    QString command = "<M1P," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::m2pwm_setbutton_clicked()
{
    QString setvalue = ui->m2pwm_setvalue->text();
    QString command = "<M2P," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::m1drive_setbutton_clicked()
{
    QString setvalue = ui->m1drive_setvalue->text();
    QString command = "<M1S," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::m2drive_setbutton_clicked()
{
    QString setvalue = ui->m2drive_setvalue->text();
    QString command = "<M2S," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::m1brake_setbutton_clicked()
{
    QString setvalue = ui->m1brake_setvalue->text();
    QString command = "<M1K," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::m2brake_setbutton_clicked()
{
    QString setvalue = ui->m2brake_setvalue->text();
    QString command = "<M2K," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::stopall_setbutton_clicked()
{
    QString command = "<M1P,0,0>";
    handle_button_clicked(command);
    command = "<M2P,0,0>";
    handle_button_clicked(command);
    command = "<M1A,0,0>";
    handle_button_clicked(command);
    command = "<M1B,0,0>";
    handle_button_clicked(command);
    command = "<M2A,0,0>";
    handle_button_clicked(command);
    command = "<M2B,0,0>";
    handle_button_clicked(command);

    //qDebug() << command;
}




void Dialog::unknowncommand_setbutton_clicked()
{
    QString command = "<CE,0,0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::redled_setbutton_clicked()
{
    QString setvalue = ui->redled_setvalue->text();
    QString command = "<LDR," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::greenled_setbutton_clicked()
{
    QString setvalue = ui->greenled_setvalue->text();
    QString command = "<LDG," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
}

void Dialog::blueled_setbutton_clicked()
{
    QString setvalue = ui->blueled_setvalue->text();
    QString command = "<LDB," + setvalue + ",0>";
    handle_button_clicked(command);
    //qDebug() << command;
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

void Dialog::buttonA_clicked()
{
    handle_button_clicked("<LDB,0,0>");
}


void Dialog::buttonB_clicked()
{
    handle_button_clicked("<LDB,1,0>");
}


void Dialog::buttonC_clicked()
{
    handle_button_clicked("C");
}

