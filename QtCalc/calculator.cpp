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

    // Clearボタンを追加
    QPushButton *clearButton = createButton("Clear");
    layout->addWidget(clearButton, row, 0, 1, 4); // 最後の行に配置
    connect(clearButton, &QPushButton::clicked, this, &Calculator::clearDisplay);

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
        bool ok;
        double result = expression.toDouble(&ok); // 簡易的な評価
        if (ok) {
            display->setText(QString::number(result));
        } else {
            display->setText("Error");
        }
    } else {
        display->setText(display->text() + buttonText);
    }
}

void Calculator::clearDisplay() {
    display->clear(); // ディスプレイをクリア
}