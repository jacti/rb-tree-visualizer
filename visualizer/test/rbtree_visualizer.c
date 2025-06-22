#include "rbtree_visualizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 색깔 코드
#define ANSI_RED     "\x1b[31m" //빨
#define ANSI_WHITE   "\x1b[37m" //흰
#define ANSI_RESET   "\x1b[0m" //디폴트 색
#define ANSI_BOLD    "\x1b[1m" //굵은 폰트

// 이미지 크기
#define MIN_WIDTH           300
#define NIL_TABLE_WIDTH     180
#define NIL_TABLE_HEIGHT    80
#define NIL_TABLE_PADDING    20

// 트리 높이 구하기
int get_tree_height(const node_t *node, const node_t *nil) {
    if (node == nil) return 0;
    int left_height = get_tree_height(node->left, nil);
    int right_height = get_tree_height(node->right, nil);
    return 1 + (left_height > right_height ? left_height : right_height);
}
// NIL 정보 11시 방향에 표시
void draw_nil_info_svg(FILE *f, const node_t *nil, int x, int y) {

    fprintf(f, "  <!-- NIL Node Information -->\n");
    fprintf(f, "  <rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"lightgray\" "
               "stroke=\"black\" stroke-width=\"1\" rx=\"5\" />\n", NIL_TABLE_PADDING, NIL_TABLE_PADDING, NIL_TABLE_WIDTH, NIL_TABLE_HEIGHT);
    
    if(nil == NULL){
        fprintf(f, "  <text x=\"%d\" y=\"%d\" font-size=\"14px\" font-weight=\"bold\" fill=\"black\">Do not use NIL ptr</text>\n",
            x + 10, y + 20);
        return;
    }
    fprintf(f, "  <text x=\"%d\" y=\"%d\" font-size=\"14px\" font-weight=\"bold\" fill=\"black\">NIL Node Info:</text>\n",
            x + 10, y + 20);
    char *font_color = nil != nil->parent ? "red" : "black";
    fprintf(f, "  <text x=\"%d\" y=\"%d\" font-size=\"12px\" fill=\"%s\">Parent: %p</text>\n",
            x + 10, y + 35, font_color, (void*)nil->parent);
    font_color = nil != nil->left ? "red" : "black";
    fprintf(f, "  <text x=\"%d\" y=\"%d\" font-size=\"12px\" fill=\"%s\">Left: %p</text>\n",
            x + 10, y + 50, font_color, (void*)nil->left);
    font_color = nil != nil->right ? "red" : "black";
    fprintf(f, "  <text x=\"%d\" y=\"%d\" font-size=\"12px\" fill=\"%s\">Right: %p</text>\n",
            x + 10, y + 65, font_color, (void*)nil->right);
}

// NIL 그리기
void draw_nil_leaf_svg(FILE *f, int x, int y, int radius) {
    // NIL 리프 노드를 작은 사각형으로 표시
    int nil_size = radius / 2;
    fprintf(f, "  <rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"gray\" "
               "stroke=\"black\" stroke-width=\"1\" />\n",
            x - nil_size, y - nil_size, nil_size * 2, nil_size * 2);

    #ifndef SENTINEL
        char *txt = "NULL";
    #else
        char *txt = "NIL";
    #endif
    fprintf(f, "  <text x=\"%d\" y=\"%d\" text-anchor=\"middle\" dy=\".3em\" font-size=\"%dpx\" "
               "fill=\"white\" font-weight=\"bold\">%s</text>\n",
            x, y, nil_size,txt);
}

void draw_node_svg(FILE *f, const node_t *node, const node_t *nil,
                   int x, int y, int depth, int h_offset,
                   int v_spacing, int radius){
    if (node == nil) return;

    // 왼쪽에 nil 아니면 그리기
    if (node->left != nil) {
        int child_x = x - h_offset;
        int child_y = y + v_spacing;
        fprintf(f, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" "
                   "stroke=\"black\" stroke-width=\"2\" />\n",
                x, y, child_x, child_y);
        draw_node_svg(f, node->left, nil, child_x, child_y, depth + 1, h_offset / 2, v_spacing, radius);
    }
    // nil 그리기
    else {
        int nil_x = x - h_offset;
        int nil_y = y + v_spacing;
        fprintf(f, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" "
                   "stroke=\"gray\" stroke-width=\"1\" stroke-dasharray=\"3,3\" />\n",
                x, y, nil_x, nil_y);
        draw_nil_leaf_svg(f, nil_x, nil_y, radius);
    }
    // 반대도 똑같

    if (node->right != nil) {
        int child_x = x + h_offset;
        int child_y = y + v_spacing;
        fprintf(f, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" "
                   "stroke=\"black\" stroke-width=\"2\" />\n",
                x, y, child_x, child_y);
        draw_node_svg(f, node->right, nil, child_x, child_y, depth + 1, h_offset / 2, v_spacing, radius);
    } else {
        int nil_x = x + h_offset;
        int nil_y = y + v_spacing;
        fprintf(f, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" "
                   "stroke=\"gray\" stroke-width=\"1\" stroke-dasharray=\"3,3\" />\n",
                x, y, nil_x, nil_y);
        draw_nil_leaf_svg(f, nil_x, nil_y, radius);
    }

    // 노드 그리기
    const char *fill_color = (node->color == RBTREE_RED) ? "red" : "black";

    fprintf(f, "  <circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"%s\" stroke=\"black\" stroke-width=\"2\" />\n",
            x, y, radius, fill_color);

    fprintf(f, "  <text x=\"%d\" y=\"%d\" text-anchor=\"middle\" dy=\".3em\" font-size=\"%dpx\" "
               "fill=\"white\" font-weight=\"bold\">%d</text>\n",
            x, y, radius - 2, node->key);
}


void draw_node_detailed_svg(FILE *f, const node_t *node, const node_t *nil,
                            int x, int y, int depth, int h_offset,
                            int v_spacing, int box_width, int box_height) {
    if (node == nil) return;

    // 자식 노드들 먼저
    if (node->left != nil) {
        int child_x = x - h_offset;
        int child_y = y + v_spacing;
        fprintf(f, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" "
                   "stroke=\"black\" stroke-width=\"1\" />\n",
                x, y + box_height/2, child_x, child_y - box_height/2);
        draw_node_detailed_svg(f, node->left, nil, child_x, child_y, depth + 1, h_offset / 2, v_spacing, box_width, box_height);
    }

    if (node->right != nil) {
        int child_x = x + h_offset;
        int child_y = y + v_spacing;
        fprintf(f, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" "
                   "stroke=\"black\" stroke-width=\"1\" />\n",
                x, y + box_height/2, child_x, child_y - box_height/2);
        draw_node_detailed_svg(f, node->right, nil, child_x, child_y, depth + 1, h_offset / 2, v_spacing, box_width, box_height);
    }

    // 노드  그리기
    const char *bg_color = (node->color == RBTREE_RED) ? "#ffe6e6" : "lightgray";

    fprintf(f, "  <rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"%s\" "
               "stroke=\"black\" stroke-width=\"1\" rx=\"5\" />\n",
            x - box_width/2, y - box_height/2, box_width, box_height, bg_color);


    int text_y = y - box_height/2 + 12;

    fprintf(f, "  <text x=\"%d\" y=\"%d\" text-anchor=\"middle\" font-size=\"12px\" "
               "font-weight=\"bold\" fill=\"%s\">Value: %d</text>\n",
            x, text_y, (node->color == RBTREE_RED) ? "#cc0000" : "black", node->key);

    text_y += 12;
    fprintf(f, "  <text x=\"%d\" y=\"%d\" text-anchor=\"middle\" font-size=\"10px\" "
               "fill=\"black\">Node: %p</text>\n",
            x, text_y, (void*)node);

    text_y += 12;
    fprintf(f, "  <text x=\"%d\" y=\"%d\" text-anchor=\"middle\" font-size=\"10px\" "
               "fill=\"black\">Parent: %p</text>\n",
            x, text_y, (void*)node->parent);

    text_y += 12;
    fprintf(f, "  <text x=\"%d\" y=\"%d\" text-anchor=\"middle\" font-size=\"10px\" "
               "fill=\"black\">Left: %p</text>\n",
            x, text_y, (void*)node->left);

    text_y += 12;
    fprintf(f, "  <text x=\"%d\" y=\"%d\" text-anchor=\"middle\" font-size=\"10px\" "
               "fill=\"black\">Right: %p</text>\n",
            x, text_y, (void*)node->right);

}

// 포인터 정보 포함한 거 그리기
void rbtree_to_svg_specific(const node_t *root, const node_t *nil, const char *filename) {
    ensure_dir(filename);

    if (root == nil) {
        fprintf(stderr, "Empty tree, SVG not generated\n");
        return;
    }

    int height = get_tree_height(root, nil);
    int v_spacing = 95;
    int box_width = 130;
    int box_height = 65;
    int width = (1 << height) * box_width + 100;
    int height_px = v_spacing * (height + 1) + 150;

    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("fopen");
        return;
    }

    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n", width, height_px);
    fprintf(f, "  <rect width=\"100%%\" height=\"100%%\" fill=\"white\"/>\n");

    draw_nil_info_svg(f, nil, 20, 20);

    int root_x = width / 2;
    int initial_offset = width / 4;

     int root_y = v_spacing;
    if (root_x < NIL_TABLE_WIDTH + 2 * NIL_TABLE_PADDING){
        root_y += NIL_TABLE_HEIGHT;
    }

    draw_node_detailed_svg(f, root, nil, root_x, root_y, 0, initial_offset, v_spacing, box_width, box_height);

    fprintf(f, "</svg>\n");
    fclose(f);
}

void rbtree_to_svg(const node_t *root, const node_t *nil, const char *filename) {
    ensure_dir(filename);

    if (root == nil) {
        fprintf(stderr, "Empty tree, SVG not generated\n");
        return;
    }

    int height = get_tree_height(root, nil);
    int v_spacing = 80;
    int radius = 20;
    int width = (1 << height) * radius * 2;
    width = MIN_WIDTH > width ? MIN_WIDTH : width;
    int height_px = v_spacing * (height + 2) + 80;

    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("fopen");
        return;
    }

    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n", width, height_px);
    fprintf(f, "  <rect width=\"100%%\" height=\"100%%\" fill=\"white\"/>\n");

    draw_nil_info_svg(f, nil, 20, 20);

    int root_x = width / 2;
    int initial_offset = width / 4;

    int root_y = v_spacing;
    if (root_x < NIL_TABLE_WIDTH + 2 * NIL_TABLE_PADDING){
        root_y += NIL_TABLE_HEIGHT;
    }

    draw_node_svg(f, root, nil, root_x, root_y, 0, initial_offset, v_spacing, radius);

    fprintf(f, "</svg>\n");
    fclose(f);
}

void print_node_color(const node_t *node, const node_t *nil) {
    if (node == nil) {
        printf("  ");
        return;
    }

    if (node->color == RBTREE_RED) {
        printf(ANSI_RED ANSI_BOLD "%2d" ANSI_RESET, node->key);
    } else {
        printf(ANSI_WHITE ANSI_BOLD "%2d" ANSI_RESET, node->key);
    }
}

void print_spaces(int count) {
    for (int i = 0; i < count; i++) {
        printf(" ");
    }
}

void store_level_nodes(const node_t *node, const node_t *nil, const node_t **arr, int idx, int level, int target_level) {
    if (level == target_level) {
        arr[idx] = (node == nil) ? NULL : node;
        return;
    }

    if (node == nil) {
        if (level + 1 <= target_level) {
            store_level_nodes(nil, nil, arr, idx * 2, level + 1, target_level);
            store_level_nodes(nil, nil, arr, idx * 2 + 1, level + 1, target_level);
        }
    } else {
        store_level_nodes(node->left, nil, arr, idx * 2, level + 1, target_level);
        store_level_nodes(node->right, nil, arr, idx * 2 + 1, level + 1, target_level);
    }
}

void print_tree_vertical(const node_t *node, const node_t *nil) {
    if (node == nil) {
        printf("\n Print is Empty \n");
        return;
    }

    int height = get_tree_height(node, nil);
    printf("\nVertical Tree:\n");

    for (int level = 0; level < height; level++) {
        int nodes_in_level = 1 << level;
        node_t **level_nodes = (node_t **)calloc(nodes_in_level, sizeof(node_t *));

        store_level_nodes(node, nil, (const node_t **)level_nodes, 0, 0, level);

        int bottom_width = 1 << (height - 1);
        int spacing = (bottom_width * 4) / nodes_in_level;
        int offset = spacing / 2;

        print_spaces(offset);

        for (int i = 0; i < nodes_in_level; i++) {
            if (level_nodes[i]) {
                print_node_color(level_nodes[i], nil);
            } else {
                printf("  ");
            }

            if (i < nodes_in_level - 1) {
                print_spaces(spacing - 2);
            }
        }
        printf("\n");

        if (level < height - 1) {
            int branch_start = offset - spacing / 4;
            print_spaces(branch_start);

            for (int i = 0; i < nodes_in_level; i++) {
                if (level_nodes[i]) {
                    printf(level_nodes[i]->left != nil ? "/" : " ");
                    print_spaces(spacing / 2);
                    printf(level_nodes[i]->right != nil ? "\\" : " ");
                } else {
                    print_spaces(spacing / 2 + 2);
                }

                if (i < nodes_in_level - 1) {
                    print_spaces(spacing - spacing / 2 - 2);
                }
            }
            printf("\n");
        }

        free(level_nodes);
    }

    printf("\n");
}

void print_tree_horizontal(const node_t *node, const node_t *nil, int space) {
    const int COUNT = 4;

    if (node == nil) return;

    space += COUNT;

    print_tree_horizontal(node->right, nil, space);

    printf("\n");
    print_spaces(space - COUNT);
    print_node_color(node, nil);

    print_tree_horizontal(node->left, nil, space);
}

/* 디렉터리 생성 (mkdir -p 동작) */
static void ensure_dir(const char *filename) {
    char *path = strdup(filename);
    if (!path) return;

    char *slash = strrchr(path, '/');
    if (slash) {
        *slash = '\0';
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "mkdir -p '%s'", path);
        system(cmd);
    }

    free(path);
}
