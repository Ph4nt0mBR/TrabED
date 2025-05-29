#include "menu.h"
#include "func.h"


/*========================================
        Funções auxiliares do Menu
  ========================================*/

// Função para limpar o buffer e evitar que não faça um scanf
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

//Verifica import de Carros.txt
static int carrosImport(HASHING* has) {
    if (has == NULL) return 0;
    pmarca atual = has->Inicio;
    while (atual != NULL) {
        if (atual->inf != NULL && atual->inf->numel > 0) return 1;
        atual = atual->prox;
    }
    return 0;
}

//Verifica se existem donos (importados ou registrados)
static int existemDonos(Listadono* Ld) {
    return(Ld != NULL && Ld->numel > 0);
}

//Verifica se existem veículos (importados ou registrados)
static int existemVeiculos(HASHING* has) {
    if (has == NULL) return 0;
    pmarca atual = has->Inicio;
    while (atual != NULL) {
        if (atual->inf != NULL && atual->inf->numel > 0) return 1;
        atual = atual->prox;
    }
    return 0;
}

//Verifica se existem sensores
static int existemSensores(Listasensor* Ls) {
    return (Ls != NULL && Ls->numel > 0);
}

/*========================================
            Submenu de importação
  ========================================*/

void submenuImportacoes(Listadono* Ld, HASHING* has, Listapassagem* Lp, Listasensor* Ls) {
    int escolha;
    do {
        system("cls");
        printf("\n=== MENU DE IMPORTACAO ===\n");
        printf("1. Importar Donos)\n");
        printf("2. Importar Veiculos\n");
        printf("3. Importar Passagens\n");
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

        case 0:
            return;

        default:
            printf("\nOpcao invalida!");
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
        system("cls");
        printf("\n=== REGISTRO MANUAL ===\n");
        printf("1. Novo Dono\n");
        printf("2. Novo Veiculo\n");
        printf("3. Nova Passagem\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
        case 1:
            regist_dono(Ld);
            break;
        case 2:
            if (existemDonos(Ld)) {
                regist_veiculo(Ld, has);
            }
            else {
                printf("\nErro: registre donos primeiro!");
            }
            break;
        case 3:
            if (existemVeiculos(has) && existemSensores(Ls)) {
                regist_pass(Lp, has, Ls);
            }
            else {
                printf("\nErro: Registre veiculos e sensores primeiro!");
            }
            break;
        }
        if (escolha != 0) pressioneParaContinuar();
    } while (escolha != 0);
}

void submenuOrganizacao(Listadono* Ld, HASHING* has) {
    int escolha;
    do {
        system("cls");
        printf("\n=== ORGANIZAR DADOS ===\n");
        printf("1. Donos\n");
        printf("2. Veiculos\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
        case 1:
            organizadonos(Ld);
            printf("\nDonos organizados.");
            break;
        case 2:
            organizacarros(has);
            printf("\nCarros organizados.");
            break;
        }
        if (escolha != 0) pressioneParaContinuar();
    } while (escolha != 0);
}

void submenuListagens(Listadono* Ld, HASHING* has) {
    int escolha;
    do {
        system("cls");
        system("cls");
        printf("\n=== LISTAR DADOS ===\n");
        printf("1. Listar Donos\n");
        printf("2. Listar Veiculos\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
        case 1:
            if (existemDonos(Ld)) {
                list_dono(Ld);
            } else {
                printf("\nNenhum dono registrado!");
            }
            break;
        case 2:
            if (existemVeiculos(has)) {
                list_veiculo(has);
            }
            else {
                printf("\nNenhum veiculo registrado!");
            }
            break;
        case 0:
            return;
        default:
            printf("\nOpcao invalida!");
        }
        pressioneParaContinuar();
    } while (escolha != 0);
}

void submenuRelatorios(Listapassagem* Lp,Listadono *Ld, HASHING* has, pdistancia d) {
    int escolha;
    do {
        system("cls");
        resetmarca(has);
        printf("\n=== RELATORIOS ===\n");
        printf("1. Ranking de Quilometragem\n");
        printf("2. ranking de marca por quilometragem\n");
        printf("3. Marca Mais Popular\n");
        printf("4. Listar Infracoes\n");
        printf("5. velocidade de condutor x\n");
        printf("0. sair\n");
        scanf("%d",&escolha);

        switch (escolha) {
        case 1:
            rankveiculos(Lp,d,has);
            break;
        case 2:
            rankmarcas(Lp,d,has);
            break;
        case 3:
            marcapopular(has);
            break;
        case 4:
            listainfracao(Lp,d,has);
            break;
        case 5:
            condutorpostal(Lp, Ld, has, d);
            marcapopular(has);
            break;
        }

        pressioneParaContinuar();
    } while (escolha != 0);
}

/*=============================================
                Menu Principal
  =============================================*/

void main_menu(Listadono* Ld, HASHING* has, Listapassagem* Lp, Listasensor* Ls, pdistancia d) {

    int escolha;
    do{
        system("cls");
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("1. Importar Dados\n");
        printf("2. Registros\n");
        printf("3. Listas\n");
        printf("4. Organizacao\n");
        printf("5. Relatorios\n");
        printf("6. Exportacao\n");
        printf("7. Memoria Utilizada\n");
        printf("0. Sair\n");
        printf("====================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);

    switch(escolha){
    case 1:
        submenuImportacoes(Ld, has, Lp, Ls);
        break;
    case 2:
        submenuRegistroManual(Ld, has, Lp, Ls);
        break;
    case 3:
        submenuListagens(Ld, has);
        break;
    case 4:
        submenuOrganizacao(Ld, has);
        break;
    case 5:
        if (existemVeiculos(has)) {
            submenuRelatorios(Lp,Ld ,has, d);
            break;
        } else {
            printf("\nImporte ou registre veiculos primeiro!");
            pressioneParaContinuar();
            break;
            }
    case 7:
        memoriateste(Ld,has,Lp,Ls,d);
        pressioneParaContinuar();
        break;
    case 6:
        int opcao = 0;
        printf("deseja exportar para .csv?\n1-Sim\n2-Nao\n");
        scanf("%d",&opcao);
        if(opcao == 1){
            exportarXl(Lp,has);
        memoriateste(Ld,has,Lp,Ls,d);
        pressioneParaContinuar();
        break;
        }
        printf("deseja exportar para .xml?\n1-Sim\n2-Nao\n");
        scanf("%d",&opcao);
        if(opcao == 1){
            exportarXML(Lp, has, Ls, d, Ld);

        }
        pressioneParaContinuar();
        break;
    }

    }while(escolha != 0);
}
