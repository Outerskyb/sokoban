#include <stdio.h>
#include <Windows.h>

#define UP 72
#define DOWN 80
#define ENTER 13

extern int choice;

int select_map() {
	int map_num=1;
	int key;

	system("cls");

	gotoxy(28, 10);
	printf("맵 선택");
	gotoxy(22, 15);
	printf("↑↓ ENTER : 선택");
	gotoxy(30, 12);
	printf("/  50");
	gotoxy(25, 13);
	printf("──");
	gotoxy(26, 12);
	printf("%2d", map_num);

	while (1)
	{
		key = getch();
		if (key == ENTER) break;

		if (key == 224){
			key = getch();

			if (key == UP)
			{
				map_num++;
				if (map_num == 51)
					map_num = 50;
			}
			else if (key == DOWN)
			{
				map_num--;
				if (map_num == 0)
					map_num = 1;
			}
			gotoxy(26, 12);
			printf("%2d", map_num);
		}
	}

	return map_num;
}
void select_menu() {
	int key;

	gotoxy(25, 11);
	printf("┌───┐");
	gotoxy(25, 13);
	printf("└───┘");

	while (1)
	{
		gotoxy(28, 7);
		printf("MENU");
		gotoxy(27, 12);
		printf("맵선택");
		gotoxy(28, 16);
		printf("종료");

		key = getch();
		system("cls");
		if (key == ENTER) break;
		key = getch();

		if (key == UP)
		{
			if (choice == 1)
				choice = 0;
		}
		else if (key == DOWN)
		{
			if (choice == 0)
				choice = 1;
		}
		
		switch (choice)
		{
		case 0:
			gotoxy(25, 11);
			printf("┌───┐");
			gotoxy(25, 13);
			printf("└───┘");
			break;
		case 1:
			gotoxy(25, 15);
			printf("┌───┐");
			gotoxy(25, 17);
			printf("└───┘");
			break;
		}
	}
}
