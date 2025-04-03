#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>


#define BUFFER_SIZE 2048
#define SAMPLE_SIZE 10

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

}*pnocarro;

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

}*sensores;

typedef struct distancias
{

}*distancias;

typedef struct passagem 
{

}*passagem;

