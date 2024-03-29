#include "SAHI.h"

SAHI::SAHI(int slice_height, int slice_width, float overlap_height_ratio, float overlap_width_ratio)
    : slice_height_(slice_height), slice_width_(slice_width),
      overlap_height_ratio_(overlap_height_ratio), overlap_width_ratio_(overlap_width_ratio) {}

std::vector<std::pair<cv::Rect, int>> SAHI::calculateSliceRegions(int image_height, int image_width) {
    int step_height = slice_height_ - static_cast<int>(slice_height_ * overlap_height_ratio_);
    int step_width = slice_width_ - static_cast<int>(slice_width_ * overlap_width_ratio_);

    int y_max = image_height - slice_height_;
    int x_max = image_width - slice_width_;

    // Estimate vector size to avoid reallocations
    int num_rows = (y_max / step_height) + 1;
    int num_cols = (x_max / step_width) + 1;
    std::vector<std::pair<cv::Rect, int>> regions;
    regions.reserve(num_rows * num_cols);

    int index = 0;

    image_width_ = image_width;
    image_height_ = image_height;

    for (int y = 0; y < image_height; y += step_height) {
        for (int x = 0; x < image_width; x += step_width) {
            int width = slice_width_;
            int height = slice_height_;

            int temp_x = x;
            int temp_y = y;

            // Adjust width and height for slices at the edges of the image
            if (x + width > image_width) temp_x -= (x + width) - image_width;
            if (y + height > image_height) temp_y -= (y + height) - image_height;

            regions.emplace_back(cv::Rect(temp_x, temp_y, width, height), index++);
        }
    }
    return regions;
}

BoundingBox SAHI::mapToOriginal(const BoundingBox& boundingBox, const cv::Rect& sliceRegion) {
    cv::Rect_<float> newRect = boundingBox.rect;
    newRect.x += sliceRegion.x;
    newRect.y += sliceRegion.y;

    return {boundingBox.label, boundingBox.probability, newRect};
}


