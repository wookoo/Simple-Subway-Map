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
}heap;//힙타입 정의
typedef heap* heap_ptr;

//함수 원형 정의 부
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
			//printf("역 이름 %s ", stationName);
			char stationWeight[20];
			c += 3;
			int weightIndex = 0;
			for (; line[c] != NULL; c++) {
				//printf("%c", line[c]);
				stationWeight[weightIndex] = line[c];
				stationWeight[++weightIndex] = NULL;
			}
			//printf("가중치 : %s\n", stationWeight);
			line = strtok(NULL, "},");


			weight[count][find_key(key, stationName, size)] = atoi(stationWeight);
		}
		count++;
	}
	fclose(fp);
	heap_ptr h = create_heap();

	int* distances = (int*)malloc(sizeof(int) * size);
	int* selected = (int*)malloc(sizeof(int) * size);

	for (int i = 0; i < size; i++) { //거리 저장 배열 초기화
		distances[i] = INF;
		selected[i] = 0;
	}
	/*
	char startStation[25];
	printf("시작점 입력 ! ");
	scanf("%s", startStation);
	printf("도착점 입력 !");
	char endStation[25];
	scanf("%s", endStation);*/
	int start = find_key(key,startStation,size);
	int end = find_key(key, endStation, size);

	distances[start] = 0; //시작점 - 시작점 거리는 0
	selected[start] = start; //선택 정점 =
	element e = { start,distances[start] };

	insert_min_heap(h, e);//현재 이어진 정점인 start를 우선순위 큐에 삽입
	while (!is_empty(h)) {//힙이 비어있지 않을때 까지 반복
		element delelted = delete_min_heap(h); //힙에서 거리가 최소인 정점과 거리를 가져옴
		int current_distance = delelted.weight; //가져온 거리
		int current_node = delelted.node; //가져온 노드


		for (int adjacent = 0; adjacent < size; adjacent++) { //인접정점
			if (weight[current_node][adjacent] == INF || weight[current_node][adjacent] == 0) {
				//인접 정점이 아닌경우는 가중치가 무한이거나, 자기 자신인 경우
				continue; //인접 정점이 아니므로 다른 인접 정점을 찾음
			}

			int newweight = weight[current_node][adjacent];
			int distance = current_distance + newweight; //힙에서 가져온 거리 + 인접정점의 거리 즉, 인접정점 경유한 길이
			if (distance < distances[adjacent]) { //인접정점 경유 길이가 배열에 저장된것보다 작다면
				distances[adjacent] = distance; //배열 값 업데이트
				element t = { adjacent,distance }; //큐에 정점과 거리 삽입
				insert_min_heap(h, t);
				selected[adjacent] = current_node; //마지막 방문 노드 업데이트
			}
		}
	}
	free(h);
	char* returnData = (char*)malloc(sizeof(char) * 8000);
	char* temp = (char*)malloc(sizeof(char) * 90);
	sprintf(temp, "소요시간 : %d\n", distances[end]);
	strcpy(returnData, temp);
	//printf("사용한 거리 : %d\n", distances[end]);


	while (weight[end][selected[end]] != 0) { //다음 정점의 가중치가 0이면
		//printf("%s - (%d) - ", key[end], weight[end][selected[end]]); //현재 정점과 다음 정점의 가중치 출력
		sprintf(temp,"%s - (%d) - ", key[end], weight[end][selected[end]]); //현재 정점과 다음 정점의 가중치 출력
		strcat(returnData, temp);
		end = selected[end]; //현재 정점은 현재정점이 마지막에 방문한 정점으로 수정
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

void heap_init(heap_ptr h) { //힙 초기화 함수
	h->heap_size = 0; //힙의 크기를 0으로 설정
}
heap_ptr create_heap() { //힙 생성 함수
	heap_ptr h = (heap_ptr)malloc(sizeof(heap)); //동적할당으로 힙 생성후 포인터 반환
	h->heap = (element*)malloc(sizeof(element) * MAX_VERTICES);
	heap_init(h);
	return h;
}

void insert_min_heap(heap_ptr h, element item) { //최소힙 삽입
	if ((h->heap_size + 1) == (MAX_VERTICES)) { //힙을 더이상 삽입 불가능하면
		printf("더이상 힙에 삽입할수 없습니다!");
		exit(0); //프로그램 종료
	}
	int i = ++(h->heap_size); //힙이 삽입되므로 1 추가
	//맨 아래에 삽입해야 함으로 i 는 최하단 위치
	while (i != 1 && item.weight < h->heap[i / 2].weight) {
		//부모가 더 크면 교환을 계속 진행한다. 여기선 가중치가 key
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = item;//정위치를 찾았으면 데이터를 삽입
}

int is_empty(heap_ptr h) {
	return (h->heap_size == 0);
}

element delete_min_heap(heap_ptr h) {//최소힙 삭제
	if (h->heap_size == 0) { //힙을 삭제할수 없으면
		printf("더이상 힙을 삭제할수 없습니다!"); //오류, 프로그램 종료
		exit(0);
	}
	int parent, child; //부모 자식 노드의 인덱스
	element item, temp;
	item = h->heap[1]; //삭제 당할 노드
	temp = h->heap[(h->heap_size)--]; //상위로 올릴노드
	//삭제가 되면 노드가 한개 삭제됨으로, h->heap_size 에 1을 뺀다.
	parent = 1;//부모 노드의 위치
	child = 2;//자식 노드의 위치. paret *2 가 자식노드임
	while (child <= h->heap_size) { //자식노드는 힙에 저장된 값을 넘어가면 종료해야함.
		if (child < h->heap_size && h->heap[child].weight > h->heap[child + 1].weight) { //가중치가 여기선 key
		   //자식 노드중, 우측 노드가 더 작으면 1을 더하여 우측노드로 설정
			child++;
		}
		if (temp.weight <= h->heap[child].weight) { //임시 노드가 제 위치를 찾아갔으면, 종료
			break;
		}
		h->heap[parent] = h->heap[child];//부모 노드랑 자식 노드랑 교환
		parent = child; //부모 노드랑 자식 노드랑 교환
		child *= 2;
	}
	h->heap[parent] = temp; //현재 부모 위치에 temp 를 씌워서 값을 정상적으로 설정
	return item; //반환할 데이터 반환
}
