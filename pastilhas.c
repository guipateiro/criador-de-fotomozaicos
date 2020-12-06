// GRR20197152 Guilherme Costa Pateiro
// Universidade Federal do Parana , programacao2 - ERE2 A
// ultima edicao: 19/11/2020 00:22
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<dirent.h>
#include<unistd.h>
#include"pastilhas.h"

T_imagem carregaimagem(FILE *arq){	
	//carregamento do cabecalho do arquivo .ppm , independe do tipo de arquivo
    //decalracao de varaiveis
    T_imagem pastilha;
    int i = 0; 
    char descarte[200] = "\0";
    //verificacao se existem comentarios no arquivo, jogando esses comentario em uam variavel de depejo
    //a seguir , apenas linhas com comentarios NAO sao verificacao de comentarios do arquivo  
    //a ideia utilizada foi de colocar todas as informacoes em uma variavel de descarte, substituir '#' por caracteres '\0', toda a strig depois do # fica invisivel para o programa
    fgets(descarte,200,arq);
    if (descarte[i] == '#')
        descarte[i] = '\0';
    while(i < 200 && descarte[i] != '\0'){
        if(descarte[i+1] == '#'){
            descarte[i+1] = '\0';
        }
        i++;
    }
    while (0 == strlen(descarte)){
        i = 0;
        fgets(descarte,200,arq);
        if (descarte[i] == '#')
            descarte[i] = '\0';
         while(i < 200 && descarte[i] != '\0'){
            if(descarte[i+1] == '#'){
                descarte[i+1] = '\0';
            }
            i++;
        }
    }
   	sscanf(descarte,"%s",pastilha.tipo); //recebe o tipo do arquivo , entre P3 e P6
    fgets(descarte,200,arq);
    i = 0;
    if (descarte[i] == '#')
        descarte[i] = '\0';
     while(i < 200 && descarte[i] != '\0'){
        if(descarte[i+1] == '#'){
            descarte[i+1] = '\0';
        }
        i++;
    }
    while (0 == strlen(descarte)){
        i = 0;
        fgets(descarte,200,arq);
        if (descarte[i] == '#')
            descarte[i] = '\0';
         while(i < 200 && descarte[i] != '\0'){
            if(descarte[i+1] == '#'){
                descarte[i+1] = '\0';
            }
            i++;
        }
    }
   	sscanf(descarte,"%i %i",&pastilha.largura, &pastilha.altura);// recebe as dimencoes do arquivo , altura e largura
    fgets(descarte,200,arq);
    i = 0;
    if (descarte[i] == '#')
        descarte[i] = '\0';
    while(i < 200 && descarte[i] != '\0'){
        if(descarte[i+1] == '#'){
            descarte[i+1] = '\0';
        }
        i++;
    }
    while (0 == strlen(descarte)){
        i = 0;
        fgets(descarte,200,arq);
        if (descarte[i] == '#')
            descarte[i] = '\0';
         while(i < 200 && descarte[i] != '\0'){
            if(descarte[i+1] == '#'){
                descarte[i+1] = '\0';
            }
            i++;
        }
    }
   	sscanf(descarte,"%i",&pastilha.max); //recebe o MAX do arquivo 

    //WARNING: CASO HAJA COMENTARIOS APOS O MAX O PROGRAMA VAI ENTERPRETAR COMO SENDO PARTE DA IMAGEM 

    //tendo o tamanho da imagem largura X altura , aloca memoria do tamanho exato do vetor de pixels
    //percorre a imagem colocando cada pixel em uma casa do vetor do tipo pixel que armazena as componentes RGB do pixel
    //usado fscanf para a leitura do vetor na imagem 
   	if (!strcmp(pastilha.tipo,"P3")){
        pastilha.imagem = (pixelp3*) malloc (pastilha.largura*pastilha.altura*sizeof(pixelp3));
   		for (i = 0 ; i<=(pastilha.largura*pastilha.altura)-1 ; i++){
   			fscanf(arq,"%i ", &pastilha.imagem[i].c_r);
   			fscanf(arq,"%i ", &pastilha.imagem[i].c_g);
   			fscanf(arq,"%i ", &pastilha.imagem[i].c_b);
   		}
    } 

    //para facilitar a conta e a compatibilitade no caso de arquivos de multiplos tipos,arquivos P6 sao trasformados em P3
    //utilizado fread pala ler o arquivos P6
    //existe uma variavel auxiliar que recebe o vetor P6 e depois converte para P3
    //ambos os vetores sao alocados dinamicamente se baseando em altura X largura 
    //a variavel auxiliar é desalocada apos a conversao de P6 para P3 
    else if (!strcmp(pastilha.tipo,"P6")){
        pixelp6 *imagemaux;
        imagemaux = (pixelp6*) malloc ((pastilha.largura*pastilha.altura +1)*sizeof(pixelp6));
        pastilha.imagem = (pixelp3*) malloc (pastilha.largura*pastilha.altura*sizeof(pixelp3));
        fread(imagemaux,sizeof(pixelp6),(pastilha.largura*pastilha.altura)+1,arq);
        for (i = 0; i<=(pastilha.largura*pastilha.altura)-1; i++){
            pastilha.imagem[i].c_r = (int) imagemaux[i].c_r;
            pastilha.imagem[i].c_g = (int) imagemaux[i].c_g;
            pastilha.imagem[i].c_b = (int) imagemaux[i].c_b;
        }
        free(imagemaux);
	}

	else {
		perror("formato de arquivo invalido");
		exit (1);
	} 
    //tira a media de RGB da imagem que posteriormente vai ser usado para o calculo de distancia entre 2 imagens  
    pastilha.mediaR = 0; 
    pastilha.mediaG = 0; 
    pastilha.mediaB = 0; 
    for (i = 0 ; i<=(pastilha.largura*pastilha.altura)-1 ; i++){
        pastilha.mediaR = pastilha.mediaR + pastilha.imagem[i].c_r;
        pastilha.mediaG = pastilha.mediaG + pastilha.imagem[i].c_g;
        pastilha.mediaB = pastilha.mediaB + pastilha.imagem[i].c_b;
    }
    pastilha.mediaR = pastilha.mediaR / (pastilha.largura*pastilha.altura);
    pastilha.mediaG = pastilha.mediaG / (pastilha.largura*pastilha.altura);
    pastilha.mediaB = pastilha.mediaB / (pastilha.largura*pastilha.altura);

    //fecha o arquivo que acabou de ser lido pois toda a sua informcaoja esta salva em uma variavel T_imagem 
   	fclose(arq);
    //devolve a variavel para o programa principal    
    return pastilha;
}


//  funcao que calcula a distancia de cor entre 2 blocos 
//  a formula usada foi a readmen 
float distanciacor(T_imagem imagem1 , T_imagem imagem2){
    float mediared;
    float distancia = 0;

    mediared = (imagem1.mediaR + imagem2.mediaR)/2;
    distancia = sqrt(((2+(mediared/256))*pow(imagem1.mediaR - imagem2.mediaR,2))
                      +(4*pow(imagem1.mediaG - imagem2.mediaG,2))
                      +((2+(255-mediared)/256)*pow(imagem1.mediaB - imagem2.mediaB,2)));
    return distancia;
}

//funcao que devolde em RESPOSTA o caminho completo do diretorio atual
//pode ser usado como ancora para voltar para o inicio em caso de mudancas de diretorio 
void pwd(char *resposta){

    getcwd(resposta,sizeof(resposta));
        if (resposta == NULL){
            perror("erro ao receber diretorio do executavel");
            exit(1);
        }    
    return;
}

//eu sei que o nome da funcao é ridiculo 
//troca a imagem principal por um conjuntos de pastilhas previamente carregadas 
void makethemagichappen(T_imagem *imagempricipal, T_imagem *bibliopastilha ,int n){
T_imagem auxiliar;
    int i,j,l,m,k;  //todos contadores, por causa do loop de 6 "fors"
    float dist;
    float compara;
    //cria uma imagem auxiliar que recebe quadrados da imagem principal
    auxiliar.imagem = (pixelp3*) malloc ((bibliopastilha[0].altura*bibliopastilha[0].largura +1)*sizeof(pixelp3)); 
    //percorre a imagem prinicipal andando N pixel equivalente ao tamanho de uma pastilha
    for(j = 0; j < imagempricipal->largura; j+=bibliopastilha[0].largura){
        for (i = 0 ; i < imagempricipal->altura; i+=bibliopastilha[0].altura){
            m = 0;
            compara = 10000;
            //dentro do quadrado do tamanho de uma pastilha, carrega seu conteudo em auxiliar
            for (k = i ; k < i + bibliopastilha[0].altura && k < imagempricipal->altura; k++){
                for (l = j ; l < j + bibliopastilha[0].largura && l < imagempricipal->largura; l++){
                    auxiliar.imagem[m].c_r = imagempricipal->imagem[l+(k*imagempricipal->largura)].c_r;
                    auxiliar.imagem[m].c_g = imagempricipal->imagem[l+(k*imagempricipal->largura)].c_g;
                    auxiliar.imagem[m].c_b = imagempricipal->imagem[l+(k*imagempricipal->largura)].c_b;
                    m++;
                }
            }
            //reinicia as medias para evitar que ela pegue dados do quadrado anterior
            //tira a media de RGB do quadrado atual para calculo da distancia 
            auxiliar.mediaR = 0; 
            auxiliar.mediaG = 0; 
            auxiliar.mediaB = 0; 
            for (l = 0 ; l<=(bibliopastilha[0].largura*bibliopastilha[0].altura)-1 ; l++){
                auxiliar.mediaR = auxiliar.mediaR + auxiliar.imagem[l].c_r;
                auxiliar.mediaG = auxiliar.mediaG + auxiliar.imagem[l].c_g;
                auxiliar.mediaB = auxiliar.mediaB + auxiliar.imagem[l].c_b;
            }
            auxiliar.mediaR = auxiliar.mediaR / (bibliopastilha[0].largura*bibliopastilha[0].altura);
            auxiliar.mediaG = auxiliar.mediaG / (bibliopastilha[0].largura*bibliopastilha[0].altura);
            auxiliar.mediaB = auxiliar.mediaB / (bibliopastilha[0].largura*bibliopastilha[0].altura);

            //compara auxiliar com a biblioteca de pastilhas retornando o indice no vetor do elemento com menor distancia 
            int o = 0;
            for(int p = 0; p < n ; p++){
                dist = distanciacor(bibliopastilha[p],auxiliar); 
                if (dist < compara){
                    o = p; 
                    compara = dist;
                }
            }
            //subistituicao, na imagem principal , do quadrado pela pastilha
            m = 0;
            for (k = i ; k < i + bibliopastilha[0].altura && k < imagempricipal->altura; k++){
                for (l = j ; l < j + bibliopastilha[0].largura && l < imagempricipal->largura; l++){
                    imagempricipal->imagem[l+(k*imagempricipal->largura)].c_r = bibliopastilha[o].imagem[m].c_r;
                    imagempricipal->imagem[l+(k*imagempricipal->largura)].c_g = bibliopastilha[o].imagem[m].c_g;
                    imagempricipal->imagem[l+(k*imagempricipal->largura)].c_b = bibliopastilha[o].imagem[m].c_b;
                    m++;
                }
            }
        }
    }
    free(auxiliar.imagem);
    return;
}

//imprime , em arq, uma variavel T_imagem
//caso o tipo da imagem seja P6 ela converte de P3 para P6 (porque ela foi anteriormente convertida de P6 para P3 para unificar os calculos) 
void imprimeimagem(T_imagem imagempricipal, FILE *arq){
    int k;
    if (!strcmp(imagempricipal.tipo,"P3")){
        fprintf(arq,"%s\n%i %i\n%i\n","P3\0", imagempricipal.largura, imagempricipal.altura,imagempricipal.max);
        for (k = 0 ; k < (imagempricipal.altura*imagempricipal.largura) ; k++){
            fprintf(arq,"%i ",imagempricipal.imagem[k].c_r);
            fprintf(arq,"%i ",imagempricipal.imagem[k].c_g);
            fprintf(arq,"%i ",imagempricipal.imagem[k].c_b);
        }
    }    
    else if (!strcmp(imagempricipal.tipo,"P6")){ 
        fprintf(arq,"%s\n%i %i\n%i\n","P6\0", imagempricipal.largura, imagempricipal.altura,imagempricipal.max);
        for (k = 0 ; k < (imagempricipal.altura*imagempricipal.largura) ; k++){
            fprintf(arq,"%c",(unsigned char) imagempricipal.imagem[k].c_r);
            fprintf(arq,"%c",(unsigned char) imagempricipal.imagem[k].c_g);
            fprintf(arq,"%c",(unsigned char) imagempricipal.imagem[k].c_b);
        }
    }
    return;
}

int meucomparador(const struct dirent *arquivo){
    int tam = 0; 
    tam = strlen(arquivo->d_name);
    if (tam < 4)
        return 0; 
    if (arquivo->d_name[tam-1] == 'm' && arquivo->d_name[tam-2] == 'p' && arquivo->d_name[tam-3] == 'p' &&arquivo->d_name[tam-4] == '.')
        return 1;
    return 0; 
}  