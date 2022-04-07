#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME 20

typedef struct node{
    int v;
    struct node *next;
}TNode;

typedef struct deg{
    int indeg, outdeg;
}Degree;

typedef struct{
    int no_of_nodes;
    TNode **adjList;
    char **idx;
    Degree *d;
}TGraph;

typedef struct qNode{
    int key;
    struct qNode *next, *prev;
}QueueNode;

typedef struct q{
    QueueNode *head;
}Queue;

Queue *createQueue(void){
    Queue *newQueue = (Queue *)malloc(sizeof(Queue));
    newQueue->head = (QueueNode *)malloc(sizeof(QueueNode));
    newQueue->head->next = newQueue->head->prev = newQueue->head;

    return newQueue;
}

void destroyQueue(Queue *queue){
    QueueNode *it = queue->head->next;

    while(it != queue->head){
        QueueNode *aux = it;
        it = it->next;
        free(aux);
    }

    free(queue->head);
    free(queue);
}

void enqueue(Queue *queue, int key){
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    newNode->key = key;

	newNode->next = queue->head->next;
	newNode->prev = queue->head;

	queue->head->next->prev = newNode;
	queue->head->next = newNode;
}

void dequeue(Queue* queue){
	QueueNode* aux = queue->head->prev;
	queue->head->prev = queue->head->prev->prev;
	queue->head->prev->next = queue->head;
	free(aux);
}

int isQueueEmpty(Queue *queue){
    return queue->head->prev == queue->head;
}

int front(Queue* queue){
	return queue->head->prev->key;
}

TGraph* createGraphAdjList(int numberOfNodes) {
    TGraph *graph = (TGraph *) malloc(sizeof(TGraph));

	graph->no_of_nodes = numberOfNodes;
	graph->adjList =(TNode **)malloc(numberOfNodes * sizeof(TNode *));
    graph->idx = (char **)malloc(numberOfNodes * sizeof(char *));
    //matrice in care tinem minte numele nodurilor
    graph->d = (Degree *)malloc(numberOfNodes * sizeof(Degree));
    
	for(int i = 0; i  < numberOfNodes; i++){
		graph->adjList[i] = NULL;
        graph->idx[i] = (char *)malloc(NAME * sizeof(char));
        graph->d[i].indeg = graph->d[i].outdeg = 0;
    }

	return graph;
}

void addEdge(TGraph* graph, char *name1, char *name2){
    
    TNode *n1;
    n1 = (TNode *)malloc(sizeof(TNode));
    int i = 0,idx1 = -1, idx2 = -1;
    

    while ((idx1 == -1 || idx2 == -1) && i < graph->no_of_nodes){
        if(strcmp(graph->idx[i], name1) == 0)
            idx1 = i;
        
        if(strcmp(graph->idx[i], name2) == 0)
            idx2 = i;
        
        i++;
    }

    if(idx1 != -1 && idx2 != -1){
    n1->v = idx2;
    n1->next = graph->adjList[idx1];
    graph->adjList[idx1] = n1;

    graph->d[idx2].indeg ++;
    graph->d[idx1].outdeg ++; 
    }
}

void removeEdgeList(TGraph* graph, int v1, int v2){
	TNode* it = graph->adjList[v1];
	TNode* prev = NULL;
	while(it != NULL && it->v != v2){
		prev = it;
		it = it->next;
	}

	if(it == NULL) return;

	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adjList[v1] = it->next;
        
    graph->d[v1].outdeg --;
    graph->d[v2].indeg --;
	free(it);
}


void destroyGraphAdjList(TGraph* graph){
	for(int i = 0; i < graph->no_of_nodes; i++){
		while(graph->adjList[i] != NULL){
			TNode* aux = graph->adjList[i];
			graph->adjList[i] = graph->adjList[i]->next;
			free(aux);
		}
	}
	free(graph->adjList);
    for (int i = 0; i < graph->no_of_nodes; i++){
        free(graph->idx[i]);
    }
    free(graph->idx);
    free(graph->d);
	free(graph);
}
