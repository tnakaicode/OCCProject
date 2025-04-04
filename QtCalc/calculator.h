#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QJSEngine> // 数式評価用
#include <QJSValue>  // 数式評価結果
#include <QTimer>
#include <QLabel>
#include <functional> // std::function を使用

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Calculator;
}
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void onButtonClicked(); // 数字や演算子ボタンが押されたとき
    void onEqualsClicked(); // "=" ボタンが押されたとき
    void onClearClicked();  // "Clear" ボタンが押されたとき
    void on_button0_clicked();
    void on_button1_clicked();
    void on_button2_clicked();
    void updateTime(); // 時刻を更新するスロット
    void onSinClicked();
    void onCosClicked();
    void onTanClicked();
    void onAsinClicked();
    void onAcosClicked();
    void onAtanClicked();
    void onExpClicked();
    void onLogClicked();
    void onLog10Clicked();
    void onSqrtClicked();
    void onAbsClicked();

private:
    Ui::Calculator *ui;
    QLabel *timeLabel; // ステータスバーに表示する時刻用ラベル
    QTimer *timer;     // 時刻更新用タイマー
    void calculateUnaryFunction(std::function<double(double)> func);
};

#endif // CALCULATOR_H