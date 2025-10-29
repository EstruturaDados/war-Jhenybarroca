/*
 * Missão: Construir uma base de dados de territórios.
 * Este programa cadastra 5 territórios usando uma struct e,
 * em seguida, exibe os dados cadastrados.
 */

// Bibliotecas necessárias
#include <stdio.h>  // Para funções de entrada/saída (printf, scanf, fgets, getchar)
#include <string.h> // Para a função strcspn (usada para limpar o '\n' do fgets)

// --- Criação da Struct ---
/*
 * Documentação:
 * Definição da struct 'Territorio'.
 * Esta é uma estrutura de dados composta que agrupa as três
 * informações necessárias para cada território.
 */
struct Territorio {
    char nome[30];  // Armazena o nome do território (limite de 29 caracteres + '\0')
    char cor[10];   // Armazena a cor do exército (limite de 9 caracteres + '\0')
    int tropas;     // Armazena a quantidade de tropas (número inteiro)
};

int main() {
    
    // --- Declaração do Vetor de Structs ---
    /*
     * Manutenibilidade:
     * Declaração de um vetor (array) chamado 'territorios' com capacidade
     * para armazenar 5 elementos do tipo 'struct Territorio'.
     * A variável 'i' será usada como índice nos laços 'for'.
     */
    struct Territorio territorios[5];
    int i;

    // --- Cadastro dos Territórios (Entrada de Dados) ---
    
    // Usabilidade: Título claro para a seção de cadastro.
    printf("--- CADASTRO DE 5 TERRITÓRIOS ---\n");
    printf("======================================\n");

    /*
     * Documentação:
     * Laço 'for' para preenchimento dos dados dos 5 territórios (índices 0 a 4).
     */
    for (i = 0; i < 5; i++) {
        printf("\n--- Dados do Território %d ---\n", i + 1);

        // Usabilidade: Mensagens claras orientando o usuário sobre o que digitar.
        
        // Leitura do NOME
        printf("Digite o nome: ");
        /*
         * Usamos fgets() para ler o nome, permitindo espaços (ex: "America do Sul").
         * O scanf("%s") pararia no primeiro espaço.
         */
        fgets(territorios[i].nome, 30, stdin);

        /*
         * Documentação: Limpeza do '\n' (Enter) do fgets.
         * O fgets() armazena o '\n' no final da string.
         * A função strcspn() encontra a posição do '\n' e o substitui
         * por '\0' (caractere de fim de string), limpando a entrada.
         */
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;

        // Leitura da COR
        printf("Digite a cor: ");
        fgets(territorios[i].cor, 10, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = 0; // Remove o '\n'

        // Leitura das TROPAS
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        /*
         * Documentação: Limpeza do buffer de entrada (stdin).
         * O scanf("%d", ...) lê o número, mas deixa o '\n' (Enter) no buffer.
         * Este laço 'while' consome esse '\n' (e qualquer outro caractere
         * extra) até encontrar o fim da linha.
         *
         * Isso é ESSENCIAL para que o próximo 'fgets()' (no início do
         * próximo loop, para ler o 'nome') não leia um '\n' vazio
         * e pule a entrada.
         */
        int c; // Variável temporária para consumir caracteres
        while ((c = getchar()) != '\n' && c != EOF);
    }

    // --- Exibição dos Dados ---
    
    /*
     * Desempenho: A exibição ocorre imediatamente após o término do cadastro,
     * atendendo ao requisito de resposta rápida.
     */
    printf("\n\n======================================\n");
    printf("--- TERRITÓRIOS REGISTRADOS ---\n");
    printf("======================================\n");

    /*
     * Documentação:
     * Laço 'for' para percorrer o vetor 'territorios' e exibir
     * os dados formatados de cada território cadastrado.
     */
    for (i = 0; i < 5; i++) {
        printf("\n[Território %d]\n", i + 1);
        printf("Nome..: %s\n", territorios[i].nome);
        printf("Cor...: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
    }

    return 0; // Indica que o programa foi executado com sucesso
}