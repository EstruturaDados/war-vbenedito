#include <stdio.h>
#include <string.h>

#define NUM_TERRITORIOS 5

/* 
 * Struct Territorio:
 * Armazena informações básicas de um território:
 * - nome: nome do território (até 29 caracteres + '\0')
 * - cor: cor do exército dono do território (até 9 caracteres + '\0')
 * - tropas: quantidade de tropas no território
 */
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Protótipos */
void cadastrarTerritorios(Territorio territorios[], int n);
void exibirTerritorios(const Territorio territorios[], int n);

int main(void) {
    Territorio territorios[NUM_TERRITORIOS];

    printf("=== Sistema simples de cadastro de territórios (WAR - fase inicial) ===\n\n");

    cadastrarTerritorios(territorios, NUM_TERRITORIOS);

    printf("\nCadastro concluído. Exibindo territórios registrados:\n\n");
    exibirTerritorios(territorios, NUM_TERRITORIOS);

    return 0;
}

/*
 * cadastrarTerritorios:
 * Lê do usuário os dados para 'n' territórios e armazena no vetor fornecido.
 * Uso de scanf com especificadores limitados para evitar overflow.
 */
void cadastrarTerritorios(Territorio territorios[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Território %d de %d\n", i + 1, n);

        /* 
         * Lê o nome do território.
         * "%29[^\n]" lê até 29 caracteres ou até newline (permitindo espaços no nome).
         * O espaço antes do % faz com que caracteres de whitespace pendentes sejam ignorados.
         */
        printf("  Nome (use até 29 caracteres, pode ter espaços): ");
        if (scanf(" %29[^\n]", territorios[i].nome) != 1) {
            /* Caso improvável: definimos string vazia */
            strcpy(territorios[i].nome, "NOME_INVALIDO");
        }

        /* Lê a cor do exército (palavra única típica). %9s limita para evitar overflow */
        printf("  Cor do exército (ex: vermelho, azul): ");
        if (scanf(" %9s", territorios[i].cor) != 1) {
            strcpy(territorios[i].cor, "SEM_COR");
        }

        /* Lê o número de tropas. Tratamento simples para entradas inválidas. */
        printf("  Número de tropas (inteiro): ");
        while (scanf("%d", &territorios[i].tropas) != 1) {
            /* Entrada inválida: limpar buffer até newline e pedir de novo */
            int c;
            printf("    Entrada inválida. Digite um número inteiro para tropas: ");
            while ((c = getchar()) != '\n' && c != EOF) { /* descarta resto da linha */ }
        }

        printf("\n"); /* linha em branco entre cadastros para melhor usabilidade */
    }
}

/*
 * exibirTerritorios:
 * Percorre o vetor de territórios e imprime os dados com formatação clara.
 */
void exibirTerritorios(const Territorio territorios[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome : %s\n", territorios[i].nome);
        printf("  Cor  : %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("-----------------------------\n");
    }
}
