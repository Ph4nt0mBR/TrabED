#include <stdlib.h>
#include <stdio.h>
#include "func.h"

int main() {
    //calcDistancia();
    //setlocale(LC_ALL, "Portuguese");
    //setlocale(LC_ALL, "pt_PT.UTF-8");

    pListasensor Ls = crialistasensores();
    pListapassagem pp = crialistapasagem();
    importsensor(Ls);
    char c[50] = "Mitsubishi";
	pListadono Ld = crialistadono();
    pmarca m = criamarca(c);
    importdono(Ld);
	//regist_dono(Ld);
	//list_dono(Ld);
    importcarro(Ld, m);
    //importpassagem(pp,m);
    //regist_veiculo(Ld,m);
    list_veiculo(m);
    //regist_pass(pp,m,Ls);
	return 0;
}
