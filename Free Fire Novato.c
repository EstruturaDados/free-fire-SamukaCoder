/*
 inventario.c
 Sistema de inventário (mochila) com struct Item e lista sequencial (até 10 itens).
 Funcionalidades: cadastrar, remover por nome, listar e busca sequencial por nome.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

Item mochila[MAX_ITENS];
int n_itens = 0;

/* Retira newline do fim de string lida por fgets */
void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}

/* Comparação case-insensitive simples (retorna 0 se iguais) */
int str_icmp(const char *a, const char *b) {
    while (*a && *b) {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb) return (unsigned char)ca - (unsigned char)cb;
        a++; b++;
    }
    return (unsigned char)tolower((unsigned char)*a) - (unsigned char)tolower((unsigned char)*b);
}

/* Lista os itens atuais */
void listar_itens() {
    printf("\n--- Conteúdo da mochila (%d/%d) ---\n", n_itens, MAX_ITENS);
    if (n_itens == 0) {
        printf("Mochila vazia.\n");
        return;
    }
    for (int i = 0; i < n_itens; ++i) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i+1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

/* Busca sequencial por nome; retorna índice ou -1 se não encontrado */
int busca_sequencial(const char *nome) {
    for (int i = 0; i < n_itens; ++i) {
        if (str_icmp(mochila[i].nome, nome) == 0) return i;
    }
    return -1;
}

/* Cadastro de item: se já existir item com mesmo nome, soma a quantidade (comportamento escolhido) */
void cadastrar_item() {
    if (n_itens >= MAX_ITENS) {
        printf("\nMochila cheia! Remova algum item antes de cadastrar.\n");
        listar_itens();
        return;
    }

    char buffer[128];
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade = 0;

    printf("\nCadastro de novo item\n");
    printf("Nome (até %d chars): ", MAX_NOME - 1);
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    trim_newline(buffer);
    strncpy(nome, buffer, MAX_NOME);
    nome[MAX_NOME-1] = '\0';

    printf("Tipo (ex: arma, munição, cura) (até %d chars): ", MAX_TIPO - 1);
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    trim_newline(buffer);
    strncpy(tipo, buffer, MAX_TIPO);
    tipo[MAX_TIPO-1] = '\0';

    printf("Quantidade: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    quantidade = atoi(buffer);
    if (quantidade <= 0) {
        printf("Quantidade inválida. Operação cancelada.\n");
        return;
    }

    int idx = busca_sequencial(nome);
    if (idx != -1) {
        // Se já existir, somamos as quantidades
        mochila[idx].quantidade += quantidade;
        printf("Item já existente. Quantidade atualizada (somada).\n");
    } else {
        // inserir no final
        strncpy(mochila[n_itens].nome, nome, MAX_NOME);
        strncpy(mochila[n_itens].tipo, tipo, MAX_TIPO);
        mochila[n_itens].quantidade = quantidade;
        n_itens++;
        printf("Item cadastrado com sucesso.\n");
    }

    listar_itens();
}

/* Remoção por nome: desloca os elementos à esquerda para manter lista compacta */
void remover_item() {
    char buffer[128];
    char nome[MAX_NOME];

    printf("\nRemover item\n");
    printf("Informe o nome do item a remover: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    trim_newline(buffer);
    strncpy(nome, buffer, MAX_NOME);
    nome[MAX_NOME-1] = '\0';

    int idx = busca_sequencial(nome);
    if (idx == -1) {
        printf("Item '%s' não encontrado.\n", nome);
    } else {
        // desloca
        for (int i = idx; i < n_itens - 1; ++i) {
            mochila[i] = mochila[i+1];
        }
        n_itens--;
        printf("Item '%s' removido com sucesso.\n", nome);
    }

    listar_itens();
}

/* Buscar e exibir item por nome */
void buscar_item() {
    char buffer[128];
    char nome[MAX_NOME];

    printf("\nBuscar item\n");
    printf("Informe o nome do item: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    trim_newline(buffer);
    strncpy(nome, buffer, MAX_NOME);
    nome[MAX_NOME-1] = '\0';

    int idx = busca_sequencial(nome);
    if (idx == -1) {
        printf("Item '%s' não encontrado.\n", nome);
    } else {
        printf("Item encontrado:\n");
        printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
               mochila[idx].nome, mochila[idx].tipo, mochila[idx].quantidade);
    }

    listar_itens();
}

/* Menu principal */
int main() {
    char linha[32];
    int opc = 0;

    printf("=== Sistema de Inventário (Mochila de Loot Inicial) ===\n");

    while (1) {
        printf("\nEscolha uma opção:\n");
        printf("1 - Cadastrar item\n");
        printf("2 - Remover item por nome\n");
        printf("3 - Buscar item por nome (busca sequencial)\n");
        printf("4 - Listar itens\n");
        printf("0 - Sair\n");
        printf("Opção: ");

        if (!fgets(linha, sizeof(linha), stdin)) break;
        opc = atoi(linha);

        switch (opc) {
            case 1: cadastrar_item(); break;
            case 2: remover_item(); break;
            case 3: buscar_item(); break;
            case 4: listar_itens(); break;
            case 0:
                printf("Saindo. Até mais!\n");
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
