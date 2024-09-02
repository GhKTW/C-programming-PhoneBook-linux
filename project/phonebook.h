#ifndef MYHEADER_H
#define MYHEADER_H

int list_selector(int size, char* listing_arr[]);
void detail(int selected, int y_max, int x_max);
void del_content(int selected_choice);
int List(int y_max, int x_max);
void print_list(int i, FILE* file, int y_max, int x_max);
void trim_whitespace(char *str);
void add_form();
int compare(const void *a, const void *b);
int sort(void);
int search(int y_max, int x_max);
int search_where(char want_sch[]);

#endif
