#include <form.h>
#include <ncurses.h>
#include <string.h>
#include <stdio.h>

void trim_whitespace(char *str) {
    	char *end;

  	//문자열의 끝에서부터 공백 제거
  	end = str + strlen(str) - 1;
   	while (end > str && *end == ' ') end--;

  	//새로운 문자열 끝에 null terminator 추가
	*(end + 1) = '\0';
}

void add_form() {		//데이터 추가를 위한 함수
	FIELD *field[5];	//field 이용
	FORM *my_form;   
	int ch;  
	   
  	char save[200] = {0};  //Save array 초기화 (크기 증가)

	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);		//키패드 받기
	
	box(stdscr, 0, 0);		//테두리
	mvprintw(0, 1, "ADD");
	refresh();
    


	//필드 생성: 높이, 길이, 시작 행, 시작 열, 0, 0
	field[0] = new_field(1, 30, 4, 18, 0, 0);
	field[1] = new_field(1, 20, 6, 18, 0, 0);
	field[2] = new_field(1, 30, 8, 18, 0, 0);
	field[3] = new_field(1, 20, 10, 18, 0, 0); //새로운 필드 추가
	field[4] = NULL;

	set_field_back(field[0], A_UNDERLINE);  //필드 배경 설정
	field_opts_off(field[0], O_AUTOSKIP);   //자동 스킵 비활성화

	set_field_back(field[1], A_UNDERLINE);  //필드 배경 설정
	field_opts_off(field[1], O_AUTOSKIP);   //자동 스킵 비활성화

	set_field_back(field[2], A_UNDERLINE);  //필드 배경 설정
	field_opts_off(field[2], O_AUTOSKIP);   //자동 스킵 비활성화

	set_field_back(field[3], A_UNDERLINE);  //필드 배경 설정
	field_opts_off(field[3], O_AUTOSKIP);   //자동 스킵 비활성화

	my_form = new_form(field);
	post_form(my_form);
	refresh();
	
	mvprintw(4, 10, "Name   :");		//이름 입력할 곳
	mvprintw(6, 10, "Number :");		//번호 입력할 곳
	mvprintw(8, 10, "Memo   :");		//메모 입력할 곳
	mvprintw(11, 10, "Press f1 to save / exit");	//탈출 방법

	box(stdscr, 0, 0);		//테두리
	mvprintw(0, 1, "ADD");
	refresh();

	while ((ch = getch()) != KEY_F(1)) {		//f1을 누르면 종료
		switch (ch) {
			case KEY_DOWN:		//아래쪽 화살표를 누르면 커서 내려가기
				form_driver(my_form, REQ_NEXT_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case 10:		//엔터를 눌러도 내려가기
				form_driver(my_form, REQ_NEXT_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_UP:		//위쪽 화살표를 누르면 커서 올라가기
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_BACKSPACE:	//백스페이스를 누르면 지우기
				form_driver(my_form, REQ_DEL_PREV);
				break;
			default:
				form_driver(my_form, ch);
				break;
		}
	}

	//입력된 값을 save 배열에 저장
	form_driver(my_form, REQ_NEXT_FIELD);  //모든 필드를 폼에 반영

	char name[30];			//이름 저장
	char number[20];		//번호 저장
	char memo1[30];		//메모 저장, 데이터가 클 수 있으므로 분할
	char memo2[20];

	strcpy(name, field_buffer(field[0], 0));	//버퍼에 저장
	strcpy(number, field_buffer(field[1], 0));
	strcpy(memo1, field_buffer(field[2], 0));
	strcpy(memo2, field_buffer(field[3], 0));

	trim_whitespace(name);		//원활한 저장 절차를 위한 여백 제거
	trim_whitespace(number);
	trim_whitespace(memo1);
	trim_whitespace(memo2);
	//아무런 데이터도 입력되지 않았다면 저장하지 않기
	if (name[0] == ' ' && number[0] == ' '&& memo1[0] == ' ' && memo2[0] == ' ') {
		mvwprintw(stdscr, 12, 10, "Data not saved");
		
		refresh();		//종료 절차
		getch();
		unpost_form(my_form);
		free_form(my_form);
		free_field(field[0]);
		free_field(field[1]);
		free_field(field[2]);
		free_field(field[3]);
	}
	else {		// save 배열에 저장된 내용을 data.txt 파일의 마지막에 저장
		snprintf(save, sizeof(save), "%s:%s:%s%s", name, number, memo1, memo2);	//각 데이터는 ':'로 구분하여 저장
		FILE *file = fopen("data.txt", "a");		//파일 열기
		if (file != NULL) {
			fprintf(file, "%s\n", save);		//fprintf를 통한 데이터 저장
			fclose(file);				//파일 닫기
		} 
		else {
			mvprintw(12, 10, "Error opening file");
		}
		mvprintw(12, 10, "Saved: %s", save);		//저장된 데이터 알려주기
		
		refresh();		//종료 절차
		getch();
		unpost_form(my_form);
		free_form(my_form);
		free_field(field[0]);
		free_field(field[1]);
		free_field(field[2]);
		free_field(field[3]);
	}

}

