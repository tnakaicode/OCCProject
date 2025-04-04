#ifndef UI_CALCULATOR_H
#define UI_CALCULATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Calculator
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLineEdit *display;
    QPushButton *button0;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;
    QPushButton *button6;
    QPushButton *button7;
    QPushButton *button8;
    QPushButton *button9;
    QPushButton *buttonAdd;
    QPushButton *buttonSubtract;
    QPushButton *buttonMultiply;
    QPushButton *buttonDivide;
    QPushButton *buttonEquals;
    QPushButton *buttonClear;

    void setupUi(QMainWindow *Calculator)
    {
        if (Calculator->objectName().isEmpty())
            Calculator->setObjectName(QString::fromUtf8("Calculator"));
        Calculator->resize(400, 300);
        centralWidget = new QWidget(Calculator);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        display = new QLineEdit(centralWidget);
        display->setObjectName(QString::fromUtf8("display"));
        display->setReadOnly(true);
        display->setAlignment(Qt::AlignRight);
        gridLayout->addWidget(display, 0, 0, 1, 4);

        button7 = new QPushButton(centralWidget);
        button7->setObjectName(QString::fromUtf8("button7"));
        gridLayout->addWidget(button7, 1, 0);

        button8 = new QPushButton(centralWidget);
        button8->setObjectName(QString::fromUtf8("button8"));
        gridLayout->addWidget(button8, 1, 1);

        button9 = new QPushButton(centralWidget);
        button9->setObjectName(QString::fromUtf8("button9"));
        gridLayout->addWidget(button9, 1, 2);

        buttonDivide = new QPushButton(centralWidget);
        buttonDivide->setObjectName(QString::fromUtf8("buttonDivide"));
        gridLayout->addWidget(buttonDivide, 1, 3);

        button4 = new QPushButton(centralWidget);
        button4->setObjectName(QString::fromUtf8("button4"));
        gridLayout->addWidget(button4, 2, 0);

        button5 = new QPushButton(centralWidget);
        button5->setObjectName(QString::fromUtf8("button5"));
        gridLayout->addWidget(button5, 2, 1);

        button6 = new QPushButton(centralWidget);
        button6->setObjectName(QString::fromUtf8("button6"));
        gridLayout->addWidget(button6, 2, 2);

        buttonMultiply = new QPushButton(centralWidget);
        buttonMultiply->setObjectName(QString::fromUtf8("buttonMultiply"));
        gridLayout->addWidget(buttonMultiply, 2, 3);

        button1 = new QPushButton(centralWidget);
        button1->setObjectName(QString::fromUtf8("button1"));
        gridLayout->addWidget(button1, 3, 0);

        button2 = new QPushButton(centralWidget);
        button2->setObjectName(QString::fromUtf8("button2"));
        gridLayout->addWidget(button2, 3, 1);

        button3 = new QPushButton(centralWidget);
        button3->setObjectName(QString::fromUtf8("button3"));
        gridLayout->addWidget(button3, 3, 2);

        buttonSubtract = new QPushButton(centralWidget);
        buttonSubtract->setObjectName(QString::fromUtf8("buttonSubtract"));
        gridLayout->addWidget(buttonSubtract, 3, 3);

        button0 = new QPushButton(centralWidget);
        button0->setObjectName(QString::fromUtf8("button0"));
        gridLayout->addWidget(button0, 4, 0, 1, 2);

        buttonClear = new QPushButton(centralWidget);
        buttonClear->setObjectName(QString::fromUtf8("buttonClear"));
        gridLayout->addWidget(buttonClear, 4, 2);

        buttonEquals = new QPushButton(centralWidget);
        buttonEquals->setObjectName(QString::fromUtf8("buttonEquals"));
        gridLayout->addWidget(buttonEquals, 4, 3);

        buttonAdd = new QPushButton(centralWidget);
        buttonAdd->setObjectName(QString::fromUtf8("buttonAdd"));
        gridLayout->addWidget(buttonAdd, 5, 3);

        Calculator->setCentralWidget(centralWidget);

        retranslateUi(Calculator);
    }

    void retranslateUi(QMainWindow *Calculator)
    {
        Calculator->setWindowTitle(QCoreApplication::translate("Calculator", "Calculator", nullptr));
        button7->setText(QCoreApplication::translate("Calculator", "7", nullptr));
        button8->setText(QCoreApplication::translate("Calculator", "8", nullptr));
        button9->setText(QCoreApplication::translate("Calculator", "9", nullptr));
        buttonDivide->setText(QCoreApplication::translate("Calculator", "/", nullptr));
        button4->setText(QCoreApplication::translate("Calculator", "4", nullptr));
        button5->setText(QCoreApplication::translate("Calculator", "5", nullptr));
        button6->setText(QCoreApplication::translate("Calculator", "6", nullptr));
        buttonMultiply->setText(QCoreApplication::translate("Calculator", "*", nullptr));
        button1->setText(QCoreApplication::translate("Calculator", "1", nullptr));
        button2->setText(QCoreApplication::translate("Calculator", "2", nullptr));
        button3->setText(QCoreApplication::translate("Calculator", "3", nullptr));
        buttonSubtract->setText(QCoreApplication::translate("Calculator", "-", nullptr));
        button0->setText(QCoreApplication::translate("Calculator", "0", nullptr));
        buttonClear->setText(QCoreApplication::translate("Calculator", "Clear", nullptr));
        buttonEquals->setText(QCoreApplication::translate("Calculator", "=", nullptr));
        buttonAdd->setText(QCoreApplication::translate("Calculator", "+", nullptr));
    }
};

namespace Ui {
    class Calculator: public Ui_Calculator {};
}

QT_END_NAMESPACE

#endif // UI_CALCULATOR_H