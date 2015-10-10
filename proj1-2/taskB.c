typedef struct quadtree {
  int leaf;
  int size;
  int x;
  int y;
  int gray_value;
  qNode *child_NW, *child_NE, *child_SE, *child_SW;
} qNode

void fill(qNode *root, int *matrix, int width) {
  traverse(root, matrix, width);
}

void traverse(qNode *node, int *matrix, int width) {
  if (node->gray_vale != 256) {
    for (int i = node->x; i < node->x + node->size; i++) {
      for (int j = node->y; j < node->y + node->size; j++) {
        matrix[j * width + i] = node->gray_value;
      }
    }
  } else {
    traverse(node->child_NW);
    traverse(node->child_NE);
    traverse(node->child_SE);
    traverse(node->child_SW);
  }
}
