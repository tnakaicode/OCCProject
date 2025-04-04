#include "calculator.h"

Calculator::Calculator(QWidget *parent) : QWidget(parent) {
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);

    layout = new QGridLayout(this);
    layout->addWidget(display, 0, 0, 1, 4);

    QStringList buttons = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", ".", "=", "+"
    };

    int row = 1, col = 0;
    for (const QString &text : buttons) {
        QPushButton *button = createButton(text);
        layout->addWidget(button, row, col);
        col++;
        if (col == 4) {
            col = 0;
            row++;
        }
    }

    setLayout(layout);
}

QPushButton *Calculator::createButton(const QString &text) {
    QPushButton *button = new QPushButton(text, this);
    connect(button, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    return button;
}

void Calculator::onButtonClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    QString buttonText = button->text();
    if (buttonText == "=") {
        // Evaluate the expression
        QString expression = display->text();
        double result = 0;
        try {
            result = expression.toDouble(); // Simplified for demonstration
        } catch (...) {
            display->setText("Error");
            return;
        }
        display->setText(QString::number(result));
    } else {
        display->setText(display->text() + buttonText);
    }
}