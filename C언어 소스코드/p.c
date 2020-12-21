#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INF 999999999
#define MAX_VERTICES 9999999
typedef struct {
	int node;
	int weight;

}element;

typedef struct heap {
	element* heap;
	int heap_size;
}heap;//��Ÿ�� ����
typedef heap* heap_ptr;

//�Լ� ���� ���� ��
void heap_init(heap_ptr h);
heap_ptr create_heap();
void insert_min_heap(heap_ptr h, element item);
int is_empty(heap_ptr h);
element delete_min_heap(heap_ptr h);
//void dijkstra(GraphType* g, int* distances, int* selected, int start);




char* dijkstra(char *startStation,char *endStation) {
	char file_buff[200];
	char** key;
	int height = 200;

	key = (char**)malloc(sizeof(char*) * height);
	for (int i = 0; i < height; i++) {
		key[i] = (char*)malloc(sizeof(char) * 25);
	}

	FILE* fp;
	int size = 0;
	fp = fopen("final.json", "r");

	while (fgets(file_buff, sizeof(file_buff), fp) != NULL) {
		//printf("%s", file_buff);
		char* line = strtok(file_buff, "\"");
		line = strtok(NULL, "\"");
		strcpy(key[size], line);
		size++;
		if (size >= height) {
			int temp = height;
			height *= 2;
			key = (char**)realloc(key,sizeof(char*) * height);
			for (int i = temp; i < height; i++) {
				key[i] = (char*)malloc(sizeof(char) * 25);
			}
		}
	}
	int** weight;
	weight = (int**)malloc(sizeof(int*) * size);
	for (int i = 0; i < size; i++) {
		weight[i] = (int*)malloc(sizeof(int) * size);
		for (int j = 0; j < size; j++) {
			weight[i][j] = INF;
			if (i == j) {
				weight[i][j] = 0;
			}
		}
	}


	fclose(fp);
	fp = fopen("final.json", "r");
	int count = 0;
	while (fgets(file_buff, sizeof(file_buff), fp) != NULL) {
		//printf("%s", file_buff);
		file_buff[strlen(file_buff) - 1] = NULL;
		char* line = strtok(file_buff, "{");
		line = strtok(NULL, "},");
		while (line != NULL) {
			char stationName[20];
			//printf("line : %s ", line);
			int c;

			for (c = 1; line[c] != '\"'; c++) {
				stationName[c - 1] = line[c];
				stationName[c] = NULL;
			}
			//printf("�� �̸� %s ", stationName);
			char stationWeight[20];
			c += 3;
			int weightIndex = 0;
			for (; line[c] != NULL; c++) {
				//printf("%c", line[c]);
				stationWeight[weightIndex] = line[c];
				stationWeight[++weightIndex] = NULL;
			}
			//printf("����ġ : %s\n", stationWeight);
			line = strtok(NULL, "},");


			weight[count][find_key(key, stationName, size)] = atoi(stationWeight);
		}
		count++;
	}
	fclose(fp);
	heap_ptr h = create_heap();

	int* distances = (int*)malloc(sizeof(int) * size);
	int* selected = (int*)malloc(sizeof(int) * size);

	for (int i = 0; i < size; i++) { //�Ÿ� ���� �迭 �ʱ�ȭ
		distances[i] = INF;
		selected[i] = 0;
	}
	/*
	char startStation[25];
	printf("������ �Է� ! ");
	scanf("%s", startStation);
	printf("������ �Է� !");
	char endStation[25];
	scanf("%s", endStation);*/
	int start = find_key(key,startStation,size);
	int end = find_key(key, endStation, size);

	distances[start] = 0; //������ - ������ �Ÿ��� 0
	selected[start] = start; //���� ���� =
	element e = { start,distances[start] };

	insert_min_heap(h, e);//���� �̾��� ������ start�� �켱���� ť�� ����
	while (!is_empty(h)) {//���� ������� ������ ���� �ݺ�
		element delelted = delete_min_heap(h); //������ �Ÿ��� �ּ��� ������ �Ÿ��� ������
		int current_distance = delelted.weight; //������ �Ÿ�
		int current_node = delelted.node; //������ ���


		for (int adjacent = 0; adjacent < size; adjacent++) { //��������
			if (weight[current_node][adjacent] == INF || weight[current_node][adjacent] == 0) {
				//���� ������ �ƴѰ��� ����ġ�� �����̰ų�, �ڱ� �ڽ��� ���
				continue; //���� ������ �ƴϹǷ� �ٸ� ���� ������ ã��
			}

			int newweight = weight[current_node][adjacent];
			int distance = current_distance + newweight; //������ ������ �Ÿ� + ���������� �Ÿ� ��, �������� ������ ����
			if (distance < distances[adjacent]) { //�������� ���� ���̰� �迭�� ����Ȱͺ��� �۴ٸ�
				distances[adjacent] = distance; //�迭 �� ������Ʈ
				element t = { adjacent,distance }; //ť�� ������ �Ÿ� ����
				insert_min_heap(h, t);
				selected[adjacent] = current_node; //������ �湮 ��� ������Ʈ
			}
		}
	}
	free(h);
	char* returnData = (char*)malloc(sizeof(char) * 8000);
	char* temp = (char*)malloc(sizeof(char) * 90);
	sprintf(temp, "�ҿ�ð� : %d\n", distances[end]);
	strcpy(returnData, temp);
	//printf("����� �Ÿ� : %d\n", distances[end]);


	while (weight[end][selected[end]] != 0) { //���� ������ ����ġ�� 0�̸�
		//printf("%s - (%d) - ", key[end], weight[end][selected[end]]); //���� ������ ���� ������ ����ġ ���
		sprintf(temp,"%s - (%d) - ", key[end], weight[end][selected[end]]); //���� ������ ���� ������ ����ġ ���
		strcat(returnData, temp);
		end = selected[end]; //���� ������ ���������� �������� �湮�� �������� ����
	}
	strcat(returnData, key[end]);
	//printf("%s", key[end]);

	free(key);
	free(weight);
	free(distances);
	free(selected);
	free(temp);
	return returnData;
}







int find_key(char** key, char* string,int size) {
	for (int i = 0; i < size; i++) {
		if (strcmp(key[i], string) == 0) {
			return i;
		}
	}

}

void heap_init(heap_ptr h) { //�� �ʱ�ȭ �Լ�
	h->heap_size = 0; //���� ũ�⸦ 0���� ����
}
heap_ptr create_heap() { //�� ���� �Լ�
	heap_ptr h = (heap_ptr)malloc(sizeof(heap)); //�����Ҵ����� �� ������ ������ ��ȯ
	h->heap = (element*)malloc(sizeof(element) * MAX_VERTICES);
	heap_init(h);
	return h;
}

void insert_min_heap(heap_ptr h, element item) { //�ּ��� ����
	if ((h->heap_size + 1) == (MAX_VERTICES)) { //���� ���̻� ���� �Ұ����ϸ�
		printf("���̻� ���� �����Ҽ� �����ϴ�!");
		exit(0); //���α׷� ����
	}
	int i = ++(h->heap_size); //���� ���ԵǹǷ� 1 �߰�
	//�� �Ʒ��� �����ؾ� ������ i �� ���ϴ� ��ġ
	while (i != 1 && item.weight < h->heap[i / 2].weight) {
		//�θ� �� ũ�� ��ȯ�� ��� �����Ѵ�. ���⼱ ����ġ�� key
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = item;//����ġ�� ã������ �����͸� ����
}

int is_empty(heap_ptr h) {
	return (h->heap_size == 0);
}

element delete_min_heap(heap_ptr h) {//�ּ��� ����
	if (h->heap_size == 0) { //���� �����Ҽ� ������
		printf("���̻� ���� �����Ҽ� �����ϴ�!"); //����, ���α׷� ����
		exit(0);
	}
	int parent, child; //�θ� �ڽ� ����� �ε���
	element item, temp;
	item = h->heap[1]; //���� ���� ���
	temp = h->heap[(h->heap_size)--]; //������ �ø����
	//������ �Ǹ� ��尡 �Ѱ� ����������, h->heap_size �� 1�� ����.
	parent = 1;//�θ� ����� ��ġ
	child = 2;//�ڽ� ����� ��ġ. paret *2 �� �ڽĳ����
	while (child <= h->heap_size) { //�ڽĳ��� ���� ����� ���� �Ѿ�� �����ؾ���.
		if (child < h->heap_size && h->heap[child].weight > h->heap[child + 1].weight) { //����ġ�� ���⼱ key
		   //�ڽ� �����, ���� ��尡 �� ������ 1�� ���Ͽ� �������� ����
			child++;
		}
		if (temp.weight <= h->heap[child].weight) { //�ӽ� ��尡 �� ��ġ�� ã�ư�����, ����
			break;
		}
		h->heap[parent] = h->heap[child];//�θ� ���� �ڽ� ���� ��ȯ
		parent = child; //�θ� ���� �ڽ� ���� ��ȯ
		child *= 2;
	}
	h->heap[parent] = temp; //���� �θ� ��ġ�� temp �� ������ ���� ���������� ����
	return item; //��ȯ�� ������ ��ȯ
}
