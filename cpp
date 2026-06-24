/*
 ============================================================
  SOKOBAN - T3
  Disciplina: Algoritmos e Programacao 
  Professor: Felski

  Autores: Arthur Ferraz & Vicenzo Radin
 ============================================================

  SIMBOLOS DO MAPA:
    #  = parede
    @  = jogador
    $  = caixa
    .  = alvo (onde a caixa deve ir)
    *  = caixa em cima do alvo (correto!)
       = espaco vazio
*/

#include <iostream>
#include <conio.h>   // _getch() - le tecla sem precisar de Enter
#include <windows.h> // system("cls") e Sleep()

using namespace std;

// ============================================================
// MAPAS DO JOGO
// Tamanho fixo: 8 linhas x 12 colunas
// Mapa 1 criado pela equipe.
// Mapas 2 a 5: cada integrante escolhe um em sokoban.info
// ============================================================

char mapa1[8][12] = {
    "###########",
    "#    .    #",
    "#  $   $  #",
    "#    @    #",
    "#  .   .  #",
    "#         #",
    "#         #",
    "###########"
};

char mapa2[8][12] = {
    "###########",
    "#         #",
    "#  $ . $  #",
    "#    @    #",
    "#  . # .  #",
    "#         #",
    "#         #",
    "###########"
};

char mapa3[8][12] = {
    "###########",
    "#   #     #",
    "# $ # $ . #",
    "#   @ .   #",
    "# $   #   #",
    "# .   #   #",
    "#         #",
    "###########"
};

char mapa4[8][12] = {
    "###########",
    "#         #",
    "#  $$$    #",
    "#   @...  #",
    "#   ###   #",
    "#         #",
    "#         #",
    "###########"
};

char mapa5[8][12] = {
    "###########",
    "#  #      #",
    "#  # $$ . #",
    "#  #  @   #",
    "#  ## # . #",
    "#    $    #",
    "#    .    #",
    "###########"
};

// ============================================================
// Copia um mapa para o mapaAtual
// ============================================================
void copiarMapa(char origem[8][12], char destino[8][12]) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 12; j++)
            destino[i][j] = origem[i][j];
}

// ============================================================
// Desenha o mapa na tela
// ============================================================
void mostrarMapa(char mapaAtual[8][12], int movimentos) {
    system("cls");
    cout << "=== SOKOBAN ===  Movimentos: " << movimentos << "\n";
    cout << "W/A/S/D = mover   R = reiniciar   Q = desistir\n\n";

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 11; j++)
            cout << mapaAtual[i][j];
        cout << "\n";
    }
}

// ============================================================
// Verifica se o jogador venceu o mapa atual
// (nao pode ter nenhum $ fora do alvo)
// ============================================================
bool ganhou(char mapaAtual[8][12]) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 12; j++)
            if (mapaAtual[i][j] == '$')
                return false;
    return true;
}

// ============================================================
// Verifica se TODOS os mapas foram resolvidos
// ============================================================
bool todosResolvidos(bool resolvido[5]) {
    for (int i = 0; i < 5; i++)
        if (!resolvido[i])
            return false;
    return true;
}

// ============================================================
// Move o jogador na direcao (dL = linha, dC = coluna)
// Exemplos: mover(-1,0) = cima, mover(0,1) = direita
// ============================================================
void mover(char mapaAtual[8][12], int dL, int dC) {
    // Acha onde o jogador esta
    int jL = 0, jC = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 12; j++)
            if (mapaAtual[i][j] == '@')
                jL = i, jC = j;

    // Para onde o jogador quer ir
    int nL = jL + dL;
    int nC = jC + dC;

    // Nao pode ir para parede
    if (mapaAtual[nL][nC] == '#') return;

    // Se tiver caixa na frente, tenta empurrar
    if (mapaAtual[nL][nC] == '$' || mapaAtual[nL][nC] == '*') {
        int cL = nL + dL; // para onde a caixa vai
        int cC = nC + dC;

        // Caixa nao pode ir para parede ou outra caixa
        if (mapaAtual[cL][cC] == '#' || mapaAtual[cL][cC] == '$' || mapaAtual[cL][cC] == '*')
            return;

        // Move a caixa: se o destino for alvo (.) vira *, senao vira $
        mapaAtual[cL][cC] = (mapaAtual[cL][cC] == '.') ? '*' : '$';

        // Onde a caixa estava: se era * (caixa em cima de alvo), volta a ser .
        mapaAtual[nL][nC] = (mapaAtual[nL][nC] == '*') ? '.' : ' ';
    }

    // Move o jogador
    mapaAtual[nL][nC] = '@';
    mapaAtual[jL][jC] = ' ';
}

// ============================================================
// Loop principal de um mapa
// Retorna true se o jogador venceu, false se desistiu
// ============================================================
bool jogar(char mapa[8][12], int numero, bool resolvido[5]) {
    char mapaAtual[8][12];
    copiarMapa(mapa, mapaAtual);
    int movimentos = 0;

    while (true) {
        mostrarMapa(mapaAtual, movimentos);

        if (ganhou(mapaAtual)) {
            cout << "\n*** PARABENS! Resolvido em " << movimentos << " movimentos! ***\n";
            resolvido[numero] = true;
            cout << "Pressione qualquer tecla...";
            _getch();
            return true;
        }

        char tecla = _getch();

        // Setas do teclado mandam 2 codigos: 224 e depois o codigo da seta
        if ((unsigned char)tecla == 224) {
            tecla = _getch();
            if (tecla == 72) mover(mapaAtual, -1, 0); // seta cima
            if (tecla == 80) mover(mapaAtual,  1, 0); // seta baixo
            if (tecla == 75) mover(mapaAtual,  0,-1); // seta esquerda
            if (tecla == 77) mover(mapaAtual,  0, 1); // seta direita
            movimentos++;
            continue;
        }

        if (tecla=='w'||tecla=='W') { mover(mapaAtual,-1, 0); movimentos++; }
        if (tecla=='s'||tecla=='S') { mover(mapaAtual, 1, 0); movimentos++; }
        if (tecla=='a'||tecla=='A') { mover(mapaAtual, 0,-1); movimentos++; }
        if (tecla=='d'||tecla=='D') { mover(mapaAtual, 0, 1); movimentos++; }
        if (tecla=='r'||tecla=='R') { copiarMapa(mapa, mapaAtual); movimentos = 0; }
        if (tecla=='q'||tecla=='Q') return false; // desistiu, volta ao menu
    }
}

// ============================================================
// MENU PRINCIPAL
// ============================================================
int main() {
    // Array que guarda quais mapas foram resolvidos
    bool resolvido[5] = {false, false, false, false, false};

    int opcao = 0;

    while (opcao != 3) {
        system("cls");
        cout << "================================\n";
        cout << "         S O K O B A N\n";
        cout << "================================\n";
        cout << " Trabalho T3 - Algoritmos e\n";
        cout << " Programacao I - UNIVALI/NID\n\n";
        cout << " Autores:\n";
        cout << "   [Nome 1]\n";
        cout << "   [Nome 2]\n";
        cout << "   [Nome 3]\n";
        cout << "   [Nome 4]\n";
        cout << "================================\n\n";
        cout << " 1 - Jogar\n";
        cout << " 2 - Como jogar\n";
        cout << " 3 - Sair\n\n";
        cout << " Escolha: ";
        cin >> opcao;

        // --- JOGAR ---
        if (opcao == 1) {
            system("cls");
            cout << "=== ESCOLHA O MAPA ===\n\n";
            cout << " 1 - Mapa da Equipe " << (resolvido[0] ? "<RESOLVIDO>" : "") << "\n";
            cout << " 2 - Mapa Classic 1 " << (resolvido[1] ? "<RESOLVIDO>" : "") << "\n";
            cout << " 3 - Mapa Classic 2 " << (resolvido[2] ? "<RESOLVIDO>" : "") << "\n";
            cout << " 4 - Mapa Desafio   " << (resolvido[3] ? "<RESOLVIDO>" : "") << "\n";
            cout << " 5 - Mapa Expert    " << (resolvido[4] ? "<RESOLVIDO>" : "") << "\n";
            cout << " 0 - Voltar\n\n";
            cout << " Escolha: ";

            int escolha;
            cin >> escolha;

            if (escolha == 1) jogar(mapa1, 0, resolvido);
            if (escolha == 2) jogar(mapa2, 1, resolvido);
            if (escolha == 3) jogar(mapa3, 2, resolvido);
            if (escolha == 4) jogar(mapa4, 3, resolvido);
            if (escolha == 5) jogar(mapa5, 4, resolvido);

            // Verifica se todos os mapas foram resolvidos
            if (todosResolvidos(resolvido)) {
                system("cls");
                cout << "\n";
                cout << "  ************************************\n";
                cout << "  *                                  *\n";
                cout << "  *   VITORIA! TODOS OS MAPAS        *\n";
                cout << "  *   FORAM RESOLVIDOS! PARABENS!    *\n";
                cout << "  *                                  *\n";
                cout << "  ************************************\n\n";
                cout << "  Pressione qualquer tecla para voltar ao menu...";
                _getch();
            }
        }

        // --- COMO JOGAR ---
        if (opcao == 2) {
            system("cls");
            cout << "=== COMO JOGAR ===\n\n";
            cout << "Objetivo: empurre todas as caixas ate os alvos.\n\n";
            cout << "Simbolos:\n";
            cout << "  @  = voce (jogador)\n";
            cout << "  $  = caixa\n";
            cout << "  .  = alvo (onde a caixa deve ir)\n";
            cout << "  *  = caixa no alvo (correto!)\n";
            cout << "  #  = parede\n\n";
            cout << "Controles:\n";
            cout << "  W / seta cima    = mover para cima\n";
            cout << "  S / seta baixo   = mover para baixo\n";
            cout << "  A / seta esq     = mover para esquerda\n";
            cout << "  D / seta dir     = mover para direita\n";
            cout << "  R                = reiniciar o mapa\n";
            cout << "  Q                = desistir e voltar ao menu\n\n";
            cout << "Pressione qualquer tecla para voltar...";
            _getch();
        }
    }

    system("cls");
    cout << "\nAte logo!\n";
    return 0;
}
