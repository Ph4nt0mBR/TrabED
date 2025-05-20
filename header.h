//Manter os inlcudes no header e incluir o header nos outros ficheiros
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

//Buffer de memoria. Seria interessante quando for ler os arquivos que sao muito grandes
#define BUFFER_SIZE 2048
#define SAMPLE_SIZE 10

//fazer ficheiros diferentes para cada struct, um pra donos, um pra carro (sugestao da stora)
//tá feito - Afonso
#pragma once

//Struct para donos
typedef struct donos
{
	int numcontibuinte;
	char nome[100];
	char codPostal[9];
}dono,*pdono;

//Struct de nos para verificacao da lista de donos. Aponta entre nos para poupar memoria
typedef struct no
{
	dono *info;
	struct no *prox;
}no,*pno;

//
typedef struct Listadono {
	int numel;
	pno inicio;
} Listadono, *pListadono;

//Struct para leitura e verificacao de carros
typedef struct carros
{
	char matricula[9];
	char marca[20];
	char modelo[20];
	int ano;
	donos *pdonos;
	int codigo = 0;
	int kilometros;
	int	tempototal;

}carro,*pcarro;

//Struct de nos para verificacao da lista de carros. Aponta entre nos para poupar memoria
typedef struct nocarro
{
	carro *info;
	struct nocarro *prox;

}nocarro,*pnocarro;

//
typedef struct Listacarro {
	int numel;
	pnocarro inicio;
} Listacarro, *pListacarro;

//
typedef struct marcas
{
	int Numcarrototal;
	int NUmcarromarca;
	int numkillmarca;
	char nome[20];
	Listacarro* inf;
    struct marcas* prox;
}marca, * pmarca;

typedef struct
{
    marca *Inicio;
    //int N_CHAVES;
   //int Flag_Destruir;
}HASHING;

//Struct para os sensores. teremos uma matriz para as distancias entre os sensores, ja que a posicao dos sensores sao imutaveis
typedef struct sensores
{
	//Perguntar(ao prof e samuel) se devemos fazer assim ja que nao alteramos os sensores!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//O Rafa ou o professor vai saber te ajudar melhor q eu. Sou uma decepcao em prog
	int codSensor;
	char Designacao[50];
	char Latitude[50];
	char Longitude[50];
}sensor,*psensor;

//
typedef struct nosensores
{
	sensores* info;
	struct nosensores* prox;

}nosensor, * pnosensor;

//
typedef struct Listasensores{
	int numel;
	pnosensor inicio;
} Listasensor, *pListasensor;


typedef struct distancias//podemos transformar isto numa matriz
{
	int cod1[10];//usamos ponteiro?
	int cod2[10];//usamos ponteiro?
	float dist[10][10];
	// Perguntar(ao prof e samuel) se devemos fazer assim ja que nao alteramos as distancias e se quer fazer doutra forma!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}distancia,*pdistancia;

//
typedef struct passagem
{
	int idsensor;
	pcarro codcarro;
	char data[100];
	int tiporegist;

}passagem,*ppassagem;

//
typedef struct nopassagem
{
	passagem* info;
	struct nopassagem* prox;

}nopassagem, * pnopassagem;

//
typedef struct Listapassagem {
	int numel;
	pnopassagem inicio;
} Listapassagem, *pListapassagem;
