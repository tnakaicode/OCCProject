#include "calculator.h"
#include "ui_calculator.h"
#include <QDebug>
#include <QGridLayout>
#include <QDateTime>
#include <QStatusBar> // ステータスバー用
#include <QtMath>     // 数学関数用
#include <QJSEngine>  // 数式評価用

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Calculator)
{
    // QWidget を QMainWindow の中央に埋め込む
    QWidget *centralWidget = new QWidget(this);
    ui->setupUi(centralWidget);
    setCentralWidget(centralWidget);

    // ステータスバーに時刻表示用の QLabel を追加
    timeLabel = new QLabel(this);
    statusBar()->addPermanentWidget(timeLabel);

    // QTimer を設定して 0.1 秒ごとに時刻を更新
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Calculator::updateTime);
    timer->start(200); // 0.1 秒ごとにタイマーを発火

    // 初期の時刻を表示
    updateTime();

    // 親ウィジェットにレイアウトを適用
    if (!this->layout())
    {
        QGridLayout *layout = new QGridLayout(this);
        this->setLayout(layout);
    }

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

    // 初等関数ボタンのシグナルとスロットを接続
    connect(ui->buttonSin, &QPushButton::clicked, this, &Calculator::onSinClicked);
    connect(ui->buttonCos, &QPushButton::clicked, this, &Calculator::onCosClicked);
    connect(ui->buttonTan, &QPushButton::clicked, this, &Calculator::onTanClicked);
    connect(ui->buttonAsin, &QPushButton::clicked, this, &Calculator::onAsinClicked);
    connect(ui->buttonAcos, &QPushButton::clicked, this, &Calculator::onAcosClicked);
    connect(ui->buttonAtan, &QPushButton::clicked, this, &Calculator::onAtanClicked);
    connect(ui->buttonExp, &QPushButton::clicked, this, &Calculator::onExpClicked);
    connect(ui->buttonLog, &QPushButton::clicked, this, &Calculator::onLogClicked);
    connect(ui->buttonLog10, &QPushButton::clicked, this, &Calculator::onLog10Clicked);
    connect(ui->buttonSqrt, &QPushButton::clicked, this, &Calculator::onSqrtClicked);
    connect(ui->buttonAbs, &QPushButton::clicked, this, &Calculator::onAbsClicked);

    // カッコボタンのシグナルとスロットを接続
    connect(ui->buttonLeftParen, &QPushButton::clicked, this, &Calculator::onLeftParenClicked);
    connect(ui->buttonRightParen, &QPushButton::clicked, this, &Calculator::onRightParenClicked);
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::updateTime()
{
    // 現在の時刻を取得して QLabel に表示
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd ddd hh:mm:ss.zzz");
    timeLabel->setText(currentTime);
}

void Calculator::onButtonClicked()
{
    // ボタンのテキストを取得してディスプレイに追加
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button)
        return;

    QString buttonText = button->text();
    ui->display->setText(ui->display->text() + buttonText);
}

void Calculator::onEqualsClicked()
{
    // ディスプレイの内容を評価して結果を表示
    QString expression = ui->display->text();
    qDebug() << "Expression:" << expression; // デバッグ用

    // 簡易的な数式評価 (QJSEngine を使用)
    QJSEngine engine;

    // JavaScriptエンジンに数学関数を登録
    engine.globalObject().setProperty("sin", engine.evaluate("Math.sin"));
    engine.globalObject().setProperty("cos", engine.evaluate("Math.cos"));
    engine.globalObject().setProperty("tan", engine.evaluate("Math.tan"));
    engine.globalObject().setProperty("asin", engine.evaluate("Math.asin"));
    engine.globalObject().setProperty("acos", engine.evaluate("Math.acos"));
    engine.globalObject().setProperty("atan", engine.evaluate("Math.atan"));
    engine.globalObject().setProperty("exp", engine.evaluate("Math.exp"));
    engine.globalObject().setProperty("log", engine.evaluate("Math.log"));
    engine.globalObject().setProperty("log10", engine.evaluate("Math.log10"));
    engine.globalObject().setProperty("sqrt", engine.evaluate("Math.sqrt"));
    engine.globalObject().setProperty("abs", engine.evaluate("Math.abs"));

    QJSValue result = engine.evaluate(expression);

    if (result.isError())
    {
        qDebug() << "Error: Invalid expression";
        ui->display->setText("Error");
    }
    else
    {
        qDebug() << "Result:" << result.toNumber();
        ui->display->setText(QString::number(result.toNumber()));
    }
}

void Calculator::onClearClicked()
{
    // ディスプレイをクリア
    ui->display->clear();
}

void Calculator::on_button0_clicked()
{
    ui->display->setText(ui->display->text() + "0");
}

void Calculator::on_button1_clicked()
{
    ui->display->setText(ui->display->text() + "1");
}

void Calculator::on_button2_clicked()
{
    ui->display->setText(ui->display->text() + "2");
}

// 他のボタンのスロットも同様に実装

void Calculator::onSinClicked()
{
    ui->display->setText(ui->display->text() + "sin(");
}

void Calculator::onCosClicked()
{
    ui->display->setText(ui->display->text() + "cos(");
}

void Calculator::onTanClicked()
{
    ui->display->setText(ui->display->text() + "tan(");
}

void Calculator::onAsinClicked()
{
    ui->display->setText(ui->display->text() + "asin(");
}

void Calculator::onAcosClicked()
{
    ui->display->setText(ui->display->text() + "acos(");
}

void Calculator::onAtanClicked()
{
    ui->display->setText(ui->display->text() + "atan(");
}

void Calculator::onExpClicked()
{
    ui->display->setText(ui->display->text() + "exp(");
}

void Calculator::onLogClicked()
{
    ui->display->setText(ui->display->text() + "log(");
}

void Calculator::onLog10Clicked()
{
    ui->display->setText(ui->display->text() + "log10(");
}

void Calculator::onSqrtClicked()
{
    ui->display->setText(ui->display->text() + "sqrt(");
}

void Calculator::onAbsClicked()
{
    ui->display->setText(ui->display->text() + "abs(");
}

void Calculator::calculateUnaryFunction(std::function<double(double)> func)
{
    QString expression = ui->display->text();
    bool ok;
    double value = expression.toDouble(&ok);

    if (ok)
    {
        double result = func(value);
        ui->display->setText(QString::number(result));
    }
    else
    {
        ui->display->setText("Error");
    }
}

void Calculator::onLeftParenClicked()
{
    // ディスプレイに "(" を追加
    ui->display->setText(ui->display->text() + "(");
}

void Calculator::onRightParenClicked()
{
    // ディスプレイに ")" を追加
    ui->display->setText(ui->display->text() + ")");
}