#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ROWNUM 4
#define COLNUM 4
#define MID_POS 3
#define MAX_DIGIT 8
#define EMPTY_REACT "       "
#define FINALGOAL 2048

int gameOver = 0; //是否胜利的标识符 
int invalidAction = 0; //是否为非法操作的标识符 
int enableMove = 0; //是否可以移动的标识符 
typedef struct{
	int isAdd; //本回合内是否已经进行过加法 
	char value[MAX_DIGIT];
}React;
typedef struct{
	React reacts[ROWNUM][COLNUM];
	int emptyReact; //场地中空的方块的个数 
}PlayGround;

void InitPlayGround(PlayGround *ground){ //对游戏数据进行初始化 
	int x, y, pos;
	
	for(x = 0;x < ROWNUM;x++){
		for(y = 0;y < COLNUM;y++){
			strcpy(ground->reacts[x][y].value, EMPTY_REACT);
			ground->reacts[x][y].isAdd = 0;
		}
	}
	ground->emptyReact = ROWNUM * COLNUM;
}

//优化随机生成的算法 
void CreateNewElem(PlayGround *ground){ //创建新的元素 
	int x, y;
	int i;
	
	srand((unsigned)time(NULL));
	x = rand() % (ROWNUM + 1);
	y = rand() % (COLNUM + 1);
	if(ground->emptyReact != 0){
		while(strcmp(ground->reacts[x][y].value, EMPTY_REACT) != 0){	
			x = rand() % (ROWNUM + 1);
			y = rand() % (COLNUM + 1);
		}
		ground->reacts[x][y].value[MID_POS] = '2';
		ground->emptyReact--;
	}
		
}

void PrintPlayGround(PlayGround *ground){ //打印游戏场地 
	int x, y;
	
	printf("\t################################################\n");
	for(x = 0;x < ROWNUM;x++){
		printf("\t#  * * * * *  * * * * *  * * * * *  * * * * *  #\n");
		printf("\t#  *       *  *       *  *       *  *       *  #\n");
		printf("\t#");
		for(y = 0;y < COLNUM;y++)
			printf("  *%s*", ground->reacts[x][y].value);
		printf("  #\n");
		printf("\t#  *       *  *       *  *       *  *       *  #\n");
		printf("\t#  * * * * *  * * * * *  * * * * *  * * * * *  #\n");
		if(x != ROWNUM - 1)
			printf("\t#                                              #\n");
	}
	printf("\t################################################\n");
}

void ReactAddition(PlayGround *ground, int hor, int ver, int preHor, int preVer, int check){ //处理方块相加的逻辑 
	int tmpInt, iterInt, tmpStrLen, forward;
	char tmpStr[MAX_DIGIT];
	
	if(strcmp(ground->reacts[hor][ver].value, EMPTY_REACT) == 0){
		enableMove = 1;
		return;
	}
	if(strcmp(ground->reacts[preHor][preVer].value, EMPTY_REACT) == 0){
		if(check){
			enableMove = 1;
			return;
		}
		strcpy(ground->reacts[preHor][preVer].value, ground->reacts[hor][ver].value);
		strcpy(ground->reacts[hor][ver].value, EMPTY_REACT);
		invalidAction = 0;
	} else if(strcmp(ground->reacts[preHor][preVer].value, ground->reacts[hor][ver].value) == 0 && !ground->reacts[preHor][preVer].isAdd && !ground->reacts[hor][ver].isAdd){
		if(check){
			enableMove = 1;
			return;
		}
		tmpInt = atoi(ground->reacts[preHor][preVer].value) + atoi(ground->reacts[hor][ver].value);
		if(tmpInt == FINALGOAL)
			gameOver = 1;
		itoa(tmpInt, tmpStr, 10);
		tmpStrLen = strlen(tmpStr);
		forward = tmpStrLen - 2;
		for(iterInt = 0;iterInt < tmpStrLen;iterInt++){
			if(tmpStrLen < 3)
				ground->reacts[preHor][preVer].value[MID_POS + iterInt] = tmpStr[iterInt];
			else
				ground->reacts[preHor][preVer].value[MID_POS + iterInt - forward] = tmpStr[iterInt];
		}
		strcpy(ground->reacts[hor][ver].value, EMPTY_REACT);
		ground->reacts[preHor][preVer].isAdd = 1;
		ground->emptyReact++;
		invalidAction = 0;
	}
}

void ReactGlide(PlayGround *ground, int isHorizontal, int moveDirection, int x, int y, int check){ //实现方块滑动的逻辑 
	int hor = x, ver = y, preHor = x, preVer = y;
	
	while(1){
		if(isHorizontal){
			ver = preVer;
			if(moveDirection){
				preVer--;
				if(preVer < 0)
					break;
			} else{
				preVer++;
				if(preVer >= COLNUM)
					break;
			}
		} else{
			hor = preHor;
			if(moveDirection){
				preHor--;
				if(preHor < 0)
					break;
			} else{
				preHor++;
				if(preHor >= ROWNUM)
					break;
			}
		}
		ReactAddition(ground, hor, ver, preHor, preVer, check);
	}
}

void Move(PlayGround *ground, int isHorizontal, int moveDirection, int check){ //选择要移动的方块 
	int iterEnd = 0, isFirst = 1;
	int x, y, hor, ver, preHor, preVer;
	
	//对x，y进行初始化
	if(moveDirection){
		//a,w 
		x = -1; 
		y = -1;
	} else{
		//d,s
		x = isHorizontal?ROWNUM:COLNUM;
		y = isHorizontal?COLNUM:ROWNUM;
	}
	//选择方块进行移动 
	while(1){
		if(moveDirection){
			x++;
			y = -1;
		}
		else{
			x--;
			y = isHorizontal?COLNUM:ROWNUM;
		}
		isFirst = 1;
		while(1){
			if(moveDirection){
				y++;
				hor = isHorizontal?x:y;
				ver = isHorizontal?y:x;
			} else{
				y--;
				hor = isHorizontal?x:y;
				ver = isHorizontal?y:x;
			} 
			//这里还可以优化 
			if(isHorizontal && (hor < 0 || hor >= ROWNUM)){
				iterEnd = 1;
				break;
			} else if(!isHorizontal && (ver < 0 || ver >= COLNUM)){
				iterEnd = 1;
				break;
			}
			if(isHorizontal && (ver < 0 || ver >= COLNUM))
				break;
			else if(!isHorizontal && (hor < 0 || hor >= ROWNUM))
				break;
			//方块移动 
			ReactGlide(ground, isHorizontal, moveDirection, hor, ver, check);
		}
		if(iterEnd)
			break;
	}
	for(x = 0;x < ROWNUM;x++)
		for(y = 0;y < COLNUM;y++)
			ground->reacts[x][y].isAdd = 0;
}

void CheckEnableMove(PlayGround *ground){ //检查是否已经无路可走 
	Move(ground, 1, 1, 1);
	Move(ground, 1, 0, 1);
	Move(ground, 0, 0, 1);
	Move(ground, 0, 1, 1);
}

void HandleOrder(PlayGround *ground, char order){ //处理用户输入 
	switch(order){
		case 65:;
		case 97:
			Move(ground, 1, 1, 0);
			break; //a
		case 68:;
		case 100:
			Move(ground, 1, 0, 0);
			break; //d
		case 83:;
		case 115:
			Move(ground, 0, 0, 0);
			break; //s
		case 87:;
		case 119:
			Move(ground, 0, 1, 0);
			break; //w
	}
}

int main(){
	PlayGround *ground;
	char order;
	
	ground = malloc(sizeof(PlayGround));
	InitPlayGround(ground);
	while(1){ //主循环 
		if(!invalidAction)
			CreateNewElem(ground);
		invalidAction = 1;
		enableMove = 0;
		PrintPlayGround(ground);
		order = getch();
		fflush(stdin);
		HandleOrder(ground, order);
		CheckEnableMove(ground);
		if(gameOver) //游戏胜利 
			break;
		if(!enableMove) //游戏失败
			break;
		system("cls");
	}
	
	return 0;
}
