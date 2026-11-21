#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void iniciar_mapa(int mapa[LINHAS][COLUNAS]) {
    int i, j;
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            mapa[i][j] = 0;
        }
    }
}

void exibir_mapa(int mapa[LINHAS][COLUNAS]) {
    int i, j;

    printf("\n    ");
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

void liberar_colecao(Colecao *c) {
    free(c->dados);
    c->dados = NULL;
    c->qtd = 0;
    c->capacidade = 0;
}

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void para_maiusculo(char *str) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

int buscar_veiculo(const Colecao *c, const char *id) {
    int i;
    char id_busca[TAM_ID + 1];
    char id_atual[TAM_ID + 1];

    if (strlen(id) > TAM_ID) {
        return -1;
    }

    strcpy(id_busca, id);
    para_maiusculo(id_busca);

    for (i = 0; i < c->qtd; i++) {
        strcpy(id_atual, c->dados[i].id);
        para_maiusculo(id_atual);

        if (strcmp(id_atual, id_busca) == 0) {
            return i;
        }
    }
    return -1;
}

void consultar_veiculo(const Colecao *c) {
    char id[100];
    int idx;

    if (c->qtd == 0) {
        printf("\nNenhum veiculo cadastrado no estacionamento.\n");
        return;
    }

    printf("\n--- CONSULTAR VEICULO ---\n");
    printf("Digite o identificador: ");
    if (scanf("%99s", id) != 1) {
        printf("Entrada invalida.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    idx = buscar_veiculo(c, id);

    if (idx == -1) {
        printf("Veiculo com identificador '%s' nao localizado.\n", id);
    } else {
        printf("\nVeiculo encontrado:\n");
        printf("Identificador: %s\n", c->dados[idx].id);
        printf("Descricao: %s\n", c->dados[idx].descricao);
        printf("Categoria: %s\n", (c->dados[idx].categoria == 1) ? "Carro" : "Moto");
        printf("Posicao: Fileira %d, Coluna %d (F%d/C%d)\n",
               c->dados[idx].fileira,
               c->dados[idx].coluna,
               c->dados[idx].fileira,
               c->dados[idx].coluna);
    }
}

void listar_veiculos(const Colecao *c) {
    int i;

    if (c->qtd == 0) {
        printf("\nNao ha veiculos cadastrados no momento.\n");
        return;
    }

    printf("\n--- VEICULOS CADASTRADOS (%d) ---\n", c->qtd);
    for (i = 0; i < c->qtd; i++) {
        printf("[%d] ID: %s | Categoria: %-5s | Posicao: F%d/C%d | Descricao: %s\n",
               i + 1,
               c->dados[i].id,
               (c->dados[i].categoria == 1) ? "Carro" : "Moto",
               c->dados[i].fileira,
               c->dados[i].coluna,
               c->dados[i].descricao);
    }
}

int main(void) {
    int mapa[LINHAS][COLUNAS];
    Colecao colecao;
    int opcao = -1;

    iniciar_mapa(mapa);

    if (!iniciar_colecao(&colecao)) {
        printf("Erro ao alocar memoria inicial.\n");
        return 1;
    }

    do {
        printf("\n====================================\n");
        printf("   SISTEMA DE ESTACIONAMENTO (C)    \n");
        printf("====================================\n");
        printf("2 - Consultar veiculo por ID\n");
        printf("6 - Listar veiculos cadastrados\n");
        printf("7 - Exibir mapa de vagas\n");
        printf("0 - Encerrar\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida! Digite um numero.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 2:
                consultar_veiculo(&colecao);
                break;
            case 6:
                listar_veiculos(&colecao);
                break;
            case 7:
                exibir_mapa(mapa);
                break;
            case 0:
                printf("\nEncerrando sistema e liberando memoria...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    liberar_colecao(&colecao);
    return 0;
}

void mover_veiculo(Colecao *c, int mapa[LINHAS][COLUNAS]) {
    char id[100];
    int idx;
    int nova_f = 0, nova_c = 0;
    int f_ant, c_ant, f_novo_idx, c_novo_idx;

    if (c->qtd == 0) {
        printf("\nNenhum veiculo cadastrado no estacionamento.\n");
        return;
    }

    printf("\n--- MOVER VEICULO ---\n");
    printf("Digite o identificador do veiculo que deseja mover: ");
    if (scanf("%99s", id) != 1) {
        printf("Entrada invalida.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    idx = buscar_veiculo(c, id);
    if (idx == -1) {
        printf("Veiculo nao localizado.\n");
        return;
    }

    printf("Digite a nova fileira (1 a %d): ", LINHAS);
    if (scanf("%d", &nova_f) != 1 || nova_f < 1 || nova_f > LINHAS) {
        printf("Erro: Fileira invalida.\n");
        limpar_buffer();
        return;
    }

    printf("Digite a nova coluna (1 a %d): ", COLUNAS);
    if (scanf("%d", &nova_c) != 1 || nova_c < 1 || nova_c > COLUNAS) {
        printf("Erro: Coluna invalida.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    f_ant = c->dados[idx].fileira;
    c_ant = c->dados[idx].coluna;

    if (f_ant == nova_f && c_ant == nova_c) {
        printf("O veiculo ja se encontra nesta vaga.\n");
        return;
    }

    f_novo_idx = nova_f - 1;
    c_novo_idx = nova_c - 1;

    if (mapa[f_novo_idx][c_novo_idx] == 1) {
        printf("Erro: A vaga de destino F%d/C%d ja esta ocupada.\n", nova_f, nova_c);
        return;
    }

    mapa[f_ant - 1][c_ant - 1] = 0;
    mapa[f_novo_idx][c_novo_idx] = 1;

    c->dados[idx].fileira = nova_f;
    c->dados[idx].coluna = nova_c;

    printf("Sucesso: Veiculo %s movido para F%d/C%d!\n", c->dados[idx].id, nova_f, nova_c);
}

void atualizar_veiculo(Colecao *c) {
    char id[100];
    int idx;
    char nova_desc[150];
    int nova_cat = 0;

    if (c->qtd == 0) {
        printf("\nNenhum veiculo cadastrado no estacionamento.\n");
        return;
    }

    printf("\n--- ATUALIZAR VEICULO ---\n");
    printf("Digite o identificador do veiculo que deseja atualizar: ");
    if (scanf("%99s", id) != 1) {
        printf("Entrada invalida.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    idx = buscar_veiculo(c, id);
    if (idx == -1) {
        printf("Veiculo nao localizado.\n");
        return;
    }

    printf("Nova descricao (max 60 caracteres): ");
    if (fgets(nova_desc, sizeof(nova_desc), stdin) == NULL) {
        printf("Erro na leitura da descricao.\n");
        return;
    }
    nova_desc[strcspn(nova_desc, "\n")] = '\0';

    if (strlen(nova_desc) == 0 || strlen(nova_desc) > TAM_DESC) {
        printf("Erro: Descricao invalida.\n");
        return;
    }

    printf("Nova categoria (1 = Carro, 2 = Moto): ");
    if (scanf("%d", &nova_cat) != 1 || (nova_cat != 1 && nova_cat != 2)) {
        printf("Erro: Categoria invalida.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    strcpy(c->dados[idx].descricao, nova_desc);
    c->dados[idx].categoria = nova_cat;

    printf("Sucesso: Dados do veiculo %s atualizados!\n", c->dados[idx].id);
}
