#ifndef RBTREE_VISUALIZER_H
#define RBTREE_VISUALIZER_H

#include <rbtree.h>

/* 기본 SVG 생성*/
void rbtree_to_svg(const node_t *root, const node_t *nil, const char *filename);


/* 상세정보(포인터) SVG 생성*/
void rbtree_to_svg_specific(const node_t *root, const node_t *nil, const char *filename);

/* 트리 세로로 출력 */
void print_tree_vertical(const node_t *node, const node_t *nil);
/* 트리 가로로 출력 */
void print_tree_horizontal(const node_t *node, const node_t *nil, int space);

/* 디렉터리 생성 (mkdir -p 동작) */
static void ensure_dir(const char *filename);

#endif