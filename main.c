
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 100000

/* ---------------------------------
   LINKED LIST
   --------------------------------- */

typedef struct Element {
        long value;
        struct Element *next;
        struct Element *previous;
} Element;

typedef struct LinkedList {
        Element *first;
        Element *last;
        long size;
} LinkedList;

void list_alloc_test(LinkedList *list) {
        if (list == NULL) { exit(EXIT_FAILURE); }
}

LinkedList *list_init() {
        LinkedList *list = malloc(sizeof(*list));
        Element *element = malloc(sizeof(*element));

        if (list == NULL || element == NULL) { exit(EXIT_FAILURE); }

        element->value = 0;
        element->next = NULL;
        element->previous = NULL;

        list->first = element;
        list->last = element;
        list->size = 0;

        return list;
}

void list_destroy(LinkedList *list) {
        list_alloc_test(list);

        Element *element = list->first;
        while (element) {
                Element *next = element->next;
                free(element);
                element = next;
        }
        free(list);
}

void list_add(LinkedList *list, long iValueToAdd) {
        list_alloc_test(list);

        Element *element = malloc(sizeof(*element));
        if (element == NULL) { exit(EXIT_FAILURE); }

        element->value = iValueToAdd;
        element->next = NULL;

        if (list->size == 0) {
                free(list->first);
                element->previous = NULL;
                list->first = element;
        } else {
                element->previous = list->last;
                list->last->next = element;
        }

        list->last = element;
        list->size += 1;
}

void list_del(LinkedList *list, long iValueToDel) {
        list_alloc_test(list);

        Element *elementCurrent, *elementPrevious;

        if (list->size == 0) {
                printf("ERROR. List is empty, nothing to delete\n");
        }
        else if (list->size == 1) {
                elementCurrent = list->first;
                if (elementCurrent->value == iValueToDel) {
                        free(elementCurrent);
                        list->first = NULL;
                        list->last = NULL;
                        list->size = 0;
                }
                else {
                        printf("ERROR. Element not in list, nothing to delete\n");
                }
        }
        else {
                elementPrevious = list->first;
                elementCurrent = elementPrevious->next;

                if (elementPrevious->value == iValueToDel) {
                        list->first = elementCurrent;
                        elementCurrent->previous = NULL;
                        list->size -= 1;
                        free(elementPrevious);
                }
                else {
                        long iKeepGoing = 1;
                        while (elementCurrent != NULL && iKeepGoing == 1) {
                                if (elementCurrent->value == iValueToDel) {
                                        iKeepGoing = 0;
                                        elementPrevious->next = elementCurrent->next;
                                        list->size -= 1;
                                        if (elementCurrent->next == NULL) {
                                                list->last = elementPrevious;
                                        }
                                        else {
                                                elementCurrent->next->previous = elementPrevious;
                                        }
                                        free(elementCurrent);
                                }
                                else {
                                        elementPrevious = elementCurrent;
                                        elementCurrent = elementCurrent->next;
                                }
                        }
                        if (iKeepGoing == 1) {
                                printf("ERROR. Element not found.\n");
                        }
                }
        }
}

long list_get(LinkedList *list, long iPosition) {
        list_alloc_test(list);

        Element *elementCurrent;

        if (iPosition > list->size - 1) {
                printf("ERROR. The list only contains %lu elements.\n", list->size);
                return -1;
        } else {
                long i;
                elementCurrent = list->first;
                for (i = 0; i < iPosition; i++) {
                        elementCurrent = elementCurrent->next;
                }
                return elementCurrent->value;
        }
}

void list_print(LinkedList *list) {
        list_alloc_test(list);

        Element *elementCurrent;

        printf("{");
        if (list->size > 0) {
                elementCurrent = list->first;
                while (elementCurrent != NULL) {
                        printf("%lu", elementCurrent->value);
                        elementCurrent = elementCurrent->next;
                        if (elementCurrent != NULL) {
                                printf(", ");
                        }
                }
        }
        printf("}\n");
}


/* ---------------------------------
   QUEUE
   --------------------------------- */

typedef struct Queue {
        LinkedList *list;
} Queue;

Queue *queue_init() {
        Queue *queue = malloc(sizeof(Queue));
        queue->list = list_init();
        return queue;
}

void queue_destroy(Queue *queue) {
        list_destroy(queue->list);
        queue->list = NULL;
        free(queue);
}

void queue_enqueue(Queue *queue, long index) {
        list_add(queue->list, index);
}

long queue_dequeue(Queue *queue) {
        long value = 0;
        if (queue->list->size == 1) {
                value = queue->list->first->value;
                free(queue->list->first);
                queue->list->first = queue->list->last = NULL;
                queue->list->size = 0;
                return value;
        }
        else {
                Element *elementPrevious = queue->list->first;
                Element *elementCurrent = elementPrevious->next;

                value = elementPrevious->value;
                queue->list->first = elementCurrent;
                elementCurrent->previous = NULL;
                queue->list->size -= 1;
                free(elementPrevious);
                return value;
        }
}


/* ---------------------------------
   GRAPH
   --------------------------------- */

typedef enum {WHITE, GRAY, BLACK} color;

typedef struct GraphNode {
        long value;
        color color;
        long gamma;
        struct GraphNode *pi;
} GraphNode;

GraphNode *graphnode_init(long value) {
        GraphNode *node = malloc(sizeof(GraphNode));
        node->color = WHITE;
        node->value = value;
        node->gamma = 0;
        node->pi = NULL;
        return node;
}

void graphnode_destroy(GraphNode *node) {
        node->pi = NULL;
        free(node);
}

typedef struct Graph {
        LinkedList *items[N];
        GraphNode *nodes[N];
} Graph;

Graph *graph_init() {
        Graph *graph = malloc(sizeof(Graph));
        size_t i;
        for (i = 0; i < N; i++) {
                graph->items[i] = NULL;
                graph->nodes[i] = NULL;
        }
        return graph;
}

void graph_destroy(Graph *graph) {
        size_t i;
        for (i = 0; i < N; i++) {
                if (graph->items[i]) {
                        list_destroy(graph->items[i]);
                }
                if (graph->nodes[i]) {
                        graphnode_destroy(graph->nodes[i]);
                }
        }
        free(graph);
}

void graph_add(Graph *graph, long x, long y) {
        if (graph->items[x] == NULL) {
                graph->items[x] = list_init();
        }
        list_add(graph->items[x], y);

        /*
        if (!graph->items[y]) {
                graph->items[y] = list_init();
        }
        list_add(graph->items[y], x);
        */
}

void graph_BFS(Graph *graph, long s_index) {
        GraphNode *s = graph->nodes[s_index];
        s->color = GRAY;
        s->gamma = 0;
        s->pi = NULL;

        Queue *q = queue_init();
        queue_enqueue(q, s_index);

        while (q->list->size != 0) {
                long u_index = queue_dequeue(q);
                GraphNode *u = graph->nodes[u_index];

                LinkedList *u_adyacents = graph->items[u_index];

                Element *elementCurrent = u_adyacents->first;
                while (elementCurrent != NULL) {
                        long v_index = elementCurrent->value;
                        GraphNode *v = graph->nodes[v_index];
                        if (v->color == WHITE) {
                                v->color = GRAY;
                                v->gamma = u->gamma + 1;
                                v->pi = u;
                                queue_enqueue(q, v_index);
                        }
                        elementCurrent = elementCurrent->next;
                }
                u->color = BLACK;
        }
        queue_destroy(q);
}

LinkedList *graph_route(Graph *graph, long from, long to) {
        graph_BFS(graph, from);

        LinkedList *path = list_init();

        GraphNode *end = graph->nodes[to];
        while (end != NULL) {
                list_add(path, end->value);
                end = end->pi;
        }
        return path;
}

void graph_print(Graph *graph) {
        size_t i;
        for (i = 0; graph->items[i]; i++) {
                list_print(graph->items[i]);
        }
}


/* ---------------------------------
   MAIN
   --------------------------------- */

int main(int argc, char const *argv[]) {
        /* Variables */
        long rounds, count, identifier, connection, k, from, to;
        Graph *graph;
        LinkedList *route;

        /* Foreach case (round) */
        scanf("%lu", &rounds);
        size_t round;
        for (round = 0; round < rounds; round++) {
                graph = graph_init();

                /* Foreach student */
                scanf("%lu", &count);
                size_t n;
                for (n = 0; n < count; n++) {

                        /* Student identifier (First column) */
                        scanf("%lu", &identifier);

                        /* Add node to graph */
                        graph->nodes[identifier] = graphnode_init(identifier);

                        /* Foreach connection (second column) */
                        scanf("%lu", &connection);
                        size_t K;
                        for (K = 0; K < connection; K++) {

                                /* Add (identifier <-> k) connection to graph */
                                scanf("%lu", &k);
                                graph_add(graph, identifier, k);
                        }
                }
                /* Query (from -> to) */
                scanf("%lu %lu", &from, &to);

                /* Solution using BFS */
                route = graph_route(graph, from, to);
                if (round != rounds - 1) {
                        /* Not last */
                        printf("%lu %lu %lu\n\n", from, to, route->size - 2);
                } else {
                        /* Last */
                        printf("%lu %lu %lu\n", from, to, route->size - 2);
                }

                /* Cleanup */
                if (route) { list_destroy(route); }
                if (graph) { graph_destroy(graph); }
        }

        return 0;
}
