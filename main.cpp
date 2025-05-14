#include <stdlib.h>
#include <stdio.h>
#include<locale>
#include "func.h"

int main() {
    //calcDistancia();
    //setlocale(LC_ALL, "Portuguese");
    //setlocale(LC_ALL, "pt_PT.UTF-8");

    pListasensor Ls = crialistasensores();
    pListapassagem pp = crialistapasagem();
    importsensor(Ls);
    char c[10] = "Renault";
	pListadono Ld = crialistadono();
    pmarca m = criamarca(c);
    importdono(Ld);
	//regist_dono(Ld);
	//list_dono(Ld);
    importcarro(Ld, m);
    importpassagem(pp,m);
    //regist_veiculo(Ld,m);
    //list_veiculo(m);
	return 0;
}
