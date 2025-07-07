# 상명대학교 2024학년도 1학기 C programming 과제
## Ncurses를 이용한 전화번호부 구현
### 프로그램 요약
![main](https://github.com/GhKTW/C-programming-PhoneBook-linux/blob/main/image/main.png)

### 파일 컴파일 및 실행
- 컴파일:
  gcc –o tel main.c list.c search.c add.c sort.c –lncurses –lform
- 실행
  ./tel

### 1. main.c
![main_page](https://github.com/GhKTW/C-programming-PhoneBook-linux/blob/main/image/main_page.png)
-int main()
: stdscr에 menu_open을 호출하여 명령을 선택할 수 있게 함. 반복문 속에서 선택된 각 명령을 수행하고 나오면 다시 menu_open을 실행하도록 하여 초기 화면을 구축함.

-int menu_open()
: List, Search 등 내릴 명령을 선택할 수 있는 선택창을 생성, 엔터를 입력하면 choice 변수를 통해 선택한 항목에 대한 integer 반환

### 2. list.c
-int List()
•리스트 출력
	: 파일을 열어서 fgets 함수를 반복적으로 수행하면서 데이터 중 이름 부분만을 선택해서 화면에 출력한다.
각 줄을 fgets 하면서 받아온 내용이 없으면 ‘------’를 출력
•출력된 이름 중 특정 항목 선택
	: 해당 이름 데이터의 세부 내용을 출력
•왼, 오른쪽 화살표 입력 받기
	: 페이지 넘김 기능 실행
•특정 이름에 대해 d 입력
	: 삭제 여부를 물어본 후 삭제/세부내용 출력 실행

-int list_selector()
	: 메뉴와 같은 방식 사용하여 각 항목을 선택하고, 선택한 항목의 순서를 반환

-void detail()
	: 선택된 항목의 순서를 받아와서 data.txt 파일의 해당 순서의 내용을 받아와 name, number, memo를 조각내서 세부 내용을 출력

-void del_content()
	: 삭제하기로 선택된 항목의 순서를 받아와서 data.txt 파일의 해당 순서 이후의 내용을 한칸 당겨서 저장함으로써 해당 데이터를 삭제

### 3. search.c
-int search()
	: 필드를 사용해 이름을 입력받아 해당 내용을 찾는 함수, 찾는 내용이 있다면 해당 내용의 세부사항을 출력, 없다면 오류를 출력. 방식은 순차 검색 사용

-int search_where()
	: 받아온 이름 데이터를 바탕으로 해당 이름의 데이터가 data.txt에서 몇 번째 줄에 있는지 받아오는 함수
fgets로 받아온 각 줄에서 ‘:’ 전까지의 내용만 가지고 와서 이름 데이터와 비교를 통해 검색을 진행

### 4. add.c
-void add_form()
	: 데이터 추가를 위해 form을 사용해 name field, number field, memo field를 생성, 각 필드에서 받아온 데이터를 하나의 ‘:’로 구분된 문자열로 합침, 저장을 진행, 저장 후 정렬을 진행한다.
 입력이 없었다면 데이터 저장을 취소하고 종료.

-void trin_whitespace()
	: 데이터를 합치고 저장 할 때 불필요한 공백을 제거하고 마지막에 ‘\0’를 추가해준다.


### 5. sort.c
-int sort()
	:data.txt 파일의 내용을 모두 sorting_arr에 저장한다. sorting_arr의 크기는 100개 이상의 내용을 다루기 위해 동적 할당을 사용한다.
 모든 데이터가 든 sorting_arr에 대해 stdlib 헤더의 qsort 함수를 실행, 정렬된 sorting_arr를 다시 data.txt에 저장하고 종료

-int compare()
	:qsort 함수를 실행함에 필요한 비교 함수, strcmp 함수를 통해 비교한다.
