#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define BUFFER_SIZE 2048
#define SAMPLE_SIZE 10

//fazer ficheiros diferentes para cada struct, um pra donos, um pra carro (sugestao da stora)
//tá feito
typedef struct donos
{
	int numcontibuinte;
	char nome[30];
	int codPostal;
}dono,*pdono;

typedef struct no
{
	struct dono *info;
	struct no *prox;
}no,*pno;

typedef struct Listadono {
	int numel;
	pno inicio;
} Listadono, *pListadono;

typedef struct carros
{
	char matricula[8];
	char marca[20];
	char modelo[20];
	int ano;
	donos *pdonos;
	int codigo;
	int kilometros;
	int	tempototal;

}carro,*pcarro;

typedef struct nocarro
{
	carro *info;
	struct nocarro *prox;

}nocarro,*pnocarro;

typedef struct Listacarro {
	int numel;
	pnocarro inicio;
} Listacarro, pListacarro;

typedef struct marcas
{
	char nome[20];
	Listacarro* inf;
	struct marca* prox;
}marca, * pmarca;

typedef struct sensores
{
	//Perguntar(ao prof e samuel) se devemos fazer assim já que ´não alteramos os sensores!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//O Rafa ou o professor vai saber te ajudar melhor q eu. Sou uma decepção em prog
	int codSensor;
	char Designacao[20];
	char Latitude[18];
	char Longitude[15];
}sensor,*psensor;

typedef struct nosensores
{
	sensores* info;
	struct nosensores* prox;

}nosensor, * pnosensor;

typedef struct Listasensores{
	int numel;
	pnosensores inicio;
} Listasensor, pListasensor;

typedef struct distancias//podemos transformar isto numa matriz
{
	int cod1;//usamos ponteiro?
	int cod2;//usamos ponteiro?
	int distancia;
	// Perguntar(ao prof e samuel) se devemos fazer assim já que ´não alteramos as distancias e se quer fazer doutra forma!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}distancia,*pdistancia;

typedef struct nodistancias
{
	distancia* info;
	struct nodistancia* prox;

}nodistancia, * pnodistancia;

typedef struct Listadistancias {
	int numel;
	pnodistancia inicio;
} Listadistancia, pListadistancia;

typedef struct passagem 
{
	int idsensor;
	char codcarro;//usamos ponteiro?
	char data;
	int tiporegist;

}passagem,*ppassagem;

typedef struct nopassagem
{
	passagem* info;
	struct nopassagem* prox;

}nopassagem, * pnopassagem;

typedef struct Listapassagem {
	int numel;
	pnopassagem inicio;
} Listapassagem, pListapassagem;