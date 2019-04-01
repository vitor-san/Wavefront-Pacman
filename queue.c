#include <stdlib.h>
#include "queue.h"

typedef struct node {
  struct node *next;
  elem value;
} Node;

struct queue {
  Node *beg, *end;
};

Queue newQueue() {
  Queue q = malloc(sizeof(QUEUE));
  if (q == NULL) return NULL;  //erro
  q->beg = NULL;
  q->end = NULL;
  return q;
}

int putQueue(Queue q, elem x) {
  Node *new = malloc(sizeof(Node));
  if (new == NULL) return 0;  //erro
  new->value = x;
  new->next = NULL;

  if (isEmptyQueue(q)) {
    q->beg = new;
    q->end = new;
  }
  else {  //nao vai mexer no comeco
    q->end->next = new; //atualiza proximo do antigo fim
    q->end = new; //atualiza aonde esta o fim
  }

  return 1; //deu certo
}

elem getQueue(Queue q) {
  if (q == NULL || isEmptyQueue(q)) return -999999;  //erro

  //resgatar valor do primeiro da fila
  elem retorno = q->beg->value;
  //auxiliar vai apontar para no que era o antigo comeco para depois apagar ele
  Node *aux = q->beg;
  //atualiza comeco
  q->beg = q->beg->next;

  free(aux);  //apaga a caixa que era o antigo comeco

  return retorno;   //retorna o valor do antigo comeco
}

int isEmptyQueue(Queue q) {
  if (q == NULL) return -1; //erro
  return (q->beg == NULL);
}

void deleteQueue(Queue q) {
  if (q == NULL) return;  //so para nao dar seg fault

  while (!isEmptyQueue(q)) {
    getQueue(q);
  }
}
