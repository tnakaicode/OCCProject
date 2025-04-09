#include "calculator.h"
#include "exprtk.hpp" // exprtk ライブラリをインクルード
#include "ui_calculator.h"
#include <QDebug>
#include <QGridLayout>
#include <QDateTime>
#include <QStatusBar> // ステータスバー用
#include <QtMath>     // 数学関数用
#include <QJSEngine>  // 数式評価用
#include <QKeyEvent>  // キーボードイベント用

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

    // ディスプレイにイベントフィルタを設定
    ui->display->installEventFilter(this);

    // 度からラジアンに変換するボタンのシグナルとスロットを接続
    connect(ui->buttonDegToRad, &QPushButton::clicked, this, &Calculator::onDegToRadClicked);

    // 小数点ボタンのシグナルとスロットを接続
    connect(ui->buttonDecimal, &QPushButton::clicked, this, &Calculator::onDecimalClicked);
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::updateTime()
{
    // 現在の時刻を取得して QLabel に表示
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd ddd hh:mm:ss");
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
    // ディスプレイの内容を取得
    QString expression = ui->display->text();
    qDebug() << "Expression:" << expression; // デバッグ用

    // exprtk を使用して数式を評価
    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double> expression_t;
    typedef exprtk::parser<double> parser_t;

    symbol_table_t symbol_table;
    double dummy = 0.0; // ダミー変数を登録（必要に応じて）
    symbol_table.add_variable("dummy", dummy);
    symbol_table.add_constants(); // 定数 (pi, e など) を追加
    qDebug() << "Symbol table initialized";

    expression_t expr;
    expr.register_symbol_table(symbol_table);
    qDebug() << "Expression registered";

    parser_t parser;
    if (!parser.compile(expression.toUtf8().constData(), expr))
    {
        // 数式が無効な場合はエラーを表示
        qDebug() << "Error: Invalid expression";
        qDebug() << "Parser error: " << QString::fromStdString(parser.error());
        ui->display->setText("Error");
        return;
    }

    qDebug() << "Parser succeeded";

    // 数式を評価して結果を取得
    double result = expr.value();
    qDebug() << "Result:" << result;

    // 結果をディスプレイに表示
    ui->display->setText(QString::number(result));
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
    insertTextAtCursor("sin(");
}

void Calculator::onCosClicked()
{
    insertTextAtCursor("cos(");
}

void Calculator::onTanClicked()
{
    insertTextAtCursor("tan(");
}

void Calculator::onAsinClicked()
{
    insertTextAtCursor("asin(");
}

void Calculator::onAcosClicked()
{
    insertTextAtCursor("acos(");
}

void Calculator::onAtanClicked()
{
    insertTextAtCursor("atan(");
}

void Calculator::onExpClicked()
{
    insertTextAtCursor("exp(");
}

void Calculator::onLogClicked()
{
    insertTextAtCursor("log(");
}

void Calculator::onLog10Clicked()
{
    insertTextAtCursor("log10(");
}

void Calculator::onSqrtClicked()
{
    insertTextAtCursor("sqrt(");
}

void Calculator::onAbsClicked()
{
    insertTextAtCursor("abs(");
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
    // 選択範囲をカッコで囲む
    QLineEdit *display = ui->display;
    QString selectedText = display->selectedText();
    if (!selectedText.isEmpty())
    {
        insertTextAtCursor("(" + selectedText + ")");
    }
    else
    {
        insertTextAtCursor("(");
    }
}

void Calculator::onRightParenClicked()
{
    insertTextAtCursor(")");
}

void Calculator::insertTextAtCursor(const QString &text)
{
    QLineEdit *display = ui->display;
    int cursorPosition = display->cursorPosition();

    // 選択範囲を取得
    QString selectedText = display->selectedText();
    if (!selectedText.isEmpty())
    {
        // 選択範囲を置き換える
        QString currentText = display->text();
        int selectionStart = display->selectionStart();
        int selectionEnd = selectionStart + selectedText.length();
        QString newText = currentText.left(selectionStart) + text + currentText.mid(selectionEnd);
        display->setText(newText);
        display->setCursorPosition(selectionStart + text.length());
    }
    else
    {
        // カーソル位置に挿入
        QString currentText = display->text();
        QString newText = currentText.left(cursorPosition) + text + currentText.mid(cursorPosition);
        display->setText(newText);
        display->setCursorPosition(cursorPosition + text.length());
    }
}

// イベントフィルタをオーバーライド
bool Calculator::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->display && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // 許可するキーを定義
        QString allowedKeys = "0123456789+-*/().abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        if (allowedKeys.contains(keyEvent->text()) ||
            keyEvent->key() == Qt::Key_Backspace ||
            keyEvent->key() == Qt::Key_Delete ||
            keyEvent->key() == Qt::Key_Space)
        {
            return false; // 通常の処理を続行
        }
        else
        {
            return true; // 不正なキー入力を無効化
        }
    }

    // 他のイベントはデフォルト処理
    return QMainWindow::eventFilter(obj, event);
}

void Calculator::onDegToRadClicked()
{
    // ディスプレイの内容を取得
    QString expression = ui->display->text();
    bool ok;
    double degreeValue = expression.toDouble(&ok);

    if (ok)
    {
        // 度をラジアンに変換
        double radianValue = qDegreesToRadians(degreeValue);
        ui->display->setText(QString::number(radianValue));
    }
    else
    {
        // エラー処理
        ui->display->setText("Error");
    }
}

void Calculator::onDecimalClicked()
{
    // ディスプレイに小数点を追加
    insertTextAtCursor(".");
}