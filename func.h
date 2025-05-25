#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "header.h"

pdistancia calcDistancia();
int importdono(Listadono *ld);
int importcarro(Listadono *L, HASHING *has);
int importpassagem(Listapassagem *L,HASHING *has);
int importsensor(Listasensores *L);
pListadono crialistadono();
pListacarro crialistacarro();
pListasensor crialistasensores();
pListapassagem crialistapasagem();
pmarca criamarca(HASHING *has, char *nome);
HASHING *CriarHASHING();
void AddDono(Listadono* l, dono* D);
void Addcarro(Listacarro* l, carro* D);
void Addsensor(Listasensor* l, sensor* D);
void Addpassagem(Listapassagem* l, passagem* D);
void regist_dono(Listadono* Ld);
void list_dono(Listadono* Ld);
void regist_veiculo(Listadono *L,HASHING *has);
void list_veiculo(HASHING *has);
void regist_pass(Listapassagem* Lp, HASHING *has, Listasensor* Ls);
void organizadonos(Listadono* Ld);
void import(Listadono *Ld, HASHING *has, Listapassagem *Lp);
void organizacarros(HASHING *has);
void listacarroperiodo(Listapassagem *pass);
void rankveiculos(Listapassagem *pass,distancia *d, HASHING *has);
void rankmarcas(Listapassagem *pass, distancia *d, HASHING *has);
void listainfracao(Listapassagem* pass, distancia* d);

float velocidademedia(carro *c);
void marcamedia(Listapassagem *pass, distancia *d, HASHING *has);

void condutorediamax(HASHING *has);
void condutorpostal(pListapassagem pass, pListadono donos, marca* marcas, distancia* d);
void marcapopular(HASHING *has);
void exportarXl(Listapassagem* Lp, HASHING* has);
void exportarXML(Listapassagem* Lp, HASHING* has, Listasensor* Ls);

void calcvelociade(Listapassagem *p, distancia *d);
float calctempo(char *ano,char *mes,char *dia,char *hora,char *minut,char *sec);
void memoriateste(Listadono *Ld, HASHING *has, Listapassagem *Lp, Listasensor *Ls, pdistancia d);
size_t calcularMemoriaDistancia(pdistancia d);
size_t calcularMemoriaListasensor(pListasensor Ls);
size_t calcularMemoriaListapassagem(pListapassagem Lp);
size_t calcularMemoriaHashing(HASHING has);
size_t calcularMemoriaListacarro(pListacarro Lc);
size_t calcularMemoriaListadono(pListadono Ld);
