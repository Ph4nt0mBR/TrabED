#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "header.h"

pdistancia calcDistancia(double matriz[SAMPLE_SIZE][SAMPLE_SIZE]);
int importdono(Listadono *ld);
int importcarro(Listadono *L, marcas *nm);
int importpassagem(Listapassagem *L,marca *m);
int importsensor(Listasensores *L);
pListadono crialistadono();
pListacarro crialistacarro();
pListasensor crialistasensores();
pListapassagem crialistapasagem();
pmarca criamarca(char *nome);
void AddDono(Listadono* l, dono* D);
void Addcarro(Listacarro* l, carro* D);
void addmarca(pmarca l, marca *nmarca);
void Addsensor(Listasensor* l, sensor* D);
void Addpassagem(Listapassagem* l, passagem* D);
void regist_dono(Listadono* Ld);
void list_dono(Listadono* Ld);
void regist_veiculo(Listadono *L,marca *nm);
void list_veiculo(marca *m);
void regist_pass(Listapassagem* Lp, Listacarro* Lc, Listasensor* Ls);
void organizadonos(Listadono* Ld);
void import(Listadono *Ld, marca *m, Listapassagem *Lp);
void organizacarros(marca m);
void listacarroperiodo(Listapassagem pass);
void rankveiculos(Listapassagem pass,distancia d);
void rankmarcas(Listapassagem pass, distancia d, marca m);

void marcapopular(marcas m);
