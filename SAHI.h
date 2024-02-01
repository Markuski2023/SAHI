#ifndef SAHI_H
#define SAHI_H
#include <vector>
#include <opencv2/opencv.hpp>

struct BoundingBox {
    float x, y, w, h, score;
    int sliceIndex;

    BoundingBox(float x, float y, float w, float h, float score, int sliceIndex)
        : x(x), y(y), w(w), h(h), score(score), sliceIndex(sliceIndex) {}

};

class SAHI {
public:
    SAHI(int slice_height, int slice_width, float overlap_height_ratio, float overlap_width_ratio);
    std::vector<std::pair<cv::Rect, int>> calculateSliceRegions(int image_height, int image_width);
    void slice(const cv::Mat& image, const std::function<void(const cv::Mat&, int)>& processSlice);
    float IoU(const BoundingBox& a, const BoundingBox& b);
    cv::Rect mapToOriginal(const BoundingBox& boundingBox, const cv::Rect& sliceRegion);
    std::vector<BoundingBox> nonMaximumSuppression(std::vector<BoundingBox>& boxes, float iouThreshold);

private:
    int slice_height_, slice_width_;
    int image_height_, image_width_;
    float overlap_height_ratio_;
    float overlap_width_ratio_;

};



#endif //SAHI_H