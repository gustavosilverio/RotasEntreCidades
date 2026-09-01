#include <stdio.h>
#include <string.h>

#define MAX_CIDADES 10
#define INF 9999999

typedef struct
{
    char nome[50];
} Cidade;

typedef struct
{
    Cidade cidades[MAX_CIDADES];
    int adj[MAX_CIDADES][MAX_CIDADES];
    int qtd_cidades;
} Grafo;

// Inicializa a matriz com "infinito" (sem arestas)
void inicializar_grafo(Grafo *g)
{
    g->qtd_cidades = 0;
    for (int i = 0; i < MAX_CIDADES; i++)
    {
        for (int j = 0; j < MAX_CIDADES; j++)
        {
            g->adj[i][j] = (i == j) ? 0 : INF;
        }
    }
}

// Adiciona uma cidade e retorna seu índice/ID
int adicionar_cidade(Grafo *g, const char *nome)
{
    if (g->qtd_cidades >= MAX_CIDADES)
        return -1;

    int id = g->qtd_cidades;
    strncpy(g->cidades[id].nome, nome, sizeof(g->cidades[id].nome) - 1);
    g->qtd_cidades++;
    return id;
}

// Adiciona rodovia bidirecional com distância/peso
void adicionar_rodovia(Grafo *g, int origem, int destino, int distancia)
{
    g->adj[origem][destino] = distancia;
    g->adj[destino][origem] = distancia; // Grafo não-direcionado
}

void imprimir_conexoes(const Grafo *g)
{
    printf("\n--- Rede Rodoviaria ---\n");
    for (int i = 0; i < g->qtd_cidades; i++)
    {
        for (int j = i + 1; j < g->qtd_cidades; j++)
        {
            if (g->adj[i][j] != INF && g->adj[i][j] != 0)
            {
                printf("%s <---> %s : %d km\n",
                       g->cidades[i].nome,
                       g->cidades[j].nome,
                       g->adj[i][j]);
            }
        }
    }
}

int main()
{
    Grafo rede;
    inicializar_grafo(&rede);

    int sp = adicionar_cidade(&rede, "Sao Paulo");
    int rj = adicionar_cidade(&rede, "Rio de Janeiro");
    int bh = adicionar_cidade(&rede, "Belo Horizonte");
    int cwb = adicionar_cidade(&rede, "Curitiba");

    adicionar_rodovia(&rede, sp, rj, 430);
    adicionar_rodovia(&rede, sp, bh, 586);
    adicionar_rodovia(&rede, sp, cwb, 408);
    adicionar_rodovia(&rede, rj, bh, 440);

    imprimir_conexoes(&rede);

    return 0;
}