// GRR20197152 Guilherme Costa Pateiro
// Universidade Federal do Parana , programacao2 - ERE2 A
// ultima edicao: 17/11/2020 18:24


/*ERROS CONHECIDOS:
-podem haver erros ao ler comentarios 
*/
//NOTA:meu teclado estilo americano nao possui acentuacao, por esse motivo gostaria de me desculpar previamente por qualquer problema de clareza gerado por falta de acentuacao 

#include<stdio.h>           //leitura e escrita 
#include<string.h>          //manipulacao de string
#include<stdlib.h>          //alocacao dinamica (malloc /free)           
#include<math.h>            //sqrt
#include<dirent.h>          //leitura de diretorios
#include<unistd.h>          //funcoes de diretorio (chdir)
#include"pastilhas.h"       //manipulacao de pastilhas e imagens em ppm 


int main (int argc , char **argv){
    int countparam , k;                             //contadores
    int n;                                          //numero de arquivos na pasta de parstilhas
    struct dirent **nomes;                          //array de strings contendo o nome de todos os arquivos ppm da pasta de pastilhas
    FILE *entrada,*saida;                           //arquivos que serao usados como entrada e saida 
    FILE *temporario;                               //variavel que serve para abrir arquivos de pastilhas 
    T_imagem *bpastilha ;                           //array onde cada elemento tem uma pastilha e suas informacoes 
    T_imagem impricipal;                            //imagem princiapl que vai ser modificada pelo programa 
    char diretorioinicial[512];                     //guarda o diretorio que o executavel esta 
    char diretoriopastilhas[512] = "./tiles/";      //guarda o diretorio das pastilhas , por defaut ./tiles

    pwd(diretorioinicial);     //devolve o diretorio en que esta o executavel, caso SAIDA nao seja definido a imagem modificada sera colocada nesse diretorio

    entrada = stdin;    //coloca o valor de ENTRADA em stdin 
    saida = stdout;     //coloca o valor de SAIDA em stdout 

    for (countparam = 0; countparam < argc ; countparam++){ //analiza os parametros do programa ignorando os que nao se encaixam
        if (!strcmp(argv[countparam],"-i")){ // caso a entrada seja "-i" a proxima informacao sera o diretorio de entrada 
            entrada = fopen(argv[countparam + 1],"r");
            countparam++;
            if (!entrada){
                perror("arquivo de origem nao encontrado \n");
                exit(1);
            }
        }
        if (!strcmp(argv[countparam],"-o")){// caso a entrada seja "-o" a proxima informacao sera o diretorio de saida 
            saida = fopen(argv[countparam + 1],"w");
            countparam++;
            if (!saida){
                perror("arquivo de saida nao encontrado \n");  
                exit(1);  
            }

        }    
        if (!strcmp(argv[countparam],"-p")){// caso a entrada seja "-p" a proxima informacao sera o diretorio das pastilhas
            strcpy (diretoriopastilhas,argv[countparam + 1]);
            countparam++;
            }    
    }
    
    //escaneia a pasta de pastilhas , devolvendo um array com strings contendo o nome de todos os arquivos
    n = scandir(diretoriopastilhas ,&nomes, meucomparador, alphasort);
    if (n == -1){
        perror("diretorio de pastilhas vazio\n");
        exit(1);
    }  
    impricipal = carregaimagem(entrada); //carrega a imagem prinicipal 

    bpastilha = (T_imagem*) malloc (n * sizeof(T_imagem)); //aloca memoria para receber a biblioteca de pastilhas 

    chdir(diretoriopastilhas); //modifica o diretorio de execusao do programa para a pasta de pastilhas 

    //abre , carrega na memoria e fecha todos os arquivos da pasta de pastilhas  
    for (k = 0 ; k < n; k++) {
        temporario = fopen (nomes[k]->d_name,"r");
        if (! temporario){
            perror ("erro ao abrir arquivo no diretorio de pastilhas\n");
            exit (1);
        }    
        bpastilha[k] = carregaimagem(temporario);
    }
    //modifica o diretorio de execusao do programa para a pasta original do executavel 
    chdir(diretorioinicial);

    //faz a magia acontecer (nome nada explicativo em relacao ao uso da funcao)
    //pega a imagem principal ,divide ela em quadrados do tamanho de pastilhas e subistitui os quadrados por pastilhas  
    makethemagichappen(&impricipal, bpastilha, n);  

    //imprime a imagem principal
    imprimeimagem(impricipal,saida);

    //desaloca todos so mallocs e realocs que foram feitos ao longo da execusao do programa 
    for(k = 0 ; k < n ; k++){
        free(bpastilha[k].imagem);
    }
    free(bpastilha);
    free(impricipal.imagem);
    for(k = 0; k < n ; k++){
        free(nomes[k]);
    }
    free(nomes);
    fclose(saida);
}