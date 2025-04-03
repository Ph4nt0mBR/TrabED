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
}donos,*donos;

typedef struct no
{
	struct donos *donos;
	struct no *prox;
}*no;

struct carros
{
	char matricula[8];
	char marca[20];
	char modelo[20];
	int ano;
	//falta adicionar o dono
	int codigo;
	int kilometros;
	int	tempototal;

}carros,*pcarros;

typedef struct nocarro
{
	carros *info;
	struct nocarro *prox;

}*nocarro;

typedef struct Lista {
	int numel;
	ptELEM inicio;
} *ptLISTA, LISTA;

struct marcas
{
	char nome[20];
	nocarro* nocarro;
	struct marcas *prox;
			
}*marcas;

struct sensores
{

}*sensores;

struct distancias
{

}*distancias;

struct passagem 
{

}*passagem;

