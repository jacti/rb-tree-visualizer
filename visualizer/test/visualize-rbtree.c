// visualize_rbtree.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rbtree.h>

// 색깔 코드
#define ANSI_RED "\x1b[31m"   // 빨
#define ANSI_WHITE "\x1b[37m" // 흰
#define ANSI_RESET "\x1b[0m"  // 디폴트 색
#define ANSI_BOLD "\x1b[1m"   // 굵은 폰트

// 선 정의
static void ensure_dir(const char *filename);

/*
rbtree svg 만드는 함수
입력 : 출력을 시작할 루트 , nil, 파일 문자열 포인터
*/
void rbtree_to_svg(const node_t *root, const node_t *nil, const char *filename);

/*
rbtree를 console에 vertical 출력
입력 : 출력을 시작할 루트 , nil
*/
void print_tree_vertical(const node_t *node, const node_t *nil);

/*
rbtree를 console에 horizontal 출력
입력 : 출력을 시작할 루트 , nil, 구분할 공간 크기
*/
void print_tree_horizontal(const node_t *node, const node_t *nil, int space);


int main()
{
    /*
     ______    ____       ____        ______
    /\__  _\  /\  _`\    /\  _`\     /\__  _\
    \/_/\ \/  \ \ \L\_\  \ \,\L\_\   \/_/\ \/
       \ \ \   \ \  _\L   \/_\__ \      \ \ \
        \ \ \   \ \ \L\ \   /\ \L\ \     \ \ \
         \ \_\   \ \____/   \ `\____\     \ \_\
          \/_/    \/___/     \/_____/      \/_/

                여기에 시각화 코드 작성
    */

    // 예제
    rbtree *t = new_rbtree();
    #ifndef SENTINEL
        t->nil = NULL;
    #endif

    char img_file_buffer[100]; // img file 이름

    // 0부터 10까지 원소를 추가하는 테스트
    for (int i = 0; i < 5; i++)
    {
        rbtree_insert(t, i);
        // 파일 이름 for 문으로 생성
        sprintf(img_file_buffer, "out/imgs/serial_i_%d.svg", i);
        printf("\n %d 노드 삽입 \n", i);
        print_tree_horizontal(t->root,t->nil, 0); //콘솔에 수평으로 출력하는 함수
        // print_tree_vertical(t->root,t->nil); //콘솔에 수직으로 출력하는 함수
        // rbtree_to_svg(t->root,t->nil,img_file_buffer);
    }

    delete_rbtree(t);
    t = new_rbtree();
    // 10개의 랜덤 원소를 넣는 테스트
    for (int i = 0; i < 5; i++)
    {
        rbtree_insert(t, rand() % 100);
        // 파일 이름 for 문으로 생성
        sprintf(img_file_buffer, "out/imgs/rand_%d.svg", i);
        // printf("\n %d 노드 삽입 \n", i);
        // print_tree_horizontal(t->root,t->nil, 0); //콘솔에 수평으로 출력하는 함수
        // print_tree_vertical(t->root,t->nil); //콘솔에 수직으로 출력하는 함수
        // rbtree_to_svg(t->root,t->nil,img_file_buffer);
    }
}

int get_tree_height(const node_t *node, const node_t *nil)
{
    if (node == nil)
        return 0;

    int left_height = get_tree_height(node->left, nil);
    int right_height = get_tree_height(node->right, nil);

    return 1 + (left_height > right_height ? left_height : right_height);
}
void draw_node_svg(FILE *f, const node_t *node, const node_t *nil,
                   int x, int depth, int h_offset,
                   int v_spacing, int radius)
{
    if (node == nil)
    {
        return;
    }

    int y = v_spacing + depth * v_spacing;

    if (node->left != nil)
    {
        int child_x = x - h_offset;
        int child_y = v_spacing + (depth + 1) * v_spacing;
        fprintf(f, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" "
                   "stroke=\"black\" stroke-width=\"2\" />\n",
                x, y, child_x, child_y);
        draw_node_svg(f, node->left, nil, child_x, depth + 1,
                      h_offset / 2, v_spacing, radius);
    }
    if (node->right != nil)
    {
        int child_x = x + h_offset;
        int child_y = v_spacing + (depth + 1) * v_spacing;
        fprintf(f, "  <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" "
                   "stroke=\"black\" stroke-width=\"2\" />\n",
                x, y, child_x, child_y);
        draw_node_svg(f, node->right, nil, child_x, depth + 1,
                      h_offset / 2, v_spacing, radius);
    }

    const char *fill_color = (node->color == RBTREE_RED) ? "red" : "black";
    fprintf(f, "  <circle cx=\"%d\" cy=\"%d\" r=\"%d\" "
               "fill=\"%s\" stroke=\"black\" stroke-width=\"2\" />\n",
            x, y, radius, fill_color);

    fprintf(f, "  <text x=\"%d\" y=\"%d\" text-anchor=\"middle\" "
               "dy=\".3em\" font-size=\"%dpx\" fill=\"white\" font-weight=\"bold\">%d</text>\n",
            x, y, radius - 2, node->key);
}

void rbtree_to_svg(const node_t *root, const node_t *nil, const char *filename)
{

    ensure_dir(filename);

    if (root == nil)
    {
        fprintf(stderr, "Empty tree, SVG not generated\n");
        return;
    }

    int height = get_tree_height(root, nil);

    int v_spacing = 80;                            // 150 -> 80으로 줄임 (수직 간격)
    int radius = 20;                               // 25 -> 20으로 줄임 (노드 크기)
    int width = (1 << height) * radius * 2;        // radius * 4 -> radius * 2로 줄임
    int height_px = v_spacing * (height + 1) + 40; // 여백도 줄임

    FILE *f = fopen(filename, "w");
    if (!f)
    {
        perror("fopen");
        return;
    }

    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n",
            width, height_px);
    fprintf(f, "  <rect width=\"100%%\" height=\"100%%\" fill=\"white\"/>\n");

    int root_x = width / 2;
    int initial_offset = width / 4; // width / 3 -> width / 4로 줄임 (수평 간격)
    draw_node_svg(f, root, nil, root_x, 0, initial_offset,
                  v_spacing, radius);

    fprintf(f, "</svg>\n");
    fclose(f);
}
void print_node_color(const node_t *node, const node_t *nil)
{
    if (node == nil)
    {
        printf("  ");
        return;
    }

    if (node->color == RBTREE_RED)
    {
        printf(ANSI_RED ANSI_BOLD "%2d" ANSI_RESET, node->key);
    }
    else
    {
        printf(ANSI_WHITE ANSI_BOLD "%2d" ANSI_RESET, node->key);
    }
}

void print_spaces(int count)
{
    for (int i = 0; i < count; i++)
    {
        printf(" ");
    }
}

void store_level_nodes(const node_t *node, const node_t *nil, node_t **arr, int idx, int level, int target_level)
{
    if (level == target_level)
    {
        arr[idx] = (node == nil) ? NULL : node;
        return;
    }

    if (node == nil)
    {
        if (level + 1 <= target_level)
        {
            store_level_nodes(nil, nil, arr, idx * 2, level + 1, target_level);
            store_level_nodes(nil, nil, arr, idx * 2 + 1, level + 1, target_level);
        }
    }
    else
    {
        store_level_nodes(node->left, nil, arr, idx * 2, level + 1, target_level);
        store_level_nodes(node->right, nil, arr, idx * 2 + 1, level + 1, target_level);
    }
}

void print_tree_vertical(const node_t *node, const node_t *nil)
{
    if (node == nil)
    {
        printf("\n Print is Empty \n");
    }

    int height = get_tree_height(node, nil);
    printf("\nVertical Tree:\n");

    for (int level = 0; level < height; level++)
    {
        int nodes_in_level = 1 << level;
        node_t **level_nodes = (node_t **)calloc(nodes_in_level, sizeof(node_t *));

        store_level_nodes(node, nil, level_nodes, 0, 0, level);

        int bottom_width = 1 << (height - 1);
        int spacing = (bottom_width * 4) / nodes_in_level;
        int offset = spacing / 2;

        print_spaces(offset);

        for (int i = 0; i < nodes_in_level; i++)
        {
            if (level_nodes[i])
            {
                print_node_color(level_nodes[i], nil);
            }
            else
            {
                printf("  ");
            }

            if (i < nodes_in_level - 1)
            {
                print_spaces(spacing - 2);
            }
        }
        printf("\n");

        if (level < height - 1)
        {
            int branch_start = offset - spacing / 4;
            print_spaces(branch_start);

            for (int i = 0; i < nodes_in_level; i++)
            {
                if (level_nodes[i])
                {
                    if (level_nodes[i]->left != nil)
                    {
                        printf("/");
                    }
                    else
                    {
                        printf(" ");
                    }

                    print_spaces(spacing / 2);

                    if (level_nodes[i]->right != nil)
                    {
                        printf("\\");
                    }
                    else
                    {
                        printf(" ");
                    }
                }
                else
                {

                    print_spaces(spacing / 2 + 2);
                }

                if (i < nodes_in_level - 1)
                {
                    print_spaces(spacing - spacing / 2 - 2);
                }
            }
            printf("\n");
        }

        free(level_nodes);
    }
    printf("\n");
}

void print_tree_horizontal(const node_t *node, const node_t *nil, int space)
{
    const int COUNT = 4;

    if (node == nil)
        return;

    space += COUNT;

    print_tree_horizontal(node->right, nil, space);

    printf("\n");
    print_spaces(space - COUNT);
    print_node_color(node, nil);

    print_tree_horizontal(node->left, nil, space);
}

// 디렉터리 확인 및 생성 (mkdir -p 기능)
static void ensure_dir(const char *filename)
{
    char *path = strdup(filename);
    if (!path)
        return;
    char *slash = strrchr(path, '/');
    if (slash)
    {
        *slash = '\0';
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "mkdir -p '%s'", path);
        system(cmd);
    }
    free(path);
}



/*
.___  ___.      ___       _______   _______    .______   ____    ____ 
|   \/   |     /   \     |       \ |   ____|   |   _  \  \   \  /   / 
|  \  /  |    /  ^  \    |  .--.  ||  |__      |  |_)  |  \   \/   /  
|  |\/|  |   /  /_\  \   |  |  |  ||   __|     |   _  <    \_    _/   
|  |  |  |  /  _____  \  |  '--'  ||  |____    |  |_)  |     |  |     
|__|  |__| /__/     \__\ |_______/ |_______|   |______/      |__|     
                                                                      

   ____         __       ___        ______ .___________. __  
  / __ \       |  |     /   \      /      ||           ||  | 
 / / _` |      |  |    /  ^  \    |  ,----'`---|  |----`|  | 
| | (_| |.--.  |  |   /  /_\  \   |  |         |  |     |  | 
 \ \__,_||  `--'  |  /  _____  \  |  `----.    |  |     |  | 
  \____/  \______/  /__/     \__\  \______|    |__|     |__| 
                                                             

   ____       _______.     ___      .______        ______   .______        ______   
  / __ \     /       |    /   \     |   _  \      /  __  \  |   _  \      /  __  \  
 / / _` |   |   (----`   /  ^  \    |  |_)  |    |  |  |  | |  |_)  |    |  |  |  | 
| | (_| |    \   \      /  /_\  \   |      /     |  |  |  | |      /     |  |  |  | 
 \ \__,_|.----)   |    /  _____  \  |  |\  \----.|  `--'  | |  |\  \----.|  `--'  | 
  \____/ |_______/    /__/     \__\ | _| `._____| \______/  | _| `._____| \______/  

*/
                                                                                    
