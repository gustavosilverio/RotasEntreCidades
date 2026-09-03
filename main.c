#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 9999999

// Declaração prévia para uso na Aresta
typedef struct Cidade Cidade;

// Estrutura para as Arestas (Rodovias)
typedef struct Aresta {
    Cidade *destino;
    int distancia;
    struct Aresta *proxima;
} Aresta;

// Estrutura para os Vértices (Cidades)
struct Cidade {
    char nome[50];
    Aresta *adjacencias; // lista encadeada de arestas (vizinhos)
    Cidade *proxima;     // proxima cidade na lista de vertices
    
    // Campos auxiliares para os algoritmos BFS e Dijkstra
    int visitado;
    int distancia_dijkstra;
    Cidade *anterior;
};

// Ponteiro global para o início do grafo (lista de cidades)
Cidade *grafo = NULL;
int total_cidades = 0;

// Função auxiliar para limpar buffer e ler strings com espaços
void ler_string(char *buffer, int max) {
    if (fgets(buffer, max, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

// Busca uma cidade no grafo pelo nome
Cidade* buscar_cidade(const char *nome) {
    Cidade *atual = grafo;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->proxima;
    }
    return NULL;
}

// 1. Cadastrar cidade
void cadastrar_cidade() {
    char nome[50];
    printf("\nCadastrar Cidade\nNome da cidade: ");
    ler_string(nome, 50);

    printf("Alocando novo no para a cidade...\n");
    
    Cidade *nova = (Cidade*)malloc(sizeof(Cidade));
    strcpy(nova->nome, nome);
    nova->adjacencias = NULL;
    nova->proxima = NULL;
    nova->visitado = 0;
    nova->distancia_dijkstra = INF;
    nova->anterior = NULL;

    if (grafo == NULL) {
        grafo = nova;
    } else {
        Cidade *atual = grafo;
        while (atual->proxima != NULL) {
            atual = atual->proxima;
        }
        atual->proxima = nova;
    }
    
    total_cidades++;
    printf("Cidade cadastrada com sucesso!\nTotal de cidades: %d\n", total_cidades);
}

// 2. Cadastrar rodovia
void cadastrar_rodovia() {
    char origem_nome[50], destino_nome[50];
    int distancia;

    printf("\nCadastrar Rodovia\nCidade de origem: ");
    ler_string(origem_nome, 50);
    printf("Cidade de destino: ");
    ler_string(destino_nome, 50);
    printf("Distancia (km): ");
    scanf("%d", &distancia);
    getchar(); // Limpar o \n do buffer

    Cidade *origem = buscar_cidade(origem_nome);
    Cidade *destino = buscar_cidade(destino_nome);

    if (origem == NULL) {
        printf("Erro: cidade de origem \"%s\" nao encontrada no grafo.\n", origem_nome);
        printf("Nenhuma memoria foi alocada.\nRodovia nao cadastrada.\n");
        return;
    }
    if (destino == NULL) {
        printf("Erro: cidade de destino \"%s\" nao encontrada no grafo.\n", destino_nome);
        printf("Nenhuma memoria foi alocada.\nRodovia nao cadastrada.\n");
        return;
    }

    printf("Alocando aresta: %s -> %s (%d km)...\n", origem->nome, destino->nome, distancia);
    Aresta *aresta1 = (Aresta*)malloc(sizeof(Aresta));
    aresta1->destino = destino;
    aresta1->distancia = distancia;
    aresta1->proxima = origem->adjacencias;
    origem->adjacencias = aresta1;

    printf("Alocando aresta: %s -> %s (%d km)...\n", destino->nome, origem->nome, distancia);
    Aresta *aresta2 = (Aresta*)malloc(sizeof(Aresta));
    aresta2->destino = origem;
    aresta2->distancia = distancia;
    aresta2->proxima = destino->adjacencias;
    destino->adjacencias = aresta2;

    printf("Rodovia cadastrada com sucesso!\n");
}

// 3. Listar cidades e conexoes
void listar_cidades() {
    printf("\nCidades Cadastradas\n");
    if (grafo == NULL) {
        printf("Nenhuma cidade cadastrada.\n");
        return;
    }
    
    Cidade *atual = grafo;
    int indice = 1;
    while (atual != NULL) {
        printf("%d. %s\n", indice++, atual->nome);
        if (atual->adjacencias == NULL) {
            printf("   (sem conexoes)\n");
        } else {
            Aresta *a = atual->adjacencias;
            while (a != NULL) {
                printf("   -> %s (%d km)\n", a->destino->nome, a->distancia);
                a = a->proxima;
            }
        }
        atual = atual->proxima;
    }
}

// 4. Verificar conectividade (BFS)
void verificar_conectividade() {
    char origem_nome[50], destino_nome[50];
    printf("\nVerificar Conectividade\nCidade de origem: ");
    ler_string(origem_nome, 50);
    printf("Cidade de destino: ");
    ler_string(destino_nome, 50);

    Cidade *origem = buscar_cidade(origem_nome);
    Cidade *destino = buscar_cidade(destino_nome);

    if (origem == NULL || destino == NULL) {
        printf("Erro: uma ou ambas as cidades nao foram encontradas.\n");
        return;
    }

    printf("Executando busca em largura (BFS)...\n");

    // Zera status de visitado
    Cidade *temp = grafo;
    while (temp != NULL) {
        temp->visitado = 0;
        temp = temp->proxima;
    }

    // Fila estática simples (como o nº de cidades é baixo nos testes)
    Cidade *fila[1000]; 
    int inicio = 0, fim = 0;

    fila[fim++] = origem;
    origem->visitado = 1;
    int achou = 0;

    while (inicio < fim) {
        Cidade *u = fila[inicio++];
        printf("Visitando: %s\n", u->nome);

        if (u == destino) {
            achou = 1;
            break;
        }

        Aresta *a = u->adjacencias;
        while (a != NULL) {
            if (!a->destino->visitado) {
                a->destino->visitado = 1;
                fila[fim++] = a->destino;
            }
            a = a->proxima;
        }
    }

    if (achou) {
        printf("Existe rota entre %s e %s.\n", origem->nome, destino->nome);
    } else {
        printf("Nao existe rota entre %s e %s.\n", origem->nome, destino->nome);
    }
}

// Auxiliar para recursivamente imprimir o caminho do Dijkstra
void imprimir_caminho_ate(Cidade *c) {
    if (c->anterior != NULL) {
        imprimir_caminho_ate(c->anterior);
        printf(" -> ");
    }
    printf("%s", c->nome);
}

// 5. Encontrar rota de menor distância (Dijkstra)
void encontrar_rota_dijkstra() {
    char origem_nome[50], destino_nome[50];
    printf("\nEncontrar Rota de Menor Distancia\nCidade de origem: ");
    ler_string(origem_nome, 50);
    printf("Cidade de destino: ");
    ler_string(destino_nome, 50);

    Cidade *origem = buscar_cidade(origem_nome);
    Cidade *destino = buscar_cidade(destino_nome);

    if (origem == NULL || destino == NULL) {
        printf("Erro: uma ou ambas as cidades nao foram encontradas.\n");
        return;
    }

    printf("Executando algoritmo de Dijkstra...\n");

    // Prepara as cidades para o algoritmo
    Cidade *temp = grafo;
    while (temp != NULL) {
        temp->distancia_dijkstra = INF;
        temp->visitado = 0;
        temp->anterior = NULL;
        temp = temp->proxima;
    }

    origem->distancia_dijkstra = 0;

    for (int i = 0; i < total_cidades; i++) {
        Cidade *u = NULL;
        int min_dist = INF;
        
        // Extrai a cidade não visitada com menor distância
        temp = grafo;
        while (temp != NULL) {
            if (!temp->visitado && temp->distancia_dijkstra < min_dist) {
                min_dist = temp->distancia_dijkstra;
                u = temp;
            }
            temp = temp->proxima;
        }

        if (u == NULL) break; // Todas as cidades alcançáveis já visitadas

        u->visitado = 1;

        Aresta *a = u->adjacencias;
        while (a != NULL) {
            Cidade *v = a->destino;
            int peso = a->distancia;

            // Log formatado exigido pelos testes durante avaliação do caminho
            if (v == destino) {
                printf("Avaliando rota: ");
                imprimir_caminho_ate(u);
                printf(" -> %s %d km\n", v->nome, u->distancia_dijkstra + peso);
            }

            // Relaxamento da aresta
            if (!v->visitado && u->distancia_dijkstra != INF && 
                (u->distancia_dijkstra + peso < v->distancia_dijkstra)) {
                v->distancia_dijkstra = u->distancia_dijkstra + peso;
                v->anterior = u;
            }
            a = a->proxima;
        }
    }

    if (destino->distancia_dijkstra == INF) {
        printf("Nao ha rota disponivel entre %s e %s.\n", origem->nome, destino->nome);
    } else {
        printf("Rota de menor distancia encontrada!\n");
        printf("Caminho: ");
        imprimir_caminho_ate(destino);
        printf("\nDistancia total: %d km\n", destino->distancia_dijkstra);
    }
}

// 6. Sair e Liberar Memória
void liberar_memoria() {
    printf("\nPercorrendo o grafo para liberar memoria...\n");
    Cidade *atual = grafo;
    while (atual != NULL) {
        printf("Liberando arestas de: %s\n", atual->nome);
        Aresta *a = atual->adjacencias;
        while (a != NULL) {
            Aresta *temp_aresta = a;
            a = a->proxima;
            free(temp_aresta);
        }
        atual = atual->proxima;
    }
    
    printf("Liberando nos das cidades...\n");
    atual = grafo;
    while (atual != NULL) {
        Cidade *temp_cidade = atual;
        atual = atual->proxima;
        free(temp_cidade);
    }
    
    printf("Memoria do grafo liberada com sucesso.\nEncerrando o programa.\n");
}

int main() {
    printf("Grafo inicializado (ponteiro grafo = NULL, 0 cidades).\n");
    int opcao;

    do {
        printf("\n===== Sistema de Rotas entre Cidades =====\n");
        printf("1. Cadastrar cidade\n");
        printf("2. Cadastrar rodovia (conexao entre cidades)\n");
        printf("3. Listar cidades e conexoes\n");
        printf("4. Verificar conectividade entre duas cidades (BFS)\n");
        printf("5. Encontrar rota de menor distancia (Dijkstra)\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            break; 
        }
        getchar(); // Limpar \n do buffer

        switch(opcao) {
            case 1: cadastrar_cidade(); break;
            case 2: cadastrar_rodovia(); break;
            case 3: listar_cidades(); break;
            case 4: verificar_conectividade(); break;
            case 5: encontrar_rota_dijkstra(); break;
            case 6: liberar_memoria(); break;
            default: printf("Opcao invalida!\n");
        }
    } while(opcao != 6);

    return 0;
}