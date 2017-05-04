#include <stdlib.h>
#include <stdio.h>
#define REQUESTNUM 10
struct Track_ {
	int id;//磁道号
	int distance;//与上一访问磁道的距离
	int flag;//是否已被访问的标记
	struct Track_ *next;
};
typedef struct Track_ Track;

struct TrackList {
	Track *head;//链头
	Track *tail;//链尾
	int length;//链长
};

struct TrackList *tra_list;//访问磁道顺序链
Track req_set[REQUESTNUM];//请求集合

//初始化访问磁道次序链
void init_track_list()
{
	tra_list = (struct TrackList*)malloc(sizeof(struct TrackList));
	tra_list->head = tra_list->tail = NULL;
	tra_list->length = 0;
}

//析构函数
void destroy()
{
	free(tra_list);
}

//自动产生请求
void generate_req()
{
	for (int i = 0; i < REQUESTNUM; i++) {
		if (i == 0) {
			req_set[i].id = rand() % 181 + 20;//磁道号从20到200随机生成
		}
		else {
			while ((req_set[i].id = rand() % 181 + 20) == req_set[i - 1].id)
				;//为避免生成的随机数与上一请求磁道号相同
		}
		req_set[i].distance = 0;//与上一磁道的移动距离初始化为0
		req_set[i].flag = 0;//flag为0表明该请求未被完成
		req_set[i].next = NULL;
	}
}

//在访问磁道次序链插入元素
void insert(Track *t)
{
	//合法性检查
	if (tra_list == NULL) {
		printf("磁道访问链表未被初始化!请初始化后再进行插入操作！\n");
		return;
	}
	if (t == NULL) {
		printf("插入的磁道不合法！请检查！");
		return;
	}
	//如果链表没有元素
	if (tra_list->length == 0) {
		tra_list->head = t;
		tra_list->tail = t;
	}
	else {
	//链表有元素在队尾插入
		tra_list->tail->next = t;
		tra_list->tail = t;
	}
	(tra_list->length)++;//链表长度增加
}

//按访问顺序打印请求以及计算平均寻道长度
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

//先来先服务算法
void FCFS()
{
	int current_id = 100;//磁头初始位置
	init_track_list();//初始化磁道次序链
	generate_req();//生成请求
	
	//按照请求顺序插入链表，实现先来先服务
	for (int i = 0; i < REQUESTNUM; i++) {
		insert(&req_set[i]);
		req_set[i].distance = abs(current_id - req_set[i].id);
		req_set[i].flag = 1;
		current_id = req_set[i].id;
	}

	printf("---------- 先来先服务 ---------\n");
	print_list();//打印访问顺序以及平均寻道长度
	destroy();
}

void SSTF()
{
	int current_id = 100;//磁头初始位置
	init_track_list();//初始化链表
	generate_req();//生成请求

	for (int i = 0; i < REQUESTNUM; i++) {
		int min_dis = 10000;
		Track *min_track = NULL;
		//寻找离当前磁头最近的请求
		for (int j = 0; j < REQUESTNUM; j++) {
			if (req_set[j].flag == 0 && (abs(current_id - req_set[j].id) < min_dis)) {
				min_dis = abs(current_id - req_set[j].id);
				min_track = &req_set[j];
			}
		}
		//如果找到了
		if (min_track != NULL) {
			insert(min_track);//把该请求插入链表表明访问
			min_track->distance = min_dis;//该请求与磁头上个位置的距离设为最短距离
			min_track->flag = 1;//设置该请求已经被处理
			current_id = min_track->id;//当前磁头位置修改为该请求的位置
		}
	}

	printf("---------- 最短寻道时间优先 ---------\n");
	print_list();//打印访问顺序以及平均寻道长度
	destroy();
}

//根据磁道号将请求从小到大排序
void sort_id()
{
	//选择排序
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

//电梯扫描算法
void SCAN()
{
	int current_id = 100;//当前磁头位置
	char c;//用来接收用户的指令
	init_track_list();//初始化链表
	generate_req();//生成请求
	sort_id();//将请求按照磁道号从小到大排序

	//与用户交互的界面
	printf("请选择磁道初次访问方向:\n");
	printf("1. 磁道号增加的方向\n");
	printf("2. 磁道号递减的方向\n");
	getchar();//接收缓冲区留下的换行符
	c = getchar();
	if (c != '1' && c != '2') {
		printf("您的输入有误！Bye！");
		exit(-1);
	}
	else if (c == '1') {
	//向磁道号增加的方向扫描
		printf("****** 向磁道号增加的方向扫描 *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			if (req_set[i].id > current_id) {
				//向磁道号增加的方向扫描找到增加方向离磁头最近的请求
				//因为请求集合已经按照磁道号从小到大排序，所以找到的第一个大于当前磁头所在位置的请求就是递增方向离当前磁头最近的
				for (int j = i; j < REQUESTNUM; j++) {
					//从该请求开始向外扫描
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				//到头了，回转由外向里扫描
				for (int j = REQUESTNUM - 1; j >= 0; j--) {
					if (req_set[j].flag == 0) {
						insert(&req_set[j]);
						req_set[j].distance = abs(current_id - req_set[j].id);
						req_set[j].flag = 1;
						current_id = req_set[j].id;
					}
				}
				break;//循环的目的是为了找到递增方向离当前磁头最近的请求，一旦找到循环必须结束
			}
		}
	}
	else {
	//向磁道号递减的方向扫描
		printf("****** 向磁道号递减的方向扫描 *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			if (req_set[i].id > current_id) {
			//因为请求集合已经排序，所以一旦找到第一个比当前磁头所在位置大的请求，那么它的上一个请求就是递减方向离当前磁头最近的请求
				for (int j = i - 1; j >= 0; j--) {
					//按递减方向扫描
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				//递减到头了返回以递增方向扫描
				for (int j = 0; j < REQUESTNUM; j++) {
					if (req_set[j].flag == 0) {
						insert(&req_set[j]);
						req_set[j].distance = abs(current_id - req_set[j].id);
						req_set[j].flag = 1;
						current_id = req_set[j].id;
					}
				}
				break;//循环的目的是为了找到递减方向离当前磁头最近的请求，一旦找到循环必须结束
			}
		}
	}

	printf("---------- 电梯扫描算法 ---------\n");
	print_list();//打印访问顺序以及平均寻道长度
	destroy();
}

void CSCAN()
{
	int current_id = 100;//当前磁头位置
	char c;//用于接收用户指令
	init_track_list();//初始化链表
	generate_req();//生成请求
	sort_id();//根据磁道号从小到大对请求进行排序

	//实现与用户交互
	printf("请选择磁道初次访问方向:\n");
	printf("1. 磁道号增加的方向\n");
	printf("2. 磁道号递减的方向\n");
	getchar();//接收缓冲区留下的换行符
	c = getchar();
	if (c != '1' && c != '2') {
		printf("您的输入有误！Bye！");
		exit(-1);
	}
	else if (c == '1') {
		printf("****** 向磁道号增加的方向扫描 *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			//向磁道号增加的方向扫描找到增加方向离磁头最近的请求
			//因为请求集合已经按照磁道号从小到大排序，所以找到的第一个大于当前磁头所在位置的请求就是递增方向离当前磁头最近的请求
			if (req_set[i].id > current_id) {
				for (int j = i; j < REQUESTNUM; j++) {
					//按递增方向进行访问
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				//递增到头了，回到最低，继续递增访问
				for (int j = 0; j < REQUESTNUM; j++) {
					if (req_set[j].flag == 0) {
						insert(&req_set[j]);
						req_set[j].distance = abs(current_id - req_set[j].id);
						req_set[j].flag = 1;
						current_id = req_set[j].id;
					}
				}
				break;//循环的目的是为了找到递减方向离当前磁头最近的请求，一旦找到循环必须结束
			}
		}
	}
	else {
		printf("****** 向磁道号递减的方向扫描 *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			//因为请求集合已经排序，所以一旦找到第一个比当前磁头所在位置大的请求，那么它的上一个请求就是递减方向离当前磁头最近的请求
			if (req_set[i].id > current_id) {
				for (int j = i - 1; j >= 0 ; j--) {
					//向递减方向访问
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				//递减到底了，回到最高，继续递减访问
				for (int j = REQUESTNUM - 1; j >= 0; j--) {
					if (req_set[j].flag == 0) {
						insert(&req_set[j]);
						req_set[j].distance = abs(current_id - req_set[j].id);
						req_set[j].flag = 1;
						current_id = req_set[j].id;
					}
				}
				break;//循环的目的是为了找到递减方向离当前磁头最近的请求，一旦找到循环必须结束
			}
		}
	}
	
	printf("---------- 循环扫描算法 ---------\n");
	print_list();//打印访问顺序以及平均寻道长度
	destroy();
}

void menu()
{
	char c;
	printf("********************************************************\n");
	printf("*******************      MENU     **********************\n");
	printf("******************* a 先来先服务 ***********************\n");
	printf("******************* b 最短寻道时间优先 *****************\n");
	printf("******************* c 电梯扫描算法 *********************\n");
	printf("******************* d 循环扫描算法 *********************\n");
	printf("********************************************************\n");
	
	printf("\n");
	printf("请输入您的选项: \n");
	c = getchar();
	if (c != 'a' && c != 'b' && c != 'c' && c != 'd') {
		printf("您输入的选项有误！\n");
		exit(-1);
	}
	else {
		switch (c)
		{
		case 'a':
			FCFS();
			break;
		case 'b':
			SSTF();
			break;
		case 'c':
			SCAN();
			break;
		case 'd':
			CSCAN();
			break;
		default:
			break;
		}
	}
	
}

int main(int argc, char *argv[])
{
	menu();

	return 0;
}