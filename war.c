/*
 * Missão: Evoluir a base de dados de territórios para
 * incluir um sistema de ataque, utilizando alocação
 * dinâmica de memória e ponteiros.
 */

// --- Bibliotecas Necessárias ---
#include <stdio.h>    // Para entrada/saída (printf, scanf, fgets, getchar)
#include <stdlib.h>   // Para alocação dinâmica (malloc, free), aleatórios (rand, srand) e exit
#include <string.h>   // Para manipulação de strings (strcpy, strcmp, strcspn)
#include <time.h>     // Para inicializar o gerador aleatório (time)

// --- Struct Atualizada ---
/*
 * Documentação:
 * A struct Territorio armazena os dados de cada
 * unidade do mapa.
 */
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// --- Protótipos das Funções (Modularização) ---
void limparBuffer();
void cadastrarTerritorios(struct Territorio* mapa, int total);
void exibirTerritorios(struct Territorio* mapa, int total);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa);

// --- Função Principal ---
int main() {
    struct Territorio* mapa = NULL; // Ponteiro para o vetor de territórios
    int totalTerritorios = 0;
    int idAtacante, idDefensor;
    int entradaValida = 0;

    /*
     * Documentação: Inicializa o gerador de números aleatórios.
     * srand(time(NULL)) usa o tempo atual como semente para
     * garantir que os resultados de rand() sejam diferentes
     * a cada execução do programa.
     */
    srand(time(NULL));

    // --- Alocação de Memória ---
    printf("--- CONFIGURAÇÃO DO MAPA (WAR ESTRUTURADO) ---\n");
    printf("Quantos territórios existirão no mapa? ");
    scanf("%d", &totalTerritorios);
    limparBuffer(); // Limpa o '\n' deixado pelo scanf

    /*
     * Documentação: Alocação dinâmica de memória.
     * Usamos malloc para alocar um bloco de memória contínuo,
     * suficiente para 'totalTerritorios' structs 'Territorio'.
     * O ponteiro 'mapa' apontará para o início deste bloco.
     */
    mapa = (struct Territorio*) malloc(totalTerritorios * sizeof(struct Territorio));

    // Verificação de falha na alocação
    if (mapa == NULL) {
        printf("Erro fatal! Falha ao alocar memória para o mapa.\n");
        exit(1); // Encerra o programa se a memória não puder ser alocada
    }

    printf("Memória alocada para %d territórios.\n", totalTerritorios);

    // --- Funções de Cadastro e Exibição Inicial ---
    cadastrarTerritorios(mapa, totalTerritorios);
    printf("\n\n--- MAPA ATUAL ---\n");
    exibirTerritorios(mapa, totalTerritorios);

    // --- Interface de Ataque ---
    printf("\n\n--- FASE DE ATAQUE ---\n");

    // Loop para garantir entrada válida
    while (!entradaValida) {
        printf("Digite o ID (1 a %d) do território ATACANTE: ", totalTerritorios);
        scanf("%d", &idAtacante);
        limparBuffer();

        printf("Digite o ID (1 a %d) do território DEFENSOR: ", totalTerritorios);
        scanf("%d", &idDefensor);
        limparBuffer();

        // Converte o ID (base 1) para índice do vetor (base 0)
        idAtacante--;
        idDefensor--;

        // --- Validações de Ataque ---

        // Validação 1: Índices dentro dos limites
        if (idAtacante < 0 || idAtacante >= totalTerritorios || idDefensor < 0 || idDefensor >= totalTerritorios) {
            printf("Erro: ID de território inválido. Tente novamente.\n\n");
            continue; // Volta ao início do 'while'
        }

        // Validação 2: Não atacar a si mesmo
        if (idAtacante == idDefensor) {
            printf("Erro: O território não pode atacar a si mesmo. Tente novamente.\n\n");
            continue;
        }

        // Validação 3: Não atacar territórios aliados (mesma cor)
        /*
         * Uso de Ponteiros: Acessamos os dados usando a sintaxe de ponteiro
         * (mapa + índice)->campo, que é equivalente a mapa[índice].campo.
         */
        if (strcmp((mapa + idAtacante)->cor, (mapa + idDefensor)->cor) == 0) {
            printf("Erro: Você não pode atacar um território da sua própria cor (%s). Tente novamente.\n\n", (mapa + idAtacante)->cor);
            continue;
        }

        // Validação 4: Deve ter mais de 1 tropa para atacar
        if ((mapa + idAtacante)->tropas <= 1) {
            printf("Erro: O território atacante (%s) precisa de mais de 1 tropa para iniciar um ataque. Tente novamente.\n\n", (mapa + idAtacante)->nome);
            continue;
        }

        // Se passou em todas as validações
        entradaValida = 1;
    }

    // --- Execução do Ataque ---
    printf("\nIniciando ataque de %s contra %s...\n", (mapa + idAtacante)->nome, (mapa + idDefensor)->nome);
    /*
     * Uso de Ponteiros: Passamos o ENDEREÇO (&) dos elementos específicos
     * do vetor para a função 'atacar', que espera ponteiros.
     */
    atacar(&mapa[idAtacante], &mapa[idDefensor]);

    // --- Exibição Pós-Ataque ---
    printf("\n\n--- MAPA APÓS O ATAQUE ---\n");
    exibirTerritorios(mapa, totalTerritorios);

    // --- Liberação de Memória ---
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções Modulares ---

/**
 * @brief Consome caracteres extras do buffer de entrada (stdin).
 * Essencial após um scanf() que é seguido por um fgets().
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Cadastra os dados de todos os territórios.
 * @param mapa Ponteiro para o início do vetor de territórios.
 * @param total Número total de territórios a cadastrar.
 */
void cadastrarTerritorios(struct Territorio* mapa, int total) {
    printf("\n--- CADASTRO DE TERRITÓRIOS ---\n");
    for (int i = 0; i < total; i++) {
        printf("\n--- Dados do Território %d ---\n", i + 1);

        /*
         * Uso de Ponteiros:
         * (mapa + i) calcula o endereço do i-ésimo elemento
         * a partir do ponteiro base 'mapa'.
         * A seta '->' acessa um membro da struct através de um ponteiro.
         * Ex: (mapa + i)->nome é o mesmo que mapa[i].nome
         */
        
        printf("Digite o nome: ");
        fgets((mapa + i)->nome, 30, stdin);
        (mapa + i)->nome[strcspn((mapa + i)->nome, "\n")] = 0;

        printf("Digite a cor: ");
        fgets((mapa + i)->cor, 10, stdin);
        (mapa + i)->cor[strcspn((mapa + i)->cor, "\n")] = 0;

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &(mapa + i)->tropas);
        limparBuffer(); // Limpa o buffer após o scanf
    }
}

/**
 * @brief Exibe os dados de todos os territórios cadastrados.
 * @param mapa Ponteiro para o início do vetor de territórios.
 * @param total Número total de territórios.
 */
void exibirTerritorios(struct Territorio* mapa, int total) {
    printf("--------------------------------------\n");
    for (int i = 0; i < total; i++) {
        // Acessando os dados via aritmética de ponteiros
        printf("[Território %d] %s (Cor: %s, Tropas: %d)\n",
               i + 1,
               (mapa + i)->nome,
               (mapa + i)->cor,
               (mapa + i)->tropas);
    }
    printf("--------------------------------------\n");
}

/**
 * @brief Simula um ataque (rolagem de dados) entre dois territórios.
 * @param atacante Ponteiro para a struct do território atacante.
 * @param defensor Ponteiro para a struct do território defensor.
 */
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    // Simula dados de 6 faces
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("--- SIMULAÇÃO DE BATALHA ---\n");
    printf("  > Atacante (%s) rolou... %d\n", atacante->nome, dadoAtaque);
    printf("  > Defensor (%s) rolou... %d\n", defensor->nome, dadoDefesa);

    // Requisito: Atacante vence se o dado for MAIOR
    if (dadoAtaque > dadoDefesa) {
        printf("VITÓRIA DO ATACANTE! %s conquistou %s.\n", atacante->nome, defensor->nome);

        // Requisito: "transfira a cor e metade das tropas"
        // O atacante deve deixar pelo menos 1 tropa para trás.
        // A validação no 'main' já garantiu que atacante->tropas > 1.
        
        // Ex: 5 tropas -> move 2 (5/2), deixa 3
        // Ex: 3 tropas -> move 1 (3/2), deixa 2
        // Ex: 2 tropas -> move 1 (2/2), deixa 1
        int tropasMovidas = atacante->tropas / 2;
        
        // Atualiza o defensor (agora conquistado)
        defensor->tropas = tropasMovidas;
        strcpy(defensor->cor, atacante->cor); // Transfere a cor

        // Atualiza o atacante (perde as tropas que moveu)
        atacante->tropas = atacante->tropas - tropasMovidas;

        printf("  > %s agora tem %d tropas.\n", defensor->nome, defensor->tropas);
        printf("  > %s ficou com %d tropas.\n", atacante->nome, atacante->tropas);

    } else {
        // Requisito: Se perder (ou empatar), o atacante perde 1 tropa
        printf("ATAQUE FALHOU! O defensor (%s) resistiu.\n", defensor->nome);
        
        // O atacante não pode ficar com menos de 1 tropa
        if (atacante->tropas > 1) {
            atacante->tropas--;
            printf("  > O atacante (%s) perdeu 1 tropa, restando %d.\n", atacante->nome, atacante->tropas);
        } else {
            printf("  > O atacante (%s) já está com a tropa mínima (1) e não perdeu tropas.\n", atacante->nome);
        }
    }
    printf("--- FIM DA BATALHA ---\n");
}

/**
 * @brief Libera a memória alocada dinamicamente para o mapa.
 * @param mapa Ponteiro para o bloco de memória a ser liberado.
 */
void liberarMemoria(struct Territorio* mapa) {
    /*
     * Gerenciamento de Memória:
     * A função 'free' libera o bloco de memória apontado por 'mapa',
     * que foi previamente alocado com 'malloc'. Isso evita
     * vazamentos de memória (memory leaks).
     */
    printf("\nLiberando memória do mapa...\n");
    free(mapa);
    mapa = NULL; // Boa prática: anular o ponteiro após liberar
    printf("Memória liberada com sucesso.\n");
}