#include <iostream>
#include "PointCloudProcessor.h"

int main() {
    std::cout << "Starting Point Cloud Processing..." << std::endl;

    // 点群処理クラスのインスタンスを作成
    PointCloudProcessor processor;

    // サンプルの点群データを処理
    processor.loadPointCloud("sample.pcd");
    processor.processPointCloud();
    processor.savePointCloud("output.pcd");

    std::cout << "Point Cloud Processing Completed." << std::endl;
    return 0;
}