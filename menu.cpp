#include "menu.h"
#include "func.h"
#include "header.h"

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
                printf("\nErro: Registre veículos e sensores primeiro!");
            }
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

void submenuListagens(Listadono* Ld, HASHING* has) {
    int escolha;
    do {
        printf("\n=== LISTAR DADOS ===\n");
        printf("1. Listar Donos\n");
        printf("2. Listar Veículos\n");
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

void submenuRelatorios(Listapassagem* Lp, HASHING* has, pdistancia p) {
    int escolha;
    do {
        printf("\n=== RELATÓRIOS ===\n");
        printf("1. Ranking de Quilometragem\n");
        printf("2. Marca Mais Popular\n");
        printf("3. Velocidades Médias\n");
        printf("4. Listar Infrações\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &escolha);

        switch (escolha) {
        case 1:
            //rankveiculos(pp,d,has);
            break;
        case 2:
            //marcapopular(has);
            break;
        case 3:
            //velocidademedia();
            break;
        case 4:
            //listainfracao();
            break;
        }
        if (escolha != 0)pressioneParaContinuar();
    } while (escolha != 0);
}

/*=============================================
                Menu Principal
  =============================================*/

/*void main_menu(Listadono* Ld, HASHING* has, Listapassagem* Lp, Listasensor* Ls, pdistancia d,) {
    int escolha;
    do{
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("1. Importar Dados\n");
        printf("2. Registros\n");
        printf("3. Listas\n");
        printf("4. Relatórios\n");
        printf("5. Memória Utilizada\n");
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
            submenuRelatorios(Lp, has, d);
        } else {
            printf("\nImporte ou registre veiculos primeiro!");
            pressioneParaContinuar();
            }
    case 6:
        memoria
        }

    }
    }while(escolha != 0);
}*/
