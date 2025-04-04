#include "calculator.h"
#include "ui_calculator.h" // 自動生成されたヘッダーファイルをインクルード
#include <QDebug>          // デバッグ用

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Calculator) { // QMainWindow を初期化
    ui->setupUi(this); // UI を初期化

    // ボタンのシグナルとスロットを接続
    connect(ui->button0, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->button1, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->button2, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->button3, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->button4, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->button5, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->button6, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->button7, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->button8, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->button9, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->buttonAdd, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->buttonSubtract, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->buttonMultiply, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->buttonDivide, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    connect(ui->buttonEquals, &QPushButton::clicked, this, &Calculator::onEqualsClicked);
    connect(ui->buttonClear, &QPushButton::clicked, this, &Calculator::onClearClicked);
}

Calculator::~Calculator() {
    delete ui; // メモリを解放
}

void Calculator::onButtonClicked() {
    // ボタンのテキストを取得してディスプレイに追加
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    QString buttonText = button->text();
    ui->display->setText(ui->display->text() + buttonText);
}

void Calculator::onEqualsClicked() {
    // ディスプレイの内容を評価して結果を表示
    QString expression = ui->display->text();
    qDebug() << "Expression:" << expression; // デバッグ用

    // 簡易的な数式評価 (QJSEngine を使用)
    QJSEngine engine;
    QJSValue result = engine.evaluate(expression);

    if (result.isError()) {
        qDebug() << "Error: Invalid expression";
        ui->display->setText("Error");
    } else {
        qDebug() << "Result:" << result.toNumber();
        ui->display->setText(QString::number(result.toNumber()));
    }
}

void Calculator::onClearClicked() {
    // ディスプレイをクリア
    ui->display->clear();
}