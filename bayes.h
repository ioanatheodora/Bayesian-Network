#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

TGraph *citire(FILE *f){
    int n, m;//n noduri m muchii
    fscanf(f, "%d %d\n", &n, &m);
    TGraph *graph = createGraphAdjList(n);

    for(int i = 0; i < n; i++){
        fscanf(f, "%s ", graph->idx[i]);
    }

    char *node1, *node2;

    node1 =(char *)malloc(NAME * sizeof(char));
    node2 =(char *)malloc(NAME * sizeof(char));

    for(int i = 0; i < m; i++){
        fscanf(f, "%s %s\n", node1, node2);
        addEdge(graph, node1, node2);
    }

    free(node1);
    free(node2);

    return graph;
}

int is_path(TGraph *graph, int node1, int node2){
    if(node1 == node2) return 0;

    int *visit = (int *)calloc(graph->no_of_nodes, sizeof(int));

    Queue *queue = createQueue();
    visit[node1] = 1;
    enqueue(queue, node1);

    int it;
    while(!isQueueEmpty(queue)){
        it = front(queue);
        dequeue(queue);

        for(TNode *i = graph->adjList[it]; i != NULL; i = i->next){
            if(i->v == node2){
                free(visit);
                destroyQueue(queue);
               return 1;
               }
            
            if (visit[i->v] == 0)
            {
                visit[i->v] = 1;
                enqueue(queue, i->v);
            }
            
        }
    }
    free(visit);
    destroyQueue(queue);
    
    return 0;
}

int interogations(char *line, char ***var){
    
    char *token;
    const char str[4] = " |;";
    int no_int = 0;
    token = strtok(line, str);
    (*var)[0] = token;
    while(token != NULL){
        token = strtok(NULL, str);
        if (token != NULL)
        {
            no_int++;
            (*var)[no_int] = token;
        }
    }
    no_int = no_int;

    return no_int;
}

int *index_nodes(TGraph *graph, char **var, int no_int){
    int *index = (int *)malloc(sizeof(int) * (no_int + 2));
    for(int i = 0; i < no_int + 1; i++){
        for(int j = 0; j < graph->no_of_nodes; j++){
            if(strcmp(var[i], graph->idx[j]) == 0)
                {index[i] = j;
                break;
                }
        }
    }
    return index;
}

void getAncestors(TGraph *graph, int *index, int **ancestors, int no_int,\
                     int *comp){
    (*ancestors) = (int *)malloc(graph->no_of_nodes * sizeof(int));
    *comp = 0;
    int *visited =(int *) calloc(graph->no_of_nodes, sizeof(int));
    
    for(int i = 0; i < graph->no_of_nodes; i++){
        (*ancestors)[i] = -1;
    }
    for(int i = 0; i < no_int + 1; i++){
        if(!visited[index[i]]){
            (*ancestors)[i] = index[i];
            (*comp) ++;
        }
        visited[index[i]] = 1;
    }
    int c = *comp;
    for(int i = 0; i < c; i++){
        for(int j = 0; j < graph->no_of_nodes; j++){
            if(is_path(graph, j, index[i]) && !visited[j])
                {(*ancestors)[*comp] = j;
                (*comp)++;
                visited[j] = 1;
                }
        }
    }

    free(visited);

}

int exists_edge(TGraph *graph, int n1, int n2){
    for(TNode *it = graph->adjList[n1]; it != NULL; it = it->next){
        if(n2 == it->v)
            return 1;
    }
    return 0;
}

TGraph *elim_nodes(TGraph *graph, int *ancestors, int comp, int *nodes_elim){
    TGraph *aux = createGraphAdjList(graph->no_of_nodes);
    for(int i = 0; i < graph->no_of_nodes; i++){
        nodes_elim[i] = 1;
        //presupunem ca toate nodurile au fost eliminate
        strcpy(aux->idx[i], graph->idx[i]);
    }
    for(int i = 0; i < comp; i++){
        nodes_elim[ancestors[i]] = 0;
        // setam nodurile care n au fost eliminate
    }
    for(int i = 0; i < aux->no_of_nodes; i++){
        for(TNode *it = graph->adjList[i]; it != NULL; it = it->next){
            addEdge(aux, graph->idx[i], graph->idx[it->v]);
        }
        
    }

    int exists;

    for(int i = 0; i < aux->no_of_nodes; i++){
        exists = 0;
        for(int j = 0; j < comp; j++)
            if(i == ancestors[j])
                exists = 1;

        if(exists == 0){
            for(int j = 0; j < aux->no_of_nodes; j++){
            removeEdgeList(aux, i, j);
            removeEdgeList(aux, j, i);
            }
        }
         
    }
    return aux;
}

void m_graph(TGraph *graph, int *nodes_elim){
    int matrix[3][2 * graph->no_of_nodes], c = 0;
    for(int i = 0; i < graph->no_of_nodes; i++){
        if(nodes_elim[i] == 0)//daca exista nodul in graf
        {
            if(graph->d[i].indeg >= 2){//daca are cel putin 2 parinti
                for(int j = 0; j < graph->no_of_nodes - 1; j++){
                    if(exists_edge(graph, j, i)){//daca j este parinte
                        for(int k = j + 1; k < graph->no_of_nodes; k++){
                            if(exists_edge(graph, k, i)){
                                if(!exists_edge(graph, j, k))
                                   {
                                    matrix[1][c] = j;
                                    matrix[2][c] = k;
                                    c++;
                                   }
                                if(!exists_edge(graph, k, j))
                                {
                                    matrix[1][c] = k;
                                    matrix[2][c] = j;
                                    c++;
                                }
                            }
                        }
                    }
                }
                    
            }
        }
    }

    for(int i = 0; i < c; i++){
        if(!exists_edge(graph, matrix[1][i], matrix[2][i]))
            addEdge(graph, graph->idx[matrix[1][i]], graph->idx[matrix[2][i]]);
    }
}

void turn_undirected(TGraph *graph, int *elim_nodes){
    for(int i = 0; i < graph->no_of_nodes; i++){
        for(TNode *it = graph->adjList[i]; it != NULL; it = it->next){
            if(!exists_edge(graph, it->v, i))
                addEdge(graph, graph->idx[it->v], graph->idx[i]);
        }
    }
}

void elim_nodes_obs(TGraph *graph,int *nodes_elim, int *var, int no_int){
    for(int i = 2; i < no_int + 1; i++){
        if(nodes_elim[var[i]] == 0){//daca nodul nu a fost eliminat
            for(int j = 0; j < graph->no_of_nodes; j++){
                if(nodes_elim[j] == 0)
                {removeEdgeList(graph, var[i], j);
                removeEdgeList(graph, j, var[i]);
                nodes_elim[var[i]] = 1;
            }
        }
    }
    }
}

void cerinta1(TGraph *graph, FILE *f){
    int size = 0;
    Queue *queue = createQueue();
    int s, *count = (int *)malloc(graph->no_of_nodes * sizeof(int));

    for(int i = 0; i < graph->no_of_nodes; i++){
        count[i] = graph->d[i].indeg;
        if(count[i] == 0)
            enqueue(queue, i);
    }

    while(isQueueEmpty(queue) == 0){
        s = front(queue);
        dequeue(queue);
        size ++;

        for(TNode* it = graph->adjList[s]; it!= NULL; it = it->next){
			count[it->v]--;

            if(count[it->v] == 0)
                enqueue(queue, it->v);
		}
    }

    destroyQueue(queue);
    free(count);

    if(size < graph->no_of_nodes)
        fprintf(f, "imposibil\n");
    else fprintf(f, "corect\n");

    destroyGraphAdjList(graph);
    return;
}

void cerinta2(TGraph *graph, FILE *f_in, FILE *f_out){
    TGraph *aux;
    int interog, no_int;
    char **var;
    var = (char **)malloc(sizeof(char *) * graph->no_of_nodes);
    
    fscanf(f_in, "%d\n", &interog);

    for(int i = 0; i < interog; i++){
        char *line = (char *)malloc(sizeof(char) * NAME * 10);

        fgets(line, NAME * 10, f_in);
        line[strlen(line) - 1] = '\0';
    
        no_int = interogations(line, &var);

        int *index, comp = 0; 
        index = index_nodes(graph, var, no_int);

        int *ancestors;
        int *nodes_elim = (int *)calloc(graph->no_of_nodes, sizeof(int)); 
        getAncestors(graph, index, &ancestors, no_int, &comp);
    
        aux = elim_nodes(graph, ancestors, comp, nodes_elim);
        m_graph(aux, nodes_elim);
        turn_undirected(aux, nodes_elim);
        elim_nodes_obs(aux,nodes_elim, index, no_int);

        if(is_path(aux, index[0], index[1])){
            fprintf(f_out, "neindependente\n");
        }else{
            fprintf(f_out, "independente\n");
        }

        free(index);
        free(ancestors);
        free(nodes_elim);
        free(line);
        destroyGraphAdjList(aux);

    }

    free(var);
    destroyGraphAdjList(graph);

    return;
}