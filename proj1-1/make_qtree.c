/*
 * PROJ1-1: YOUR TASK B CODE HERE
 *
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

#define ABS(x) (((x) < 0) ? (-(x)) : (x))

/* return the index of (x, y) in the depth_map. */
qNode *convert(unsigned char *depth_map, int map_width, int x, int y, int section_width);

int idx(int x, int y, int map_width) {
    return y * map_width + x;
}

int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width) {

    /* YOUR CODE HERE */
    int curr = depth_map[idx(x, y, map_width)];
    int i, j;
    for (i = x; i < x + section_width; i++) {
      for (j = y; j < y + section_width; j++) {
        if (depth_map[idx(i, j, map_width)] != curr) {
          return 256;
        }
      }
    }
    return curr;
}

qNode *depth_to_quad(unsigned char *depth_map, int map_width) {

    /* YOUR CODE HERE */
    return convert(depth_map, map_width, 0, 0, map_width);
}

/* recursively converting depth to quad. */
qNode *convert(unsigned char *depth_map, int map_width, int x, int y, int section_width) {
    int half = section_width / 2; // the width of its child.
    qNode *me = malloc(sizeof(qNode));
    if (me == NULL) {
      allocation_failed();
    }
    me->size = section_width;
    me->x = x;
    me->y = y;
    if (homogenous(depth_map, map_width, x, y, section_width) != 256) {
      me->leaf = 1;
      me->gray_value = depth_map[idx(x, y, map_width)];
      me->child_NW = NULL;
      me->child_NE = NULL;
      me->child_SE = NULL;
      me->child_SW = NULL;
    } else {
      me->leaf = 0;
      me->gray_value = 256;
      me->child_NW = convert(depth_map, map_width, x, y, half);
      me->child_NE = convert(depth_map, map_width, x + half, y, half);
      me->child_SE = convert(depth_map, map_width, x + half, y + half, half);
      me->child_SW = convert(depth_map, map_width, x, y + half, half);
    }
    return me;
}


void free_qtree(qNode *qtree_node) {
    if(qtree_node) {
        if(!qtree_node->leaf){
            free_qtree(qtree_node->child_NW);
            free_qtree(qtree_node->child_NE);
            free_qtree(qtree_node->child_SE);
            free_qtree(qtree_node->child_SW);
        }
        free(qtree_node);
    }
}

