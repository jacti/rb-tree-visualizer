# 레드블랙 트리 시각화 v.3.1.0

> 안녕 친구들~ 편의성패치 아저씨야
> 

> Develop with [@saroro1](https://github.com/saroro1)
> 

> Special thanks : Chat gpt
> 

rbtree *t를 입력하면 현재 트리 모습을 콘솔 출력 또는 svg 파일로 저장하는 코드입니다.

# 공지

<p>${\color{#DD6565}이전\ 버전\ (v 3.0.0\ 이하)에서\ make\ test\ 함수가\ 실행이\ 안되는\ 버그가\ 있었습니다.}$</p>

<p color="red"></p>

**현 버전으로 업데이트하여 해결할 수 있습니다.**

이전 버전에서 업데이트 할 유저들도 repo를 pull 한 후 ./setup.sh를 한번 더 실행해 주시기 바랍니다.

- **주의** 업데이트 시 이전 visualize-rbtree.c 가 자동으로 삭제 됩니다. main에 본인만의 테스트 코드를 짜놓았다면 미리 복사해두길 바랍니다.

# 사용 준비

> 우와 정말 간단해!
> 
1. 프로젝트 루트에서 git clone
    
    ```bash
    git clone https://github.com/jacti/rb-tree-visualizer.git
    ```
    
2. .setup.sh 실행
    
    ```bash
    cd ./rb-tree-visualizer
    ./setup.sh
    ```
    
    - setup.sh는 세가지 값의 입력을 요구합니다.
        - docker 프로젝트인지 y/n
        - 프로젝트의 루트 경로 (기본 값은 ../)
        - (docker 프로젝트인 경우만) rbtree_lab 폴더 이름 (기본값은 rbtree_lab)

## 사용 법

visualize-main.c 파일로 이동합니다.

main 함수 안에 테스트할 상황을 구현해주세요.

( 예제는 0~5까지 연속된 노드 삽입, 100 미만 5개 노드 삽입 하고 각 과정별 트리 모습을 출력하는 코드입니다.)

tree의 모습을 확인하고 싶은 위치에 아래 세개의 테스트 함수 중 원하는 것을 사용합니다.

### 콘솔에 트리 출력

- 수직 출력

```c
/*
rbtree를 console에 vertical 출력
입력 : 출력을 시작할 루트 , nil
*/
void print_tree_vertical(const node_t *node, const node_t *nil);
```

- 출력 예시

```markdown
3 노드 삽입 

Vertical Tree:
         1
    /        \
     0       2
               \
               3

4 노드 삽입 

Vertical Tree:
         1
    /        \
     0       3
          /    \
           2   4
```

- 수평 출력

```c

/*
rbtree를 console에 horizontal 출력
입력 : 출력을 시작할 루트 , nil, 구분할 공간 크기
*/
void print_tree_horizontal(const node_t *node, const node_t *nil, int space);
```

- 출력 예시

```markdown
 3 노드 삽입 

         3
     2
 1
     0

 4 노드 삽입 

         4
     3
         2
 1
     0
```

### svg 이미지 저장

- 일반 트리 이미지 생성

```c
/*
rbtree svg 만드는 함수
입력 : 출력을 시작할 루트 , nil, 파일 문자열 포인터
*/
void rbtree_to_svg(const node_t *root, const node_t *nil, const char *filename);
```

- 출력 예시

<figure class="half">  <a href="link"><img src="https://github.com/user-attachments/assets/9bc535b7-ab8b-4b01-9a63-8a7daccaa74e"></a>  <a href="link"><img src="https://github.com/user-attachments/assets/18a785d6-9b41-479b-8623-b2254cc37b59"></a> </figure>

- 상세 (포인터 정보 포함) 노드 이미지 생성

```c
/*
상세 정보 svg 만드는 함수
입력 : 출력을 시작할 루트 , nil, 파일 문자열 포인터
*/
void rbtree_to_svg_specific(const node_t *root, const node_t *nil, const char *filename);
```

- 출력 예시

![image](https://github.com/user-attachments/assets/f8bd7442-6807-4a34-8f52-780079f964c0)

## 업데이트 로그
- v3.1.0
    - Docker, 비 Docker 프로젝트 Makefile 구조 통합
    - Docker 프로젝트도 out/ 안에 모든 .o, 실행 파일이 모이도록 수정
    - 모든 생성 이미지가 out/ 안에 생길 수 있도록 수정
    - <font color="red">make test 동작하지 않던 버그 수정</font>
    
- v3.0.0
    > 아래 기능을 개발한 사람 [@saroro1](https://github.com/saroro1)
    > 
    - <font color="red">NIL node 정보 추가</font>
    - 상세 이미지 생성 함수 추가
    - visualizer 헤더파일 추가
    - test main과 함수 정의부 코드 분리
 
      
    - 버그 수정

- v2.1.0
    - [git hub repo](https://github.com/jacti/rb-tree-visualizer.git) 생성
    - pure project 사용자를 위한 setup 설정
    - gdb path 추적 후 launch.json 최신화
- v2.0.0
    - 설치 sh 스크립트 생성
    
    > 아래 기능을 개발한 사람 [@saroro1](https://github.com/saroro1)
    > 
    - 라이브러리 의존성 제거
    - png에서 svg로 이미지 타입 변경
    - 콘솔 시각화 함수 추가
