#ifndef POINTCLOUDPROCESSOR_H
#define POINTCLOUDPROCESSOR_H

#include <string>

class PointCloudProcessor {
public:
    PointCloudProcessor();
    ~PointCloudProcessor();

    void loadPointCloud(const std::string& filename);
    void processPointCloud();
    void savePointCloud(const std::string& filename);
};

#endif // POINTCLOUDPROCESSOR_H