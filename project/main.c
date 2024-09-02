#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <form.h>
#include "phonebook.h"

int y_max, x_max;	//창 크기 전역 변수로 선언

int menu_open(void) {		//첫 시작 메뉴
	WINDOW *menuwin = newwin(8, 12, y_max / 2 - 4, x_max / 2 - 6);		//윈도우 생성
	box(menuwin, 0, 0);			//테두리
	mvwprintw(menuwin, 0, 1, "MENU");	//제목
	refresh();
	wrefresh(menuwin);

	
	keypad(menuwin, true);			//키패드 입력 받기

	char *choices[4] = {"List", "Search", "Add", "Exit"};	//메뉴 내용
	int choice;		//어떤 것을 선택하는지 키를 받을 변수
	int highlight = 0;	//선택한 항목을 반환하는 변수
	refresh();
	wrefresh(menuwin);
	
	while(1) {
		for (int i = 0; i < 4; i++) {
			if (i == highlight)	{
				wattron(menuwin, A_REVERSE);	//attron
			}
			mvwprintw(menuwin, i + 1, 1, choices[i]);	//메뉴 내용 출력하기
			wattroff(menuwin, A_REVERSE);		//attroff
			wrefresh(menuwin);
		}
		choice = wgetch(menuwin);	//키 받아서
		
		switch (choice) {
			case KEY_UP:		//위 방향키
				highlight--;	//위로 가기
				if (highlight == -1)	
					highlight = 0;	//-1로 넘어가는거 방지
				break;
			case KEY_DOWN:		//아래 방향키
				highlight++;	//아래로 가기
				
				if (highlight == 4)
					highlight = 3;	//넘어가는거 방지
				break;
			default:
				break;
		}
		if (choice == 10) {		//10은 엔터키
			werase(menuwin);	//종료
			wrefresh(menuwin);
			delwin(menuwin);
			return highlight;
		}
		
	}
}


int main(void) {
	initscr();		//initialization
	cbreak();		//글자를 받을 것임
	noecho();		

	//int x_max, y_max;
	getmaxyx(stdscr, y_max, x_max);	//윈도우 크기 구하기
	
	
	//menu 열고 선택한 항목 가져오기
	while(1) {			//반복적으로 실행하여 각 항목에 대한 일을 수행하고 다시 메인 메뉴를 띄울 수 있도록 함
		box(stdscr, 0, 0);		//테두리
		mvprintw(0, 1, "PHONEBOOK");	//제목
		refresh();
		
		int menu_selection = menu_open();
		switch(menu_selection) {
			case 0:			//0번(=list)이라면 
				List(y_max, x_max);	//list 함수 실행
				continue;
			case 1:			//1번(=search)이라면
				search(y_max, x_max);		 
				continue;		
			case 2:			//2번(=add)이라면 
				add_form();	
				sort();	
				continue;
			case 3:			//3번(=exit)이라면
				endwin();		//종료
				return 0;			
		}
	}

	refresh();		//종료
	getch();
	endwin();
	return 0;
}
