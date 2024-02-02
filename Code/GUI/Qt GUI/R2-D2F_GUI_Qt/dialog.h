#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>


QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:

    void m1a_setbutton_clicked();
    void m1b_setbutton_clicked();
    void m2a_setbutton_clicked();
    void m2b_setbutton_clicked();

    void m1pwm_setbutton_clicked();
    void m2pwm_setbutton_clicked();

    void m1drive_setbutton_clicked();
    void m2drive_setbutton_clicked();

    void m1brake_setbutton_clicked();
    void m2brake_setbutton_clicked();

    void txu_setbutton_clicked();

    void stopall_setbutton_clicked();

    void unknowncommand_setbutton_clicked();

    void redled_setbutton_clicked();
    void greenled_setbutton_clicked();
    void blueled_setbutton_clicked();





    void handle_button_clicked(QString data);

    void buttonA_clicked();
    void buttonB_clicked();
    void buttonC_clicked();

    void readSerial();

private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    static const quint16 arduino_nano_33_vid = 9025;
    static const quint16 arduino_nano_33_pid = 32858;

    QByteArray serialBuffer;

    QString arduino_port_name;
    bool arduino_is_available;

    void reverseAndCopy(float* destination, const char* source);

};
#endif // DIALOG_H
