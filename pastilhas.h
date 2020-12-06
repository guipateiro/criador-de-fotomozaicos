#ifndef pastilas
#define pastilas

// GRR20197152 Guilherme Costa Pateiro
// Universidade Federal do Parana , programacao2 - ERE2 A
// ultima edicao; 16/11/2020 22:17

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<dirent.h>
#include<unistd.h>

struct pixelp3{
	int c_r;
	int c_g;
	int c_b;
};
typedef struct pixelp3 pixelp3;

struct pixelp6{
    unsigned char c_r;
    unsigned char c_g;
    unsigned char c_b;
};
typedef struct pixelp6 pixelp6;

struct T_imagem{
    char tipo[3];
    int largura;
    int altura;
    int max; 
    pixelp3 *imagem;
    int mediaR;
    int mediaG;
    int mediaB; 
};
typedef struct T_imagem T_imagem;

T_imagem carregaimagem(FILE *arq);

float distanciacor(T_imagem imagem1 , T_imagem imagem2);

void pwd(char *resposta);

void makethemagichappen(T_imagem *imagempricipal, T_imagem *bibliopastilha ,int n);

void imprimeimagem(T_imagem imagempricipal, FILE *arq);

int meucomparador(const struct dirent *arquivo);

#endif