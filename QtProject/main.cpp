#include <QApplication>
#include <QWidget>
#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << argc << std::endl;
    std::cout << argv << std::endl;
    for (int i = 0; i < 5; ++i) {
        std::cout << i << std::endl;
    }

    // Qtアプリケーションの初期化
    QApplication app(argc, argv);

    // シンプルなウィジェットを作成して表示
    QWidget widget;
    widget.resize(800, 600); // ウィジェットのサイズを設定
    widget.setWindowTitle("Qt Project Example"); // ウィンドウタイトルを設定
    widget.show(); // ウィジェットを表示

    // イベントループを開始
    return app.exec();
}