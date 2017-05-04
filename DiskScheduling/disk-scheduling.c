#include <stdlib.h>
#include <stdio.h>
#define REQUESTNUM 10
struct Track_ {
	int id;//�ŵ���
	int distance;//����һ���ʴŵ��ľ���
	int flag;//�Ƿ��ѱ����ʵı��
	struct Track_ *next;
};
typedef struct Track_ Track;

struct TrackList {
	Track *head;//��ͷ
	Track *tail;//��β
	int length;//����
};

struct TrackList *tra_list;//���ʴŵ�˳����
Track req_set[REQUESTNUM];//���󼯺�

//��ʼ�����ʴŵ�������
void init_track_list()
{
	tra_list = (struct TrackList*)malloc(sizeof(struct TrackList));
	tra_list->head = tra_list->tail = NULL;
	tra_list->length = 0;
}

//��������
void destroy()
{
	free(tra_list);
}

//�Զ���������
void generate_req()
{
	for (int i = 0; i < REQUESTNUM; i++) {
		if (i == 0) {
			req_set[i].id = rand() % 181 + 20;//�ŵ��Ŵ�20��200�������
		}
		else {
			while ((req_set[i].id = rand() % 181 + 20) == req_set[i - 1].id)
				;//Ϊ�������ɵ����������һ����ŵ�����ͬ
		}
		req_set[i].distance = 0;//����һ�ŵ����ƶ������ʼ��Ϊ0
		req_set[i].flag = 0;//flagΪ0����������δ�����
		req_set[i].next = NULL;
	}
}

//�ڷ��ʴŵ�����������Ԫ��
void insert(Track *t)
{
	//�Ϸ��Լ��
	if (tra_list == NULL) {
		printf("�ŵ���������δ����ʼ��!���ʼ�����ٽ��в��������\n");
		return;
	}
	if (t == NULL) {
		printf("����Ĵŵ����Ϸ������飡");
		return;
	}
	//�������û��Ԫ��
	if (tra_list->length == 0) {
		tra_list->head = t;
		tra_list->tail = t;
	}
	else {
	//������Ԫ���ڶ�β����
		tra_list->tail->next = t;
		tra_list->tail = t;
	}
	(tra_list->length)++;//����������
}

//������˳���ӡ�����Լ�����ƽ��Ѱ������
void print_list()
{
	Track *tmp = tra_list->head;
	double sum = 0;
	printf("-----------------------------\n");
	printf("��һ���ŵ���         �ƶ�����\n");
	printf("-----------------------------\n");
	while (tmp != NULL) {
		printf("%6d %17d\n", tmp->id, tmp->distance);
		sum += tmp->distance;
		tmp = tmp->next;
	}
	printf("-----------------------------\n");
	printf("ƽ��Ѱ������: %f\n", sum / REQUESTNUM);
	printf("-----------------------------\n");
}

//�����ȷ����㷨
void FCFS()
{
	int current_id = 100;//��ͷ��ʼλ��
	init_track_list();//��ʼ���ŵ�������
	generate_req();//��������
	
	//��������˳���������ʵ�������ȷ���
	for (int i = 0; i < REQUESTNUM; i++) {
		insert(&req_set[i]);
		req_set[i].distance = abs(current_id - req_set[i].id);
		req_set[i].flag = 1;
		current_id = req_set[i].id;
	}

	printf("---------- �����ȷ��� ---------\n");
	print_list();//��ӡ����˳���Լ�ƽ��Ѱ������
	destroy();
}

void SSTF()
{
	int current_id = 100;//��ͷ��ʼλ��
	init_track_list();//��ʼ������
	generate_req();//��������

	for (int i = 0; i < REQUESTNUM; i++) {
		int min_dis = 10000;
		Track *min_track = NULL;
		//Ѱ���뵱ǰ��ͷ���������
		for (int j = 0; j < REQUESTNUM; j++) {
			if (req_set[j].flag == 0 && (abs(current_id - req_set[j].id) < min_dis)) {
				min_dis = abs(current_id - req_set[j].id);
				min_track = &req_set[j];
			}
		}
		//����ҵ���
		if (min_track != NULL) {
			insert(min_track);//�Ѹ�������������������
			min_track->distance = min_dis;//���������ͷ�ϸ�λ�õľ�����Ϊ��̾���
			min_track->flag = 1;//���ø������Ѿ�������
			current_id = min_track->id;//��ǰ��ͷλ���޸�Ϊ�������λ��
		}
	}

	printf("---------- ���Ѱ��ʱ������ ---------\n");
	print_list();//��ӡ����˳���Լ�ƽ��Ѱ������
	destroy();
}

//���ݴŵ��Ž������С��������
void sort_id()
{
	//ѡ������
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

//����ɨ���㷨
void SCAN()
{
	int current_id = 100;//��ǰ��ͷλ��
	char c;//���������û���ָ��
	init_track_list();//��ʼ������
	generate_req();//��������
	sort_id();//�������մŵ��Ŵ�С��������

	//���û������Ľ���
	printf("��ѡ��ŵ����η��ʷ���:\n");
	printf("1. �ŵ������ӵķ���\n");
	printf("2. �ŵ��ŵݼ��ķ���\n");
	getchar();//���ջ��������µĻ��з�
	c = getchar();
	if (c != '1' && c != '2') {
		printf("������������Bye��");
		exit(-1);
	}
	else if (c == '1') {
	//��ŵ������ӵķ���ɨ��
		printf("****** ��ŵ������ӵķ���ɨ�� *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			if (req_set[i].id > current_id) {
				//��ŵ������ӵķ���ɨ���ҵ����ӷ������ͷ���������
				//��Ϊ���󼯺��Ѿ����մŵ��Ŵ�С�������������ҵ��ĵ�һ�����ڵ�ǰ��ͷ����λ�õ�������ǵ��������뵱ǰ��ͷ�����
				for (int j = i; j < REQUESTNUM; j++) {
					//�Ӹ�����ʼ����ɨ��
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				//��ͷ�ˣ���ת��������ɨ��
				for (int j = REQUESTNUM - 1; j >= 0; j--) {
					if (req_set[j].flag == 0) {
						insert(&req_set[j]);
						req_set[j].distance = abs(current_id - req_set[j].id);
						req_set[j].flag = 1;
						current_id = req_set[j].id;
					}
				}
				break;//ѭ����Ŀ����Ϊ���ҵ����������뵱ǰ��ͷ���������һ���ҵ�ѭ���������
			}
		}
	}
	else {
	//��ŵ��ŵݼ��ķ���ɨ��
		printf("****** ��ŵ��ŵݼ��ķ���ɨ�� *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			if (req_set[i].id > current_id) {
			//��Ϊ���󼯺��Ѿ���������һ���ҵ���һ���ȵ�ǰ��ͷ����λ�ô��������ô������һ��������ǵݼ������뵱ǰ��ͷ���������
				for (int j = i - 1; j >= 0; j--) {
					//���ݼ�����ɨ��
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				//�ݼ���ͷ�˷����Ե�������ɨ��
				for (int j = 0; j < REQUESTNUM; j++) {
					if (req_set[j].flag == 0) {
						insert(&req_set[j]);
						req_set[j].distance = abs(current_id - req_set[j].id);
						req_set[j].flag = 1;
						current_id = req_set[j].id;
					}
				}
				break;//ѭ����Ŀ����Ϊ���ҵ��ݼ������뵱ǰ��ͷ���������һ���ҵ�ѭ���������
			}
		}
	}

	printf("---------- ����ɨ���㷨 ---------\n");
	print_list();//��ӡ����˳���Լ�ƽ��Ѱ������
	destroy();
}

void CSCAN()
{
	int current_id = 100;//��ǰ��ͷλ��
	char c;//���ڽ����û�ָ��
	init_track_list();//��ʼ������
	generate_req();//��������
	sort_id();//���ݴŵ��Ŵ�С����������������

	//ʵ�����û�����
	printf("��ѡ��ŵ����η��ʷ���:\n");
	printf("1. �ŵ������ӵķ���\n");
	printf("2. �ŵ��ŵݼ��ķ���\n");
	getchar();//���ջ��������µĻ��з�
	c = getchar();
	if (c != '1' && c != '2') {
		printf("������������Bye��");
		exit(-1);
	}
	else if (c == '1') {
		printf("****** ��ŵ������ӵķ���ɨ�� *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			//��ŵ������ӵķ���ɨ���ҵ����ӷ������ͷ���������
			//��Ϊ���󼯺��Ѿ����մŵ��Ŵ�С�������������ҵ��ĵ�һ�����ڵ�ǰ��ͷ����λ�õ�������ǵ��������뵱ǰ��ͷ���������
			if (req_set[i].id > current_id) {
				for (int j = i; j < REQUESTNUM; j++) {
					//������������з���
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				//������ͷ�ˣ��ص���ͣ�������������
				for (int j = 0; j < REQUESTNUM; j++) {
					if (req_set[j].flag == 0) {
						insert(&req_set[j]);
						req_set[j].distance = abs(current_id - req_set[j].id);
						req_set[j].flag = 1;
						current_id = req_set[j].id;
					}
				}
				break;//ѭ����Ŀ����Ϊ���ҵ��ݼ������뵱ǰ��ͷ���������һ���ҵ�ѭ���������
			}
		}
	}
	else {
		printf("****** ��ŵ��ŵݼ��ķ���ɨ�� *******\n");
		for (int i = 0; i < REQUESTNUM; i++) {
			//��Ϊ���󼯺��Ѿ���������һ���ҵ���һ���ȵ�ǰ��ͷ����λ�ô��������ô������һ��������ǵݼ������뵱ǰ��ͷ���������
			if (req_set[i].id > current_id) {
				for (int j = i - 1; j >= 0 ; j--) {
					//��ݼ��������
					insert(&req_set[j]);
					req_set[j].distance = abs(current_id - req_set[j].id);
					req_set[j].flag = 1;
					current_id = req_set[j].id;
				}
				//�ݼ������ˣ��ص���ߣ������ݼ�����
				for (int j = REQUESTNUM - 1; j >= 0; j--) {
					if (req_set[j].flag == 0) {
						insert(&req_set[j]);
						req_set[j].distance = abs(current_id - req_set[j].id);
						req_set[j].flag = 1;
						current_id = req_set[j].id;
					}
				}
				break;//ѭ����Ŀ����Ϊ���ҵ��ݼ������뵱ǰ��ͷ���������һ���ҵ�ѭ���������
			}
		}
	}
	
	printf("---------- ѭ��ɨ���㷨 ---------\n");
	print_list();//��ӡ����˳���Լ�ƽ��Ѱ������
	destroy();
}

void menu()
{
	char c;
	printf("********************************************************\n");
	printf("*******************      MENU     **********************\n");
	printf("******************* a �����ȷ��� ***********************\n");
	printf("******************* b ���Ѱ��ʱ������ *****************\n");
	printf("******************* c ����ɨ���㷨 *********************\n");
	printf("******************* d ѭ��ɨ���㷨 *********************\n");
	printf("********************************************************\n");
	
	printf("\n");
	printf("����������ѡ��: \n");
	c = getchar();
	if (c != 'a' && c != 'b' && c != 'c' && c != 'd') {
		printf("�������ѡ������\n");
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