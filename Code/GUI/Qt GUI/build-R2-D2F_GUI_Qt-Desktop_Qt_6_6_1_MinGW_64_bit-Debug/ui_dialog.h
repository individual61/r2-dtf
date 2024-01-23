/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonA;
    QPushButton *buttonB;
    QPushButton *buttonC;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_lcd;
    QLCDNumber *lcdNumber;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(800, 600);
        widget = new QWidget(Dialog);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(80, 80, 254, 26));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        buttonA = new QPushButton(widget);
        buttonA->setObjectName("buttonA");

        horizontalLayout->addWidget(buttonA);

        buttonB = new QPushButton(widget);
        buttonB->setObjectName("buttonB");

        horizontalLayout->addWidget(buttonB);

        buttonC = new QPushButton(widget);
        buttonC->setObjectName("buttonC");

        horizontalLayout->addWidget(buttonC);

        widget1 = new QWidget(Dialog);
        widget1->setObjectName("widget1");
        widget1->setGeometry(QRect(120, 190, 251, 31));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_lcd = new QLabel(widget1);
        label_lcd->setObjectName("label_lcd");

        horizontalLayout_2->addWidget(label_lcd);

        lcdNumber = new QLCDNumber(widget1);
        lcdNumber->setObjectName("lcdNumber");

        horizontalLayout_2->addWidget(lcdNumber);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        buttonA->setText(QCoreApplication::translate("Dialog", "A", nullptr));
        buttonB->setText(QCoreApplication::translate("Dialog", "B", nullptr));
        buttonC->setText(QCoreApplication::translate("Dialog", "C", nullptr));
        label_lcd->setText(QCoreApplication::translate("Dialog", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
