#include "calculator.h"
#include "ui_calculator.h"
#include <QDebug>
#include <QGridLayout>
#include <QDateTime>
#include <QStatusBar> // ステータスバー用

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