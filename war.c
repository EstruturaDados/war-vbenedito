#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Struct Territorio:
 * - nome: nome do território (até 29 caracteres + '\0')
 * - cor : cor do exército dono do território (até 9 caracteres + '\0')
 * - tropas: quantidade de tropas no território
 */
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Protótipos das funções */
Territorio* cadastrarTerritorios(int n);
void exibirTerritorios(const Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);
int lerInteiro(const char* prompt, int min, int max);

int main(void) {
    srand((unsigned) time(NULL)); /* inicializa gerador de números aleatórios */

    printf("=== WAR Estruturado: Simulação de Ataque entre Territórios ===\n\n");

    int n = lerInteiro("Digite o número de territórios a cadastrar: ", 1, 1000);

    /* Alocação dinâmica do vetor de territórios */
    Territorio* mapa = cadastrarTerritorios(n);
    if (mapa == NULL) {
        fprintf(stderr, "Erro ao alocar memória para territórios.\n");
        return 1;
    }

    printf("\nMapa inicial:\n");
    exibirTerritorios(mapa, n);

    /* Menu de interação para realizar ataques */
    while (1) {
        printf("\n=== Menu ===\n");
        printf("1 - Realizar ataque\n");
        printf("2 - Exibir territórios\n");
        printf("0 - Sair\n");
        int opc = lerInteiro("Escolha uma opção: ", 0, 2);

        if (opc == 0) break;

        if (opc == 2) {
            exibirTerritorios(mapa, n);
            continue;
        }

        /* Realizar ataque */
        printf("\nEscolha o território atacante (índice de 1 a %d):\n", n);
        exibirTerritorios(mapa, n);
        int idxA = lerInteiro("Índice do atacante: ", 1, n) - 1;
        int idxD = lerInteiro("Índice do defensor:  ", 1, n) - 1;

        /* Validações: não permitir atacar si mesmo */
        if (idxA == idxD) {
            printf("Erro: o território atacante e defensor não podem ser o mesmo.\n");
            continue;
        }

        Territorio* atacante = &mapa[idxA];
        Territorio* defensor = &mapa[idxD];

        /* Validar que não são da mesma cor */
        if (strcmp(atacante->cor, defensor->cor) == 0) {
            printf("Erro: não é permitido atacar um território de sua própria cor (%s).\n", atacante->cor);
            continue;
        }

        /* Exigir tropas mínimas para atacar (1 tropa fica para manter o território) */
        if (atacante->tropas < 2) {
            printf("Erro: o atacante '%s' precisa ter pelo menos 2 tropas para realizar um ataque. Tropas atuais: %d\n",
                   atacante->nome, atacante->tropas);
            continue;
        }

        /* Executa o ataque (passagem por ponteiros) */
        atacar(atacante, defensor);

        /* Exibir resultado pós-ataque */
        printf("\nEstado dos territórios após o ataque:\n");
        exibirTerritorios(mapa, n);
    }

    /* Libera memória alocada */
    liberarMemoria(mapa);
    printf("Memória liberada. Programa finalizado.\n");

    return 0;
}

/* cadastrarTerritorios:
 * Aloca dinamicamente um vetor de Territorio com 'n' posições e lê os dados do usuário.
 * Retorna o ponteiro para o vetor ou NULL em caso de erro de alocação.
 */
Territorio* cadastrarTerritorios(int n) {
    /* calloc inicializa memória com zero */
    Territorio* mapa = (Territorio*) calloc((size_t)n, sizeof(Territorio));
    if (mapa == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do território %d/%d ---\n", i + 1, n);

        /* Lê nome (permite espaços), limite 29 caracteres */
        printf("Nome (use até 29 caracteres, pode ter espaços): ");
        if (scanf(" %29[^\n]", mapa[i].nome) != 1) {
            strcpy(mapa[i].nome, "NOME_INVALIDO");
        }

        /* Lê cor (palavra única típica), limite 9 caracteres */
        printf("Cor do exército (ex: vermelho, azul): ");
        if (scanf(" %9s", mapa[i].cor) != 1) {
            strcpy(mapa[i].cor, "SEM_COR");
        }

        /* Lê tropa com validação (>=0) */
        mapa[i].tropas = lerInteiro("Número de tropas (inteiro >= 0): ", 0, 1000000);
    }

    return mapa;
}

/* exibirTerritorios:
 * Percorre o vetor e imprime os dados de cada território com formatação clara.
 * Usa ponteiro constante para indicar que não modifica o mapa.
 */
void exibirTerritorios(const Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("[%2d] %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* atacar:
 * Simula um ataque entre dois territórios.
 * - Usa rand() para gerar 1 a 6 para cada lado (como um dado).
 * - Se atacante vence (soma do dado do atacante > soma do dado do defensor):
 *     -> o defensor troca de cor para a cor do atacante
 *     -> metade (integer division) das tropas do atacante é transferida para o defensor
 *     -> as tropas do atacante são reduzidas na mesma quantidade transferida
 * - Caso o atacante perca ou empate:
 *     -> atacante perde 1 tropa (representando custo do ataque)
 *
 * Todos os acessos são feitos via ponteiros.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n--- Ataque: %s (cor %s, tropas %d)  ->  %s (cor %s, tropas %d) ---\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    /* Gera um "dado" 1..6 para cada lado */
    int dadoAtq = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("Rolagem: atacante %d x defensor %d\n", dadoAtq, dadoDef);

    if (dadoAtq > dadoDef) {
        /* Vitória do atacante */
        int transferencia = atacante->tropas / 2; /* metade das tropas do atacante */
        if (transferencia < 1) transferencia = 1; /* garantir pelo menos 1 tropa transferida */

        /* Transferência: defensor muda de cor e recebe as tropas transferidas.
         * Escolhi sobrescrever as tropas do defensor com o número transferido (transfer)
         * conforme enunciado que pede "transferir a cor e metade das tropas".
         * Se preferir somar às tropas atuais do defensor, troque a linha abaixo por:
         *   defensor->tropas += transferencia;
         */
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = transferencia;
        atacante->tropas -= transferencia;

        printf("Resultado: ATACANTE VENCEU! Território '%s' agora pertence a '%s' e tem %d tropas.\n",
               defensor->nome, defensor->cor, defensor->tropas);
        printf("Tropas restantes no atacante '%s': %d\n", atacante->nome, atacante->tropas);
    } else {
        /* Derrota ou empate: atacante perde 1 tropa */
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("Resultado: ATACANTE PERDEU (ou empatou). '%s' perde 1 tropa. Tropas agora: %d\n",
               atacante->nome, atacante->tropas);
    }
}

/* liberarMemoria:
 * Libera a memória alocada dinamicamente para o mapa.
 */
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

/* lerInteiro:
 * Função utilitária para ler um inteiro entre min e max (inclusive).
 * Repete a leitura até o usuário inserir um valor válido.
 */
int lerInteiro(const char* prompt, int min, int max) {
    int val;
    int lidos;
    while (1) {
        printf("%s", prompt);
        lidos = scanf("%d", &val);
        if (lidos != 1) {
            /* limpa buffer até newline */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada inválida. Digite um número inteiro.\n");
            continue;
        }
        if (val < min || val > max) {
            printf("Valor fora do intervalo [%d, %d]. Tente novamente.\n", min, max);
            continue;
        }
        /* consome eventual newline remanescente (não estritamente necessário aqui) */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return val;
    }
}
