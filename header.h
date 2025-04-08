#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define BUFFER_SIZE 2048
#define SAMPLE_SIZE 10

//fazer ficheiros diferentes para cada struct, um pra donos, um pra carro (sugestao da stora)
typedef struct donos
{
	int numcontibuinte;
	char nome[30];
	int codPostal;
}donos,*pdonos;

typedef struct no
{
	struct donos *info;
	struct no *prox;
}no,*pno;

typedef struct Listadono {
	int numel;
	pno inicio;
} Listadono, pListadono;

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

}carros,*pcarros;

typedef struct nocarro
{
	carros *info;
	struct nocarro *prox;

}nocarro,*pnocarro;

typedef struct Listacarro {
	int numel;
	pnocarro inicio;
} Listacarro, pListacarro;

typedef struct marcas
{
	char nome[20];
	Lista *inf;
	struct marcas *prox;			
}*marcas;

typedef struct sensores
{
	//Perguntar(ao prof e samuel) se devemos fazer assim já que ´não alteramos os sensores!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int codSensor;
	char Designacao[20];
	char Latitude[18];
	char Longitude[15];
}sensores,*psensores;

typedef struct nosensores
{
	sensores* info;
	struct nosensores* prox;

}nosensores, * pnosensores;

typedef struct Listasensores{
	int numel;
	pnosensores inicio;
} Listasensores, pListasensores;

typedef struct distancias
{
	int cod1;//usamos ponteiro?
	int cod2;//usamos ponteiro?
	int distancia;
	// Perguntar(ao prof e samuel) se devemos fazer assim já que ´não alteramos as distancias e se quer fazer doutra forma!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}distancias;*pdistancias;

typedef struct nodistancias
{
	distancias* info;
	struct nodistancias* prox;

}nodistancias, * pnodistancias;

typedef struct Listadistancias {
	int numel;
	pnodistancias inicio;
} Listadistancias, pListadistancias;

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