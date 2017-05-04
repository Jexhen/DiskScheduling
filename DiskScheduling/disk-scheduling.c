#include <stdlib.h>
#include <stdio.h>
#define REQUESTNUM 10
struct Track_ {
	int id;
	int distance;
	int flag;//是否已被访问的标记
	struct Track_ *next;
};
typedef struct Track_ Track;

struct TrackList {
	Track *head;
	Track *tail;
	int length;
};

struct TrackList *tra_list;
Track req_set[REQUESTNUM];

void init_track_list()
{
	tra_list = (struct TrackList*)malloc(sizeof(struct TrackList));
	tra_list->head = tra_list->tail = NULL;
	tra_list->length = 0;
}

void destroy()
{
	free(tra_list);
}

void generate_req()
{
	for (int i = 0; i < REQUESTNUM; i++) {
		if (i == 0) {
			req_set[i].id = rand() % 181 + 20;
		}
		else {
			while ((req_set[i].id = rand() % 181 + 20) == req_set[i - 1].id)
				;
		}
		req_set[i].distance = 0;
		req_set[i].flag = 0;
		req_set[i].next = NULL;
	}
}

void insert(Track *t)
{
	if (tra_list == NULL) {
		printf("磁道访问链表未被初始化!请初始化后再进行插入操作！\n");
		return;
	}
	if (t == NULL) {
		printf("插入的磁道不合法！请检查！");
		return;
	}
	if (tra_list->length == 0) {
		tra_list->head = t;
		tra_list->tail = t;
	}
	else {
		tra_list->tail->next = t;
		tra_list->tail = t;
	}
	(tra_list->length)++;
}

void print_list()
{
	Track *tmp = tra_list->head;
	double sum = 0;
	printf("-----------------------------\n");
	printf("下一个磁道号         移动距离\n");
	printf("-----------------------------\n");
	while (tmp != NULL) {
		printf("%6d %17d\n", tmp->id, tmp->distance);
		sum += tmp->distance;
		tmp = tmp->next;
	}
	printf("-----------------------------\n");
	printf("平均寻道长度: %f\n", sum / REQUESTNUM);
	printf("-----------------------------\n");
}

void FCFS()
{
	int current_id = 100;
	init_track_list();
	generate_req();

	for (int i = 0; i < REQUESTNUM; i++) {
		insert(&req_set[i]);
		req_set[i].distance = abs(current_id - req_set[i].id);
		req_set[i].flag = 1;
		current_id = req_set[i].id;
	}

	print_list();
	destroy();
}

void SSTF()
{
	int current_id = 100;
	init_track_list();
	generate_req();

	for (int i = 0; i < REQUESTNUM; i++) {
		int min_dis = 10000;
		Track *min_track = NULL;
		for (int j = 0; j < REQUESTNUM; j++) {
			if (req_set[j].flag == 0 && (abs(current_id - req_set[j].id) < min_dis)) {
				min_dis = abs(current_id - req_set[j].id);
				min_track = &req_set[j];
			}
		}
		if (min_track != NULL) {
			insert(min_track);
			min_track->distance = min_dis;
			min_track->flag = 1;
			current_id = min_track->id;
		}
	}

	print_list();
	destroy();
}

void sort_id()
{
	for (int i = 0; i < REQUESTNUM; i++) {
		Track *min = &req_set[i];
		for (int j = i + 1; j < REQUESTNUM; j++) {
			if (req_set[j].id < min->id) {
				min = &req_set[j];
			}
		}
		if (min != &req_set[i]) {
			int tmp = req_set[i].id;
			req_set[i].id = min->id;
			min->id = tmp;
		}
	}
}


void SCAN()
{
	int current_id = 100;
	char c;
	init_track_list();
	generate_req();
	sort_id();


	printf("请选择磁道初次访问方向:\n");
	printf("1. 磁道号增加的方向\n");
	printf("2. 磁道号递减的方向\n");
	c = getchar();
	if (c != '1' && c != '2') {
		printf("您的输入有误！Bye！");
		exit(-1);
	}
	else if (c == '1') {
		printf("****** 向磁道号增加的方向扫描 *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			if (req_set[i].id > current_id) {
				for (int j = i; j < REQUESTNUM; j++) {
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				for (int j = REQUESTNUM - 1; j >= 0; j--) {
					if (req_set[j].flag == 0) {
						insert(&req_set[j]);
						req_set[j].distance = abs(current_id - req_set[j].id);
						req_set[j].flag = 1;
						current_id = req_set[j].id;
					}
				}
				break;
			}
		}
	}
	else {
		printf("****** 向磁道号递减的方向扫描 *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			if (req_set[i].id > current_id) {
				for (int j = i - 1; j >= 0; j--) {
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				for (int j = 0; j < REQUESTNUM; j++) {
					if (req_set[j].flag == 0) {
						insert(&req_set[j]);
						req_set[j].distance = abs(current_id - req_set[j].id);
						req_set[j].flag = 1;
						current_id = req_set[j].id;
					}
				}
				break;
			}
		}
	}

	print_list();
	destroy();
}

void CSCAN()
{
	int current_id = 100;
	char c;
	init_track_list();
	generate_req();
	sort_id();


	printf("请选择磁道初次访问方向:\n");
	printf("1. 磁道号增加的方向\n");
	printf("2. 磁道号递减的方向\n");
	c = getchar();
	if (c != '1' && c != '2') {
		printf("您的输入有误！Bye！");
		exit(-1);
	}
	else if (c == '1') {
		printf("****** 向磁道号增加的方向扫描 *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			if (req_set[i].id > current_id) {
				for (int j = i; j < REQUESTNUM; j++) {
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				for (int j = 0; j < i; j++) {
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				break;
			}
		}
	}
	else {
		printf("****** 向磁道号递减的方向扫描 *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			if (req_set[i].id > current_id) {
				for (int j = i - 1; j >= 0 ; j--) {
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				for (int j = REQUESTNUM - 1; j >= i; j--) {
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				break;
			}
		}
	}
	
	print_list();
	destroy();
}

int main(int argc, char *argv[])
{
	//FCFS();
	//SSTF();
	SCAN();
	//CSCAN();
}