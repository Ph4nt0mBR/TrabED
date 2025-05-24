#include "menu.h"
#include "func.h"
#include "header.h"

/*========================================
        Funções auxiliares do Menu
  ========================================*/

static void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void pressioneParaContinuar() {
    printf("\nPressione Enter para continuar...");
    limpaBuffer();
    getchar();
}

//verifica import de Donos.txt
static  int donosImport(Listadono* Ld) {
    return(Ld != NULL && Ld->numel > 0);
}

//Vericica import de Carros.txt
static int carrosImport(HASHING* has) {
    if (has == NULL) retunr 0;
    pmarca atual = has->Inicio;
    while (atual != NULL) {
        if (atual->inf != NULL && atual->inf->numel > 0) return 1;
        atual = atual->prox;
    }
    return 0;
}
/*========================================
            Submenu de importação
  ========================================*/

void submenuImportacoes(Listadono* Ld, HASHING* has, Listapassagem* Lp, Listasensor) {
    int escolha;
    do {
        printf("\n=== MENU DE IMPORTACAO ===\n");
        printf("1. Importar Donos)\n");
        printf("2. Importar Veículos\n");
        printf("3. Importar Passagens\n");
        printf("4. Importar Sensores\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
        case 1:
            if (importdono(Ld))
                printf("\nDonos importados com sucesso!");
            else
                printf("\nErro na importcao!");
            break;

        case 2:
            if (Ld->numel > 0) { //Verifica se donos.txt foi importado
                if (importcarro(Ld, has))
                    printf("\nVeiculos impotados com sucesso!");
                else
                    printf("\nErro na importacao de veiculos!");
            }
            else {
                printf("\nErro: Importe Donos primeiro!");
            }
            break;

        case 3:
            if (has->Inicio != NULL) { //Verifica se carros.txt foi importado
                if (importpassagem(Lp, has))
                    printf("\nPassagens importadas com sucesso!");
                else
                    printf("\nErro na importacao!");
            }
            else {
                printf("\nErro: Importe veiculos primeiro!");
            }
            break;

        case 4:
            if (importsensor(Ls))
                printf("\nSensores importados com sucesso!");
            else
                printf("\nErro na importacao");
            break;

        case 0:
            return;

        default:
            printf("Opcao invalida!");
    }
        pressioneParaContinuar();
    } while (1);
}

/*=============================================
              Submenus Especializados
  =============================================*/

void submenuRegistroManual(Listadono* Ld, HASHING* has, Listapassagem* Lp, Listasensor* Ls) {
    int escolha;
    do {
        printf("\n=== REGISTRO MANUAL ===\n");
        printf("1. Novo Dono\n");
        printf("2. Novo Veículo\n");
        printf("3. Nova Passagem\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
        case 1:
            regist_dono(Ld);
            break;
        case 2:
            regist_veiculo(Ld, has);
            break;
        case 3:
            regist_pass(Lp, has, Ls);
            break;
        }
        if (escolha != 0) pressioneParaContinuar();
    } while (escolha != 0);
}

void submenuOrganizacao(Listadono* Ld, HASHING* has) {
    int escolha;
    do {
        printf("\n=== ORGANIZAR DADOS ===\n");
        printf("1. Donos\n");
        printf("2. Veículos\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
        case 1:
            organizadonos(Ld);
            printf("\nDonos organizados.");

        case 2:
            organizacarros(has);
            printf("\nCarros organizados.");
        }
        if (escolha != 0) pressioneParaContinuar();
    } while (escolha != 0);
}

void submenuRelatorios()


/*=============================================
                Menu Principal
  =============================================*/

void main_menu(Listadono* Ld, HASHING* has, Listapassagem* Lp, Listasensor* Ls, pdistancia d, int importouDados) {
    int escolha;
    do{
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("1. Importar Dados\n");
        printf("2. Registrar Novo Dono\n");
        printf("3. Registrar Novo Veículo\n");
        printf("4. Listar Dados\n");
        printf("5. Relatórios\n");
        printf("6. Memória Utilizada\n");
        printf("0. Sair\n");
        printf("====================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d" & escolha);

    switch(escolha){
    case 1:
        submenuImportacoes(Ld, has, Lp, Ls);
        importouDados = 1;
        break;
    
    
    
    }
