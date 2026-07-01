/*
 ============================================================
  SOKOBAN - T3
  Disciplina: Algoritmos e Programacao I
  Professor: Felski

  Autores: Arthur Ferraz
           Vicenzo Radin
           Gustavo de Lima
           Dimitri Cordeiro
 ============================================================

  SIMBOLOS DO MAPA:
    #  = parede
    @  = jogador
    $  = caixa
    .  = alvo (onde a caixa deve ir)
    *  = caixa em cima do alvo (correto!)
    +  = jogador em cima de um alvo
       = espaco vazio

  CONTROLES:
    W = cima   S = baixo   A = esquerda   D = direita
    R = reiniciar   Q = desistir
*/

#include <iostream>
#include <termios.h>   // para ler tecla sem Enter (Linux/GDB Online)
#include <unistd.h>    // para usar STDIN_FILENO

using namespace std;

// ============================================================
// Constantes do tamanho do mapa e quantidade de mapas
// ============================================================
const int LINHAS   = 8;
const int COLUNAS  = 12;
const int TOTAL_MAPAS = 5;

// ============================================================
// Limpa a tela (funciona no Linux/GDB Online)
// ============================================================
void limparTela() {
    cout << "\033[2J\033[H";
}

// ============================================================
// Le uma tecla sem precisar apertar Enter
// (substituto do _getch() do Windows)
// ============================================================
char lerTecla() {
    struct termios t_old, t_new;
    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO); // desliga buffer e eco
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);
    char c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &t_old); // restaura configuracao
    return c;
}

// ============================================================
// Copia um mapa para o destino
// ============================================================
void copiarMapa(char origem[LINHAS][COLUNAS], char destino[LINHAS][COLUNAS]) {
    for (int i = 0; i < LINHAS; i++)
        for (int j = 0; j < COLUNAS; j++)
            destino[i][j] = origem[i][j];
}

// ============================================================
// Desenha o mapa na tela
// ============================================================
void mostrarMapa(char mapaAtual[LINHAS][COLUNAS], int movimentos, int numero) {
    limparTela();

    // Nomes dos mapas para exibicao
    const char* nomes[TOTAL_MAPAS] = {
        "Mapa da Equipe",
        "Microban #1   ",
        "Microban 2 #1 ",
        "Microban 3 #1 ",
        "Dimitri&Yorick"
    };

    cout << "=== SOKOBAN ===  Mapa: " << nomes[numero]
         << "   Movimentos: " << movimentos << "\n";
    cout << "W/A/S/D = mover   R = reiniciar   Q = desistir\n\n";

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS - 1; j++)
            cout << mapaAtual[i][j];
        cout << "\n";
    }
    cout << "\n";
}

// ============================================================
// Verifica se o jogador venceu (nenhum $ fora do alvo)
// ============================================================
bool ganhou(char mapaAtual[LINHAS][COLUNAS]) {
    for (int i = 0; i < LINHAS; i++)
        for (int j = 0; j < COLUNAS; j++)
            if (mapaAtual[i][j] == '$')
                return false;
    return true;
}

// ============================================================
// Verifica se todos os mapas foram resolvidos
// ============================================================
bool todosResolvidos(bool resolvido[TOTAL_MAPAS]) {
    for (int i = 0; i < TOTAL_MAPAS; i++)
        if (!resolvido[i])
            return false;
    return true;
}

// ============================================================
// Move o jogador na direcao (dL = linha, dC = coluna)
//
// ============================================================
void mover(char mapaAtual[LINHAS][COLUNAS], int dL, int dC, int& movimentos) {
    // Acha onde o jogador esta
    int jL = -1, jC = -1;
    for (int i = 0; i < LINHAS && jL == -1; i++)
        for (int j = 0; j < COLUNAS && jL == -1; j++)
            if (mapaAtual[i][j] == '@' || mapaAtual[i][j] == '+')
                jL = i, jC = j;

    // Para onde o jogador quer ir
    int nL = jL + dL;
    int nC = jC + dC;

    // Nao pode ir para parede
    if (mapaAtual[nL][nC] == '#') return;

    // Se o jogador estava sobre um alvo ('+'), restaura o alvo; senao vira espaco
    char celulaJogador = (mapaAtual[jL][jC] == '+') ? '.' : ' ';

    // ---- Tenta empurrar caixa ----
    if (mapaAtual[nL][nC] == '$' || mapaAtual[nL][nC] == '*') {
        int cL = nL + dL;
        int cC = nC + dC;

        // Caixa nao pode ir para parede ou outra caixa
        if (mapaAtual[cL][cC] == '#' ||
            mapaAtual[cL][cC] == '$' ||
            mapaAtual[cL][cC] == '*')
            return;

        // Move a caixa: se cair em alvo vira '*', senao vira '$'
        mapaAtual[cL][cC] = (mapaAtual[cL][cC] == '.') ? '*' : '$';

        // se era '*' (caixa sobre alvo) o alvo deve ser marcado com '+'
        // se era '$' simplesmente o jogador vai para la com '@'
        mapaAtual[nL][nC] = (mapaAtual[nL][nC] == '*') ? '+' : '@';
    } else {
        // Sem caixa: jogador entra
        // Se for alvo ('.') usa '+', senao '@'
        mapaAtual[nL][nC] = (mapaAtual[nL][nC] == '.') ? '+' : '@';
    }

    // Restaura a celula de origem do jogador
    mapaAtual[jL][jC] = celulaJogador;

    movimentos++;
}

// ============================================================
// Loop principal de um mapa
// ============================================================
void jogar(char mapa[LINHAS][COLUNAS], int numero, bool resolvido[TOTAL_MAPAS]) {
    char mapaAtual[LINHAS][COLUNAS];
    copiarMapa(mapa, mapaAtual);
    int movimentos = 0;

    while (true) {
        mostrarMapa(mapaAtual, movimentos, numero);

        if (ganhou(mapaAtual)) {
            cout << "*** PARABENS! Resolvido em " << movimentos << " movimentos! ***\n";
            resolvido[numero] = true;
            cout << "Pressione qualquer tecla para voltar ao menu...";
            lerTecla();
            return;
        }

        char tecla = lerTecla();

        // WASD: passa movimentos por referencia para so incrementar em movimentos validos
        if (tecla == 'w' || tecla == 'W') mover(mapaAtual, -1,  0, movimentos);
        if (tecla == 's' || tecla == 'S') mover(mapaAtual,  1,  0, movimentos);
        if (tecla == 'a' || tecla == 'A') mover(mapaAtual,  0, -1, movimentos);
        if (tecla == 'd' || tecla == 'D') mover(mapaAtual,  0,  1, movimentos);
        if (tecla == 'r' || tecla == 'R') { copiarMapa(mapa, mapaAtual); movimentos = 0; }
        if (tecla == 'q' || tecla == 'Q') return;
    }
}

// ============================================================
// Inicializa os 5 mapas dentro do main (sem variaveis globais)
//
// mapa[0] = Mapa da Equipe  (criado pela equipe)
// mapa[1] = Microban #1     (sokoban.info - David W Skinner)
// mapa[2] = Microban 2 #1   (sokoban.info - David W Skinner)
// mapa[3] = Microban 3 #1   (sokoban.info - David W Skinner)
// mapa[4] = Dimitri&Yorick  (sokoban.info - Jaques Duthen, 4x3-01)
// ============================================================
void inicializarMapas(char mapas[TOTAL_MAPAS][LINHAS][COLUNAS]) {

    // --- Mapa 0: Mapa da Equipe (criado pela equipe) ---
    // Layout proprio: 2 caixas, 2 alvos
    const char m0[LINHAS][COLUNAS] = {
        "           ",
        "  #####    ",
        "  #   #    ",
        "  #$ $#    ",
        "  #. .#    ",
        "  #  @#    ",
        "  #####    ",
        "           "
    };

    // --- Mapa 1: Microban #1 - David W Skinner (sokoban.info) ---
    const char m1[LINHAS][COLUNAS] = {
        "  ####     ",
        "  # .#     ",
        "  #  ###   ",
        "## $@  #   ",
        "#      #   ",
        "#   ###    ",
        "####       ",
        "           "
    };

    // --- Mapa 2: Microban 2 #1 - David W Skinner (sokoban.info) ---
    const char m2[LINHAS][COLUNAS] = {
        " ######    ",
        " #    #    ",
        " #.$  #    ",
        " #$ @ #    ",
        "## ####    ",
        " #. #      ",
        " ####      ",
        "           "
    };

    // --- Mapa 3: Microban 3 #1 - David W Skinner (sokoban.info) ---
    const char m3[LINHAS][COLUNAS] = {
        "  ####     ",
        " #    #    ",
        "## $$  #   ",
        "#  $@  #   ",
        "# .    #   ",
        "##  .###   ",
        " #  .#     ",
        " ####      "
    };

    // --- Mapa 4: Dimitri & Yorick 4x3-01 - Jaques Duthen (sokoban.info) ---
    const char m4[LINHAS][COLUNAS] = {
        "  #####    ",
        " ##   ##   ",
        " # $$.#    ",
        " # @. #    ",
        " ##   ##   ",
        "  #####    ",
        "           ",
        "           "
    };

    // Copia cada mapa para o array tridimensional
    for (int j = 0; j < LINHAS; j++)
        for (int k = 0; k < COLUNAS; k++)
            mapas[0][j][k] = m0[j][k];

    for (int j = 0; j < LINHAS; j++)
        for (int k = 0; k < COLUNAS; k++)
            mapas[1][j][k] = m1[j][k];

    for (int j = 0; j < LINHAS; j++)
        for (int k = 0; k < COLUNAS; k++)
            mapas[2][j][k] = m2[j][k];

    for (int j = 0; j < LINHAS; j++)
        for (int k = 0; k < COLUNAS; k++)
            mapas[3][j][k] = m3[j][k];

    for (int j = 0; j < LINHAS; j++)
        for (int k = 0; k < COLUNAS; k++)
            mapas[4][j][k] = m4[j][k];
}

// ============================================================
// MENU PRINCIPAL
// ============================================================
int main() {
    // Array tridimensional: 5 mapas, cada um 8x12
    char mapas[TOTAL_MAPAS][LINHAS][COLUNAS];
    inicializarMapas(mapas);

    bool resolvido[TOTAL_MAPAS] = {false, false, false, false, false};

    const char* nomes[TOTAL_MAPAS] = {
        "Mapa da Equipe",
        "Microban #1   ",
        "Microban 2 #1 ",
        "Microban 3 #1 ",
        "Dimitri&Yorick"
    };

    int opcao = 0;

    while (opcao != 3) {
        limparTela();
        cout << "================================\n";
        cout << "         S O K O B A N\n";
        cout << "================================\n";
        cout << " Trabalho T3 - Algoritmos e\n";
        cout << " Programacao I - UNIVALI\n\n";
        cout << " Autores:\n";
        cout << "   Arthur Ferraz\n";
        cout << "   Vicenzo Radin\n";
        cout << "   Gustavo de Lima\n";
        cout << "   Dimitri Cordeiro\n";
        cout << "================================\n\n";
        cout << " 1 - Jogar\n";
        cout << " 2 - Como jogar\n";
        cout << " 3 - Sair\n\n";
        cout << " Escolha: ";
        cin >> opcao;
        cin.ignore(1000, '\n'); // limpa o Enter que sobra no buffer, evita que lerTecla() o capture

        // --- JOGAR ---
        if (opcao == 1) {
            limparTela();
            cout << "=== ESCOLHA O MAPA ===\n\n";

            for (int i = 0; i < TOTAL_MAPAS; i++) {
                cout << " " << (i + 1) << " - " << nomes[i]
                     << (resolvido[i] ? " <RESOLVIDO>" : "") << "\n";
            }
            cout << " 0 - Voltar\n\n";
            cout << " Escolha: ";

            int escolha;
            cin >> escolha;
            cin.ignore(1000, '\n'); // limpa o Enter que sobra no buffer

            // Valida a escolha e joga o mapa selecionado
            if (escolha >= 1 && escolha <= TOTAL_MAPAS)
                jogar(mapas[escolha - 1], escolha - 1, resolvido);

            // Verifica se todos os mapas foram resolvidos
            if (todosResolvidos(resolvido)) {
                limparTela();
                cout << "\n";
                cout << "  ************************************\n";
                cout << "  *                                  *\n";
                cout << "  *   VITORIA! TODOS OS MAPAS        *\n";
                cout << "  *   FORAM RESOLVIDOS! PARABENS!    *\n";
                cout << "  *                                  *\n";
                cout << "  ************************************\n\n";
                cout << "  Pressione qualquer tecla...";
                lerTecla();
            }
        }

        // --- COMO JOGAR ---
        if (opcao == 2) {
            limparTela();
            cout << "=== COMO JOGAR ===\n\n";
            cout << "Objetivo: empurre todas as caixas ate os alvos.\n\n";
            cout << "Simbolos:\n";
            cout << "  @  = voce (jogador)\n";
            cout << "  +  = voce sobre um alvo\n";
            cout << "  $  = caixa\n";
            cout << "  .  = alvo (onde a caixa deve ir)\n";
            cout << "  *  = caixa no alvo (correto!)\n";
            cout << "  #  = parede\n\n";
            cout << "Controles:\n";
            cout << "  W = mover para cima\n";
            cout << "  S = mover para baixo\n";
            cout << "  A = mover para esquerda\n";
            cout << "  D = mover para direita\n";
            cout << "  R = reiniciar o mapa\n";
            cout << "  Q = desistir e voltar ao menu\n\n";
            cout << "Pressione qualquer tecla para voltar...";
            lerTecla();
        }
    }

    limparTela();
    cout << "\nAte logo!\n";
    return 0;
}
