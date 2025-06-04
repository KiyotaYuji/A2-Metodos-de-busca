#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CIDADES 100

// criando a estrutura das rotas
typedef struct Rota {
    int destino;
    int distancia;
    struct Rota* prox;
} Rota;

Rota* lista[MAX_CIDADES]; // lista de adjacencia das rotas 
char nomesCidades[MAX_CIDADES][100]; // vetor que contem os nomes das cidades
int num_cidades = 0; // contador das cidades que foram adicionadas

// Função para adicionar uma cidade
void adicionarCidade(char* nome) {
    strcpy(nomesCidades[num_cidades], nome); // copiar para o nome da variavel para o vetor
    lista[num_cidades] = NULL; // inicia a lista de adjacência da cidade como vazia
    printf("Cidade '%s' adicionada com índice %d.\n", nome, num_cidades);
    num_cidades++;
}

// função para buscar uma cidade pelo nome
int buscarCidade(char* nome) {
    for (int i = 0; i < num_cidades; i++) {
        if (strcmp(nomesCidades[i], nome) == 0)
            return i;
    }
    return -1;
}

// função para refazer o caminho
void reconstruirCaminho(int anterior[], int destino) {
    if (anterior[destino] != -1) {
        reconstruirCaminho(anterior, anterior[destino]);
    }
    printf("%s -> ", nomesCidades[destino]);
}

// função para adicionar uma rota
void adicionarRota(char* origem, char* destino, int distancia) {

    // fazemos as buscas das cidades
    int i = buscarCidade(origem);
    int j = buscarCidade(destino);

    // verificamos a existencia dos dois caminhos para criar a rota
    if (i != -1 && j != -1) {
        // Adiciona j na lista de i (ida)
        Rota* novaRota = (Rota*)malloc(sizeof(Rota)); // inciamos a variável da nova rota 
        // adicionamos os valores a essa nova rota (origem, destino e distancia)
        novaRota->destino = j;
        novaRota->distancia = distancia;
        novaRota->prox = lista[i];
        // adicionamos essa rota na lista de rotas com o seu index sendo a origem
        lista[i] = novaRota;

        // mesmo processo so que para o outro caminho
        novaRota = (Rota*)malloc(sizeof(Rota));
        novaRota->destino = i;
        novaRota->distancia = distancia;
        novaRota->prox = lista[j];
        lista[j] = novaRota;

        printf("Rota entre '%s' e '%s' adicionada com distancia %d\n", origem, destino, distancia);
    } else {
        printf("Cidade(s) não encontrada(s).\n");
    }
}

// Função auxiliar para encontrar a menor distancia
int minDistancia(int dist[], int visitado[]) {
    int min = INT_MAX, min_index = -1; // se incia com o maior valor possível para um inteiro e o min_index irá armazenar o index do menor nó

    for (int v = 0; v < num_cidades; v++) {  
        if (!visitado[v] && dist[v] <= min) { // verifica se o nó não foi visitado e se ele é menor ou igual ao menor valor
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// função para calcular o menor caminho usando Dijkstra
void calcularMenorCaminho(char* origem, char* destino) {
    // mesma lógica anterior 
    int id_origem = buscarCidade(origem);
    int id_destino = buscarCidade(destino);
    
    if (id_origem == -1 || id_destino == -1) {
        printf("Cidade(s) não encontrada(s).\n");
        return;
    }

    int dist[MAX_CIDADES]; // irá armazenar a menor distancia ate o destino
    int visitado[MAX_CIDADES] = {0}; // usado para verificar se alguma cidade ja foi visitada
    int anterior[MAX_CIDADES]; // guarda a cidade anterior para reconstruir
    
    // iniciando o vetor de distancia como vazia 
    for (int i = 0; i < num_cidades; i++) {
        dist[i] = INT_MAX; // começamos como todas as distancias em infinito
        anterior[i] = -1; // -1 para representar que não tem caminho anterior
    }
    
    dist[id_origem] = 0; // como é a origem, sua distancia até ela mesma é de 0 

    for (int count = 0; count < num_cidades - 1; count++) {
        int u = minDistancia(dist, visitado); // retorna o indice da menor distancia de uma cidade não visitada
        if (u == -1) break; // se não há mais vértices acessíveis
        visitado[u] = 1; // marca que foi visitada

        // Percorre os vizinhos na lista de adjacência
        Rota* atual = lista[u]; // ponteiro que caminha na lista de vizinhos
        while (atual != NULL) {
            int v = atual->destino;
            // verificar se não foi visitado
            // verificar se temos uma distancia válida para o menor caminho
            // verificar se o caminho de u até v é menor que o atual
            if (!visitado[v] && dist[u] != INT_MAX && 
                dist[u] + atual->distancia < dist[v]) {
                // caso verdadeiro atualizamos a distancia de v para esse caminho menor
                dist[v] = dist[u] + atual->distancia;
                anterior[v] = u; // atualizamos o anterior de v, então agora o caminho menos custuso para v seria vindo de u
            }
            atual = atual->prox;
        }
    }

    // Exibe resultados
    if (dist[id_destino] == INT_MAX) {
        printf("Não existe caminho entre %s e %s\n", origem, destino);
    } else {
        printf("Distância: %d km\nCaminho: ", dist[id_destino]);
        reconstruirCaminho(anterior, id_destino);
        printf("\n");
    }
}

// menu principal
int main() {
    int op, distancia;
    char nome1[100], nome2[100];

    // iniciando a lista como NULL
    for (int i = 0; i < MAX_CIDADES; i++) {
        lista[i] = NULL;
    }

    while (1) {
        printf("\nMenu:\n1. Adicionar cidade\n2. Adicionar rota\n3. Calcular menor caminho\n0. Sair\nEscolha: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("Nome da cidade: ");
                scanf("%s", nome1);
                adicionarCidade(nome1);
                break;
            case 2:
                printf("Cidade origem: ");
                scanf("%s", nome1);
                printf("Cidade destino: ");
                scanf("%s", nome2);
                printf("Distância: ");
                scanf("%d", &distancia);
                adicionarRota(nome1, nome2, distancia);
                break;
           case 3:
                printf("Cidade origem: ");
                scanf("%s", nome1);
                printf("Cidade destino: ");
                scanf("%s", nome2);
                calcularMenorCaminho(nome1, nome2);
                break;
            case 0:
                return 0;
            default:
                printf("Opção inválida!\n");
        }
    }
}
