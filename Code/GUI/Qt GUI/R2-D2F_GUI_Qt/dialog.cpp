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


    connect(ui->txu_setbutton, SIGNAL(clicked()), this, SLOT(txu_setbutton_clicked()));

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
        arduino->setBaudRate(QSerialPort::Baud115200);
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
        ui->arduinoconnected_connected->setText("False");
        ui->arduinoconnected_comport->setText(" ");
    }


}

Dialog::~Dialog()
{
    if(arduino->isOpen())
    {
        arduino->close();
        qDebug() << "Closed Arduino serial connection.";
        ui->arduinoconnected_connected->setText("False");
        ui->arduinoconnected_comport->setText(" ");
    }
    delete ui;
}


/*
uint8_t:        1
int8_t:         1
uint16_t:       2
int16_t:        2
uint32_t:       4
int32_t:        4
float:          4
*/

void Dialog::readSerial() {
    //qDebug() << "In readSerial()";

    const byte SERIAL_HEADER = 0x3E; // decimal 62 '>'
    const byte SERIAL_PAD = 0x5F;    // decimal 95 '_'

    const byte SERIAL_MESSAGE_BUFFER_SIZE[3] = {28, 25, 25};
    byte SERIAL_SEND_BUFFER_SIZE[3] = {0, 0, 0};

    for (int i = 0; i <= 2; i++)
    {
        SERIAL_SEND_BUFFER_SIZE[i] =  (2 * SERIAL_MESSAGE_BUFFER_SIZE[i] + 2);
        //qDebug() << "SERIAL_MESSAGE_BUFFER_SIZE[" << i << "]: " << SERIAL_MESSAGE_BUFFER_SIZE[i] << ", SERIAL_SEND_BUFFER_SIZE[" << i << "]: " << SERIAL_SEND_BUFFER_SIZE[i];
    }


    serialBuffer = arduino->readAll();
    serialBuffer += serialBuffer; // Accumulate binary data in serialBuffer
    //qDebug() << "serialbuffer size: " << serialBuffer.length();
    //qDebug() << "serialbuffer: " << serialBuffer.toHex(':');

    //serialBuffer.length() >= SERIAL_MESSAGE_BUFFER_SIZE_2

    bool serial_buffer_long_enough = false;

    int while_loop_count = 0;

    int headerByteIndex = -1;
    while (  (headerByteIndex = serialBuffer.indexOf((char)SERIAL_HEADER)) >= 0  )  // Ensure there is at least one > in buffer and get its index
    {
        while_loop_count++;
        //qDebug() << "Entering while loop to process serialBuffer. Iteration number " << while_loop_count;
        QString statusText = QString::number(while_loop_count);
        ui->receiveserial_while_count_indicator->setText(statusText);

        //qDebug() << "Index of '>': " << headerByteIndex;
        //qDebug() << "serialbuffer is length " << serialBuffer.length();
        //qDebug() << "Is this greater than 3? serialBuffer.length() - headerByteIndex = " << serialBuffer.length() - headerByteIndex;

        if ( serialBuffer.length() - headerByteIndex >= 3) // There are enough characters in serial buffer to identify message type >>N
        {
            //qDebug() << "There are at least 3 characters in serialBuffer.";

            if ( serialBuffer[headerByteIndex + 1] == (char)SERIAL_HEADER ) // Byte after > is also >
            {
                //qDebug() << "The byte after > is also >";

                byte messageType = serialBuffer[headerByteIndex+2]; // Get message type
                //qDebug() << "Message type is " << messageType;
                //qDebug() << "serialBuffer.length() - headerByteIndex is " << serialBuffer.length() - headerByteIndex << " and should be >= "<< SERIAL_SEND_BUFFER_SIZE[(int) messageType];
                if (serialBuffer.length() - headerByteIndex >= SERIAL_SEND_BUFFER_SIZE[(int) messageType])
                {
                    //qDebug() << "Serial buffer is long enough to hold this message type.";


                    QByteArray message = serialBuffer.mid(headerByteIndex, SERIAL_SEND_BUFFER_SIZE[(int) messageType]); // Extract message as QByteArray
                    //qDebug() << "Extracting message from serialBuffer. Message length: " << message.length();
                    //qDebug() << "Message: " <<  message.toHex(':');

                    //qDebug() << "Removing message (And anything prior to it) from serialBuffer. Old length: " << serialBuffer.size();
                    serialBuffer.remove(0, headerByteIndex + SERIAL_SEND_BUFFER_SIZE[(int) messageType]); // Remove processed message from buffer
                    //qDebug() << "New serialBuffer length: " << serialBuffer.size();

                    processMessage(message);

                }
                else
                {
                    qDebug() << "serialBuffer was not long enough to hold this message type. Leaving receiveSerial() ------------";
                    return;
                }

            }
            else
            {
                qDebug() << "Second byte after first > was not also >. Leaving receiveSerial() ------------";
                return;
            }
        }
        else
        {
            qDebug() << "serialBuffer was not long enough to hold three characters. Leaving receiveSerial() ------------";
            return;
        }
        //qDebug() << "---------------------End of while() in readSerial()------------------";
    } // End of while()
}


void Dialog::processMessage(QByteArray message)
{
    //qDebug() << "In processMessage, message is " << message.toHex(':');
    byte messageType = message[2];
    QString statusText;

    uint index = 4;
    //qDebug() << "Processing message type " << messageType;

    if (messageType == 0)
    {
        // Unpack the boolean values

        byte boolsPacked1 = message[index];
        bool status_motor1_ina = boolsPacked1 & 1;
        bool status_motor1_inb = boolsPacked1 & 2;
        bool status_motor1_endiag = boolsPacked1 & 4;
        bool status_motor2_ina = boolsPacked1 & 8;
        bool status_motor2_inb = boolsPacked1 & 16;
        bool status_motor2_endiag = boolsPacked1 & 32;
        bool status_m1_rot_dir = boolsPacked1 & 64;
        bool status_m2_rot_dir = boolsPacked1 & 128;
        index = index + (1 + 1);

        byte boolsPacked2 = message[index];
        bool status_txu_oe = boolsPacked2 & 1;
        bool status_d3 = boolsPacked2 & 2;
        bool status_d11 = boolsPacked2 & 4;
        bool status_d13 = boolsPacked2 & 8;
        bool status_rgb_led_r = boolsPacked2 & 16;
        bool status_rgb_led_g = boolsPacked2 & 32;
        bool status_rgb_led_b = boolsPacked2 & 64;
        bool status_flag_unknown_message = boolsPacked2 & 128;
        index = index + (1 + 1);

        byte boolsPacked3 = message[index];
        bool status_flag_1 = boolsPacked3 & 1;
        bool status_flag_2 = boolsPacked3 & 2;
        bool status_flag_3 = boolsPacked3 & 4;
        bool status_flag_4 = boolsPacked3 & 8;
        bool status_flag_5 = boolsPacked3 & 16;
        bool status_flag_6 = boolsPacked3 & 32;
        bool status_flag_7 = boolsPacked3 & 64;
        bool status_flag_8 = boolsPacked3 & 128;
        index = index + (1 + 1);

        // status_motor1_pwm
        byte lowByte = message[index];
        index += 2;
        byte highByte = message[index];
        index += 2; // Skip the padding byte
        uint16_t status_motor1_pwm = ((uint16_t)highByte << 8) | lowByte;

        // status_motor1_cs
        lowByte = message[index];
        index += 2;
        highByte = message[index];
        index += 2; // Skip the padding byte
        uint16_t status_motor1_cs = ((uint16_t)highByte << 8) | lowByte;

        //status_motor2_pwm
        lowByte = message[index];
        index += 2;
        highByte = message[index];
        index += 2; // Skip the padding byte
        uint16_t status_motor2_pwm = ((uint16_t)highByte << 8) | lowByte;

        // status_motor2_cs
        lowByte = message[index];
        index += 2;
        highByte = message[index];
        index += 2; // Skip the padding byte
        uint16_t status_motor2_cs = ((uint16_t)highByte << 8) | lowByte;

        // status_m1_enc_count
        lowByte = message[index];
        index += 2;
        highByte = message[index];
        index += 2; // Skip the padding byte
        byte higherByte = message[index];
        index += 2;
        byte highestByte = message[index];
        index += 2; // Skip the padding byte
        int32_t status_m1_enc_count =  ((int32_t)highestByte << 24) | ((int32_t)higherByte << 16) | ((int32_t)highByte << 8) | lowByte;

        // status_m1_rot_rate
        lowByte = message[index];
        index += 2;
        highByte = message[index];
        index += 2; // Skip the padding byte
        higherByte = message[index];
        index += 2;
        highestByte = message[index];
        index += 2; // Skip the padding byte
        int32_t status_m1_rot_rate =  ((int32_t)highestByte << 24) | ((int32_t)higherByte << 16) | ((int32_t)highByte << 8) | lowByte;

        // status_m2_enc_count
        lowByte = message[index];
        index += 2;
        highByte = message[index];
        index += 2; // Skip the padding byte
        higherByte = message[index];
        index += 2;
        highestByte = message[index];
        index += 2; // Skip the padding byte
        int32_t status_m2_enc_count =  ((int32_t)highestByte << 24) | ((int32_t)higherByte << 16) | ((int32_t)highByte << 8) | lowByte;

        // status_m2_rot_rate
        lowByte = message[index];
        index += 2;
        highByte = message[index];
        index += 2; // Skip the padding byte
        higherByte = message[index];
        index += 2;
        highestByte = message[index];
        index += 2; // Skip the padding byte
        int32_t status_m2_rot_rate =  ((int32_t)highestByte << 24) | ((int32_t)higherByte << 16) | ((int32_t)highByte << 8) | lowByte;

/*
        qDebug() << "Message 0 -----------------------------";
        qDebug() << "status_motor1_ina:\t\t" << status_motor1_ina;
        qDebug() << "status_motor1_inb:\t\t" << status_motor1_inb;
        qDebug() << "status_motor1_endiag:\t\t" << status_motor1_endiag;
        qDebug() << "status_motor2_ina:\t\t" << status_motor2_ina;
        qDebug() << "status_motor2_inb:\t\t" << status_motor2_inb;
        qDebug() << "status_motor2_endiag:\t\t" << status_motor2_endiag;
        qDebug() << "status_m1_rot_dir:\t\t" << status_m1_rot_dir;
        qDebug() << "status_m2_rot_dir:\t\t" << status_m1_rot_dir;
        qDebug() << "\t";
        qDebug() << "status_txu_oe:\t\t" << status_txu_oe;
        qDebug() << "status_d3:\t\t\t" << status_d3;
        qDebug() << "status_d11:\t\t\t" << status_d11;
        qDebug() << "status_d13:\t\t\t" << status_d13;
        qDebug() << "status_rgb_led_r:\t\t" << status_rgb_led_r;
        qDebug() << "status_rgb_led_g:\t\t" << status_rgb_led_g;
        qDebug() << "status_rgb_led_b:\t\t" << status_rgb_led_b;
        qDebug() << "status_flag_unknown_message:\t" << status_flag_unknown_message;
        qDebug() << "\t";
        qDebug() << "status_flag_1:\t\t" << status_flag_1;
        qDebug() << "status_flag_2:\t\t" << status_flag_2;
        qDebug() << "status_flag_3:\t\t" << status_flag_3;
        qDebug() << "status_flag_4:\t\t" << status_flag_4;
        qDebug() << "status_flag_5:\t\t" << status_flag_5;
        qDebug() << "status_flag_6:\t\t" << status_flag_6;
        qDebug() << "status_flag_7:\t\t" << status_flag_7;
        qDebug() << "status_flag_8:\t\t" << status_flag_8;
        qDebug() << "\t";
        qDebug() << "status_motor1_pwm:\t\t" << status_motor1_pwm;
        qDebug() << "status_motor1_cs:\t\t" << status_motor1_cs;
        qDebug() << "status_motor2_pwm:\t\t" << status_motor2_pwm;
        qDebug() << "status_motor2_cs:\t\t" << status_motor2_cs;
        qDebug() << "\t";
        qDebug() << "status_m1_enc_count:\t\t" << status_m1_enc_count;
        qDebug() << "status_m1_rot_rate:\t\t" << status_m1_rot_rate;
        qDebug() << "status_m2_enc_count:\t\t" << status_m2_enc_count;
        qDebug() << "status_m2_rot_rate:\t\t" << status_m2_rot_rate;
        qDebug() << "-----------------------------";
        qDebug() << "\t";
*/

        // M1 INA
        ui->m1a_indicator->setChecked(status_motor1_ina);
        statusText = status_motor1_ina ? "1" : "0";
        ui->m1a_readvalue->setText(statusText.toUtf8().constData());

        // M1 INB
        ui->m1b_indicator->setChecked(status_motor1_inb);
        statusText = status_motor1_inb ? "1" : "0";
        ui->m1b_readvalue->setText(statusText.toUtf8().constData());

        // M1 EN/DIAG
        ui->m1diag_indicator->setChecked(status_motor1_endiag);
        statusText = status_motor1_endiag ? "1" : "0";
        ui->m1diag_readvalue->setText(statusText.toUtf8().constData());

        // M2 INA
        ui->m2a_indicator->setChecked(status_motor2_ina);
        statusText = status_motor2_ina ? "1" : "0";
        ui->m2a_readvalue->setText(statusText.toUtf8().constData());

        // M2 INB
        ui->m2b_indicator->setChecked(status_motor2_inb);
        statusText = status_motor2_inb ? "1" : "0";
        ui->m2b_readvalue->setText(statusText.toUtf8().constData());

        // M2 EN/DIAG
        ui->m2diag_indicator->setChecked(status_motor2_endiag);
        statusText = status_motor2_endiag ? "1" : "0";
        ui->m2diag_readvalue->setText(statusText.toUtf8().constData());

        // M1 Rot Dir
        ui->m1_enc_dir_fwd_indicator->setChecked(status_m1_rot_dir);
        ui->m1_enc_dir_rev_indicator->setChecked(!status_m1_rot_dir);

        // M2 Rot Dir
        ui->m2_enc_dir_fwd_indicator->setChecked(status_m2_rot_dir);
        ui->m2_enc_dir_rev_indicator->setChecked(!status_m2_rot_dir);




        // TXU
        statusText = status_txu_oe ? "On" : "Off";
        ui->txu_readvalue->setText(statusText.toUtf8().constData());

        // D3
        ui->d3_indicator->setChecked(status_d3);

        // D11
        ui->d11_indicator->setChecked(status_d11);

        // D13
        ui->d13_indicator->setChecked(status_d13);

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

        // Unknown Command
        ui->unkcommand_indicator->setChecked(status_flag_unknown_message);
        statusText = status_flag_unknown_message ? "Unknown Command" : " ";
        ui->unkcommand_readvalue->setText(statusText.toUtf8().constData());


        // Flag 1
        ui->flag1_indicator->setChecked(status_flag_1);

        // Flag 2
        ui->flag2_indicator->setChecked(status_flag_2);

        // Flag 3
        ui->flag3_indicator->setChecked(status_flag_3);

        // Flag 4
        ui->flag4_indicator->setChecked(status_flag_4);

        // Flag 5
        ui->flag5_indicator->setChecked(status_flag_5);

        // Flag 6
        ui->flag6_indicator->setChecked(status_flag_6);

        // Flag 7
        ui->flag7_indicator->setChecked(status_flag_7);

        // Flag 8
        ui->flag8_indicator->setChecked(status_flag_8);



        // M1 PWM
        statusText = QString::number(status_motor1_pwm);
        ui->m1pwm_readvalue->setText(statusText.toUtf8().constData());

        // M1 Current
        statusText = QString::number(status_motor1_cs);
        ui->m1current_readvalue->setText(statusText.toUtf8().constData());

        // M2 PWM
        statusText = QString::number(status_motor2_pwm);
        ui->m2pwm_readvalue->setText(statusText.toUtf8().constData());

        // M2 Current
        statusText = QString::number(status_motor2_cs);
        ui->m2current_readvalue->setText(statusText.toUtf8().constData());



        // M1 Enc Count
        statusText = QString::number(status_m1_enc_count);
        ui->m1encct_readvalue->setText(statusText.toUtf8().constData());

        // M1 Enc Rate
        statusText = QString::number(status_m1_rot_rate);
        ui->m1encrate_readvalue->setText(statusText.toUtf8().constData());

        // M2 Enc Count
        statusText = QString::number(status_m2_enc_count);
        ui->m2encct_readvalue->setText(statusText.toUtf8().constData());

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
    else if (messageType == 1)
    {
        byte floatBytes[4];

        // status_acc_x
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_acc_x;
        memcpy(&status_acc_x, floatBytes, sizeof(float));

        // status_acc_y
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_acc_y;
        memcpy(&status_acc_y, floatBytes, sizeof(float));


        // status_acc_z
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_acc_z;
        memcpy(&status_acc_z, floatBytes, sizeof(float));

        // status_angle_x
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_angle_x;
        memcpy(&status_angle_x, floatBytes, sizeof(float));

        // status_angle_y
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_angle_y;
        memcpy(&status_angle_y, floatBytes, sizeof(float));

        // status_angle_z
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_angle_z;
        memcpy(&status_angle_z, floatBytes, sizeof(float));

        /*
        qDebug() << "Message 1 -----------------------------";
        qDebug() << "status_acc_x:\t\t" << status_acc_x;
        qDebug() << "status_acc_y:\t\t" << status_acc_y;
        qDebug() << "status_acc_z:\t\t" << status_acc_z;
        qDebug() << "\t";
        qDebug() << "status_angle_x:\t\t" << status_angle_x;
        qDebug() << "status_angle_y:\t\t" << status_angle_y;
        qDebug() << "status_angle_z:\t\t" << status_angle_z;
        qDebug() << "-----------------------------";
        qDebug() << "\t";
*/


        // Acc X
        statusText = QString::number(status_acc_x);
        ui->accx_readvalue->setText(statusText.toUtf8().constData());

        // Acc Y
        statusText = QString::number(status_acc_y);
        ui->accy_readvalue->setText(statusText.toUtf8().constData());

        // Acc Z
        statusText = QString::number(status_acc_z);
        ui->accz_readvalue->setText(statusText.toUtf8().constData());


        // Angle X
        statusText = QString::number(status_angle_x);
        ui->anglex_readvalue->setText(statusText.toUtf8().constData());

        // Angle Y
        statusText = QString::number(status_angle_y);
        ui->angley_readvalue->setText(statusText.toUtf8().constData());

        // Angle Z
        statusText = QString::number(status_angle_z);
        ui->anglez_readvalue->setText(statusText.toUtf8().constData());


    }
    else if (messageType == 2)
    {
        byte floatBytes[4];

        // status_ang_rate_x
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_ang_rate_x;
        memcpy(&status_ang_rate_x, floatBytes, sizeof(float));

        // status_ang_rate_y
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_ang_rate_y;
        memcpy(&status_ang_rate_y, floatBytes, sizeof(float));

        // status_ang_rate_z
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_ang_rate_z;
        memcpy(&status_ang_rate_z, floatBytes, sizeof(float));



        // status_mag_x
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_mag_x;
        memcpy(&status_mag_x, floatBytes, sizeof(float));

        // status_mag_y
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_mag_y;
        memcpy(&status_mag_y, floatBytes, sizeof(float));

        // status_ang_rate_z
        floatBytes[0] = message[index];
        index += 2;
        floatBytes[1] = message[index];
        index += 2; // Skip the padding byte
        floatBytes[2] = message[index];
        index += 2;
        floatBytes[3] = message[index];
        index += 2; // Skip the padding byte
        float status_mag_z;
        memcpy(&status_mag_z, floatBytes, sizeof(float));


        qDebug() << "Message 2 -----------------------------";
        qDebug() << "status_ang_rate_x:\t\t" << status_ang_rate_x;
        qDebug() << "status_ang_rate_y:\t\t" << status_ang_rate_y;
        qDebug() << "status_ang_rate_z:\t\t" << status_ang_rate_z;
        qDebug() << "status_mag_x:\t\t" << status_mag_x;
        qDebug() << "status_mag_y:\t\t" << status_mag_y;
        qDebug() << "status_mag_z:\t\t" << status_mag_z;
        qDebug() << "-----------------------------";
        qDebug() << "\t";


        // Ang  X
        statusText = QString::number(status_ang_rate_x);
        ui->rotratex_readvalue->setText(statusText.toUtf8().constData());

        // Ang  Y
        statusText = QString::number(status_ang_rate_y);
        ui->rotratey_readvalue->setText(statusText.toUtf8().constData());

        // Ang  Z
        statusText = QString::number(status_ang_rate_z);
        ui->rotratez_readvalue->setText(statusText.toUtf8().constData());


        // Mag X
        statusText = QString::number(status_mag_x);
        ui->magx_readvalue->setText(statusText.toUtf8().constData());

        // Mag Y
        statusText = QString::number(status_mag_y);
        ui->magy_readvalue->setText(statusText.toUtf8().constData());

        // Mag Z
        statusText = QString::number(status_mag_z);
        ui->magz_readvalue->setText(statusText.toUtf8().constData());

    }
    else
    {
        qDebug() << "Unknown message type: " << messageType;
    }
}


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


void Dialog::txu_setbutton_clicked()
{
    QString setvalue = ui->txu_setvalue->text();
    QString command = "<TXU," + setvalue + ",0>";
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

