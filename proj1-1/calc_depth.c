/*
 * PROJ1-1: YOUR TASK A CODE HERE
 *
 * Feel free to define additional helper functions.
 */

#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
        int maximum_displacement) {

    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement) / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;

}

// helper function definitions here.
int get_row(int index, int width);
int get_col(int index, int height);
int in_image(int row, int col, int width, int height);
int idx(int x, int y, int width);
int abs(int x);

// abbreviate long names.
#define maxd maximum_displacement

void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {

    /* YOUR CODE HERE */
    int i; // the index in the left image.
    for (i = 0; i < image_width * image_height; i++) {
      depth_map[i] = 0;
    }
    if (!maxd) {
      return;
    }

    int row, col; // the row and column number of the px in the left image.
    int j, k; // j, k are row/col position of the center of current search.
    int p, q; // relative location of pixel being compared.
    unsigned int sum = 0; // the sum of each iteration
    unsigned int currmin;
    int minj = 0, mink = 0;
    int tmpidx;
    int iidx;
    
    // start iteration for pixel in the left image.
    for (i = 0; i < image_width * image_height; i++) {
      row = get_row(i, image_width);
      col = get_col(i, image_width);
      if (row - feature_height < 0 ||
          row + feature_height >= image_height ||
          col - feature_width < 0 || col + feature_width >= image_width) {
        // if Pixels on the edge of the image, whose left-image features don't 
        // fit inside the image, should have a distance of 0 (infinity).
        depth_map[i] = 0;
      } else {
        // now we really start our search!
        currmin = 0xFFFFFFFF;
        minj = row;
        mink = col;
        for (j = row - maxd; j <= row + maxd; j++) {
          for (k = col - maxd; k <= col + maxd; k++) {
            // we ignore the case if a right feature is out.
            if (j - feature_height >= 0 && j + feature_height < image_height &&
                k - feature_width >= 0 && k + feature_width < image_width) {
              sum = 0;
              for (p = j - feature_height; p <= j + feature_height; p++) {
                for (q = k - feature_width; q <= k + feature_width; q++) {
                  tmpidx = idx(p, q, image_width);
                  iidx = idx(p + row - j, q + col - k, image_width);
                  sum += (left[iidx] - right[tmpidx]) *
                    (left[iidx] - right[tmpidx]);
                }
              }
              if (sum < currmin ||
                  (sum == currmin && pow((j-row), 2) + pow((k-col), 2) <
                   pow((minj-row), 2) + pow((mink-col), 2))) {
                currmin = sum;
                minj = j;
                mink = k;
              }
            }
          }
        }
      depth_map[i] = normalized_displacement(
          abs(mink-col), abs(minj-row), maxd);
      }
    }
}

/* get the row number of a pixel with given index and width. */
int get_row(int index, int width) {
  return index / width;
}

/* get the column number of a pixel with given index and width. */
int get_col(int index, int width) {
  return index % width;
}

/* return 1 iff a pixel is in an image. */
int in_image(int row, int col, int width, int height) {
  return row >= 0 && col >= 0 && row < height && col < width;
}

/* return the index of a given point (p, q) p:row q:col. */
int idx(int p, int q, int width) {
  return p * width + q;
}

/* return the absolute value of x */
int abs(int x) {
  if (x > 0) {
    return x;
  }
  return -x;
}
