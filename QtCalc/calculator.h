#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>

class Calculator : public QWidget
{
Q_OBJECT // このマクロが必要です

    public : Calculator(QWidget *parent = nullptr);

private slots:
    void onButtonClicked();
    void clearDisplay(); // Clearボタン用のスロット

private:
    QLineEdit *display;
    QGridLayout *layout;
    QPushButton *createButton(const QString &text);
};

#endif // CALCULATOR_H