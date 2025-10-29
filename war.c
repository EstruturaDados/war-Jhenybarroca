/*
 * Missão: Implementar missões estratégicas individuais.
 *
 * Esta é a V3 do sistema, baseada na V2 (ataques e ponteiros).
 * Novas funcionalidades:
 * - Lista de missões pré-definidas.
 * - Alocação dinâmica para as missões dos jogadores (P1 e P2).
 * - Funções para atribuir e verificar missões.
 * - Um loop de jogo principal baseado em turnos.
 */

// --- Bibliotecas Necessárias ---
#include <stdio.h>    // Para entrada/saída (printf, scanf, fgets, getchar)
#include <stdlib.h>   // Para alocação dinâmica (malloc, free), aleatórios (rand, srand) e exit
#include <string.h>   // Para manipulação de strings (strcpy, strcmp, strcspn)
#include <time.h>     // Para inicializar o gerador aleatório (time)

// --- Struct de Territórios (Existente) ---
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// --- Vetor Global de Missões ---
/*
 * Documentação:
 * Criamos um vetor global (const) de strings (ponteiros para char)
 * que armazena as descrições das missões pré-definidas.
 */
const char* LISTA_MISSOES[] = {
    "Conquistar 4 territorios",
    "Eliminar a cor Vermelha",
    "Eliminar a cor Azul",
    "Ter 15 tropas no total",
    "Conquistar o territorio 'Brasil'" // A lógica de verificação procurará este nome exato
};
// Armazena o total de missões para o sorteio
const int TOTAL_MISSOES = 5;


// --- Protótipos das Funções (Modularização) ---
void limparBuffer();

// Funções do Mapa (V2)
void cadastrarTerritorios(struct Territorio* mapa, int total);
void exibirTerritorios(struct Territorio* mapa, int total);

// Função de Batalha (V2)
void atacar(struct Territorio* atacante, struct Territorio* defensor);

// Funções de Missão (V3)
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
void exibirMissao(const char* missao, const char* nomeJogador);
int verificarMissao(const char* missao, struct Territorio* mapa, int totalTerritorios, const char* corJogador);

// Função de Gerenciamento de Memória (V3 - Atualizada)
void liberarMemoria(struct Territorio* mapa, char* missaoP1, char* missaoP2);


// --- Função Principal ---
int main() {
    // Variáveis do Jogo
    struct Territorio* mapa = NULL; // Ponteiro para o vetor de territórios (V2)
    int totalTerritorios = 0;
    int idAtacante, idDefensor;
    
    // Variáveis dos Jogadores (V3)
    const char* corP1 = "Azul";
    const char* corP2 = "Vermelho";
    char* missaoP1 = NULL; // Ponteiro para a string da missão do P1
    char* missaoP2 = NULL; // Ponteiro para a string da missão do P2

    // Variáveis de Controle de Jogo (V3)
    int vencedor = 0; // 0 = Jogo rodando, 1 = P1 venceu, 2 = P2 venceu
    int turno = 1;    // 1 = Turno do P1, 2 = Turno do P2
    
    /*
     * Documentação: Inicializa o gerador de números aleatórios (srand)
     * para garantir que o sorteio de missões e os dados de ataque
     * sejam diferentes a cada execução.
     */
    srand(time(NULL));

    // --- Alocação de Memória (Mapa) ---
    printf("--- CONFIGURAÇÃO DO MAPA (WAR ESTRUTURADO V3) ---\n");
    printf("Quantos territórios existirão no mapa? ");
    scanf("%d", &totalTerritorios);
    limparBuffer(); // Limpa o '\n'

    mapa = (struct Territorio*) malloc(totalTerritorios * sizeof(struct Territorio));

    if (mapa == NULL) {
        printf("Erro fatal! Falha ao alocar memória para o mapa.\n");
        exit(1);
    }

    // --- Alocação de Memória (Missões) ---
    /*
     * Documentação: Aloca dinamicamente espaço para as strings das
     * missões dos jogadores. Usamos 100 char como tamanho seguro
     * para qualquer descrição de missão.
     */
    missaoP1 = (char*) malloc(100 * sizeof(char));
    missaoP2 = (char*) malloc(100 * sizeof(char));

    if (missaoP1 == NULL || missaoP2 == NULL) {
        printf("Erro fatal! Falha ao alocar memória para as missões.\n");
        // Chama a função de limpeza para liberar o que já foi alocado
        liberarMemoria(mapa, missaoP1, missaoP2);
        exit(1);
    }
    printf("Memória alocada para o mapa e missões.\n");

    // --- Atribuição e Exibição das Missões ---
    atribuirMissao(missaoP1, LISTA_MISSOES, TOTAL_MISSOES);
    atribuirMissao(missaoP2, LISTA_MISSOES, TOTAL_MISSOES);

    printf("\n--- DISTRIBUIÇÃO DE MISSÕES ---\n");
    // Interface: Exibe a missão uma única vez no início
    exibirMissao(missaoP1, "Jogador 1 (Azul)");
    exibirMissao(missaoP2, "Jogador 2 (Vermelho)");
    printf("\n(Estas missões são secretas no jogo real!)\n");
    printf("Pressione ENTER para cadastrar os territórios...");
    limparBuffer(); // Pausa para o usuário ler

    // --- Cadastro e Exibição Inicial do Mapa ---
    cadastrarTerritorios(mapa, totalTerritorios);
    printf("\n\n--- MAPA INICIAL ---\n");
    exibirTerritorios(mapa, totalTerritorios);

    // --- Loop Principal do Jogo (Turnos) ---
    while (vencedor == 0) {
        const char* corTurno;
        const char* nomeTurno;

        // Define de quem é o turno
        if (turno == 1) {
            corTurno = corP1;
            nomeTurno = "Jogador 1 (Azul)";
        } else {
            corTurno = corP2;
            nomeTurno = "Jogador 2 (Vermelho)";
        }

        printf("\n\n======================================\n");
        printf("--- TURNO DE: %s ---\n", nomeTurno);
        printf("======================================\n");
        
        // --- Fase de Ataque (Adaptada da V2) ---
        int entradaValida = 0;
        int pulouTurno = 0;

        while (!entradaValida && !pulouTurno) {
            printf("Digite o ID (1 a %d) do território ATACANTE (ou 0 para pular turno): ", totalTerritorios);
            scanf("%d", &idAtacante);
            limparBuffer();

            if (idAtacante == 0) {
                printf("%s pulou o turno.\n", nomeTurno);
                pulouTurno = 1;
                continue; // Sai do loop de validação
            }
            
            printf("Digite o ID (1 a %d) do território DEFENSOR: ", totalTerritorios);
            scanf("%d", &idDefensor);
            limparBuffer();

            // Converte ID (base 1) para índice (base 0)
            idAtacante--;
            idDefensor--;

            // Validação 1: Índices
            if (idAtacante < 0 || idAtacante >= totalTerritorios || idDefensor < 0 || idDefensor >= totalTerritorios) {
                printf("Erro: ID de território inválido. Tente novamente.\n\n");
                continue;
            }
            // Validação 2: Não atacar a si mesmo
            if (idAtacante == idDefensor) {
                printf("Erro: O território não pode atacar a si mesmo. Tente novamente.\n\n");
                continue;
            }
            
            /*
             * Validação 3 (V3): O jogador do turno SÓ PODE atacar
             * COM territórios da SUA PRÓPRIA COR.
             */
            if (strcmp((mapa + idAtacante)->cor, corTurno) != 0) {
                printf("Erro: %s só pode atacar com territórios da cor %s. Tente novamente.\n\n", nomeTurno, corTurno);
                continue;
            }
            
            // Validação 4 (V2): Não atacar territórios aliados
            if (strcmp((mapa + idAtacante)->cor, (mapa + idDefensor)->cor) == 0) {
                printf("Erro: Você não pode atacar um território que já é seu (%s). Tente novamente.\n\n", (mapa + idAtacante)->cor);
                continue;
            }
            
            // Validação 5 (V2): Ter mais de 1 tropa para atacar
            if ((mapa + idAtacante)->tropas <= 1) {
                printf("Erro: O território atacante (%s) precisa de mais de 1 tropa. Tente novamente.\n\n", (mapa + idAtacante)->nome);
                continue;
            }

            entradaValida = 1; // Passou em todas as validações
        }

        // --- Execução do Ataque ---
        if (entradaValida) {
            printf("\nIniciando ataque de %s contra %s...\n", (mapa + idAtacante)->nome, (mapa + idDefensor)->nome);
            // Passa os endereços por referência para a função 'atacar'
            atacar(&mapa[idAtacante], &mapa[idDefensor]);

            // Exibição Pós-Ataque
            printf("\n--- MAPA APÓS O ATAQUE ---\n");
            exibirTerritorios(mapa, totalTerritorios);
        }

        // --- Verificação da Missão (Ao final do turno) ---
        /*
         * Documentação:
         * Após o ataque (ou pulo), o sistema verifica silenciosamente
         * se algum dos jogadores cumpriu sua missão.
         */
        if (verificarMissao(missaoP1, mapa, totalTerritorios, corP1)) {
            vencedor = 1;
        } else if (verificarMissao(missaoP2, mapa, totalTerritorios, corP2)) {
            vencedor = 2;
        }

        // Troca o turno, se ninguém venceu
        if (vencedor == 0) {
            turno = (turno == 1) ? 2 : 1;
        }
        
    } // --- Fim do Loop Principal (while vencedor == 0) ---

    // --- Declaração do Vencedor ---
    printf("\n\n======================================\n");
    printf("--- FIM DE JOGO! ---\n");
    if (vencedor == 1) {
        printf("O JOGADOR 1 (AZUL) VENCEU!\n");
        printf("Missão Cumprida: %s\n", missaoP1);
    } else {
        printf("O JOGADOR 2 (VERMELHO) VENCEU!\n");
        printf("Missão Cumprida: %s\n", missaoP2);
    }
    printf("======================================\n");


    // --- Liberação de Memória ---
    liberarMemoria(mapa, missaoP1, missaoP2);

    return 0;
}


// --- Implementação das Funções Modulares ---

/**
 * @brief (V2) Consome caracteres extras do buffer de entrada (stdin).
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief (V2) Cadastra os dados de todos os territórios via ponteiros.
 */
void cadastrarTerritorios(struct Territorio* mapa, int total) {
    printf("\n--- CADASTRO DE TERRITÓRIOS ---\n");
    printf("Use as cores '%s' e '%s' para os jogadores.\n", "Azul", "Vermelho");
    for (int i = 0; i < total; i++) {
        printf("\n--- Dados do Território %d ---\n", i + 1);
        
        printf("Digite o nome: ");
        fgets((mapa + i)->nome, 30, stdin);
        (mapa + i)->nome[strcspn((mapa + i)->nome, "\n")] = 0;

        printf("Digite a cor (Azul/Vermelho/Neutro): ");
        fgets((mapa + i)->cor, 10, stdin);
        (mapa + i)->cor[strcspn((mapa + i)->cor, "\n")] = 0;

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &(mapa + i)->tropas);
        limparBuffer();
    }
}

/**
 * @brief (V2) Exibe os dados de todos os territórios cadastrados.
 */
void exibirTerritorios(struct Territorio* mapa, int total) {
    printf("--------------------------------------\n");
    for (int i = 0; i < total; i++) {
        printf("[ID %d] %s (Cor: %s, Tropas: %d)\n",
               i + 1, // Exibe o ID (base 1) para o usuário
               (mapa + i)->nome,
               (mapa + i)->cor,
               (mapa + i)->tropas);
    }
    printf("--------------------------------------\n");
}

/**
 * @brief (V2) Simula um ataque (rolagem de dados) entre dois territórios.
 */
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("--- SIMULAÇÃO DE BATALHA ---\n");
    printf("  > Atacante (%s) rolou... %d\n", atacante->nome, dadoAtaque);
    printf("  > Defensor (%s) rolou... %d\n", defensor->nome, dadoDefesa);

    // Atacante vence se o dado for MAIOR
    if (dadoAtaque > dadoDefesa) {
        printf("VITÓRIA DO ATACANTE! %s conquistou %s.\n", atacante->nome, defensor->nome);
        
        // Regra: "metade das tropas" (arredondado para baixo)
        // O atacante deve deixar pelo menos 1 tropa para trás.
        int tropasMovidas = atacante->tropas / 2;
        if (tropasMovidas == 0) tropasMovidas = 1; // Garante mover pelo menos 1
        
        // Atualiza o defensor (agora conquistado)
        defensor->tropas = tropasMovidas;
        strcpy(defensor->cor, atacante->cor); // Transfere a cor

        // Atualiza o atacante (perde as tropas que moveu)
        atacante->tropas = atacante->tropas - tropasMovidas;

        printf("  > %s agora tem %d tropas.\n", defensor->nome, defensor->tropas);
        printf("  > %s ficou com %d tropas.\n", atacante->nome, atacante->tropas);

    } else {
        // Regra: Se perder (ou empatar), o atacante perde 1 tropa
        printf("ATAQUE FALHOU! O defensor (%s) resistiu.\n", defensor->nome);
        
        if (atacante->tropas > 1) {
            atacante->tropas--;
            printf("  > O atacante (%s) perdeu 1 tropa, restando %d.\n", atacante->nome, atacante->tropas);
        } else {
            printf("  > O atacante (%s) já está com a tropa mínima (1).\n", atacante->nome);
        }
    }
    printf("--- FIM DA BATALHA ---\n");
}

/**
 * @brief (V3) Sorteia uma missão e a copia para o destino.
 * @param destino Ponteiro de destino (alocado) para onde a missão será copiada. (Passagem por Referência)
 * @param missoes Vetor de strings com as missões disponíveis.
 * @param totalMissoes Tamanho do vetor de missões.
 */
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    // Sorteia um índice aleatório
    int indice = rand() % totalMissoes;
    // Copia a string da lista global para a memória alocada do jogador
    strcpy(destino, missoes[indice]);
}

/**
 * @brief (V3) Exibe a missão de um jogador.
 * @param missao Ponteiro para a string da missão a ser lida. (Passagem por Valor do ponteiro)
 * @param nomeJogador Nome do jogador para exibição.
 */
void exibirMissao(const char* missao, const char* nomeJogador) {
    printf("  > Missão [%s]: %s\n", nomeJogador, missao);
}

/**
 * @brief (V3) Verifica se a condição de vitória de uma missão foi atingida.
 * @param missao A string da missão do jogador.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param totalTerritorios Número de territórios no mapa.
 * @param corJogador A cor do jogador ("Azul" ou "Vermelho").
 * @return 1 (verdadeiro) se a missão foi cumprida, 0 (falso) caso contrário.
 */
int verificarMissao(const char* missao, struct Territorio* mapa, int totalTerritorios, const char* corJogador) {
    
    // --- Lógica de Verificação (Simples) ---
    // A função compara a string da missão do jogador com as missões conhecidas.

    // Missão 1: "Conquistar 4 territorios"
    if (strcmp(missao, "Conquistar 4 territorios") == 0) {
        int contador = 0;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp((mapa + i)->cor, corJogador) == 0) {
                contador++;
            }
        }
        if (contador >= 4) return 1; // Cumpriu!
    }
    
    // Missão 2: "Eliminar a cor Vermelha"
    else if (strcmp(missao, "Eliminar a cor Vermelha") == 0) {
        // Um jogador não pode ter a missão de se auto-eliminar
        if (strcmp(corJogador, "Vermelho") == 0) return 0;
        
        int contadorInimigo = 0;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp((mapa + i)->cor, "Vermelho") == 0) {
                contadorInimigo++; // Encontrou um território vermelho
            }
        }
        if (contadorInimigo == 0) return 1; // Cumpriu (nenhum território vermelho encontrado)
    }

    // Missão 3: "Eliminar a cor Azul"
    else if (strcmp(missao, "Eliminar a cor Azul") == 0) {
        if (strcmp(corJogador, "Azul") == 0) return 0; // Não pode se auto-eliminar

        int contadorInimigo = 0;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp((mapa + i)->cor, "Azul") == 0) {
                contadorInimigo++;
            }
        }
        if (contadorInimigo == 0) return 1; // Cumpriu!
    }

    // Missão 4: "Ter 15 tropas no total"
    else if (strcmp(missao, "Ter 15 tropas no total") == 0) {
        int totalTropas = 0;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp((mapa + i)->cor, corJogador) == 0) {
                totalTropas += (mapa + i)->tropas;
            }
        }
        if (totalTropas >= 15) return 1; // Cumpriu!
    }
    
    // Missão 5: "Conquistar o territorio 'Brasil'"
    else if (strcmp(missao, "Conquistar o territorio 'Brasil'") == 0) {
        for (int i = 0; i < totalTerritorios; i++) {
            // Procura o território "Brasil" E verifica se a cor é a do jogador
            if (strcmp((mapa + i)->nome, "Brasil") == 0 && 
                strcmp((mapa + i)->cor, corJogador) == 0) {
                return 1; // Cumpriu!
            }
        }
    }
    
    // Nenhuma missão foi cumprida
    return 0;
}

/**
 * @brief (V3) Libera toda a memória alocada dinamicamente.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param missaoP1 Ponteiro para a string da missão do P1.
 * @param missaoP2 Ponteiro para a string da missão do P2.
 */
void liberarMemoria(struct Territorio* mapa, char* missaoP1, char* missaoP2) {
    /*
     * Gerenciamento de Memória:
     * Libera todos os blocos de memória alocados com malloc.
     * A verificação '!= NULL' garante que o free não seja chamado
     * em um ponteiro nulo (o que é seguro, mas redundante) ou
     * em um ponteiro que falhou na alocação inicial.
     */
    printf("\nLiberando memória...\n");
    
    if (mapa != NULL) {
        free(mapa);
        mapa = NULL;
        printf("Memória do mapa liberada.\n");
    }
    if (missaoP1 != NULL) {
        free(missaoP1);
        missaoP1 = NULL;
        printf("Memória da missão P1 liberada.\n");
    }
    if (missaoP2 != NULL) {
        free(missaoP2);
        missaoP2 = NULL;
        printf("Memória da missão P2 liberada.\n");
    }
}