#include <stdlib.h>
#include <stdio.h>
#include "func.h"

int main() {
    char c[10] = "Renault";
	pListadono Ld = crialistadono();
    pmarca m = criamarca(c);
    importdono(Ld);
	//regist_dono(Ld);
	//list_dono(Ld);
    importcarro(Ld, m);
    //regist_veiculo(Ld,m);
    //list_veiculo(m);
	return 0;
}
