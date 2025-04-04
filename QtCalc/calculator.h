#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow> // QMainWindow をインクルード
#include <QJSEngine>   // 数式評価用
#include <QJSValue>    // 数式評価結果

QT_BEGIN_NAMESPACE
namespace Ui {
class Calculator;
}
QT_END_NAMESPACE

class Calculator : public QMainWindow { // QMainWindow を継承
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void onButtonClicked();   // 数字や演算子ボタンが押されたとき
    void onEqualsClicked();   // "=" ボタンが押されたとき
    void onClearClicked();    // "Clear" ボタンが押されたとき

private:
    Ui::Calculator *ui;       // 自動生成された UI クラスのポインタ
};

#endif // CALCULATOR_H