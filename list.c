// list.c
// 전화번호부 목록을 출력하고, 선택한 항목의 세부사항을 보여주는 기능을 구현하는 파일

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <form.h>

int list_selector(int size, char* listing_arr[]) {
	keypad(stdscr, true);			//키패드 입력 받기

	int choice;			//어떤 것을 선택하는지 키를 받을 변수
	int highlight = 0;	//선택한 항목을 반환하는 변수
	refresh();

	while (1) {
		werase(stdscr);
		box(stdscr, 0, 0);				//테두리 그리기
		mvprintw(0, 1, "List");			//제목
		mvprintw(size + 1, 1, "Press f1 to EXIT, d to DELETE");
		refresh();
		for (int j = 0; j < size; j++) {
			if (j == highlight) {
				attron(A_REVERSE);		//attron
			}
			mvprintw(j + 1, 1, listing_arr[j]);	//메뉴 내용 출력하기
			attroff(A_REVERSE);			//attroff
			refresh();
		}
		choice = wgetch(stdscr);		//키 받아서

		switch (choice) {
		case KEY_UP:					//위 방향키
			highlight--;				//위로 가기
			if (highlight == -1)
				highlight = 0;		//-1로 넘어가는거 방지
			break;
		case KEY_DOWN:				//아래 방향키
			highlight++;			//아래로 가기

			if (highlight == size)
				highlight = size - 1;	//넘어가는거 방지
			break;
		case KEY_LEFT:
			return -2;
		case KEY_RIGHT:
			return -3;
		case KEY_F(1):
			return -1;
		case 'd':
			return -highlight - 10;	//highlight를 살리면서 동시에 음의 숫자를 가지게 함으로써 다른 선택지와 충돌 방지
		case 10:		
			return highlight;	
		default:
			break;
		}
		
	}
}

void detail(int selected, int y_max, int x_max) {	//highlight를 받아 와서 그 항목에 대해 세부사항을 출력하는 함수
	FILE* file;					//data.txt 파일을 열기 위한 포인터
	char buffer_det[105];		//세부사항을 받아올 buffer

	WINDOW* detailwin;			//세부사항을 표시할 새 윈도우
	detailwin = newwin(8, 41, 8, x_max / 2 - 20);

	box(detailwin, 0, 0);			//테두리 그리기

	file = fopen("data.txt", "r");	//data.txt 파일 열기(읽기, 쓰기 동시에)
	if (file == NULL) {
		perror("Error opening file");
		refresh();
		return;
	}

	char name_det[31] = {};		//data.txt에서 받아온 데이터를 이름, 번호, 메모로 구분하여 출력하기 위한 배열
	char phone_det[21] = {};
	char memo_det[51] = {};

	//선택된 항목의 순서를 받아 와서 그 순서만큼 fgets 호출, 마지막 fgets 호출에서 원하는 항목을 읽게 됨
	for (int i = 0; i <= selected; i++)
		if ((fgets(buffer_det, sizeof(buffer_det), file)) == NULL) {	//받아온 데이터 buffer_det에 저장
			werase(detailwin);
			wrefresh(detailwin);
			delwin(detailwin);
			fclose(file);
			return;
		}

	//:로 구분돼있는 data에서 각 데이터들을 이름, 번호, 메모로 분해
	char* tmp;
	tmp = strtok(buffer_det, ":");	//strtok로 buffer_det에서 ':' 가 오기 전까지 읽음
	if (tmp != NULL) {
		strncpy(name_det, tmp, 30);	//첫 ':' 전까지의 내용은 이름, 이름 배열에 저장
		name_det[30] = '\0';		//끝을 나타내기 위해 널 문자로 초기화
	}
	tmp = strtok(NULL, ":");		//재호출, 첫 ':' 뒤에서부터 읽음
	if (tmp != NULL) {
		strncpy(phone_det, tmp, 20);
		phone_det[20] = '\0';
	}
	tmp = strtok(NULL, ":");		//재호출, 두번째 ':' 뒤에서부터 읽음
	if (tmp != NULL) {
		strncpy(memo_det, tmp, 50);
		memo_det[50] = '\0';
	}

	mvwprintw(detailwin, 1, 1, "Name  : %s", name_det);		//이름 출력
	mvwprintw(detailwin, 2, 1, "Phone : %s", phone_det);		//번호 출력
	mvwprintw(detailwin, 3, 1, "Memo  :");//메모 출력, 메모는 데이터가 최대 50글자로 크기 때문에, 30글자와 20글자로 나눠서 불러옴

	int len;	//나눠서 출력하기 위해 30과 50을 세기 위한 변수
	for (len = 0; len < 30; len++) {	//30번째 글자까지 출력
		mvwprintw(detailwin, 4, len + 1, "%c", memo_det[len]);
	}
	for (; len < 50; len++) {		//이후 50번째 마지막까지 출력
		mvwprintw(detailwin, 5, len - 29, "%c", memo_det[len]);
	}

	box(detailwin, 0, 0);			//지워진 테두리 다시그리기
	mvwprintw(detailwin, 0, 1, "Content");
	refresh();
	wrefresh(detailwin);

	getch();

	werase(detailwin);
	wrefresh(detailwin);
	delwin(detailwin);
	fclose(file);
}

void del_content(int selected_choice) {	//삭제 함수, 기본적으로 sort.c에서 파일을 읽어와서 배열에 저장하는 방식과 유사
	FILE* file;
	file = fopen("data.txt", "r");	//data.txt 파일 열기
	if (file == NULL) {			
		perror("Error opening file");
		refresh();
		return;
	}
	
	char buffer[150];		//data.txt의 한줄을 읽어서 저장할 buffer
	int capacity = 100; 	//초기 배열 크기, 100개 이상의 데이터를 다루기 위해 동적할당
	char** temp_arr = malloc(capacity * sizeof(char*)); //동적 메모리 할당
	if (temp_arr == NULL) {		//빈 경우 오류 출력
		perror("Memory alloc failed");
		fclose(file);
		return;
	}

	int i = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL) { //한 줄씩 읽어오기
		temp_arr[i] = malloc(strlen(buffer) + 1);	 //temp_arr 크기 동적할당
		if (temp_arr[i] == NULL) {
			perror("Memory allocation failed");
			fclose(file);
			return;
		}
		if (i >= capacity) {	//다음에 읽어올 데이터의 순서가 100번째를 넘어가면
			capacity *= 2;	//크기 늘려주기
			temp_arr = realloc(temp_arr, capacity * sizeof(char*));	//temp_arr재할당
			if (temp_arr == NULL) {
				perror("Memory allocation failed");
				fclose(file);
				return;
			}
		}
		/*selected_choice로 받아온 순서의 내용은 저장하지 않고 넘어간다, 해당 순서 뒷부분의 내용을 앞당겨 저장함으로 삭제*/
		if (i == selected_choice) {
			fgets(buffer, sizeof(buffer), file);
		}
		strcpy(temp_arr[i], buffer); //temp_arr 데이터 저장
		i++;
	}

	fclose(file); //파일 닫기
	
	//해당 항목이 없어진 내용을 저장
	file = fopen("data.txt", "w"); //data.txt 파일 쓰기 모드로 열기
	if (file == NULL) {
		perror("Unable to open file for writing");
		return;
	}

	for (int j = 0; j < i; j++) {		//읽어온 항목 수 만큼 다시 써주기
		fputs(temp_arr[j], file);
		free(temp_arr[j]); 		//동적 메모리 해제
	}

	free(temp_arr); //temp_arr에 할당된 메모리 해제
	fclose(file); //파일 닫기
}


int List(int y_max, int x_max) {		//data.txt에 저장된 데이터 출력
	FILE* file;
	char buffer[105];	//출력하기 위해 받은 데이터 저장할 buffer
	box(stdscr, 0, 0);				//테두리 그리기
	mvprintw(0, 1, "List");	//제목
	mvprintw(y_max - 1, 1, "Press f1 to EXIT, d to DELETE");
	refresh();

	file = fopen("data.txt", "r");	//파일 열기
	if (file == NULL) {
		perror("Error opening file");
		refresh();
		return -1;
	}

	//페이지 기능을 위한 변수
	int start_line = 0; 		//현재 페이지의 시작 줄
	int total_lines = 0; 		//총 줄 수 계산
	while (fgets(buffer, sizeof(buffer), file)) {	//한번 마지막까지 쭉 읽으며 크기 계산
		total_lines++;
	}
	rewind(file); //다시 처음부터 읽기 위해 되돌림

	char* listing_arr[y_max - 2];
	for (int k = 0; k < y_max - 2; k++) {
		listing_arr[k] = (char*)malloc(21 * sizeof(char));
	}

	while (1) {
		for (int i = 0; i < y_max - 2; i++) {			//일단 첫 페이지, 창의 크기만큼 데이터 출력
			if (!fgets(buffer, sizeof(buffer), file)) {	//파일에서 버퍼에 데이터 저장
				strcpy(listing_arr[i], "------");
				continue;					//다음 반복
			}
			char name_print[20] = {};	//이름을 받아와서 저장할 배열
			int j = 0;			//buffer의 내용을 한글자씩 읽기 위한 변수
			while ((buffer[j] != '\0') && (buffer[j] != ':')) {	//첫 ':'가 오기 전까지 읽고 저장
				name_print[j] = buffer[j];
				j++;
			}
			strcpy(listing_arr[i], name_print);
		}

		int selected_choice = list_selector(y_max - 2, listing_arr);	
		if (selected_choice == -1) {				//selector에서 f1인 경우에 받아온 0
			fclose(file);			//종료
			refresh();
			werase(stdscr);
			for (int k = 0; k < y_max - 2; k++) {
				free(listing_arr[k]);
			}
			return 0;
		} else if (selected_choice == -2) {		//왼쪽 화살표인 경우 받아온 -2
			start_line -= (y_max - 2);
			if (start_line < 0) {
				start_line = 0;
			}
			rewind(file);
			for (int i = 0; i < start_line; i++) {
				//읽어오는것을 반복함을 통해 다음에 읽기를 호출한다면 페이지의 첫 단어를 읽게 됨 
				fgets(buffer, sizeof(buffer), file);
			}
			continue;
			
		} else if (selected_choice == -3) {		//오른쪽 화살표인 경우 받아온 -3
			start_line += (y_max - 2);
			if (start_line >= total_lines) {
				start_line -= (y_max - 2);
			}
			rewind(file);
			for (int i = 0; i < start_line; i++) {
				//읽어오는것을 반복함을 통해 다음에 읽기를 호출한다면 페이지의 첫 단어를 읽게 됨 
				fgets(buffer, sizeof(buffer), file);
			}
			continue;
		} else if (selected_choice <= -10) {	//d를 눌렀을 때
			selected_choice = 0 - (selected_choice + 10);	//받아온 숫자 복원 => 삭제하고자 하는 데이터의 위치
			WINDOW* deletewin = newwin(3, 19, y_max / 2 - 1, x_max/ 2 - 5);	//삭제 알림
			box(deletewin, 0, 0);
			mvwprintw(deletewin, 1, 1, "delete? ( y / n )");		//삭제 의사 확인
			wrefresh(deletewin);
			refresh();
			int ans = wgetch(deletewin);
			if (ans == 'y') {				//확인 시 삭제 진행
				del_content(selected_choice);		//삭제 함수 호출
				mvwprintw(deletewin, 1, 1, "deleted.         ");	//삭제 알림
				wrefresh(deletewin);
				getch();
				werase(deletewin);
				delwin(deletewin);
				refresh();
				break;			//종료
			} else {		//y가 아닐 시 세부사항 출력하기
				detail(start_line + selected_choice, y_max, x_max);	//해당 순서의 데이터의 세부사항 출력
				fseek(file, 0, SEEK_SET);
				for (int i = 0; i < start_line; i++)
				fgets(buffer, sizeof(buffer), file);
				continue;
			}
		}
		detail(start_line + selected_choice, y_max, x_max);	//해당 순서의 데이터의 세부사항 출력
		rewind(file);
		for (int i = 0; i < start_line; i++)
			fgets(buffer, sizeof(buffer), file);
	}
	refresh();
	werase(stdscr);
}
