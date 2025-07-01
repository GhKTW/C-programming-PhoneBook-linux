// search.c
// 전화번호부 검색 기능을 구현하는 파일

#include <ncurses.h>
#include <form.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phonebook.h"

//binary search를 사용하려고 했으나, 그러기 위해서는 입력하는 값이 :가 포함된 데이터 전체를 정확히 입력해야 함
//이름만 받아와서 배열에 저장, 이용하려고 했으나 fgets를 반복해서 호출하는 과정이 포함된다면 원시적인 search 과정이 낫다고 생각함

//찾을 데이터가 어느 순서에 있는지 찾는 함수
int search_where(char want_sch[]) {		//찾고싶은 단어를 받아옴
	FILE* file = fopen("data.txt", "r"); 	//파일 열기
	if (file == NULL) {
		perror("Unable to open file"); 
		return -1; 
	}

	char buffer[150]; 			//받은 데이터를 임시로 저장할 buffer
	int line_number = 0;			//줄 수를 나타내는 변수

	while (fgets(buffer, sizeof(buffer), file) != NULL) { 	//NULL이 오기 전까지 호출 반복
		char name_sch[20] = {}; 			//받아온 데이터 중 이름만 저장할 문자열
		int j = 0; 			//순서 세기 위한 변수
		while ((buffer[j] != '\0') && (buffer[j] != ':')) { 	//첫번째 ':'이 오기 전까지, 그리고 NULL이 아닐 때
			name_sch[j] = buffer[j];		//한글자씩 저장해 나감
			j++;
		}

		name_sch[j] = '\0'; 	//저장 후 마지막에는 널 문자 추가

		if (strcmp(want_sch, name_sch) == 0) { 	//찾고자 하는 단어와 얻어온 데이터를 비교
			fclose(file); 		//값을 찾은 경우 파일 닫기
			return line_number; 	//line_number 반환하기
		}

		line_number++; //아닌 경우 다음 줄로 이동
	}

	fclose(file); 	//찾지 못한 경우 파일 닫기
	return -1; 	//-1 반환
}

//검색하기 위한 함수
int search(int y_max, int x_max) {

	initscr();		//initialize
	cbreak();
	noecho();
	
	//필드를 만들어 데이터를 받음
	WINDOW* searchwin = newwin(3, 40, 3, 3);

	refresh();
	
	FORM *my_form;  
	FIELD *field[2];
	field[0] = new_field(1, x_max / 2 - 3, 5, 5, 0, 0);
	field[1] = NULL;
	
	set_field_back(field[0], A_UNDERLINE);  //필드 배경 설정
	field_opts_off(field[0], O_AUTOSKIP);   //자동 스킵 비활성화
	
	my_form = new_form(field);
	post_form(my_form);
	mvprintw(4, 3, "Enter name to search:");		//이름 입력할 곳
	refresh();
	
	box(stdscr, 0, 0);		//테두리
	mvprintw(0, 1, "Search");	//제목
	mvprintw(y_max - 1, 1, "Press f1 to exit");
	refresh();
	keypad(stdscr, true);
	keypad(searchwin, true);
	
	int ch;
	while ((ch = getch()) != 10) {
		switch (ch) {
			case KEY_F(1):		//f1이 입력되면 종료
				wrefresh(searchwin);
				werase(searchwin);
				werase(stdscr);
				return 0;
			case KEY_BACKSPACE:	//백스페이스를 누르면 지우기
				form_driver(my_form, REQ_DEL_PREV);
				break;
			default:		//기본적으로 데이터 입력받기
				form_driver(my_form, ch);
				break;
		}
	}
	
	form_driver(my_form, REQ_NEXT_FIELD);  //모든 필드를 폼에 반영
	
	char want_sch[30];			//찾고 싶은 데이터 저장
	strcpy(want_sch, field_buffer(field[0], 0));	
	
	trim_whitespace(want_sch);		//여백 제거
	
	unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);			//필드 종료
	
	if (want_sch[0] == ' ' ) {		//입력된 값이 없다면
		werase(stdscr);		//종료
		box(stdscr, 0, 0);		
		mvprintw(0, 1, "Search");	
		mvwprintw(searchwin, 1, 1, "Enter text");
		refresh();
		wrefresh(searchwin);
		getch();
		werase(stdscr);
		return 0;
	}
	int searched = search_where(want_sch);	//입력된 값이 있을 경우 그 데이터의 순서를 찾는 함수 실행
	if(searched == -1) {		//찾지 못했다면
		werase(stdscr);	//종료
		box(stdscr, 0, 0);		
		mvprintw(0, 1, "Search");	
		mvwprintw(searchwin, 1, 1, "Could't find %s", want_sch);
		refresh();
		wrefresh(searchwin);
		getch();
		werase(stdscr);
		return 0;
	} else {				//찾았다면
		delwin(searchwin);
		werase(stdscr);
		box(stdscr, 0, 0);		
		mvprintw(0, 1, "Search");
		detail(searched, y_max, x_max);	//찾은 데이터 내용 출력하기
		refresh();
	}
	delwin(searchwin);		//종료
	refresh();
	werase(stdscr);
	

	return 0;
	
}
