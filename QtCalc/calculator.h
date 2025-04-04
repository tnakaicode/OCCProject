#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow> // QMainWindow をインクルード
#include <QJSEngine>   // 数式評価用
#include <QJSValue>    // 数式評価結果

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Calculator;
}
QT_END_NAMESPACE

class Calculator : public QWidget
{ // QMainWindow → QWidget に変更
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
    // void on_button3_clicked();
    // void on_button4_clicked();
    // void on_button5_clicked();
    // void on_button6_clicked();
    // void on_button7_clicked();
    // void on_button8_clicked();
    // void on_button9_clicked();
    // void on_buttonAdd_clicked();
    // void on_buttonSubtract_clicked();
    // void on_buttonMultiply_clicked();
    // void on_buttonDivide_clicked();
    // void on_buttonEquals_clicked();
    // void on_buttonClear_clicked();

private:
    Ui::Calculator *ui; // 自動生成された UI クラスのポインタ
};

#endif // CALCULATOR_H