// sort.c
// qsort를 사용한 전화번호부 내용 정렬 파일

#include <stdlib.h>
#include <string.h>

//비교 함수
int compare(const void* p, const void* q)
{
	return strcmp(*(const char**)p, *(const char**)q);
}

int sort(void)
{
	FILE* file = fopen("data.txt", "r"); //data.txt 파일 열기
	if (file == NULL) {
		perror("Empty file");	//비어 있는 경우 오류 출력
		return 1;
	}

	char buffer[150];		//data.txt의 한줄을 읽어서 저장할 buffer
	int capacity = 100; 	//초기 배열 크기, 100개 이상의 데이터를 다루기 위해 동적할당
	char** sorting_arr = malloc(capacity * sizeof(char*)); //동적 메모리 할당
	if (sorting_arr == NULL) {		//빈 경우 오류 출력
		perror("Memory alloc failed");
		fclose(file);
		return 1;
	}

	int i = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL) { //한 줄씩 읽어오기
		sorting_arr[i] = malloc(strlen(buffer) + 1);	 //sorting_arr의 크기 동적할당
		if (sorting_arr[i] == NULL) {
			perror("Memory allocation failed");
			fclose(file);
			return 1;
		}
		if (i >= capacity) {	//다음에 읽어올 데이터의 순서가 100번째를 넘어가면
			capacity *= 2;	//크기 늘려주기
			sorting_arr = realloc(sorting_arr, capacity * sizeof(char*));	//sorting_arr재할당
			if (sorting_arr == NULL) {
				perror("Memory allocation failed");
				fclose(file);
				return 1;
			}
		}
			
		strcpy(sorting_arr[i], buffer); //sorting_arr에 데이터 저장
		i++;
		}

	fclose(file); //파일 닫기

	//정렬 수행
	qsort(sorting_arr, i, sizeof(char*), compare);

	//정렬된 결과를 data.txt에 다시 저장
	file = fopen("data.txt", "w"); //data.txt 파일 쓰기 모드로 열기
	if (file == NULL) {
		perror("Unable to open file for writing");
		return 1;
	}

	for (int j = 0; j < i; j++) {		//읽어온 항목 수 만큼 다시 써주기
		fputs(sorting_arr[j], file);
		free(sorting_arr[j]); 		//동적 메모리 해제
	}

	free(sorting_arr); //sorting_arr에 할당된 메모리 해제
	fclose(file); //파일 닫기

	return 0;
}
