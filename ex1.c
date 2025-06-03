#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 100   // Número máximo de usuários
#define MAX_NOME 100       // Tamanho máximo do nome do usuário

// Estrutura para lista de adjacência (amizades)
typedef struct No {
    int vertice;           // Índice do amigo
    struct No* prox;       // Próximo amigo na lista
} No;

// Estrutura para armazenar o nome do usuário
typedef struct {
    char nome[MAX_NOME];
} Usuario;

Usuario usuarios[MAX_VERTICES]; // Vetor de usuários
No* adj[MAX_VERTICES];          // Vetor de listas de adjacência (amizades)
int qtdUsuarios = 0;            // Contador de usuários cadastrados

// Estrutura de fila para BFS
typedef struct {
    int itens[MAX_VERTICES];
    int frente, tras;
} Fila;

// Inicializa a fila
void inicializarFila(Fila* f) {
    f->frente = -1;
    f->tras = -1;
}

// Verifica se a fila está vazia
int estaVazia(Fila* f) {
    return f->frente == -1;
}

// Adiciona elemento na fila
void enfileirar(Fila* f, int valor) {
    if (f->tras == MAX_VERTICES - 1) return;
    if (f->frente == -1) f->frente = 0;
    f->tras++;
    f->itens[f->tras] = valor;
}

// Remove elemento da fila
int desenfileirar(Fila* f) {
    if (estaVazia(f)) return -1;
    int valor = f->itens[f->frente];
    f->frente++;
    if (f->frente > f->tras) f->frente = f->tras = -1;
    return valor;
}

// Busca usuário pelo nome e retorna o índice
int buscarUsuario(char* nome) {
    for (int i = 0; i < qtdUsuarios; i++)
        if (strcmp(usuarios[i].nome, nome) == 0)
            return i;
    return -1;
}

// Adiciona novo usuário ao sistema
void adicionarUsuario(char* nome) {
    if (qtdUsuarios >= MAX_VERTICES) {
        printf("Limite de usuarios atingido.\n");
        return;
    }
    strcpy(usuarios[qtdUsuarios].nome, nome);
    adj[qtdUsuarios] = NULL; // Inicializa lista de amigos vazia
    printf("Usuario %s adicionado com id %d\n", nome, qtdUsuarios);
    qtdUsuarios++;
}

// Cria amizade (aresta não direcionada) entre dois usuários
void adicionarAresta(int u, int v) {
    // Adiciona v na lista de u
    No* novo = (No*)malloc(sizeof(No));
    novo->vertice = v;
    novo->prox = adj[u];
    adj[u] = novo;

    // Adiciona u na lista de v
    novo = (No*)malloc(sizeof(No));
    novo->vertice = u;
    novo->prox = adj[v];
    adj[v] = novo;
}

// Mostra todos os amigos de um usuário
void mostrarAmigos(int id) {
    printf("Amigos de %s: ", usuarios[id].nome);
    No* atual = adj[id];
    if (!atual) printf("Nenhum amigo.");
    while (atual) {
        printf("%s ", usuarios[atual->vertice].nome);
        atual = atual->prox;
    }
    printf("\n");
}

// Sugere amigos usando BFS (amigos de amigos que não são amigos diretos)
void sugerirAmigos(int id) {
    int visitado[MAX_VERTICES] = {0};
    int amigosDiretos[MAX_VERTICES] = {0};
    Fila f;
    inicializarFila(&f);

    visitado[id] = 1;
    enfileirar(&f, id);

    // Marca amigos diretos
    No* temp = adj[id];
    while (temp) {
        amigosDiretos[temp->vertice] = 1;
        temp = temp->prox;
    }

    printf("Sugestoes de amigos para %s: ", usuarios[id].nome);

    while (!estaVazia(&f)) {
        int atual = desenfileirar(&f);
        No* ptr = adj[atual];
        while (ptr) {
            int v = ptr->vertice;
            if (!visitado[v]) {
                enfileirar(&f, v);
                visitado[v] = 1;
                // Se não é amigo direto e não é ele mesmo, sugere
                if (!amigosDiretos[v] && v != id)
                    printf("%s ", usuarios[v].nome);
            }
            ptr = ptr->prox;
        }
    }
    printf("\n");
}

// Função auxiliar para imprimir o caminho de amizades
void imprimirCaminho(int* caminho, int tam) {
    for (int i = 0; i < tam; i++) {
        printf("%s", usuarios[caminho[i]].nome);
        if (i < tam - 1) printf(" -> ");
    }
    printf("\n");
}

// DFS que mostra o caminho de amizades até cada usuário do grupo
void dfsCaminho(int v, int* visitado, int* caminho, int tam) {
    visitado[v] = 1;
    caminho[tam] = v;
    tam++;

    if (tam > 1) { // Não imprime o próprio usuário como caminho
        printf("Caminho: ");
        imprimirCaminho(caminho, tam);
    }

    No* atual = adj[v];
    while (atual) {
        if (!visitado[atual->vertice])
            dfsCaminho(atual->vertice, visitado, caminho, tam);
        atual = atual->prox;
    }
}

// Mostra todos do grupo conectado ao usuário (DFS), exibindo o caminho de amizades
void mostrarGrupoComCaminho(int id) {
    int visitado[MAX_VERTICES] = {0};
    int caminho[MAX_VERTICES];
    printf("Caminhos de amizade a partir de %s:\n", usuarios[id].nome);
    dfsCaminho(id, visitado, caminho, 0);
}

int main() {
    int op;
    char nome1[MAX_NOME], nome2[MAX_NOME];

    // Inicializa listas de adjacência
    for (int i = 0; i < MAX_VERTICES; i++)
        adj[i] = NULL;

    // Menu principal
    do {
        printf("\n1-Adicionar usuario\n2-Criar amizade\n3-Mostrar amigos\n4-Sugerir amigos\n5-Mostrar grupo\n6-Mostrar grupo com caminhos\n0-Sair\nOpcao: ");
        scanf("%d", &op);
        switch(op) {
            case 1:
                printf("Nome: ");
                scanf("%s", nome1);
                adicionarUsuario(nome1);
                break;
            case 2:
                printf("Nome 1: ");
                scanf("%s", nome1);
                printf("Nome 2: ");
                scanf("%s", nome2);
                {
                    int id1 = buscarUsuario(nome1);
                    int id2 = buscarUsuario(nome2);
                    if (id1 != -1 && id2 != -1)
                        adicionarAresta(id1, id2);
                    else
                        printf("Usuario(s) nao encontrado(s).\n");
                }
                break;
            case 3:
                printf("Nome: ");
                scanf("%s", nome1);
                {
                    int id = buscarUsuario(nome1);
                    if (id != -1)
                        mostrarAmigos(id);
                    else
                        printf("Usuario nao encontrado.\n");
                }
                break;
            case 4:
                printf("Nome: ");
                scanf("%s", nome1);
                {
                    int id = buscarUsuario(nome1);
                    if (id != -1)
                        sugerirAmigos(id);
                    else
                        printf("Usuario nao encontrado.\n");
                }
                break;
            case 5:
                printf("Nome: ");
                scanf("%s", nome1);
                {
                    int id = buscarUsuario(nome1);
                    if (id != -1) {
                        // Mostra grupo sem caminhos (apenas nomes)
                        int visitado[MAX_VERTICES] = {0};
                        printf("Grupo de %s: ", usuarios[id].nome);
                        // Função DFS simples
                        void dfs(int v, int* visitado) {
                            visitado[v] = 1;
                            printf("%s ", usuarios[v].nome);
                            No* atual = adj[v];
                            while (atual) {
                                if (!visitado[atual->vertice])
                                    dfs(atual->vertice, visitado);
                                atual = atual->prox;
                            }
                        }
                        dfs(id, visitado);
                        printf("\n");
                    } else
                        printf("Usuario nao encontrado.\n");
                }
                break;
            case 6:
                printf("Nome: ");
                scanf("%s", nome1);
                {
                    int id = buscarUsuario(nome1);
                    if (id != -1)
                        mostrarGrupoComCaminho(id);
                    else
                        printf("Usuario nao encontrado.\n");
                }
                break;
        }
    } while(op != 0);

    return 0;
}