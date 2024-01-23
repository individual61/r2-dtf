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
    void handle_button_clicked(QString data);

    void on_buttonA_clicked();

    void on_buttonB_clicked();

    void on_buttonC_clicked();

    void readSerial();

private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    static const quint16 arduino_nano_33_vid = 9025;
    static const quint16 arduino_nano_33_pid = 32858;

    QByteArray serialData;
    QString serialBuffer;

    QString arduino_port_name;
    bool arduino_is_available;
};
#endif // DIALOG_H
