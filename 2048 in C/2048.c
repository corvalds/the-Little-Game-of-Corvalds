#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ROWNUM 4
#define COLNUM 4
#define MID_POS 3
#define MAX_DIGIT 8
#define EMPTY_REACT "       "

typedef struct{
	int x, y;
	char value[MAX_DIGIT];
}React;
typedef struct{
	React reacts[ROWNUM * COLNUM];
	int emptyReact; //场地中空的方块的个数 
}PlayGround;

void InitPlayGround(PlayGround *ground){
	int x, y, pos;
	
	for(x = 1;x <= ROWNUM;x++){
		for(y = 1;y <= COLNUM;y++){
			pos = (x - 1) * COLNUM + y - 1;
			strcpy(ground->reacts[pos].value, EMPTY_REACT);
		}
	}
	ground->emptyReact = ROWNUM * COLNUM;
}

//优化随机生成的算法 
void CreateNewElem(PlayGround *ground){
	int x, y, pos;
	int i; //定义循环变量 
	
	srand((unsigned)time(NULL));
	x = rand() % 5;
	y = rand() % 5;
	pos = (x - 1) * COLNUM + y -1;
	if(ground->emptyReact != 0){
		while(strcmp(ground->reacts[pos].value, EMPTY_REACT) != 0){	
			x = rand() % 5;
			y = rand() % 5;
			pos = (x - 1) * COLNUM + y -1;
		}
		ground->reacts[pos].value[MID_POS] = '2';
		ground->emptyReact--;
	}
		
}

void PrintPlayGround(PlayGround *ground){
	int x, y = 1, pos;
	
	printf("\t################################################\n");
	for(x = 1;x <= ROWNUM;x++){
		pos = (x - 1) * COLNUM + y - 1;
		printf("\t#  * * * * *  * * * * *  * * * * *  * * * * *  #\n");
		printf("\t#  *       *  *       *  *       *  *       *  #\n");
		printf("\t#  *%s*  *%s*  *%s*  *%s*  #\n", ground->reacts[pos].value, ground->reacts[pos + 1].value, ground->reacts[pos + 2].value, ground->reacts[pos + 3].value);
		printf("\t#  *       *  *       *  *       *  *       *  #\n");
		printf("\t#  * * * * *  * * * * *  * * * * *  * * * * *  #\n");
		if(x != ROWNUM)
			printf("\t#                                              #\n");
	}
	printf("\t################################################\n");
}

int main(){
	PlayGround *ground;
	
	ground = malloc(sizeof(PlayGround));
	InitPlayGround(ground);
	CreateNewElem(ground);
	PrintPlayGround(ground);
	
	return 0;
}
