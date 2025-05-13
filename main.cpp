#include <stdlib.h>
#include <stdio.h>
#include "func.h"

int main() {
	pListadono Ld = crialistadono();
    importdono(Ld);
	//regist_dono(Ld);
	list_dono(Ld);
	return 0;
}
