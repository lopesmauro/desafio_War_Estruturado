#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Definição da Estrutura ---
typedef struct {
    char nome[30];
    char cor[10]; // Cor do jogador que domina o território
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
void inicializarJogo(Territorio** mapa, int* tamanhoMapa, char** missaoJogador, const char* missoes[], int totalMissoes);
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
void exibirMissao(const char* missao);
void exibirMapa(const Territorio* mapa, int tamanho);
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, const char* corJogador);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa, char* missaoJogador);

// --- Implementação da Função Principal ---
int main() {
    // Inicialização para números aleatórios
    srand(time(NULL));

    // Vetor de Missões Estratégicas
    const char* missoes[] = {
        "Conquistar 3 territorios seguidos.",       // Condição 1: Conquistar 3 territórios seguidos (exemplo de lógica)
        "Eliminar todas as tropas da cor Vermelha.", // Condição 2: Ter a maioria dos territórios
        "Dominar um total de 4 territorios.",       // Condição 3: Dominar 4 territórios no total
        "Ter no minimo 10 tropas no total.",        // Condição 4: Ter 10 ou mais tropas
        "Conquistar o territorio 'Gondor'."         // Condição 5: Conquistar um território específico
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // Variáveis para o jogo
    Territorio* mapa = NULL;
    int tamanhoMapa = 0;
    // Armazenamento dinâmico da missão do jogador (ponteiro para a missão)
    char* missaoJogador = NULL;
    const char* corJogador = "Azul"; // A cor do nosso jogador principal

    // 1. Inicialização do Jogo e Atribuição da Missão
    inicializarJogo(&mapa, &tamanhoMapa, &missaoJogador, missoes, totalMissoes);

    // 2. Exibição Condicional (apenas no início)
    exibirMissao(missaoJogador); // Passagem por valor para exibição

    printf("\n--- Inicio do Jogo ---\n");
    exibirMapa(mapa, tamanhoMapa);
    
    // --- Simulação do Jogo em Turnos ---
    int vencedorEncontrado = 0;
    int turno = 1;
    
    // Simulação de 3 turnos (ou até a vitória)
    while (turno <= 3 && !vencedorEncontrado) {
        printf("\n\n####################\n# TURNO %d\n####################\n", turno);

        // Exemplo: Jogador "Azul" ataca
        // Tenta atacar Gondor (índice 3) com um território seu (índice 0)
        // Se a cor do atacante e defensor for a mesma, o ataque não é permitido (Validação do ataque)
        if (strcmp(mapa[0].cor, mapa[3].cor) != 0) {
            printf("\n--> Jogador %s (atacante) vs %s (defensor) <--\n", mapa[0].cor, mapa[3].cor);
            printf("Ataque de %s (%d tropas) contra %s (%d tropas)\n", mapa[0].nome, mapa[0].tropas, mapa[3].nome, mapa[3].tropas);
            
            // Simulação de ataque
            atacar(&mapa[0], &mapa[3]); // Passagem de ponteiros (referência)
        } else {
            printf("\nNao pode atacar %s, pois ja e seu territorio.\n", mapa[3].nome);
        }

        // Exemplo: Outro movimento (se a missão for baseada em tropas totais, adicionamos mais)
        if (strstr(missaoJogador, "10 tropas")) {
             mapa[0].tropas += 3; // Simula ganho de tropas
             printf("\n[Simulacao] %s ganhou 3 tropas, totalizando %d em %s.\n", corJogador, mapa[0].tropas, mapa[0].nome);
        }
        
        exibirMapa(mapa, tamanhoMapa);

        // 3. Verificação da Missão (ao final de cada turno)
        // Passagem por referência para a verificação (embora não modifique, o requisito pedia "referência")
        if (verificarMissao(missaoJogador, mapa, tamanhoMapa, corJogador)) {
            printf("\n\n************************************\n");
            printf("* VITORIA! O Jogador %s CUMPRIU SUA MISSAO:\n", corJogador);
            printf("* \"%s\"\n", missaoJogador);
            printf("************************************\n");
            vencedorEncontrado = 1;
        } else {
            printf("\nMissao ainda nao cumprida. Proximo turno.\n");
        }

        turno++;
    }

    if (!vencedorEncontrado) {
        printf("\n\n--- Fim da simulacao apos %d turnos. Sem vencedor. ---\n", turno - 1);
    }

    // 4. Liberação de Memória
    liberarMemoria(mapa, missaoJogador);

    return 0;
}

// --- Implementação das Funções Modulares ---

/**
 * @brief Inicializa o mapa do jogo e atribui uma missão ao jogador.
 * * @param mapa Ponteiro para o ponteiro do mapa (para alocação dinâmica).
 * @param tamanhoMapa Ponteiro para o tamanho do mapa.
 * @param missaoJogador Ponteiro para o ponteiro da missão (para alocação dinâmica).
 * @param missoes Vetor de missões pré-definidas.
 * @param totalMissoes Quantidade total de missões.
 */
void inicializarJogo(Territorio** mapa, int* tamanhoMapa, char** missaoJogador, const char* missoes[], int totalMissoes) {
    *tamanhoMapa = 5; // Define o tamanho do mapa
    
    // Alocação de memória para o mapa
    // Uso de calloc para garantir que a memória seja inicializada com zeros
    *mapa = (Territorio*)calloc(*tamanhoMapa, sizeof(Territorio));
    if (*mapa == NULL) {
        perror("Erro ao alocar memoria para o mapa");
        exit(1);
    }
    
    // Configuração dos Territórios
    strcpy((*mapa)[0].nome, "Rivendell");
    strcpy((*mapa)[0].cor, "Azul"); // Território do Jogador Principal (Azul)
    (*mapa)[0].tropas = 5;

    strcpy((*mapa)[1].nome, "Mordor");
    strcpy((*mapa)[1].cor, "Vermelho");
    (*mapa)[1].tropas = 4;
    
    strcpy((*mapa)[2].nome, "Shire");
    strcpy((*mapa)[2].cor, "Azul");
    (*mapa)[2].tropas = 3;

    strcpy((*mapa)[3].nome, "Gondor");
    strcpy((*mapa)[3].cor, "Verde");
    (*mapa)[3].tropas = 4;

    strcpy((*mapa)[4].nome, "Rohan");
    strcpy((*mapa)[4].cor, "Vermelho");
    (*mapa)[4].tropas = 2;

    // Alocação de memória para a missão do jogador
    // Uma missão típica não deve exceder 256 caracteres
    *missaoJogador = (char*)malloc(256 * sizeof(char));
    if (*missaoJogador == NULL) {
        perror("Erro ao alocar memoria para a missao");
        // Devemos liberar o mapa antes de sair
        free(*mapa); 
        exit(1);
    }

    // Atribuição e cópia da missão
    atribuirMissao(*missaoJogador, missoes, totalMissoes);
}

/**
 * @brief Sorteia uma missão do vetor e a copia para a variável de destino.
 * * @param destino Ponteiro para a string onde a missão será copiada.
 * @param missoes Vetor de strings com as missões.
 * @param totalMissoes Quantidade total de missões.
 */
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    // Sorteia um índice aleatório
    int indiceSorteado = rand() % totalMissoes;
    
    // Copia a missão sorteada para a variável de destino usando strcpy
    strcpy(destino, missoes[indiceSorteado]);
}

/**
 * @brief Exibe a missão atual do jogador.
 * * @param missao String contendo a descrição da missão (passagem por valor conceitual - const char*).
 */
void exibirMissao(const char* missao) {
    printf("\n--- Sua Missao Secreta ---\n");
    printf("Objetivo: %s\n", missao);
    printf("--------------------------\n");
}

/**
 * @brief Exibe o estado atual de todos os territórios no mapa.
 * * @param mapa Ponteiro para o vetor de territórios.
 * @param tamanho Quantidade de territórios.
 */
void exibirMapa(const Territorio* mapa, int tamanho) {
    printf("\n--- Estado Atual do Mapa ---\n");
    printf("Nome\t\tDominio\t\tTropas\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%s\t\t%s\t\t%d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("----------------------------\n");
}

/**
 * @brief Avalia se a missão do jogador foi cumprida.
 * * @param missao String da missão a ser verificada (Passagem por referência conceitual).
 * @param mapa Ponteiro para o vetor de territórios.
 * @param tamanho Quantidade de territórios.
 * @param corJogador Cor do jogador principal.
 * @return int 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, const char* corJogador) {
    // Lógica de Verificação Simplificada: Apenas procura substrings na missão
    
    // Missão 1: Dominar um total de 4 territorios.
    if (strstr(missao, "4 territorios")) {
        int territoriosDominados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                territoriosDominados++;
            }
        }
        return (territoriosDominados >= 4);
    }
    
    // Missão 2: Ter no minimo 10 tropas no total.
    if (strstr(missao, "10 tropas")) {
        int totalTropas = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                totalTropas += mapa[i].tropas;
            }
        }
        return (totalTropas >= 10);
    }
    
    // Missão 3: Conquistar o territorio 'Gondor'.
    if (strstr(missao, "Gondor")) {
        // Encontra o território "Gondor"
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].nome, "Gondor") == 0) {
                return (strcmp(mapa[i].cor, corJogador) == 0); // Vence se for do jogador
            }
        }
    }
    
    // Missão 4 (Default/Simples - para as outras missões):
    // Se não for uma das específicas, usaremos a lógica de maioria de territórios (simples).
    int territoriosDominados = 0;
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            territoriosDominados++;
        }
    }
    // Vence se dominar a maioria dos territórios
    return (territoriosDominados > tamanho / 2);
}

/**
 * @brief Simula um ataque entre dois territórios.
 * * @param atacante Ponteiro para a estrutura do território atacante.
 * @param defensor Ponteiro para a estrutura do território defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Validação: Jogador só pode atacar territórios inimigos
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("ERRO: Ataque invalido. Territorios da mesma cor (%s).\n", atacante->cor);
        return;
    }
    
    // Simulação da rolagem de dados (valores entre 1 e 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("Dados: Atacante (%d) vs Defensor (%d)\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // Atacante vence
        printf("RESULTADO: Atacante vence!\n");
        
        // Atualização de campos: Transfere a cor e metade das tropas
        int tropasTransferidas = defensor->tropas / 2;
        
        // Conquista: defensor perde todas as suas tropas. Atacante ganha o território.
        strcpy(defensor->cor, atacante->cor);
        
        // Transfere tropas do atacante para o novo território (metade das tropas originais do atacante)
        int tropasParaMovimentar = atacante->tropas / 2;
        
        if (tropasParaMovimentar == 0 && atacante->tropas > 0) {
            // Garante que pelo menos 1 tropa se mova se o atacante tinha tropas
            tropasParaMovimentar = 1; 
        } else if (atacante->tropas == 0) {
            // Não deve acontecer, pois para atacar deve ter tropas
            return;
        }

        atacante->tropas -= tropasParaMovimentar;
        defensor->tropas = tropasParaMovimentar;
        
        printf("%s foi conquistado! Tropas de %s agora estao em %s (%d tropas).\n", 
               defensor->nome, atacante->nome, defensor->nome, defensor->tropas);
        
    } else {
        // Defensor vence ou Empate (Defensor vence no War)
        printf("RESULTADO: Defensor vence!\n");
        
        // Atacante perde uma tropa
        if (atacante->tropas > 0) {
            atacante->tropas--;
            printf("%s perdeu 1 tropa. Tropas restantes: %d.\n", atacante->nome, atacante->tropas);
        }
    }
}

/**
 * @brief Libera a memória alocada dinamicamente para o mapa e a missão do jogador.
 * * @param mapa Ponteiro para o vetor de territórios.
 * @param missaoJogador Ponteiro para a string da missão.
 */
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    // Libera a memória da missão do jogador
    if (missaoJogador != NULL) {
        free(missaoJogador);
        missaoJogador = NULL;
    }

    // Libera a memória do mapa
    if (mapa != NULL) {
        free(mapa);
        mapa = NULL;
    }

    printf("\nMemoria dinamica liberada com sucesso.\n");
}
