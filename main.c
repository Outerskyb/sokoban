#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

#define HEIGHT 20 //세로
#define WIDTH 20 //가로
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

typedef struct {
	int x;
	int y;
}ordered_pair;

int map[HEIGHT][WIDTH]; // H*W 맵
int stone;
int move;
int previous_floor = 4;

int choice;
int map_num=1;

ordered_pair character;
ordered_pair *stone_position;

void remove_cursor(void) {
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y)//gotoxy
{
	COORD pos = { x - 1, y - 1 };//커서가 X좌표에서 -1 한값. Y좌표에서 -1한 값으로 이동
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void init_map(void) {
	memset(map, 4, sizeof(map));
}

void load_map(int num) {
	char route[19] = ".\\maps\\map";
	char number[3];
	char txt[5] = ".txt";
	strcat(route, itoa(num, number, 10));
	strcat(route, txt);
	FILE* mapfile = fopen(route, "rt");
	int i, j, temp;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			temp = fgetc(mapfile);
			map[i][j] = temp - '0';
			if (temp - '0' == 1) {
				character.x = j;
				character.y = i;
			}
			else if (temp - '0' == 2) {
				stone++;
			}
		}
		temp = fgetc(mapfile);
	}
	fclose(mapfile);
}

void memorise_stone_position(void) {
	int i, j;
	int count = 0;
	stone_position = (ordered_pair*)malloc(sizeof(ordered_pair)*stone);
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (map[i][j] == 2) {
				stone_position[count].y = i;
				stone_position[count++].x = j;
			}
			if (count == stone) {
				return;
			}
		}
	}
}

void move_character(int way) {
	if (way == UP) {
		if (map[character.y - 1][character.x] == 4) {//빈칸으로 이동
			map[character.y][character.x] = previous_floor;
			map[character.y - 1][character.x] = 1;
			previous_floor = 4;
			character.y--;
		}
		else if (map[character.y - 1][character.x] == 2) {//hole로 이동
			map[character.y][character.x] = previous_floor;
			map[character.y - 1][character.x] = 1;
			previous_floor = 2;
			character.y--;
		}
		else if (map[character.y - 1][character.x] == 3 && (map[character.y - 2][character.x] == 4 || map[character.y - 2][character.x] == 2)) {//밀기
			map[character.y][character.x] = previous_floor;
			map[character.y - 1][character.x] = 1;
			map[character.y - 2][character.x] = 3;
			character.y--;
		}
	}
	if (way == DOWN) {
		if (map[character.y + 1][character.x] == 4) {//빈칸으로 이동
			map[character.y][character.x] = previous_floor;
			map[character.y + 1][character.x] = 1;
			previous_floor = 4;
			character.y++;
		}
		else if (map[character.y + 1][character.x] == 2) {//hole로 이동
			map[character.y][character.x] = previous_floor;
			map[character.y + 1][character.x] = 1;
			previous_floor = 2;
			character.y++;
		}
		else if (map[character.y + 1][character.x] == 3 && (map[character.y + 2][character.x] == 4 || map[character.y + 2][character.x] == 2)) {//밀기
			map[character.y][character.x] = previous_floor;
			map[character.y + 1][character.x] = 1;
			map[character.y + 2][character.x] = 3;
			character.y++;
		}
	}
	if (way == RIGHT) {
		if (map[character.y][character.x + 1] == 4) {//빈칸으로 이동
			map[character.y][character.x] = previous_floor;
			map[character.y][character.x + 1] = 1;
			previous_floor = 4;
			character.x++;
		}
		else if (map[character.y][character.x + 1] == 2) {//hole로 이동
			map[character.y][character.x] = previous_floor;
			map[character.y][character.x + 1] = 1;
			previous_floor = 2;
			character.x++;
		}
		else if (map[character.y][character.x + 1] == 3 && (map[character.y][character.x + 2] == 4 || map[character.y][character.x + 2] == 2)) {//밀기
			map[character.y][character.x] = previous_floor;
			map[character.y][character.x + 1] = 1;
			map[character.y][character.x + 2] = 3;
			character.x++;
		}
	}
	if (way == LEFT) {
		if (map[character.y][character.x - 1] == 4) {//빈칸으로 이동
			map[character.y][character.x] = previous_floor;
			map[character.y][character.x - 1] = 1;
			previous_floor = 4;
			character.x--;
		}
		else if (map[character.y][character.x - 1] == 2) {//hole로 이동
			map[character.y][character.x] = previous_floor;
			map[character.y][character.x - 1] = 1;
			previous_floor = 2;
			character.x--;
		}
		else if (map[character.y][character.x - 1] == 3 && (map[character.y][character.x - 2] == 4 || map[character.y][character.x - 2] == 2)) {//밀기
			map[character.y][character.x] = previous_floor;
			map[character.y][character.x - 1] = 1;
			map[character.y][character.x - 2] = 3;
			character.x--;
		}
	}
}

int fill_check(void) {
	int i;
	int count = 0;
	for (i = 0; i < stone; i++) {
		if (map[stone_position[i].y][stone_position[i].x] == 3) {
			count++;
		}
		if (map[stone_position[i].y][stone_position[i].x] == 4) {
			map[stone_position[i].y][stone_position[i].x] = 2;
		}
	}
	if (count == stone) {
		return 1;
	}
	else {
		return 0;
	}
}

void show_map(void) { //맵표현 gotoxy
	int i, j;

	gotoxy(1, 1);
	printf("\n");


	printf("┌");
	for (j = 0; j < WIDTH; j++) { //메인창 바닥
		printf("─");
	}
	printf("┐");
	printf("\n");

	for (i = 0; i <= HEIGHT; i++) { //메인창의 높이
		if (i != HEIGHT) {
			printf("┃"); //메인창 테두리
			for (j = 0; j < WIDTH; j++) { //메인창 너비
				if (map[i][j] == 0) {
					printf("▩");//벽
				}
				else if (map[i][j] == 1) {
					printf("＠");//전각문자
				}
				else if (map[i][j] == 2) {
					printf("○");//hole
				}
				else if (map[i][j] == 3) {
					printf("●");//stone
				}
				else {
					printf("　");//길
				}
			}
			printf("┃");// 테두리
			if (i == 4) {
				printf("moves:%d", move);
			}
			printf("\n");
		}
		else {
			printf("└");
			for (j = 0; j < WIDTH; j++) { //메인창 바닥
				printf("─");
			}
			printf("┘");
			printf("\n");
		}

	}
}

void game_over(void) {
	init_map();
	show_map();
	gotoxy((WIDTH + 7) / 2, (HEIGHT + 1) / 2);
	printf("Game Over");
	gotoxy((WIDTH + 7) / 2, ((HEIGHT + 1) / 2) + 1);
	printf("move :%d", move);
}

int main(void) {
	int key;
	int map_num;

	remove_cursor(); 
	system("mode con:cols=60 lines=25");
	srand(time(NULL));

	while(1)
	{
		select_menu();
		if (choice == 1) break;
		map_num=select_map();

		init_map();
		load_map(map_num);
		memorise_stone_position();
		show_map();

		gotoxy(45, 15);
		printf("처음으로 r");

		while (!fill_check()) {
			// 키보드 입력시 동작//
			key = getch();
			if (key == 'r') break;
			key = getch();
			if (key == UP || key == DOWN || key == RIGHT || key == LEFT) {
				move_character(key);
				move++;
				show_map();
			}
			fflush(stdin);
			Sleep(10);
		}
		game_over();

		Sleep(1000);
		system("cls");
	}

	return 0;
}
