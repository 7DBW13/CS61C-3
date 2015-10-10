// CS 61C Fall 2014 Project 3

#include <math.h>
// include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif

#include "calcDepthOptimized.h"
#include "calcDepthNaive.h"

float displacement(int dx, int dy) {
    float squaredDisplacement = dx * dx + dy * dy;
    float displacement = sqrt(squaredDisplacement);
    return displacement;
}

float sqsum(int x, int y) {
    return (x * x + y * y) / 2;
}

void calcDepthOptimized(float *depth, float *left, float *right, int imageWidth, int imageHeight, int featureWidth, int featureHeight, int maximumDisplacement)
{
    for (int i = 0; i < imageWidth * imageHeight; i++) {
        depth[i] = 0;
    }

    if (maximumDisplacement == 0) {
        return;
    }

    int row_index;
    for (int x = 0; x < imageWidth; x++)  {
        for (int y = 0; y < imageHeight; y++) {
            row_index = y * imageWidth;
            if ((y < featureHeight) || (y >= imageHeight - featureHeight) ||
                    (x < featureWidth) || (x >= imageWidth - featureWidth)) {
                depth[row_index + x] = 0;
                continue;
            }

            int minimumDy = 0;
            int minimumDx = 0;
            float minimumSquaredDifference = -1;

            for (int dx = -maximumDisplacement; dx <= maximumDisplacement; dx++) {
                for (int dy = -maximumDisplacement; dy <= maximumDisplacement; dy++) {
                    int y_plus_dy = y + dy;
                    int x_plus_dx = x + dx;
                    if (y_plus_dy < featureHeight || y_plus_dy >= imageHeight - featureHeight ||
                            x_plus_dx < featureWidth || x_plus_dx >= imageWidth - featureWidth) {
                        continue;
                    }

                    float squaredDifference;
                    float sq_diff[4];
                    __m128 sq_diff_reg = _mm_setzero_ps();
                    int boxX = -featureWidth;
                    while (boxX <= featureWidth-3) {
                        int leftX = x + boxX;
                        int rightX = x + boxX + dx;
                        for (int boxY = -featureWidth; boxY <= featureWidth; boxY++) {
                            int leftY = y + boxY;
                            int rightY = leftY + dy;
                            int l_row_index = leftY * imageWidth;
                            int r_row_index = rightY * imageWidth;
                            __m128 left_four = _mm_loadu_ps(left + l_row_index + leftX);
                            __m128 rite_four = _mm_loadu_ps(right + r_row_index + rightX);
                            __m128 diff = _mm_sub_ps(left_four, rite_four);
                            sq_diff_reg = _mm_add_ps(sq_diff_reg, _mm_mul_ps(diff, diff));
                        }
                        boxX += 4;
                    }

                    sq_diff_reg = _mm_hadd_ps(sq_diff_reg, sq_diff_reg);
                    sq_diff_reg = _mm_hadd_ps(sq_diff_reg, sq_diff_reg);
                    _mm_store_ps(sq_diff, sq_diff_reg);
                    squaredDifference = sq_diff[0];

                    if (minimumSquaredDifference != -1 &&  squaredDifference > minimumSquaredDifference) {
                        continue;
                    } else if (!(featureWidth & 1)) {
                        float stored = 0;
                        int leftX = x + featureWidth;
                        for (int boxY = -featureWidth; boxY <= featureWidth; boxY++) {
                            int leftY = y + boxY;
                            int rightY = leftY + dy;
                            int l_row_index = leftY * imageWidth;
                            int r_row_index = rightY * imageWidth;
                            float difference = left[l_row_index + leftX] - right[r_row_index + leftX + dx];
                            stored += difference * difference;
                        }
                        squaredDifference += stored;

                    } else {
                        __m128 stored = _mm_setzero_ps();
                        float store_fl[4];
                        int leftX = x + featureWidth - 3;
                        int rightX = dx + leftX;
                        for (int boxY = -featureHeight; boxY <= featureHeight; boxY++) {
                            int leftY = y + boxY;
                            int rightY = dy + leftY;

                            __m128 left_four = _mm_loadu_ps(left + leftY * imageWidth + leftX);
                            __m128 rite_four = _mm_loadu_ps(right + rightY * imageWidth + rightX);
                            __m128 diff = _mm_sub_ps(left_four, rite_four);
                            __m128 remainder = _mm_set_ps(1.0, 1.0, 1.0, 0.0);
                            diff = _mm_mul_ps(diff, remainder);
                            stored = _mm_add_ps(stored, _mm_mul_ps(diff, diff));
                        }
                        _mm_store_ps(store_fl, stored);
                        squaredDifference += store_fl[0];
                        squaredDifference += store_fl[1];
                        squaredDifference += store_fl[2];
                        squaredDifference += store_fl[3];
                    }

                    if (((minimumSquaredDifference == squaredDifference) && sqsum(dx, dy) < sqsum(minimumDx, minimumDy)) || minimumSquaredDifference < 0 ||
                            minimumSquaredDifference > squaredDifference) {
                        minimumDx = dx;
                        minimumDy = dy;
                        minimumSquaredDifference = squaredDifference;
                    }
                }
            }
            depth[row_index + x] = displacement(minimumDx, minimumDy);
        }
    }
}
