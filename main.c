#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <math.h>
//Kelly Ohana, Luiz Eduardo dos Santos Cunha, Yunguer Dvorak Mendes
typedef struct jogada
{
  int linhaDe, colunaDe, linhaPara, colunaPara;
  struct jogada *prox, *ant;
}stjogada;

typedef struct Peca
{
  int codigo;
  int linha, coluna;
  int ataques;
  struct Peca *prox,*ant;
}stpeca;

typedef struct Posicao
{
  int qtdBrancas;
  struct Peca *brancas;
  int qtdPretas;
  struct Peca *pretas;
  int jogVez;
  struct Peca *tab[8][8];
}stposicao;

void torreBranca(stjogada *lista, int lin, int col, stposicao atual);
void cavaloBranco(stjogada *lista, int lin, int col, stposicao atual);
void bispoBranco(stjogada *lista, int lin, int col, stposicao atual);
void damaBranca(stjogada *lista, int lin, int col, stposicao atual);
void reiBranco(stjogada *lista, int lin, int col, stposicao atual);
void peaoBranco(stjogada *lista, int lin, int col, stposicao atual);
void torrePreta(stjogada *lista, int lin, int col, stposicao atual);
void cavaloPreto(stjogada *lista, int lin, int col, stposicao atual);
void bispoPreto(stjogada *lista, int lin, int col, stposicao atual);
void damaPreta(stjogada *lista, int lin, int col, stposicao atual);
void reiPreto(stjogada *lista, int lin, int col, stposicao atual);
void peaoPreto(stjogada *lista, int lin, int col, stposicao atual);


stpeca *CriaLista()
{
  stpeca *novo = (stpeca*) malloc(sizeof(stpeca));
  novo->prox = novo->ant = novo;
  return novo;
};

stjogada *CriaListajogada()
{
  stjogada *novo = (stjogada*) malloc(sizeof(stjogada));
  novo->prox = novo->ant = novo;
  return novo;
};

stpeca* inserePecas(stpeca* elemento, int codigo, int linha, int coluna, int ataque)
{
    
  stpeca *novo = (stpeca*) malloc(sizeof(stpeca));
  novo->codigo = codigo;
  novo->linha = linha;
  novo->coluna = coluna;
  novo->ataques = ataque;

  novo->prox = elemento->prox;
  novo->ant = elemento;

  elemento->prox->ant = novo;
  elemento->prox =  novo;

  return novo;
}

void RemoveElemento(stpeca *lista, int x, int y){
  stpeca *aux = lista->prox;
  while (aux != lista && (aux->linha != x || aux->coluna != y)){
    aux = aux->prox;
  }
  if (aux!=lista){
    aux->prox->ant = aux->ant;
    aux->ant->prox = aux->prox;
    free(aux);
  }
}

stposicao IniciaTabuleiro()
{
  int i,j,k;
  stposicao tabuleiro;
  tabuleiro.qtdPretas = 16;
  tabuleiro.qtdBrancas = 16;
  tabuleiro.jogVez = 1;
  tabuleiro.brancas = CriaLista();
  tabuleiro.pretas = CriaLista();
  
  for(i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
    {
      tabuleiro.tab[i][j] = NULL;
    }
  }

  tabuleiro.tab[0][0] =  inserePecas(tabuleiro.brancas, 4, 0, 0, 0);
  tabuleiro.tab[0][1] =  inserePecas(tabuleiro.brancas, 2, 0, 1, 0);
  tabuleiro.tab[0][2] =  inserePecas(tabuleiro.brancas, 3, 0, 2, 0);
  tabuleiro.tab[0][3] =  inserePecas(tabuleiro.brancas, 5, 0, 1, 0);
  tabuleiro.tab[0][4] =  inserePecas(tabuleiro.brancas, 6, 0, 1, 0);
  tabuleiro.tab[0][5] =  inserePecas(tabuleiro.brancas, 3, 0, 5, 0);
  tabuleiro.tab[0][6] =  inserePecas(tabuleiro.brancas, 2, 0, 6, 0);
  tabuleiro.tab[0][7] =  inserePecas(tabuleiro.brancas, 4, 0, 7, 0);
  
  for(k=0 ; k<8 ; k++)
  {
    tabuleiro.tab[1][k] =  inserePecas(tabuleiro.brancas, 1, 1, k, 0);
    tabuleiro.tab[6][k] =  inserePecas(tabuleiro.pretas, -1, 6, k, 0);
  }
  
  tabuleiro.tab[7][0] =  inserePecas(tabuleiro.pretas, -4, 7, 0, 0);
  tabuleiro.tab[7][1] =  inserePecas(tabuleiro.pretas, -2, 7, 1, 0);
  tabuleiro.tab[7][2] =  inserePecas(tabuleiro.pretas, -3, 7, 2, 0);
  tabuleiro.tab[7][3] =  inserePecas(tabuleiro.pretas, -5, 7, 3, 0);
  tabuleiro.tab[7][4] =  inserePecas(tabuleiro.pretas, -6, 7, 4, 0);
  tabuleiro.tab[7][5] =  inserePecas(tabuleiro.pretas, -3, 7, 5, 0);
  tabuleiro.tab[7][6] =  inserePecas(tabuleiro.pretas, -2, 7, 6, 0);
  tabuleiro.tab[7][7] =  inserePecas(tabuleiro.pretas, -4, 7, 7, 0);

  return tabuleiro;
}

stjogada* insereJogada(stjogada* elemento, int linhaDe, int colunaDe, int linhaPara, int colunaPara)
{
  stjogada *novo = (stjogada *) malloc(sizeof(stjogada));

  novo->linhaDe = linhaDe;
  novo->colunaDe = colunaDe;
  novo->linhaPara = linhaPara;
  novo->colunaPara = colunaPara;

  novo->prox = elemento->prox;
  novo->ant = elemento;

  elemento->prox->ant = novo;
  elemento->prox =  novo;

  return novo;
}

stjogada *CalculaMovimentosPossiveis(stposicao atual)
{
  //lista de movimentos possiveis para tds as peças e zerar qtd de ataques
  stjogada *movimentos;
  movimentos = CriaListajogada();
  
  if( atual.jogVez == 1)
  {
    stpeca *aux = atual.brancas->prox;
    while(aux != atual.brancas)
    {
      aux->ataques = 0;

      if(aux->codigo == 1)
      {
        peaoBranco(movimentos, aux->linha, aux->coluna, atual);
      }
      if(aux->codigo == 2)
      {
        cavaloBranco(movimentos, aux->linha, aux->coluna, atual);
      }
      if(aux->codigo == 3)
      {
        bispoBranco(movimentos, aux->linha, aux->coluna, atual);
      }
      if(aux->codigo == 4)
      {
        torreBranca(movimentos, aux->linha, aux->coluna, atual);
      }
      if(aux->codigo == 5)
      {
        damaBranca(movimentos, aux->linha, aux->coluna, atual);
      }
      if(aux->codigo == 6)
      {
        reiBranco(movimentos, aux->linha, aux->coluna, atual);
      }
      aux = aux->prox;
    }
  }
  else
  {
     stpeca *aux2 = atual.pretas->prox;
  
    while(aux2 != atual.pretas)
    {
      aux2->ataques = 0;
      if(aux2->codigo == -1)
      {
        peaoPreto(movimentos, aux2->linha, aux2->coluna, atual);
      }
      
      if(aux2->codigo == -2)
      {
        cavaloPreto(movimentos, aux2->linha, aux2->coluna, atual);
      }

      if(aux2->codigo == -3)
      {
        bispoPreto(movimentos, aux2->linha, aux2->coluna, atual);
      }

      if(aux2->codigo == -4)
      {
        torrePreta(movimentos, aux2->linha, aux2->coluna, atual);
      }
      
      if(aux2->codigo == -5)
      {
        damaPreta(movimentos, aux2->linha, aux2->coluna, atual);
      }
      if(aux2->codigo == -6)
      {
        reiPreto(movimentos, aux2->linha, aux2->coluna, atual);
      }
      
      aux2 = aux2->prox;
    }
  }
   
  return movimentos;
}

void tabuleiro(stposicao atual)
{
  int i, j;
  printf("     0  1  2  3  4  5  6  7\n");
  printf("    ------------------------\n");
  for(i=0;i<8;i++)
  {
    printf("%d |", i);
    for(j=0;j<8;j++)
    {
      if(atual.tab[i][j] != NULL)
      {
        if(atual.tab[i][j]->codigo > 0)
        {
        
          printf("  %d", atual.tab[i][j]->codigo);
          
        }
        else
        {
          printf(" %d", atual.tab[i][j]->codigo);
        }
      }
      else
      printf("  0");
      
    }
    printf(" |\n");
  }
  printf("    ------------------------\n");
}

stjogada* buscajogada(stjogada *lista, int lin, int col, int linP, int colP)
{
  stjogada *aux = lista->prox;
  while(aux != lista && (col != aux->colunaDe || lin != aux->linhaDe || linP != aux->linhaPara || colP != aux->colunaPara))
    {
      aux = aux->prox;
    }
    if(aux != lista)
    {
      return aux;
    }
    else
    {
      printf("A jogada não existe! Entre com uma jogada valida\n");
      return NULL;
    }
}

stjogada *escolha(stjogada *lista)
{
  int lin, col, linP, colP;
  
  stjogada *teste;

  do
  {
    printf("Qual peça deseja mover?\n");
    printf("Linha:\n");
    scanf("%d", &lin);
    printf("Coluna:\n");
    scanf("%d", &col);
    printf("Linha para:\n");
    scanf("%d", &linP);
    printf("Coluna para:\n");
    scanf("%d", &colP);

    teste = buscajogada(lista, lin, col, linP, colP);

  }while(teste == NULL);

  return teste;
}

int ExecutaJogada(stjogada *jog, stposicao *tabuleiro)
{
  if(tabuleiro->tab[jog->linhaPara][jog->colunaPara] == NULL)
  {
    tabuleiro->tab[jog->linhaPara][jog->colunaPara] = tabuleiro->tab[jog->linhaDe][jog->colunaDe];
    tabuleiro->tab[jog->linhaPara][jog->colunaPara]->linha = jog->linhaPara;
    tabuleiro->tab[jog->linhaPara][jog->colunaPara]->coluna = jog->colunaPara;
    tabuleiro->tab[jog->linhaDe][jog->colunaDe] = NULL;
    tabuleiro->jogVez = tabuleiro->jogVez * -1;
    
    return 0;
  }
  else
  {
   if(tabuleiro->tab[jog->linhaPara][jog->colunaPara]->codigo == 6 || tabuleiro->tab[jog->linhaPara][jog->colunaPara]->codigo == -6)
    {
      if(tabuleiro->jogVez == 1)
      {
        RemoveElemento(tabuleiro->pretas, jog->linhaPara, jog->colunaPara);
        tabuleiro->qtdPretas--;
        printf("O Jogador 1 Ganhou\n");
      }
      else
      {
        RemoveElemento(tabuleiro->brancas, jog->linhaPara, jog->colunaPara);
        tabuleiro->qtdBrancas--;
        printf("O Jogador -1 Ganhou\n");
      }
      tabuleiro->tab[jog->linhaPara][jog->colunaPara] = tabuleiro->tab[jog->linhaDe][jog->colunaDe];
      tabuleiro->tab[jog->linhaPara][jog->colunaPara]->linha = jog->linhaPara;
      tabuleiro->tab[jog->linhaPara][jog->colunaPara]->coluna = jog->colunaPara;
      tabuleiro->tab[jog->linhaDe][jog->colunaDe] = NULL;
      tabuleiro->jogVez = tabuleiro->jogVez * -1;
      
      
      return 1;
    }
    else
    {
      if(tabuleiro->jogVez == 1)
      {
        RemoveElemento(tabuleiro->pretas, jog->linhaPara, jog->colunaPara);
        tabuleiro->qtdPretas--;
      }
      else
      {
        RemoveElemento(tabuleiro->brancas, jog->linhaPara, jog->colunaPara);
        tabuleiro->qtdBrancas--;
      }
      
      tabuleiro->tab[jog->linhaPara][jog->colunaPara] = tabuleiro->tab[jog->linhaDe][jog->colunaDe];
      tabuleiro->tab[jog->linhaPara][jog->colunaPara]->linha = jog->linhaPara;
      tabuleiro->tab[jog->linhaPara][jog->colunaPara]->coluna = jog->colunaPara;
      tabuleiro->tab[jog->linhaDe][jog->colunaDe] = NULL;
      tabuleiro->jogVez = tabuleiro->jogVez * -1;
      
      
      return 0;
    }

  }

}

stjogada *liberajogada(stjogada *lista)
{
  stjogada *aux = lista;

  lista->ant->prox = NULL;

  while(aux != NULL)
  {
    lista = lista->prox;
    free(aux);
    aux = lista;
  }
  return NULL;
}

stpeca *liberapeca(stpeca *lista)
{
  stpeca *aux = lista;

  lista->ant->prox = NULL;

  while(aux != NULL)
  {
    lista = lista->prox;
    free(aux);
    aux = lista;
  }
  return NULL;
}

void torreBranca(stjogada *lista, int lin, int col, stposicao atual)
{
  int i=1;
  while((lin+i)<8 && atual.tab[lin+i][col] == NULL)
  {
    insereJogada(lista, lin, col, lin+i, col);
    i++;
  }
  if((lin+i)<8 && atual.tab[lin+i][col]->codigo < 0)
  {
    insereJogada(lista, lin, col, lin+i, col);
    atual.tab[lin+i][col]->ataques++;
  }
  i=1;
  while((lin-i)>=0 && atual.tab[lin-i][col] == NULL)
  {
    insereJogada(lista, lin, col, lin-i, col);
    i++;
  }
  if((lin-i)>=0 && atual.tab[lin-i][col]->codigo < 0)
  {
    insereJogada(lista, lin, col, lin-i, col);
    atual.tab[lin-i][col]->ataques++;
  }
  i=1;
  while((col-i)>=0 && atual.tab[lin][col-i] == NULL)
  {
    insereJogada(lista, lin, col, lin, col-i);
    i++;
  }
  if((col-i)>=0 && atual.tab[lin][col-i]->codigo < 0)
  {
    insereJogada(lista, lin, col, lin, col-i);
    atual.tab[lin][col-i]->ataques++;
  }
  i=1;
  while((col+i)<8 && atual.tab[lin][col+i] == NULL)
  {
    insereJogada(lista, lin, col, lin, col+i);
    i++;
  }
  if((col+i)<8 && atual.tab[lin][col+i]->codigo < 0)
  {
    insereJogada(lista, lin, col, lin, col+i);
    atual.tab[lin][col+i]->ataques++;
  }
}

void cavaloBranco(stjogada *lista, int lin, int col, stposicao atual)
{
  if( (lin+2) < 8 && (col+1) < 8 && atual.tab[lin+2][col+1] != NULL)
  {
    if((lin+2) < 8 && (col+1) < 8 && atual.tab[lin+2][col+1]->codigo < 0) 
    {
      insereJogada(lista, lin, col, lin+2, col+1);
      atual.tab[lin+2][col+1]->ataques++;
    }
  }
  if((lin+2) < 8 && (col+1) < 8 && atual.tab[lin+2][col+1] == NULL)
  {
    insereJogada(lista, lin, col, lin+2, col+1);
  }
  if((lin+2) < 8 && (col-1) >= 0 && atual.tab[lin+2][col-1] != NULL)
  {
    if((lin+2) < 8 && (col-1) >= 0 && atual.tab[lin+2][col-1]->codigo < 0) 
    {
      insereJogada(lista, lin, col, lin+2, col-1);
      atual.tab[lin+2][col-1]->ataques++;
    } 
  }
  if((lin+2) < 8 && (col-1) >= 0 && atual.tab[lin+2][col-1] == NULL)
  {
    insereJogada(lista, lin, col, lin+2, col-1);
  }
  if((lin-2) >= 0 && (col+1) < 8 && atual.tab[lin-2][col+1] != NULL)
  {
    if((lin-2) >= 0 && (col+1) < 8 && atual.tab[lin-2][col+1]->codigo < 0) 
    {
      insereJogada(lista, lin, col, lin-2, col+1);
      atual.tab[lin-2][col+1]->ataques++;
    }
  }
  if((lin-2) >= 0 && (col+1) < 8 && atual.tab[lin-2][col+1] == NULL)
  {
    insereJogada(lista, lin, col, lin-2, col+1);
  }
  if((lin-2) >= 0 && (col+1) < 8 && atual.tab[lin-2][col-1] != NULL)
  {
    if((lin-2) >= 0 && (col-1) >= 0 && atual.tab[lin-2][col-1]->codigo < 0) 
    {
      insereJogada(lista, lin, col, lin-2, col-1);
      atual.tab[lin-2][col-1]->ataques++;
    }
  }
  if((lin-2) >= 0 && (col-1) >= 0 && atual.tab[lin-2][col-1] == NULL)
  {
    insereJogada(lista, lin, col, lin-2, col-1);
  }
  if((lin+1) < 8 && (col+2) < 8 && atual.tab[lin+1][col+2] != NULL)
  {
    if((lin+1) < 8 && (col+2) < 8 && atual.tab[lin+1][col+2]->codigo < 0 )
    {
      insereJogada(lista, lin, col, lin+1, col+2);
      atual.tab[lin+1][col+2]->ataques++;   
    }
  }
  if((lin+1) < 8 && (col+2) < 8 && atual.tab[lin+1][col+2] == NULL )
  {
    insereJogada(lista, lin, col, lin+1, col+2);
  }
  if((lin+1) < 8 && (col-2) >= 0 && atual.tab[lin+1][col-2] != NULL )
  {
    if((lin+1) < 8 && (col-2) >= 0 && atual.tab[lin+1][col-2]->codigo < 0 ) 
    {
      insereJogada(lista, lin, col, lin+1, col-2);
      atual.tab[lin+1][col-2]->ataques++;   
    }
  }
  if((lin+1) < 8 && (col-2) >= 0 && atual.tab[lin+1][col-2] == NULL )
  {
    insereJogada(lista, lin, col, lin+1, col-2);
  }
  if((lin-1) >= 0 && (col+2) < 8 && atual.tab[lin-1][col+2] != NULL )
  {
    if((lin-1) >= 0 && (col+2) < 8 && atual.tab[lin-1][col+2]->codigo < 0 ) 
    {
      insereJogada(lista, lin, col, lin-1, col+2);
      atual.tab[lin-1][col+2]->ataques++; 
    }
  }
  if((lin-1) >= 0 && (col+2) < 8 && atual.tab[lin-1][col+2] == NULL )
  {
    insereJogada(lista, lin, col, lin-1, col+2);
  }
  if((lin-1) >= 0 && (col-2) >= 0 && atual.tab[lin-1][col-2] != NULL )
  {
    if((lin-1) >= 0 && (col-2) >= 0 && atual.tab[lin-1][col-2]->codigo < 0) 
    {
      insereJogada(lista, lin, col, lin-1, col-2);
      atual.tab[lin-1][col-2]->ataques++; 
    }
  }
  if((lin-1) >= 0 && (col-2) >= 0 && atual.tab[lin-1][col-2] == NULL )
  {
    insereJogada(lista, lin, col, lin-1, col-2);
  }
}

void bispoBranco(stjogada *lista, int lin, int col, stposicao atual)
{
  int i=1;
    while((lin+i)<8 && (col+i)<8 && atual.tab[lin+i][col+i]==NULL)
    {
      insereJogada(lista, lin, col, lin+i, col+i);
      i++;
    }
    if((lin+i)<8 && (col+i)<8 && atual.tab[lin+i][col+i]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin+i, col+i);
      atual.tab[lin+i][col+i]->ataques++;
    }
    i=1;
    while((lin-i)>=0 && (col+i)<8 && atual.tab[lin-i][col+i] == NULL)
    {
      insereJogada(lista, lin, col, lin-i, col+i);
      i++;
    }
    if((lin-i)>=0 && (col+i)<8 && atual.tab[lin-i][col+i]->codigo <0)
    {
      insereJogada(lista, lin, col, lin-i, col+i);
      atual.tab[lin-i][col+i]->ataques++;
    }
    i=1;
    while((lin-i)>=0 && (col-i)>=0 && atual.tab[lin-i][col-i]==NULL)
    {
      insereJogada(lista, lin, col, lin-i, col-i);
      i++;
    }
    if((lin-i)>=0 && (col-i)>=0 && atual.tab[lin-i][col-i]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin-i, col-i);
      atual.tab[lin-i][col-i]->ataques++;
    }
    i=1;
    while((lin+i)<8 && (col-i)>=0 && atual.tab[lin+i][col-i]==NULL)
    {
      insereJogada(lista, lin, col, lin+i, col-i);
      i++;
    }
    if((lin+i)<8 && (col-i)>=0 && atual.tab[lin+i][col-i]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin+i, col-i);
      atual.tab[lin+i][col-i]->ataques++;
    }
}

void damaBranca(stjogada *lista, int lin, int col, stposicao atual)
{
  int i=1;
  while((lin+i)<8 && atual.tab[lin+i][col] == NULL)
  {
    insereJogada(lista, lin, col, lin+i, col);
    i++;
  }
  if((lin+i)<8 && atual.tab[lin+i][col]->codigo < 0)
  {
    insereJogada(lista, lin, col, lin+i, col);
    
    atual.tab[lin+i][col]->ataques++;
  }
  i=1;
  while((lin-i)>=0 && atual.tab[lin-i][col] == NULL)
  {
    insereJogada(lista, lin, col, lin-i, col);
    i++;
  }
  if((lin-i)>=0 && atual.tab[lin-i][col]->codigo < 0)
  {
    insereJogada(lista, lin, col, lin-i, col);
    atual.tab[lin-i][col]->ataques++;
  }
  i=1;
  while((col-i)>=0 && atual.tab[lin][col-i] == NULL)
  {
    insereJogada(lista, lin, col, lin, col-i);
    i++;
  }
  if((col-i)>=0 && atual.tab[lin][col-i]->codigo < 0)
  {
    insereJogada(lista, lin, col, lin, col-i);
    atual.tab[lin][col-i]->ataques++;
  }
  i=1;
  while((col+i)<8 && atual.tab[lin][col+i] == NULL)
  {
    insereJogada(lista, lin, col, lin, col+i);
    i++;
  }
  if((col+i)<8 && atual.tab[lin][col+i]->codigo < 0)
  {
    insereJogada(lista, lin, col, lin, col+i);
    atual.tab[lin][col+i]->ataques++;
  }
  while((lin+i)<8 && (col+i)<8 && atual.tab[lin+i][col+i]==NULL)
  {
    insereJogada(lista, lin, col, lin+i, col+i);
    i++;
  }
  if((lin+i)<8 && (col+i)<8 && atual.tab[lin+i][col+i]->codigo <0)
  {
    insereJogada(lista, lin, col, lin+i, col+i);
    atual.tab[lin+i][col+i]->ataques++;
  }
  i=1;
  while((lin-i)>=0 && (col+i)<8 && atual.tab[lin-i][col+i]==NULL)
  {
    insereJogada(lista, lin, col, lin-i, col+i);
    i++;
  }
  if((lin-i)>=0 && (col+i)<8 && atual.tab[lin-i][col+i]->codigo <0)
  {
    insereJogada(lista, lin, col, lin-i, col+i);
    atual.tab[lin-i][col+i]->ataques++;
  }
  i=1;
  while((lin-i)>=0 && (col-i)>=0 && atual.tab[lin-i][col-i]==NULL)
  {
    insereJogada(lista, lin, col, lin-i, col-i);
    i++;
  }
  if((lin-i)>=0 && (col-i)>=0 && atual.tab[lin-i][col-i]->codigo <0)
  {
    insereJogada(lista, lin, col, lin-i, col-i);
    atual.tab[lin-i][col-i]->ataques++;
  }
  i=1;
  while((lin+i)<8 && (col-i)>=0 && atual.tab[lin+i][col-i]==NULL)
  {
    insereJogada(lista, lin, col, lin+i, col-i);
    i++;
  }
  if((lin+i)<8 && (col-i)>=0 && atual.tab[lin+i][col-i]->codigo <0)
  {
    insereJogada(lista, lin, col, lin+i, col-i);
    atual.tab[lin+i][col-i]->ataques++;
  }
  
}

void reiBranco(stjogada *lista, int lin, int col, stposicao atual)
{
  if((lin+1) < 8 && atual.tab[lin+1][col] == NULL)
  {
    insereJogada(lista, lin, col, lin+1, col);
  }
  
  if((lin+1) < 8 && atual.tab[lin+1][col] != NULL)
  {
    if((lin+1) < 8 && atual.tab[lin+1][col]->codigo < 0)
    {
    insereJogada(lista, lin, col, lin+1, col);
    atual.tab[lin+1][col]->ataques++; 
    }
  }
  if((lin-1) >= 0 && atual.tab[lin-1][col] == NULL)
  {
    insereJogada(lista, lin, col, lin-1, col);
  }
  
  if((lin-1) >= 0 && atual.tab[lin-1][col] != NULL)
  {
    if((lin-1) >= 0 && atual.tab[lin-1][col]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin-1, col);
      atual.tab[lin-1][col]->ataques++; 
      }
  }
  if((col+1) < 8 && atual.tab[lin][col+1] == NULL)
  {
    insereJogada(lista, lin, col, lin, col+1);
  }
  
  if((col+1) < 8 && atual.tab[lin][col+1] != NULL)
  {
    if((col+1) < 8 && atual.tab[lin][col+1]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin, col+1);
      atual.tab[lin][col+1]->ataques++; 
    }
  }
  if((col-1) >= 0 && atual.tab[lin][col-1] == NULL)
  {
    insereJogada(lista, lin, col, lin, col-1);
  }
  
  if((col-1) >= 0 && atual.tab[lin][col-1] != NULL)
  {
    if((col-1) >= 0 && atual.tab[lin][col-1]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin, col-1);
      atual.tab[lin][col-1]->ataques++; 
    }
  }
  if((lin+1) < 8 && (col+1) < 8 && atual.tab[lin+1][col+1] == NULL)
  {
    insereJogada(lista, lin, col, lin+1, col+1);
  }
  
  if((lin+1) < 8 && (col+1) < 8 && atual.tab[lin+1][col+1] != NULL)
  {
    if((lin+1) < 8 && (col+1) < 8 && atual.tab[lin+1][col+1]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin+1, col+1);
      atual.tab[lin+1][col+1]->ataques++; 
    }
  }
  if((lin+1) < 8 && (col-1) >= 0 && atual.tab[lin+1][col-1] == NULL)
  {
    insereJogada(lista, lin, col, lin+1, col-1);
  }
  if((lin+1) < 8 && (col-1) >= 0 && atual.tab[lin+1][col-1] != NULL)
  {
    if((lin+1) < 8 && (col-1) >= 0 && atual.tab[lin+1][col-1]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin+1, col-1);
      atual.tab[lin+1][col-1]->ataques++; 
    }
  }
  if((lin-1) >= 0 && (col+1) < 8 && atual.tab[lin-1][col+1] == NULL)
  {
    insereJogada(lista, lin, col, lin-1, col+1);
  }
  if((lin-1) >= 0 && (col+1) < 8 && atual.tab[lin-1][col+1] != NULL)
  {
    if((lin-1) >= 0 && (col+1) < 8 && atual.tab[lin-1][col+1]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin-1, col+1);
      atual.tab[lin-1][col+1]->ataques++; 
    }
  }
  if((lin-1) >= 0 && (col-1) >= 0 && atual.tab[lin-1][col-1] == NULL)
  {
    insereJogada(lista, lin, col, lin-1, col-1);
  }
  if((lin-1) >= 0 && (col-1) >= 0 && atual.tab[lin-1][col-1] != NULL)
  {
    if((lin-1) >= 0 && (col-1) >= 0 && atual.tab[lin-1][col-1]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin-1, col-1);
      atual.tab[lin-1][col-1]->ataques++; 
    }
  }
}

void peaoBranco(stjogada *lista, int lin, int col, stposicao atual)
{
  if(lin == 1)
  {
    if(atual.tab[lin+2][col] == NULL)
    {
      insereJogada(lista, lin, col, lin+2, col);
    }
  }
  if((lin+1) < 8 && atual.tab[lin+1][col] == NULL)
  {
    insereJogada(lista, lin, col, lin+1, col);
  }
  else
  if((lin+1) < 8 && atual.tab[lin+1][col-1] != NULL)
  {
    if((lin+1) < 8 && atual.tab[lin+1][col-1]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin+1, col-1);
      atual.tab[lin+1][col-1]->ataques++;
    }
  }
  if((lin+1) < 8 && atual.tab[lin+1][col+1] != NULL)
  {
    if((lin+1) < 8 && atual.tab[lin+1][col+1]->codigo < 0)
    {
      insereJogada(lista, lin, col, lin+1, col+1);
      atual.tab[lin+1][col+1]->ataques++;
    }
  }
  if(lin == 7)
  {
    atual.tab[lin][col]->codigo = 5;
  }
}

void torrePreta(stjogada *lista, int lin, int col, stposicao atual)
{
  int i=1;
  while((lin+i)<8 && atual.tab[lin+i][col]==NULL)
  {
    insereJogada(lista, lin, col, lin+i, col);
    i++;
  }
  if((lin+i)<8 && atual.tab[lin+i][col]->codigo >0)
  {
    insereJogada(lista, lin, col, lin+i, col);
    atual.tab[lin+i][col]->ataques++;
  }
  i=1;
  while((lin-i)>=0 && atual.tab[lin-i][col]==NULL)
  {
    insereJogada(lista, lin, col, lin-i, col);
    i++;
  }
  if((lin-i)>=0 && atual.tab[lin-i][col]->codigo >0)
  {
    insereJogada(lista, lin, col, lin-i, col);
    atual.tab[lin-i][col]->ataques++;
  }
  i=1;
  while((col-i)>=0 && atual.tab[lin][col-i]==NULL)
  {
    insereJogada(lista, lin, col, lin, col-i);
    i++;
  }
  if((col-i)>=0 && atual.tab[lin][col-i]->codigo >0)
  {
    insereJogada(lista, lin, col, lin, col-i);
    atual.tab[lin][col-i]->ataques++;
  }
  i=1;
  while((col+i)<8 && atual.tab[lin][col+i]==NULL)
  {
    insereJogada(lista, lin, col, lin, col+i);
    i++;
  }
  if((col+i)<8 && atual.tab[lin][col+i]->codigo >0)
  {
    insereJogada(lista, lin, col, lin, col+i);
    atual.tab[lin][col+i]->ataques++;
  }
  
}

void cavaloPreto(stjogada *lista, int lin, int col, stposicao atual)
{
  if((lin+2) < 8 && (col+1) < 8 && atual.tab[lin+2][col+1] != NULL)
  {
    if((lin+2) < 8 && (col+1) < 8 && atual.tab[lin+2][col+1]->codigo > 0) 
    {
      insereJogada(lista, lin, col, lin+2, col+1);
      atual.tab[lin+2][col+1]->ataques++;
    }
  }
  else
  if((lin+2) < 8 && (col+1) < 8 && atual.tab[lin+2][col+1] == NULL)
  {
    insereJogada(lista, lin, col, lin+2, col+1);
  }
  if((lin+2) < 8 && (col-1) >= 0 && atual.tab[lin+2][col-1] != NULL)
  {
    if((lin+2) < 8 && (col-1) >= 0 && atual.tab[lin+2][col-1]->codigo > 0) 
    {
      insereJogada(lista, lin, col, lin+2, col-1);
      atual.tab[lin+2][col-1]->ataques++;
    }
  }
  else
  if((lin+2) < 8 && (col-1) >= 0 && atual.tab[lin+2][col-1] == NULL)
  {
    insereJogada(lista, lin, col, lin+2, col-1);
  }
  if((lin-2) >= 0 && (col+1) < 8 && atual.tab[lin-2][col+1] != NULL)
  {
    if((lin-2) >= 0 && (col+1) < 8 && atual.tab[lin-2][col+1]->codigo > 0) 
    {
      insereJogada(lista, lin, col, lin-2, col+1);
      atual.tab[lin-2][col+1]->ataques++;
    }
  }
  else
  if((lin-2) >= 0 && (col+1) < 8 && atual.tab[lin-2][col+1] == NULL)
  {
    insereJogada(lista, lin, col, lin-2, col+1);
  }
  if((lin-2) >= 0 && (col-1) >= 0 && atual.tab[lin-2][col-1] != NULL)
  {
    if((lin-2) >= 0 && (col-1) >= 0 && atual.tab[lin-2][col-1]->codigo > 0) 
    {
      insereJogada(lista, lin, col, lin-2, col-1);
      atual.tab[lin-2][col-1]->ataques++;
    }
  }
  else  
  if((lin-2) >= 0 && (col-1) >= 0 && atual.tab[lin-2][col-1] == NULL)
  {
    insereJogada(lista, lin, col, lin-2, col-1);
  }
  if((lin+1) < 8 && (col+2) < 8 && atual.tab[lin+1][col+2] != NULL)
  {
    if((lin+1) < 8 && (col+2) < 8 && atual.tab[lin+1][col+2]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin+1, col+2);
      atual.tab[lin+1][col+2]->ataques++;
    }
  }
  else
  if((lin+1) < 8 && (col+2) < 8 && atual.tab[lin+1][col+2] == NULL)
  {
    insereJogada(lista, lin, col, lin+1, col+2);
  }
  if((lin+1) < 8 && (col-2) >= 0 && atual.tab[lin+1][col-2] != NULL)
  {
    if((lin+1) < 8 && (col-2) >= 0 && atual.tab[lin+1][col-2]->codigo > 0) 
    {
      insereJogada(lista, lin, col, lin+1, col-2);
      atual.tab[lin+1][col-2]->ataques++;
    }
  }
  else  
  if((lin+1) < 8 && (col-2) >= 0 && atual.tab[lin+1][col-2] == NULL)
  {
    insereJogada(lista, lin, col, lin+1, col-2);
  }
  if((lin-1) >= 0 && (col+2) < 8 && atual.tab[lin-1][col+2] != NULL)
  {
    if((lin-1) >= 0 && (col+2) < 8 && atual.tab[lin-1][col+2]->codigo > 0) 
    {
      insereJogada(lista, lin, col, lin-1, col+2);
      atual.tab[lin-1][col+2]->ataques++;
    }
  }
  else
  if((lin-1) >= 0 && (col+2) < 8 && atual.tab[lin-1][col+2] == NULL)
  {
    insereJogada(lista, lin, col, lin-1, col+2);
  }
  if((lin-1) >= 0 && (col-2) >= 0 && atual.tab[lin-1][col-2] != NULL)
  {
    if((lin-1) >= 0 && (col-2) >= 0 && atual.tab[lin-1][col-2]->codigo > 0) 
    {
      insereJogada(lista, lin, col, lin-1, col-2);
      atual.tab[lin-1][col-2]->ataques++;
    }
  }
  else
  if((lin-1) >= 0 && (col-2) >= 0 && atual.tab[lin-1][col-2] == NULL)
  {
    insereJogada(lista, lin, col, lin-1, col-2);
  }
}

void bispoPreto(stjogada *lista, int lin, int col, stposicao atual)
{
  int i=1;
    while((lin+i)<8 && (col+i)<8 && atual.tab[lin+i][col+i]==NULL)
    {
      insereJogada(lista, lin, col, lin+i, col+i);
      i++;
    }
    if((lin+i)<8 && (col+i)<8 && atual.tab[lin+i][col+i]->codigo >0)
    {
      insereJogada(lista, lin, col, lin+i, col+i);
      atual.tab[lin+i][col+i]->ataques++;
    }
    i=1;
    while((lin-i)>=0 && (col+i)<8 && atual.tab[lin-i][col+i]==NULL)
    {
      insereJogada(lista, lin, col, lin-i, col+i);
      i++;
    }
    if((lin-i)>=0 && (col+i)<8 && atual.tab[lin-i][col+i]->codigo >0)
    {
      insereJogada(lista, lin, col, lin-i, col+i);
      atual.tab[lin-i][col+i]->ataques++;
    }
    i=1;
    while((lin-i)>=0 && (col-i)>=0 && atual.tab[lin-i][col-i]==NULL)
    {
      insereJogada(lista, lin, col, lin-i, col-i);
      i++;
    }
    if((lin-i)>=0 && (col-i)>=0 && atual.tab[lin-i][col-i]->codigo >0)
    {
      insereJogada(lista, lin, col, lin-i, col-i);
      atual.tab[lin-i][col-i]->ataques++;
    }
    i=1;
    while((lin+i)<8 && (col-i)>=0 && atual.tab[lin+i][col-i]==NULL)
    {
      insereJogada(lista, lin, col, lin+i, col-i);
      i++;
    }
    if((lin+i)<8 && (col-i)>=0 && atual.tab[lin+i][col-i]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin+i, col-i);
      atual.tab[lin+i][col-i]->ataques++;
    }
}

void damaPreta(stjogada *lista, int lin, int col, stposicao atual)
{
  int i=1;
  while((lin+i)<8 && atual.tab[lin+i][col]==NULL)
  {
    insereJogada(lista, lin, col, lin+i, col);
    i++;
  }
  if((lin+i)<8 && atual.tab[lin+i][col]->codigo >0)
  {
    insereJogada(lista, lin, col, lin+i, col);
    atual.tab[lin+i][col]->ataques++;
  }
  i=1;
  while((lin-i)>=0 && atual.tab[lin-i][col]==NULL)
  {
    insereJogada(lista, lin, col, lin-i, col);
    i++;
  }
  if((lin-i)>=0 && atual.tab[lin-i][col]->codigo >0)
  {
    insereJogada(lista, lin, col, lin-i, col);
    atual.tab[lin-i][col]->ataques++;
  }
  i=1;
  while((col-i)>=0 && atual.tab[lin][col-i]==NULL)
  {
    insereJogada(lista, lin, col, lin, col-i);
    i++;
  }
  if((col-i)>=0 && atual.tab[lin][col-i]->codigo >0)
  {
    insereJogada(lista, lin, col, lin, col-i);
    atual.tab[lin][col-i]->ataques++;
  }
  i=1;
  while((col+i)<8 && atual.tab[lin][col+i]==NULL)
  {
    insereJogada(lista, lin, col, lin, col+i);
    i++;
  }
  if((col+i)<8 && atual.tab[lin][col+i]->codigo >0)
  {
    insereJogada(lista, lin, col, lin, col+i);
    atual.tab[lin][col+i]->ataques++;
  }
  i=1;
  while((lin+i)<8 && (col+i)<8 && atual.tab[lin+i][col+i]==NULL)
  {
    insereJogada(lista, lin, col, lin+i, col+i);
    i++;
  }
  if((lin+i)<8 && (col+i)<8 && atual.tab[lin+i][col+i]->codigo >0)
  {
    insereJogada(lista, lin, col, lin+i, col+i);
    atual.tab[lin+i][col+i]->ataques++;
  }
  i=1;
  while((lin-i)>=0 && (col+i)<8 && atual.tab[lin-i][col+i]==NULL)
  {
    insereJogada(lista, lin, col, lin-i, col+i);
    i++;
  }
  if((lin-i)>=0 && (col+i)<8 && atual.tab[lin-i][col+i]->codigo >0)
  {
    insereJogada(lista, lin, col, lin-i, col+i);
    atual.tab[lin-i][col+i]->ataques++;
  }
  i=1;
  while((lin-i)>=0 && (col-i)>=0 && atual.tab[lin-i][col-i]==NULL)
  {
    insereJogada(lista, lin, col, lin-i, col-i);
    i++;
  }
  if((lin-i)>=0 && (col-i)>=0 && atual.tab[lin-i][col-i]->codigo >0)
  {
    insereJogada(lista, lin, col, lin-i, col-i);
    atual.tab[lin-i][col-i]->ataques++;
  }
  i=1;
  while((lin+i)<8 && (col-i)>=0 && atual.tab[lin+i][col-i]==NULL)
  {
    insereJogada(lista, lin, col, lin+i, col-i);
    i++;
  }
  if((lin+i)<8 && (col-i)>=0 && atual.tab[lin+i][col-i]->codigo >0)
  {
    insereJogada(lista, lin, col, lin+i, col-i);
    atual.tab[lin+i][col-i]->ataques++;
  }
}

void reiPreto(stjogada *lista, int lin, int col, stposicao atual)
{
  if((lin+1) < 8 && atual.tab[lin+1][col] == NULL) 
  {
    insereJogada(lista, lin, col, lin+1, col);
  }
  if((lin+1) < 8 && atual.tab[lin+1][col] != NULL)
  {
    if(atual.tab[lin+1][col]->codigo > 0 && (lin+1) < 8)
    {
      insereJogada(lista, lin, col, lin+1, col);
      atual.tab[lin+1][col]->ataques++;
    }
  }
  else  
  if((lin-1) >= 0 && atual.tab[lin-1][col] == NULL)
  {
    insereJogada(lista, lin, col, lin-1, col);
  }
  if((lin-1) >= 0 && atual.tab[lin-1][col] != NULL)
  {
    if((lin-1) >= 0 && atual.tab[lin-1][col]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin-1, col);
      atual.tab[lin-1][col]->ataques++;
    }
  }
  else  
  if((col+1) < 8 && atual.tab[lin][col+1] == NULL)
  {
    insereJogada(lista, lin, col, lin, col+1);
  }
  if((col+1) < 8 && atual.tab[lin][col+1] != NULL)
  {
    if((col+1) < 8 && atual.tab[lin][col+1]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin, col+1);
      atual.tab[lin][col+1]->ataques++;
    }
  }
  else
  if((col-1) >= 0 && atual.tab[lin][col-1] == NULL)
  {
    insereJogada(lista, lin, col, lin, col-1);
  }
  if((col-1) >= 0 && atual.tab[lin][col-1] != NULL)
  {
    if((col-1) >= 0 && atual.tab[lin][col-1]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin, col-1);
      atual.tab[lin][col-1]->ataques++;
    }
  }
  else  
  if((col+1) < 8 && atual.tab[lin+1][col+1] == NULL && (lin+1) < 8)
  {
    insereJogada(lista, lin, col, lin+1, col+1);
  }
  if((lin+1) < 8 && (col+1) < 8 && atual.tab[lin+1][col+1] != NULL)
  {
    if((lin+1) < 8 && (col+1) < 8 && atual.tab[lin+1][col+1]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin+1, col+1);
      atual.tab[lin+1][col+1]->ataques++;
    }
  }
  else
  if((lin+1) < 8 && (col-1) >= 0 && atual.tab[lin+1][col-1] == NULL)
  {
    insereJogada(lista, lin, col, lin+1, col-1);
  }
  if((lin+1) < 8 && (col-1) >= 0 && atual.tab[lin+1][col-1] != NULL)
  {
    if((lin+1) < 8 && (col-1) >= 0 && atual.tab[lin+1][col-1]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin+1, col-1);
      atual.tab[lin+1][col-1]->ataques++;
    }
  }
  else  
  if((lin-1) >= 0 && (col+1) < 8 && atual.tab[lin-1][col+1] == NULL)
  {
    insereJogada(lista, lin, col, lin-1, col+1);
  }
  if((lin-1) >= 0 && (col+1) < 8 && atual.tab[lin-1][col+1] != NULL)
  {
    if((lin-1) >= 0 && (col+1) < 8 && atual.tab[lin-1][col+1]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin-1, col+1);
      atual.tab[lin-1][col+1]->ataques++;
    }
  }
  else  
  if((lin-1) >= 0 && (col-1) >= 0 && atual.tab[lin-1][col-1] == NULL)
  {
    insereJogada(lista, lin, col, lin-1, col-1);
  }
  if((lin-1) >= 0 && (col-1) >= 0 && atual.tab[lin-1][col-1] != NULL)
  {
    if((lin-1) >= 0 && (col-1) >= 0 && atual.tab[lin-1][col-1]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin-1, col-1);
      atual.tab[lin-1][col-1]->ataques++;
    }
  }
}

void peaoPreto(stjogada *lista, int lin, int col, stposicao atual)
{
  if(lin == 6)
  {
    if(atual.tab[lin-2][col] == NULL)
    {
      insereJogada(lista, lin, col, lin-2, col);
    }
  }
  if((lin-1) >= 0 && atual.tab[lin-1][col] == NULL)
  {
    insereJogada(lista, lin, col, lin-1, col);
  }
  else
  if((lin-1) >= 0 && atual.tab[lin-1][col-1] != NULL)
  {
    if((lin-1) >= 0 && atual.tab[lin-1][col-1]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin-1, col-1);
      atual.tab[lin-1][col-1]->ataques++;
    }
  }
  if((lin-1) >= 0 && atual.tab[lin-1][col+1] != NULL)
  {
    if((lin-1) >= 0 && atual.tab[lin-1][col+1]->codigo > 0)
    {
      insereJogada(lista, lin, col, lin-1, col+1);
      atual.tab[lin-1][col+1]->ataques++;
    }
  }
  if(lin == 0)
  {
    atual.tab[lin][col]->codigo = -5;
  }
}

//funções parte 3

stposicao CopiaPosicao(stposicao atual) 
{
  stposicao copia;
  int i, j;
  
  copia.pretas = CriaLista();
  copia.brancas = CriaLista();


  for(i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
    {
      copia.tab[i][j] = NULL;
    }
  }
  
  copia.qtdPretas = atual.qtdPretas;
  copia.qtdBrancas = atual.qtdBrancas;
  copia.jogVez = atual.jogVez;

  for(i = 0; i < 8 ; i++)
  {
    for(j = 0; j < 8 ; j++)
    {
      if(atual.tab[i][j] != NULL)
      {
        if(atual.tab[i][j]->codigo > 0)
        {
        copia.tab[i][j] = inserePecas(copia.brancas, atual.tab[i][j]->codigo, atual.tab[i][j]->linha, atual.tab[i][j]->coluna, atual.tab[i][j]->ataques);
        }
        else
          if(atual.tab[i][j]->codigo < 0)
          {
            copia.tab[i][j] = inserePecas(copia.pretas, atual.tab[i][j]->codigo, atual.tab[i][j]->linha, atual.tab[i][j]->coluna, atual.tab[i][j]->ataques);
          }
      }
    }
  }

  return copia;
}

double AvaliaPosicao(stposicao *lista)
{
  double valor = 0, valorpreta = 0, valortotal = 0; 
  
  stpeca *aux, *aux2;

  aux = lista->brancas->prox;

  while(aux != lista->brancas)
  {
    if(lista->brancas->codigo == 1)
    {
      valor += 1;
      if(lista->brancas->ataques > 0)
      {
        valor -= lista->brancas->ataques * 0.5;
      }
    }
    else
      if(lista->brancas->codigo == 2)
      {
        valor += 3;
        if(lista->brancas->ataques > 0)
        {
          valor -= lista->brancas->ataques * 0.5;
        }
      }
      else
        if(lista->brancas->codigo == 3)
        {
          valor += 3;
          if(lista->brancas->ataques > 0)
          {
            valor -= lista->brancas->ataques * 0.5;
          }
        }
        else
          if(lista->brancas->codigo == 4)
          {
            valor += 5;
            if(lista->brancas->ataques > 0)
            {
              valor -= lista->brancas->ataques * 0.5;
            }
          }
          else
            if(lista->brancas->codigo == 5)
            {
              valor += 9;
              if(lista->brancas->ataques > 0)
              {
                valor -= lista->brancas->ataques * 0.5;
              }
            }
            else
              if(lista->brancas->codigo == 6)
              {
                valor += 1000;
                if(lista->brancas->ataques > 0)
                {
                  valor -= lista->brancas->ataques * 0.5;
                }
              }
    aux = aux->prox;
  }
  
  aux2 = lista->pretas->prox;
  while(aux2 != lista->pretas)
  {
    if(lista->pretas->codigo == -1)
    {
      valorpreta += 1;
      if(lista->pretas->ataques > 0)
      {
        valorpreta -= lista->pretas->ataques * 0.5;
      }
    }
    else
      if(lista->pretas->codigo == -2)
      {
        valorpreta += 3;
        if(lista->pretas->ataques > 0)
        {
          valorpreta -= lista->pretas->ataques * 0.5;
        }
      }
      else
        if(lista->pretas->codigo == -3)
        {
          valorpreta += 3;
          if(lista->pretas->ataques > 0)
          {
            valorpreta -= lista->pretas->ataques * 0.5;
          }
        }
        else
          if(lista->pretas->codigo == -4)
          {
            valor += 5;
            if(lista->pretas->ataques > 0)
            {
              valorpreta -= lista->pretas->ataques * 0.5;
            }
          }
          else
            if(lista->pretas->codigo == -5)
            {
              valorpreta += 9;
              if(lista->pretas->ataques > 0)
              {
                valorpreta -= lista->pretas->ataques * 0.5;
              }
            }
            else
              if(lista->pretas->codigo == -6)
              {
                valorpreta += 1000;
                if(lista->pretas->ataques > 0)
                {
                  valorpreta -= lista->pretas->ataques * 0.5;
                }
              }
    aux2 = aux2->prox;
  }
  
  valortotal = valor - valorpreta;
  return valortotal;
}

stpeca *LiberaMemoria(stposicao *lista)
{
  stpeca *aux = lista->brancas;
  int i, j;

  lista->brancas->ant->prox = NULL;

  while(aux != NULL)
  {
    lista->brancas = lista->brancas->prox;
    free(aux);
    aux = lista->brancas;
  }

  stpeca *aux2 = lista->pretas;

  lista->pretas->ant->prox = NULL;

  while(aux2 != NULL)
  {
    lista->pretas = lista->pretas->prox;
    free(aux2);
    aux2 = lista->pretas;
  }

  for(i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
    {
      lista->tab[i][j] = NULL;
    }
  }

  return NULL;
}

stjogada ExecutaIA(stposicao atual, int nivel, double alfa, double beta)
{
  double melhorValor, valorJogada;
  int MAX_NIVEL = 9;
  int podado = 0;
  stjogada melhorJogada, jogadaIA, *lista, *jogadaAux, *listaBrancas = CriaListajogada(), *listaPretas = CriaListajogada();
  stposicao posCopia;

  if(nivel % 2 == 0)
  { 
    melhorValor = -INFINITY;
  }
  else
  {
    melhorValor = INFINITY;
  }

  //CalculaMovimentosPossiveis();

  //CalculaMovimentosPossiveis();

  stpeca *aux = atual.brancas->prox;
    while(aux != atual.brancas)
    {
      //aux->ataques = 0;

      if(aux->codigo == 1)
      {
        peaoBranco(listaBrancas, aux->linha, aux->coluna, atual);
      }
      if(aux->codigo == 2)
      {
        cavaloBranco(listaBrancas, aux->linha, aux->coluna, atual);
      }
      if(aux->codigo == 3)
      {
        bispoBranco(listaBrancas, aux->linha, aux->coluna, atual);
      }
      if(aux->codigo == 4)
      {
        torreBranca(listaBrancas, aux->linha, aux->coluna, atual);
      }
      if(aux->codigo == 5)
      {
        damaBranca(listaBrancas, aux->linha, aux->coluna, atual);
      }
      if(aux->codigo == 6)
      {
        reiBranco(listaBrancas, aux->linha, aux->coluna, atual);
      }
      aux = aux->prox;
    }

    stpeca *aux2 = atual.pretas->prox;
  
    while(aux2 != atual.pretas)
    {
      //aux2->ataques = 0;
      if(aux2->codigo == -1)
      {
        peaoPreto(listaPretas, aux2->linha, aux2->coluna, atual);
      }
      
      if(aux2->codigo == -2)
      {
        cavaloPreto(listaPretas, aux2->linha, aux2->coluna, atual);
      }

      if(aux2->codigo == -3)
      {
        bispoPreto(listaPretas, aux2->linha, aux2->coluna, atual);
      }

      if(aux2->codigo == -4)
      {
        torrePreta(listaPretas, aux2->linha, aux2->coluna, atual);
      }
      
      if(aux2->codigo == -5)
      {
        damaPreta(listaPretas, aux2->linha, aux2->coluna, atual);
      }
      if(aux2->codigo == -6)
      {
        reiPreto(listaPretas, aux2->linha, aux2->coluna, atual);
      }
      
      aux2 = aux2->prox;
    }
  
  if(nivel % 2 == 0)
  {
    stjogada *aux3 = listaPretas->prox;

    while(aux3 != listaPretas && podado==0 )
    {
      posCopia = CopiaPosicao(atual);
      ExecutaJogada(aux3, &posCopia);
      if(MAX_NIVEL > nivel)
      {
        if(nivel % 2 == 0)
        {
          jogadaIA = ExecutaIA(posCopia, (nivel+1) , melhorValor , beta);  
        }
        if(nivel % 2 != 0)
        {
          jogadaIA = ExecutaIA(posCopia, (nivel+1) , alfa , melhorValor);
        }
        ExecutaJogada(&jogadaIA, &posCopia);
      }
       valorJogada = AvaliaPosicao(&posCopia);
       if(valorJogada < alfa || valorJogada > beta)
       {
         podado+= 1;
       }
       if(nivel % 2 == 0 && valorJogada >= melhorValor)
      {
        melhorValor = valorJogada;
        melhorJogada = *aux3;
      }
      if(nivel % 2 != 0 && valorJogada <= melhorValor)
      {
        melhorValor = valorJogada;
        melhorJogada = *aux3;
      }
      LiberaMemoria(&posCopia);
      aux3 = aux3->prox;
    }
  }
  else
  {
    stjogada *aux3 = listaBrancas->prox;

    while(aux3 != listaBrancas && podado==0)
    {
      posCopia = CopiaPosicao(atual);
      ExecutaJogada(aux3, &posCopia);
      if(MAX_NIVEL > nivel)
      {
        if(nivel % 2 == 0)
        {
          jogadaIA = ExecutaIA(posCopia, (nivel+1) , melhorValor , beta);  
        }
        if(MAX_NIVEL > nivel && nivel % 2 != 0)
        {
          jogadaIA = ExecutaIA(posCopia, (nivel+1) , alfa , melhorValor);
        }
        ExecutaJogada(&jogadaIA, &posCopia);
      }
      valorJogada = AvaliaPosicao(&posCopia);
      if(valorJogada < alfa || valorJogada > beta)
      {
        podado+= 1;
      }
      if(nivel % 2 == 0 && valorJogada >= melhorValor)
      {
        melhorValor = valorJogada;
        melhorJogada = *aux3;
      }
      if(nivel % 2 != 0 && valorJogada <= melhorValor)
      {
        melhorValor = valorJogada;
        melhorJogada = *aux3;
      }
      LiberaMemoria(&posCopia);
      aux3 = aux3->prox;
    }
  }
  liberapeca(atual.brancas);
  liberapeca(atual.pretas);
  return melhorJogada; 
}

void SalvaJogada(stjogada *jog, FILE *arq)
{
  if(jog == NULL)
  {
    arq = fopen("jogadas.bin", "wb");
    fwrite(0,sizeof(int),0,arq);
    fclose(arq);
  }
  if(jog != NULL)
  {
    arq = fopen("jogadas.bin", "ab");

    fwrite(&jog->linhaDe, sizeof(int),1,arq);
    fwrite(&jog->colunaDe, sizeof(int),1,arq);
    fwrite(&jog->linhaPara, sizeof(int),1,arq);
    fwrite(&jog->colunaPara, sizeof(int),1,arq);

    fclose(arq);
  }
}

int main(void) 
{
  setlocale(LC_ALL, "Portuguese");
  int op, lin, col, peca, ctjogadas, verifica;
  stjogada *listajogada, *jogadaexecutada;
  struct elemento *lista = NULL;
  stposicao posAtual;
  stposicao listabranca, listapreta;
  stjogada jogadaEscolhida;

  FILE *arq;
  
  posAtual = IniciaTabuleiro();
  
  while(verifica != 1)
  {
    listajogada = CalculaMovimentosPossiveis(posAtual);
  
    printf("Tabuleiro: \n");
    tabuleiro(posAtual);
    if(posAtual.jogVez == 1)
    {
      printf("\nJogador 1\n");
      jogadaexecutada = escolha(listajogada);
      verifica = ExecutaJogada(jogadaexecutada, &posAtual);
      SalvaJogada(jogadaexecutada, arq);
      liberajogada(listajogada);
    }
    else
    {
      printf("\nJogador -1\n");
      jogadaEscolhida = ExecutaIA(posAtual, 0, -INFINITY, INFINITY);
      ExecutaJogada(&jogadaEscolhida, &posAtual);
      SalvaJogada(&jogadaEscolhida, arq);
    }
    
    
    system("cls||clear");
  }

  liberapeca(posAtual.brancas);
  liberapeca(posAtual.pretas);
  
  //AvaliaPosicao(&posAtual);

  return 0;
}