#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include<time.h>
#include<Windows.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
struct coord {
	float x = 0;
	float y = 0;
	float z = 0;
	float r = 0;
	float g = 0;
	float b = 0;
};
typedef struct node {
	coord* vertex;//정점 정보 저장 근데 이거맞냐?
	int vertex_counter = 0;
	struct node* next;//// 단일 연결 리스트로 구성
}TR_node;
node* createnode(coord data[],node*next,int i) {
	node* newnode;
	newnode = (node*)malloc(sizeof(node));
	newnode->vertex = data;
	newnode->vertex_counter = i;
	newnode->next = next;
	return newnode;
	//정점 정보를 받는다 정점은
	//1. 랜덤정점-> 뭐라도 떨어져야 하니까
	//2. 슬라이스 결과 정점-> 이후 추가 슬라이스 가능
}
void TR_insert(TR_node** head, TR_node* newnode) {
	if ((*head) == NULL) {
		(*head) = newnode;
	}
	else {
		TR_node* cur = (*head);
		while (cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = newnode;
	}
}
void TR_listmaker(TR_node** TR_list) {
	for (int i = 0; i < 20; i++) {
		coord* vertex;
		int get_rand_vertex = rand() % 4 + 3;// 3~6
		
		vertex = (coord*)malloc(get_rand_vertex*sizeof(coord));
		for (int j = 0; j < get_rand_vertex; j++) {
			vertex[j].r = static_cast<float>(rand()) / RAND_MAX;
			vertex[j].g = static_cast<float>(rand()) / RAND_MAX;
			vertex[j].b = static_cast<float>(rand()) / RAND_MAX;
			vertex[j].x = static_cast<float>(rand()) / RAND_MAX;
			vertex[j].y = static_cast<float>(rand()) / RAND_MAX;
			vertex[j].z = 0;			
		}
		TR_insert(TR_list, createnode(vertex, NULL, get_rand_vertex));
		//정점 정보/정점 개수/다음 노드 정보
	}
}

void starter();// make list
void printlist(TR_node** list);
void TR_delete(node** node);
void main() {
	//random seed
	srand(unsigned(time(NULL)));
	starter();
}
void starter() {
	TR_node* TR_list = NULL;
	TR_listmaker(&TR_list);
	printlist(&TR_list);

}
void TR_delete(node** node) {
	TR_node* current = *node;
	TR_node* previous = NULL;
	int idx = 0;

	while (current != NULL) {
		if (current->vertex[0].y<0.5f) {
			if (previous == NULL) {
				// If the first node meets the condition, update the head
				*node = current->next;
			}
			else {
				// For other nodes, update the 'next' pointer of the previous node
				previous->next = current->next;
			}
			TR_node* del = current;
			current = current->next;
			free(del);
		}
		else {
			previous = current;
			current = current->next;
		}
		printf("%d\n", idx);
		idx++;
	}
} //삭제 조건 완성
void printlist(TR_node** list) {
	TR_node* horse = NULL;
	horse = (*list);
	int idx = 0;
	while (horse != NULL) {
		printf("%d\n", idx);
		for (int i = 0; i < horse->vertex_counter; i++) {
			printf("dot %d:\t y:%f\n",i, horse->vertex[i].y);
		}
		horse = horse->next;
		idx++;
	}
}

// 노드 운영 중점
// 개 복잡한데??
// 하나로는 못쓰나
// 
// 노드에 뭘 담을것인가-> 1 삼각형 정점 정보 
// 삭제가 매우 중요해졌다. -> 이미 밑으로 떨어진 폴리곤은 데이터 낭비가 된다.
// 그럼 어쩌라고
// 삼각형만 먼저 만들어본다.
//2D 필요한거
// 정점 회전/이동 끝?
// 물체 이동 x축 시야 밖 생성
// 이동은? -1.0~1.0 사이에 도착
// 슬라이스
// 정점 레이저 검사 하면 교점 위치 나옴->그걸로 슬라이스 하면 되지않나
// 
//10-25 일지
//동적할당을 썼다.
//삭제,생성을 고쳤음
//리스트 하나로 여러개의 정점 얻을수 있다.
// 
//중요 1.
// 이제 그리기
// 자르기 
// 마지막 이동 
//  
// //