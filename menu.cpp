#include "menu.h"
#include "func.h"


/*========================================
        Fun��es auxiliares do Menu
  ========================================*/

// Fun��o para limpar o buffer e evitar que n�o fa�a um scanf

/**
 * @brief Limpa o buffer de entrada.
 *
 * A fun��o l� e descarta todos os caracteres do buffer de entrada at� encontrar uma nova linha (`'\n'`) ou o fim do ficheiro (`EOF`).
 *
 */
static void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Espera que o utilizador pressione "Enter" para continuar.
 *
 * Exibe uma mensagem e espera que o utilizador pressione a tecla "Enter".
 * Utiliza limpaBuffer() para garantir que o buffer de entrada esteja limpo.
 */
static void pressioneParaContinuar() {
    printf("\nPressione Enter para continuar...");
    limpaBuffer();
    getchar();
}

/**
 * @brief Verifica se a lista de donos foi importada corretamente.
 *
 * @param Ld Ponteiro para a lista de donos.
 * @return 1 se a lista estiver importada e contiver elementos, 0 caso contr�rio.
 */
static  int donosImport(Listadono* Ld) {
    return(Ld != NULL && Ld->numel > 0);
}

/**
 * @brief Verifica se os carros foram importados corretamente.
 *
 * @param has Ponteiro para a estrutura de hashing contendo marcas e listas de carros.
 * @return 1 se houver pelo menos uma marca com carros importados, 0 caso contrario.
 */
static int carrosImport(HASHING* has) {
    if (has == NULL) return 0;
    pmarca atual = has->Inicio;
    while (atual != NULL) {
        if (atual->inf != NULL && atual->inf->numel > 0) return 1;
        atual = atual->prox;
    }
    return 0;
}

/**
 * @brief Verifica se existem donos (importados ou registados).
 *
 * @param Ld Ponteiro para a lista de donos.
 * @return 1 se existirem donos na lista, 0 caso contr�rio.
 */
static int existemDonos(Listadono* Ld) {
    return(Ld != NULL && Ld->numel > 0);
}

/**
 * @brief Verifica se existem veiculos (importados ou registados).
 *
 * @param has Ponteiro para a estrutura de hashing contendo marcas e listas de carros.
 * @return 1 se houver pelo menos uma marca com ve�culos registados, 0 caso contr�rio.
 */
static int existemVeiculos(HASHING* has) {
    if (has == NULL) return 0;
    pmarca atual = has->Inicio;
    while (atual != NULL) {
        if (atual->inf != NULL && atual->inf->numel > 0) return 1;
        atual = atual->prox;
    }
    return 0;
}

/**
 * @brief Verifica se existem sensores na lista.
 *
 * Esta fun��o verifica se a lista de sensores foi iniciada e se cont�m elementos.
 *
 * @param Ls Ponteiro para a lista de sensores.
 * @return 1 se existirem sensores na lista, 0 caso contr�rio.
 */
static int existemSensores(Listasensor* Ls) {
    return (Ls != NULL && Ls->numel > 0);
}

/*========================================
            Submenu de importa��o
  ========================================*/

  /**
   * @brief Submenu para importa��o de dados.
   *
   * Permite ao utilizador importar dados de donos, ve�culos e passagens, desde que as depend�ncias sejam respeitadas (por exemplo, ve�culos s� podem ser importados ap�s donos).
   *
   * @param Ld Ponteiro para a lista de donos.
   * @param has Ponteiro para a tabela de hash contendo marcas e ve�culos.
   * @param Lp Ponteiro para a lista de passagens.
   * @param Ls Ponteiro para a lista de sensores.
   */
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

  /**
   * @brief Submenu para registo manual de dados.
   *
   * Permite ao utilizador registar manualmente novos donos, ve�culos e passagens, assegurando que as depend�ncias est�o satisfeitas (por exemplo, ve�culos s� podem ser registados ap�s donos).
   *
   * @param Ld Ponteiro para a lista de donos.
   * @param has Ponteiro para a tabela de hash contendo marcas e ve�culos.
   * @param Lp Ponteiro para a lista de passagens.
   * @param Ls Ponteiro para a lista de sensores.
   */
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

/**
 * @brief Submenu para organiza��o de dados.
 *
 * Permite ao utilizador organizar a lista de donos e os ve�culos associados �s marcas.
 *
 * @param Ld Ponteiro para a lista de donos.
 * @param has Ponteiro para a tabela de hash contendo marcas e ve�culos.
 */
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

/**
 * @brief Submenu para listagem de dados.
 *
 * Permite listar todos os donos ou todos os ve�culos, caso existam na base de dados.
 *
 * @param Ld Ponteiro para a lista de donos.
 * @param has Ponteiro para a tabela de hash contendo marcas e ve�culos.
 */
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

/**
 * @brief Submenu para gera��o de relat�rios.
 *
 * Gera relat�rios como ranking de quilometragem por vei�culo e por marca, marca mais popular, lista de infra��es e velocidade m�dia por condutor.
 *
 * @param Lp Ponteiro para a lista de passagens.
 * @param Ld Ponteiro para a lista de donos.
 * @param has Ponteiro para a tabela de hash contendo marcas e ve�culos.
 * @param d Ponteiro para a estrutura de dist�ncias entre sensores.
 */
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


  /**
   * @brief Exibe o menu principal da aplica��o e executa as opera��es conforme a escolha do utilizador.
   *
   * A fun��o apresenta o menu principal com v�rias op��es, como importa��o de dados, registro manual, listagens, organiza��o de dados, gera��o de relat�rios, exporta��o
   * e verifica��o de mem�ria utilizada. 
   * O menu repete at� o utilizador escolher a op��o de sair (0).
   *
   * @param Ld Ponteiro para a lista de donos.
   * @param has Ponteiro para a tabela de hash contendo marcas e ve�culos.
   * @param Lp Ponteiro para a lista de passagens.
   * @param Ls Ponteiro para a lista de sensores.
   * @param d Ponteiro para a estrutura de dist�ncias entre sensores.
   */
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
