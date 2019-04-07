#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <graphviz/cgraph.h>

//------------------------------------------------------------------------------
// structs especificação
struct grafo {
    Agraph_t *g;
};

typedef struct vertice {
    Agrec_t h;
    Agnode_t *v;
    int cor;
    int tam_rotulo;
    int *rotulo;
} record;

//------------------------------------------------------------------------------
// struct adicional
typedef struct elemento {
    Agnode_t *v;
    struct elemento *prox;
} elemento;

//------------------------------------------------------------------------------
// protótipos
void addNaLista(elemento *e, Agnode_t *v);
void adicionaNoRotulo(Agnode_t *v, int valor);
int comparaRotulo(Agnode_t *v1, Agnode_t *v2);
Agnode_t *verticeMaiorRotulo(elemento *c);
int corGulosa(grafo g, Agnode_t *v);

//------------------------------------------------------------------------------
// funções adicionais
void addNaLista(elemento *e, Agnode_t *v){
    while(e->prox) e = e->prox;

    // cria novo elemento e insere no final da lista
    elemento *novo = malloc(sizeof(elemento));
    e->prox = novo;

    //inicializa novo elemento
    novo->v = v;
    novo->prox = NULL;
}

void adicionaNoRotulo(Agnode_t *v, int valor){
    // acessa rótulo
    record *rot = (record *) aggetrec(v, (char*)(unsigned long) "record", FALSE);

    // adiciona valor na primeira posição vazia e incrementa tamanho do rótulo
    rot->rotulo[rot->tam_rotulo++] = valor;
}

int comparaRotulo(Agnode_t *v1, Agnode_t *v2){
    // acessa rótulos
    record *rot1 = (record *) aggetrec(v1, (char*)(unsigned long) "record", FALSE);
    record *rot2 = (record *) aggetrec(v2, (char*)(unsigned long) "record", FALSE);

    // compara elemento a elemento até um dos rótulos acabar
    for(int i = 0; i < rot1->tam_rotulo && i < rot2->tam_rotulo; i++){
        // se o elemento do primeiro rótulo for maior, retorna 1
        if(rot1->rotulo[i] > rot2->rotulo[i]){
            return 1;
        // caso contrário, se o segundo for maior, retorna -1
        } else if(rot2->rotulo[i] > rot1->rotulo[i]){
            return -1;
        }
    }

    // retorna diferença entra rótulos se não encontrar elementos distintos
    return (rot1->tam_rotulo - rot2->tam_rotulo);
}

Agnode_t *verticeMaiorRotulo(elemento *c){
    elemento *e = c->prox;
    elemento *maior = e;

    // econtra vértice de maior rótulo
    while(e->prox){
        e = e->prox;
        if (comparaRotulo(maior->v, e->v) < 0) maior = e;
    }

    // encontra elemento da lista anterior ao vértice de maior rótulo
    while(c->prox != maior) c = c->prox;

    // remove vértice da lista
    c->prox = maior->prox;

    Agnode_t *v = maior->v;
    free(maior);

    //retorna o ponteiro para vértice de maior rótulo
    return v;
}

int corGulosa(grafo g, Agnode_t *v){
    int n = n_vertices(g);

    // para cada cor de 1 até n visita os vizinhos
    for(int cor = 1; cor <= n; cor++){
        int usado = FALSE;
        for(Agedge_t *e = agfstedge (g->g, v); e; e = agnxtedge(g->g, e, v)){
            record *r = (record *) aggetrec(e->node, (char*)(unsigned long) "record", FALSE);
            // se algum vizinho já tiver a cor, ativa flag
            if(r->cor == cor) usado = TRUE;
        }
        // se nenhum vizinho tiver a cor, retorna ela
        if(!usado) return cor;
    }

    // em caso de erro, retorna -1
    return -1;
}

//------------------------------------------------------------------------------
// funções da especificação - entrada e saída
grafo escreve_grafo(FILE *output, grafo g) {
    // cria atributo color para vértices
    agattr(g->g, AGNODE, (char*)(unsigned long) "color", (char*)(unsigned long) "");

    // para todos os vértices, calcula 'color' baseado na cor do vértice
    for(Agnode_t *v = agfstnode(g->g); v; v = agnxtnode(g->g, v)){
        char attr_str[10];
        record *r = (record *) aggetrec(v, (char*)(unsigned long) "record", FALSE);
        sprintf(attr_str, "#%06x", r->cor);
        agset(v, (char*)(unsigned long) "color", attr_str);
    }

    // escreve grafo
    agwrite(g->g, output);
    return g;
}

grafo le_grafo(FILE *input) {
	grafo g = malloc(sizeof(struct grafo));
    // lê grafo
    g->g = agread(input, 0);

    // para todo vértice, associa e inicializa record
    for(Agnode_t *vert = agfstnode(g->g); vert; vert = agnxtnode(g->g, vert)){
        record *rec = agbindrec(vert, (char*)(unsigned long) "record", sizeof(record), FALSE);
        rec->rotulo = malloc(sizeof(int) * (size_t) n_vertices(g));
        rec->v = vert;
    }
    return g;
}

//------------------------------------------------------------------------------
// funções da especificação - auxiliares
int n_vertices(grafo g){
    return agnnodes(g->g);
}

vertice vertice_de_nome(char *nome, grafo g){
    Agnode_t *vert = agnode(g->g, nome, 0);
    return (vertice) aggetrec(vert, (char*)(unsigned long) "record", FALSE);
}

int destroi_grafo(grafo g) {
    // para todo vértice, libera memória do rótulo
    for(Agnode_t *v = agfstnode(g->g); v; v = agnxtnode(g->g, v)){
        record *rec = (record *) aggetrec(v, (char*)(unsigned long) "record", FALSE);
        free(rec->rotulo);
    }

    // fecha grafo da cgraph
    agclose(g->g);
    free(g);
    return 1;
}

//------------------------------------------------------------------------------
// funções da especificação - lexBFS e coloração
vertice *busca_lexicografica(vertice r, grafo g, vertice *v) {
    int n = n_vertices(g);
    int tamV = n;
    elemento *cabeca = malloc(sizeof(elemento));
    cabeca->prox = NULL;

    // adiciona todos os vértices na lista
    for(Agnode_t *vert = agfstnode(g->g); vert; vert = agnxtnode(g->g, vert)){
        record *rec = agbindrec(vert, (char*)(unsigned long) "record", sizeof(record), FALSE);
        rec->tam_rotulo = 0;
        addNaLista(cabeca, vert);
    }

    // escolhe o primeiro e coloca n no rótulo
    Agnode_t *vert = r->v;
    adicionaNoRotulo(vert, n);

    while(cabeca->prox){
        //busca vértice de maior rótulo na lista e remove
        vert = verticeMaiorRotulo(cabeca);
        tamV--;
        // para todos os vizinhos, adiciona tamanho da lista ao rótulo
        for(Agedge_t *e = agfstedge(g->g, vert); e; e = agnxtedge(g->g, e, vert)) adicionaNoRotulo(e->node, tamV);

        // insere o vértice no vetor da ordenação lexicográfica
        v[n-tamV-1] = (record *) aggetrec(vert, (char*)(unsigned long) "record", FALSE);
    }

    free(cabeca);
    return v;
}

unsigned int cor(vertice v, grafo g){
    g = g;
    return (unsigned int) v->cor;
}

unsigned int colore(grafo g, vertice *v){
    int n = n_vertices(g);
    int maior = 1;

    // inicializa cor de todos os vértices como 0
    for(int i = 0; i < n; i++) v[i]->cor = 0;

    // para todos os vértices da ordem lexicográfica reversa
    for(int i = n-1; i >= 0; i--){
        // atribui a cor do vértice de maneira gulosa
        v[i]->cor = corGulosa(g, v[i]->v);
        maior = v[i]->cor > maior ? v[i]->cor : maior;
    }

    // retorna quantidade total de cores utilizadas
    return (unsigned int) maior;
}
