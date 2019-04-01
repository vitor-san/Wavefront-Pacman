#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "graph.h"
#include "queue.h"

#define MODO_FACIL 0

void drawScene(Graph g, int pac, int ghost, int *dist);

int main (void) {
  srand(time(NULL));

  Graph g = newGraph(100, 0);  // grafo não-direcionado de nxn vertices (o numero de vertices deve ser um quadrado perfeito)
  int nv = getNumVertices(g);
  int lado = sqrt(getNumVertices(g));
  //addEdges
  for (int i = 1; i <= lado; i++) {
    for (int j = 1; j <= lado; j++) {
      //em relação as linhas...
      if (j == 1) {   //se for um no da borda, então...
        addEdge(g, ((i-1)*lado + j), ((i-1)*lado + j + 1));
      }
      else if (j == lado) {   //se for um no da borda, então...
        addEdge(g, ((i-1)*lado + j), ((i-1)*lado + j - 1));
      }
      else {  //caso contrario, se for um no do "meio"...
        addEdge(g, ((i-1)*lado + j), ((i-1)*lado + j - 1)); //liga com o cara de tras
        addEdge(g, ((i-1)*lado + j), ((i-1)*lado + j + 1)); //e liga com o cara da frente
      }
      //já em relação as colunas...
      if (i == 1) {   //se for um no da borda, então...
        addEdge(g, ((i-1)*lado + j), ((i-1)*lado + j + lado));
      }
      else if (i == lado) {   //se for um no da borda, então...
        addEdge(g, ((i-1)*lado + j), ((i-1)*lado + j - lado));
      }
      else {  //caso contrario, se for um no do "meio"...
        addEdge(g, ((i-1)*lado + j), ((i-1)*lado + j - lado)); //liga com o cara de cima
        addEdge(g, ((i-1)*lado + j), ((i-1)*lado + j + lado)); //e liga com o cara de baixp
      }
    }
  }

  int pacPos = (rand()%nv)+1;
  int gstPos = (rand()%nv)+1;

  while (pacPos == gstPos) {
    gstPos = (rand()%nv)+1;
  }

  setVertexValue(g, pacPos, 'P');
  setVertexValue(g, gstPos, 'G');

  printf("\nSituacao inicial: \n\n");
  printf("==============================================================\n");
  printGraph(g, 1);
  drawScene(g, pacPos, gstPos, NULL);
  printf("================================================================\n");

  int passos = 0;
  while (pacPos != gstPos) {
    char delay;    //unicamente usado para o usuario controlar o passo a passo das etapas
    scanf("%c", &delay);  //pressione só ENTER (caso contrario nao vai mostrar um passo por vez)

    //pacman determina como ele vai se movimentar (algoritmo Wavefront)
    int dist[nv];
    Queue q = newQueue();

    for (int i = 0; i < nv; i++) {
      dist[i] = 99999999; //representando "infinito"
    }
    dist[gstPos-1] = 0;
    putQueue(q, gstPos);

    //faço uma BFS (Breadth-first Search) para determinar as distancias
    while (!isEmptyQueue(q)) {
      int vert = getQueue(q);
      int *vizinhos = neighbors(g, vert);
      int grau = getVertexDegree(g, vert);

      for (int i = 0; i < grau; i++) {
        if (dist[vizinhos[i]-1] == 99999999) {
          dist[vizinhos[i]-1] = dist[vert-1] + 1;
          putQueue(q, vizinhos[i]);
        }
      }
      free(vizinhos);
    }
    deleteQueue(q);

    //para se movimentar, o PacMan sempre vai escolher o bloco que tenha o menor valor de distancia ao fantasma

    int destino, menorDist = 99999999;
    int *poss = neighbors(g, pacPos);   //tem os blocos acessiveis a ele
    int grau = getVertexDegree(g, pacPos);    //quantos blocos sao acessiveis a ele

    for (int i = 0; i < grau; i++) {
      if (dist[poss[i]-1] < menorDist) {
        menorDist = dist[poss[i]-1];
        destino = poss[i];
      }
    }
    free(poss);

    if (destino == pacPos-1) {    //ele tem que ir para a esquerda
      setVertexValue(g, pacPos, 0);
      pacPos--;
      setVertexValue(g, pacPos, 'P');
    }
    else if (destino == pacPos+1) {    //ele tem que ir para a direita
      setVertexValue(g, pacPos, 0);
      pacPos++;
      setVertexValue(g, pacPos, 'P');
    }
    else if (destino == pacPos-lado) {    //ele tem que ir para cima
      setVertexValue(g, pacPos, 0);
      pacPos -= lado;
      setVertexValue(g, pacPos, 'P');
    }
    else {    //ele tem que ir para baixo
      setVertexValue(g, pacPos, 0);
      pacPos += lado;
      setVertexValue(g, pacPos, 'P');
    }

    //fantasma determina como ele vai se movimentar
    int deuCerto = 0;
    int escolha;

    while (!deuCerto) {
      escolha = rand()%17;  // escolha PONDERADA: 16 chances dele se movimentar VS apenas 1 de não (diminui as chances dele ficar parado e ser uma presa mais facil)
      if (pacPos == gstPos) escolha = 16; //se o pacman o pegou primeiro, ele nao pode fugir
      if (MODO_FACIL) escolha = 16; // no modo facil, o fantasma nao se move

      if (escolha < 4) {  //vai para a esquerda
        if (adjacent(g, gstPos, gstPos-1) == 1 && pacPos != gstPos-1) {
          setVertexValue(g, gstPos, 0);
          setVertexValue(g, gstPos-1, 'G');
          gstPos = gstPos-1;
          deuCerto = 1;
        }
        else continue;
      }
      else if (escolha < 8) {  //vai para a direita
        if (adjacent(g, gstPos, gstPos+1) == 1 && pacPos != gstPos+1) {
          setVertexValue(g, gstPos, 0);
          setVertexValue(g, gstPos+1, 'G');
          gstPos = gstPos+1;
          deuCerto = 1;
        }
        else continue;
      }
      else if (escolha < 12) {  //vai para cima
        if (adjacent(g, gstPos, gstPos-lado) == 1 && pacPos != gstPos-lado) {
          setVertexValue(g, gstPos, 0);
          setVertexValue(g, gstPos-lado, 'G');
          gstPos = gstPos-lado;
          deuCerto = 1;
        }
        else continue;
      }
      else if (escolha < 16) {  //vai para baixo
        if (adjacent(g, gstPos, gstPos+lado) == 1 && pacPos != gstPos+lado) {
          setVertexValue(g, gstPos, 0);
          setVertexValue(g, gstPos+lado, 'G');
          gstPos = gstPos+lado;
          deuCerto = 1;
        }
        else continue;
      }
      else break;  //ele escolheu ficar parado
    }

    //printa a situação atual do jogo
    printf("==============================================================\n");
    printGraph(g, 1);
    drawScene(g, pacPos, gstPos, NULL);
    printf("================================================================\n");
    //conta quantos passos foram necessarios para o pacman chegar ao fantasma
    passos++;
  }

  printf("\nLevaram %d passos para o PacMan pegar o fantasma!\n\n", passos);
  deleteGraph(g);
  return 0;
}

void drawScene(Graph g, int pac, int ghost, int *dist) {
  int n = getNumVertices(g);
  n = sqrt(n);

  printf("\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("|");
      if ((i*n + j+1) == pac) printf("P");
      else if ((i*n + j+1) == ghost) printf("G");
      else {
        if (dist != NULL) printf("%d", dist[i*n + j]);
        else printf(" ");
      }
    }
    printf("|\n");
  }
}
