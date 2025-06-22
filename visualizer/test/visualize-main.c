// visualize_rbtree.c
#include <stdio.h>
#include "rbtree_visualizer.h"

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
        sprintf(img_file_buffer, "imgs/serial_%d.svg", i);
        printf("\n--- %d번째 노드 삽입 ---\n", i+1);
        // print_tree_horizontal(t->root,t->nil, 0); //콘솔에 수평으로 출력하는 함수
        print_tree_vertical(t->root,t->nil); //콘솔에 수직으로 출력하는 함수
        rbtree_to_svg(t->root,t->nil,img_file_buffer);  //이미지 출력 하는 함수
        // rbtree_to_svg_specific(t->root,t->nil,img_file_buffer);  // 상세 이미지 출력 하는 함수
    }

    delete_rbtree(t);
    t = new_rbtree();
    // 10개의 랜덤 원소를 넣는 테스트
    // 랜덤 seed 입력
    srand((unsigned)time(NULL));
    for (int i = 0; i < 5; i++)
    {
        rbtree_insert(t, rand() % 100);
        // 파일 이름 for 문으로 생성
        sprintf(img_file_buffer, "imgs/rand_%d.svg", i);
        printf("\n--- %d번째 노드 삽입 ---\n", i+1);
        // print_tree_horizontal(t->root,t->nil, 0); //콘솔에 수평으로 출력하는 함수
        print_tree_vertical(t->root,t->nil); //콘솔에 수직으로 출력하는 함수
        rbtree_to_svg(t->root,t->nil,img_file_buffer);  //이미지 출력 하는 함수
        // rbtree_to_svg_specific(t->root,t->nil,img_file_buffer);  // 상세 이미지 출력 하는 함수
    }
}