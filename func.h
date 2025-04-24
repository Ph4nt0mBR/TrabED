#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "header.h"

pListadono crialistadono();
pListacarro crialistacarro();
pListasensor crialistasensores();
pListapassagem crialistapasagem();
void AddDono(Listadono* l, dono* D);
void regist_dono(Listadono* Ld);

