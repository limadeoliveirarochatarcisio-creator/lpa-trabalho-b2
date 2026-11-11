#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINHAS 4
#define COLUNAS 5
#define TAM_ID 7
#define TAM_DESC 60
#define CAPACIDADE_INICIAL 2

typedef struct {
    char id[TAM_ID + 1];
    char descricao[TAM_DESC + 1];
    int categoria;   /* 1 = carro, 2 = moto */
    int fileira;
    int coluna;
} Veiculo;

typedef struct {
    Veiculo *dados;
    int qtd;
    int capacidade;
} Colecao;

/* Coloca 0 (livre) em todas as vagas */
void iniciar_mapa(int mapa[LINHAS][COLUNAS]) {
    int i, j;
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            mapa[i][j] = 0;
        }
    }
}

/* Mostra o mapa: X = ocupada, . = livre */
void exibir_mapa(int mapa[LINHAS][COLUNAS]) {
    int i, j;

    printf("    ");
    for (j = 0; j < COLUNAS; j++) {
        printf("C%d ", j + 1);
    }
    printf("\n");

    for (i = 0; i < LINHAS; i++) {
        printf("F%d  ", i + 1);
        for (j = 0; j < COLUNAS; j++) {
            if (mapa[i][j] == 1) {
                printf("X  ");
            } else {
                printf(".  ");
            }
        }
        printf("\n");
    }
    printf("Legenda: X = ocupada; . = livre\n");
}

/* Reserva a memoria inicial da lista (2 registros) */
int iniciar_colecao(Colecao *c) {
    c->qtd = 0;
    c->capacidade = 0;
    c->dados = (Veiculo *)malloc(CAPACIDADE_INICIAL * sizeof(Veiculo));
    if (c->dados == NULL) {
        return 0;
    }
    c->capacidade = CAPACIDADE_INICIAL;
    return 1;
}

/* Devolve a memoria ao computador */
void liberar_colecao(Colecao *c) {
    free(c->dados);
    c->dados = NULL;
    c->qtd = 0;
    c->capacidade = 0;
}

int main(void) {
    int mapa[LINHAS][COLUNAS];
    Colecao colecao;

    iniciar_mapa(mapa);

    if (!iniciar_colecao(&colecao)) {
        printf("Erro de memoria.\n");
        return 1;
    }

    printf("Registros: %d | Capacidade: %d\n", colecao.qtd, colecao.capacidade);
    exibir_mapa(mapa);

    liberar_colecao(&colecao);
    return 0;
}
