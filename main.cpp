#include <stdlib.h>
#include <stdio.h>
#include "func.h"
#include "menu.h"







int main() {
    pdistancia d =calcDistancia();

    //setlocale(LC_ALL, "Portuguese");
    //setlocale(LC_ALL, "pt_PT.UTF-8");

    pListasensor Ls = crialistasensores();
    pListapassagem pp = crialistapasagem();
    importsensor(Ls);
    HASHING *has = CriarHASHING();
	pListadono Ld = crialistadono();
    import(Ld, has, pp);
    //organizadonos(Ld);
	//list_dono(Ld);
    //regist_veiculo(Ld,m);
   //regist_pass(pp,has,Ls);
   //organizacarros(has);
  // list_veiculo(has);
    //listacarroperiodo(pp);
   // marcapopular(has);
    //rankveiculos(pp,d);
    //rankmarcas(pp,d, has);
    //calcvelociade( pp,d);
    //condutorediamax(has);
    memoriateste(Ld, has, pp, Ls, d);
    //exportarXl(pp, has);
/*
        // Inicializa estruturas
        Listadono* Ld = crialistadono();
        marca* marcas = criamarca(""); // Inicialização simplificada
        Listapassagem* Lp = crialistapasagem();

        int opcao;
        do {
            exibirMenuPrincipal();
            scanf("%d", &opcao);

            switch (opcao) {
            case 1: {
                // Submenu de importação
                printf("\n--- Importar Dados ---\n");
                printf("1. Donos\n2. Carros\n3. Passagens\n4. Sensores\n");
                int subop;
                scanf("%d", &subop);
                if (subop == 1) importdono(Ld);
                else if (subop == 2) importcarro(Ld, marcas);
                else if (subop == 3) importpassagem(Lp, marcas);
                else if (subop == 4) importsensor(Ld);
                break;
            }
            case 2:
                regist_dono(Ld);
                break;
            case 3:
                regist_veiculo(Ld, marcas);
                break;
            case 4:
                regist_pass(Lp, marcas->inf, NULL); // Ajuste conforme sua implementação
                break;
            case 5:
                list_dono(Ld);
                break;
            case 6:
                list_veiculo(marcas);
                break;
            case 7:
                organizadonos(Ld);
                break;
            case 8:
                organizacarros(*marcas);
                break;
            case 9: {
                // Submenu de estatísticas
                printf("\n--- Estatisticas ---\n");
                printf("1. Ranking por quilometragem\n2. Ranking por marca\n");
                int subop;
                scanf("%d", &subop);
                if (subop == 1) rankveiculos(*Lp, calcDistancia());
                else if (subop == 2) rankmarcas(*Lp, calcDistancia(), *marcas);
                break;
            }
            case 10:
                marcapopular(*marcas);
                break;
            case 11:
                printf("\n--- Exportar ---\n");
                printf("1. CSV\n2. XML\n");
                int subop;
                scanf("%d", &subop);
                if (subop == 1) exportarXl();
                else if (subop == 2) exportarXML();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
            }
        } while (opcao != 0);
        */
        // Liberar memória (implemente conforme necessário)
        return 0;
    }

