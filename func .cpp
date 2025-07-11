#include "header.h"
#include "func.h"
#include <time.h>
#include <locale>


/** L� dist�ncias entre pares de n�s do arquivo "distancias.txt" e armazena-as numa matriz 10x10 dentro da struct "distancias".
Cada linha do arquivo deve conter: <no1> <no2> <distancia>. Os n�s devem estar no intervalo [1, 10]. A dist�ncia � registada nos dois sentidos.
*/

/**
 * @brief L� dist�ncias entre pares de n�s do arquivo "distancias.txt" e armazena-as numa matriz 10x10 dentro da struct "distancias".
Cada linha do arquivo deve conter: <no1> <no2> <distancia>. Os n�s devem estar no intervalo [1, 10]. A dist�ncia � registada nos dois sentidos.
 *
 * @return ponteiro para estrutura distancias preenchida
 */
pdistancia calcDistancia()
{
    printf("sizeof(distancias) = %d\n", sizeof(distancias));
	// Inicializa a matriz com zeros
	pdistancia nd =  (pdistancia)malloc(sizeof(distancias));
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			nd->dist[i][j] = 0.0;
		}
	}
	// Abre o arquivo
	FILE* F = fopen("distancias.txt", "r");
	if (F == NULL) {
		perror("Erro ao abrir o arquivo");
		exit(EXIT_FAILURE);
	}

	// L� cada linha e preenche a matriz
	int no1, no2;

	float distancia;

	while (fscanf(F, "%d %d %f", &no1, &no2, &distancia) == 3) {
		if (no1 < 1 || no1 > 10 || no2 < 1 || no2 > 10) {
			fprintf(stderr, "No invalido no arquivo: %d %d\n", no1, no2);
			continue;
		}
        //printf("\n\n\ndist :%f\n",distancia);
		nd->cod1[no1 - 1] = no1;
		nd->cod2[no2 - 1] = no2;
		nd->dist[no1 - 1][no2 - 1] = distancia;
		nd->dist[no2 - 1][no1 - 1] = distancia; // Matriz sim�trica
        //printf("\n%d\n,%d : %f\n",nd->cod1[no1-1],nd->cod2[no2-1],nd->dist[no1-1][no2-1]);
	}
	fclose(F);
    return nd;
}




/**
 * \brief Importa dados de donos de ve�culos a partir de um arquivo
 *
 * L� dados de donos a partir do arquivo "donos.txt" e adiciona-os � lista ligada "Listadono".
 * Cada linha deve conter: <c�digo> <nome> <c�digo postal>, separados por tabula��o ou espa�o.
 *
 * \param ld Ponteiro para a lista de donos onde os dados ser�o importados
 * \return int 1 se pelo menos um registro foi importado com sucesso, 0 caso contr�rio
 *
 *
 *
 */
int importdono(Listadono *ld) {
    pListadono L = ld;
    FILE* F = fopen("donos.txt", "r");
    if (F == NULL) {
        printf("\nErro ao abrir o ficheiro para leitura!\n");
        return 0;
    }

    char buffer[1024];
    int line = 0;
    int count = 0;
    int problemas = 0;

    printf("Iniciando leitura...\n");

    while (fgets(buffer, sizeof(buffer), F)) {
        line++;
        size_t len = strlen(buffer);

        // Remove caracteres de nova linha
        if (len > 0 && buffer[len-1] == '\n') buffer[--len] = '\0';
        if (len > 0 && buffer[len-1] == '\r') buffer[--len] = '\0';

        // Pula linhas vazias
        if (len == 0) continue;

        int COD;
        char NOME[100] = {0};
        char CP[10] = {0};
        int campos_lidos;

        // Tentativa 1: Formato com tabs
        campos_lidos = sscanf(buffer, "%d\t%99[^\t]\t%9s", &COD, NOME, CP);

        // Tentativa 2: Formato com espa�os (se falhou com tabs)
        if (campos_lidos != 3) {
            campos_lidos = sscanf(buffer, "%d %99[^\n] %9s", &COD, NOME, CP);
        }

        if (campos_lidos == 3) {
            // Verifica dados num�ricos
            if (COD <= 0) {
                printf("Linha %d: Codigo invalido (%d)\n", line, COD);
                problemas++;
                continue;
            }

            // Cria e adiciona o dono
            pdono ndono = (pdono)malloc(sizeof(dono));
            if (!ndono) {
                printf("Erro de memoria na linha %d\n", line);
                problemas++;
                continue;
            }
           // printf("%d\t",COD);
           // printf("%s\t",NOME);
            //printf("%s\n",CP);
            ndono->numcontibuinte = COD;
            strncpy(ndono->nome, NOME, sizeof(ndono->nome)-1);
            strncpy(ndono->codPostal, CP, sizeof(CP));

            AddDono(L, ndono);
            count++;
        } else {
            printf("ERRO - Linha %d: Formato invalido\n", line);
            printf("Conteudo: [%s]\n\n", buffer);
            problemas++;
        }
    }

    fclose(F);

    printf("\nRelatorio Final:\n");
    printf("Total de linhas: %d\n", line);
    printf("Registos importados: %d\n", count);
    printf("Problemas detetados: %d\n\n", problemas);

    return (count > 0) ? 1 : 0;
}


/**
 * \brief Importa dados de ve�culos a partir de um arquivo
 *
 * Esta fun��o l� dados de ve�culos do arquivo "carros.txt" e associa-os:
 * 1. Ao respectivo dono na lista de donos (atraves do NIF)
 * 2. A marca correspondente no hashing
 *
 * \param L Ponteiro para a lista de donos
 * \param has Ponteiro para o hashing das marcas
 * \return int 1 se a importa�ao teve sucesso (pelo menos 1 carro importado), 0 se n�o tiver
 */

int importcarro(Listadono *L, HASHING *has){
    FILE* F = fopen("carros.txt", "r");
    if (F == NULL) {
        printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
        return 0;
    }
    long ndono;
    int cod, ano;
    char Marca[100], modelo[100], Mat[10];
    while (1) {
        int result = fscanf(F, "%9[^\t]\t%99[^\t]\t%99[^\t]\t%d\t%d\t%d\n",Mat, Marca, modelo, &ano, &ndono, &cod);
        if (result != 6) {
            if (feof(F)) break;
            printf("Erro ao ler linha do ficheiro.\n");
            continue;
        }

        //printf("IMPORTOU carro: COD = %d, MARCA: [%s], DONO: [%d], MATRICULA[%s]\n", cod, Marca, ndono,Mat);

        pcarro ncarro = (pcarro)malloc(sizeof(carro));
        if (ncarro == NULL) {
            printf("Erro de alocacao de memoria.\n");
            fclose(F);
            return 0;
        }
        strcpy(ncarro->matricula, Mat);
        strcpy(ncarro->marca, Marca);
        strcpy(ncarro->modelo, modelo);
        ncarro->ano = ano;
        ncarro->codigo = cod;
        ncarro->kilometros = 0;
        ncarro->tempototal = 0;
        pno ldono = L->inicio;
        while ((ldono != NULL )&& (ldono->info->numcontibuinte != ndono)){
            ldono = ldono->prox;
        }
        if (ldono == NULL) {
            printf("Dono '%d' nao encontrado. Carro '%s' ignorado.\n", ndono, Mat);
            free(ncarro);
            continue;
        }
       ncarro->pdonos = ldono->info;

        pmarca m = has->Inicio;
        while (m != NULL && strcmp(m->nome, ncarro->marca) != 0)
            m = m->prox;

        if (m == NULL) {
            pmarca nmarca = criamarca(has,ncarro->marca);
            if (!nmarca) {
                printf("Erro ao criar marca '%s'.\n", ncarro->marca);
                free(ncarro);
                continue;
            }
            m = nmarca;
          //  m->inf = NULL;
            m->Numcarrototal = 0;
        }

        m->Numcarrototal++;

        Addcarro(m->inf, ncarro);
    }

    fclose(F);
    return 1;
}




/**
 * \brief Importa dados de passagens a partir de um arquivo
 *
 * L� dados de passagens do arquivo "passagem.txt" e associa cada uma ao carro correspondente na estrutura "HASHING".
 * Cada linha deve conter: <ID sensor> <c�digo do carro> <data> <tipo de registo>, separados por tabula��es.
 *
 * \param L Ponteiro para a lista de passagens
 * \param has Ponteiro para a estrutura de hashing contendo as marcas e seus ve�culos
 * \return int 1 se a importa��o teve sucesso (pelo menos uma passagem foi importada), 0 caso contr�rio
 */
int importpassagem(Listapassagem *L, HASHING *has) {
    FILE* F = fopen("passagem.txt", "r");
    if (F == NULL) {
        printf("\nErro ao abrir o ficheiro!\n");
        return 0;
    }

    int Id, COD, regist;
    char Data[50];

    int contador =0;
    while (fscanf(F, "%d\t%d\t%49[^\t]\t%d", &Id, &COD, Data, &regist) == 4) {
        //printf("Processando: ID=%d, COD=%d, Data=%s, Regist=%d\n", Id, COD, Data, regist);
        contador++;
if (contador%100000 == 0)
    printf("\n+ %d passagens lidas",contador);
        passagem* npass = (passagem*)malloc(sizeof(passagem));
        if (!npass) {
            printf("Erro de alocacao de memoria\n");
            fclose(F);
            return 0;
        }

        npass->idsensor = Id;
        strcpy(npass->data, Data);
        npass->tiporegist = regist;
        // Busca pelo carro correspondente
        pmarca pm = has->Inicio;
        pnocarro plc = NULL;
        int encontrado = 0;

        while (pm != NULL && encontrado != 1) {
            if (pm->inf == NULL || pm->inf->inicio == NULL) {
                pm = pm->prox;
                continue;
            }
            nocarro *current = pm->inf->inicio;
            while (current != NULL && encontrado != 1) {
                if (current->info != NULL && current->info->codigo == COD) {
                    plc = current;
                    encontrado = 1;
                    //printf("Carro COD %d encontrado na marca\n", COD);
                }
                current = current->prox;
            }

            if (encontrado != 1) pm = pm->prox;
        }

        if (!encontrado) {
            printf("Carro COD %d nao encontrado! Registo ignorado.\n", COD);
            free(npass);
            continue;  // Continua para pr�ximo registro ao inv�s de retornar
        }

    npass->codcarro = plc->info;
    Addpassagem(L,npass);
    }

    fclose(F);
    printf("\nImportacao concluida com sucesso!\n");

    return 1;
}



/**
 * \brief Importa dados de sensores a partir de um arquivo
 *
 * L� dados dos sensores do arquivo "sensores.txt" e adiciona-os � lista "Listasensores".
 * Cada linha deve conter: <c�digo> <designa��o> <latitude> <longitude>, separados por tabula��es.
 *
 * Linhas vazias s�o ignoradas.
 *
 * \param L Ponteiro para a lista de sensores
 * \return int 1 se a importa�ao teve sucesso (pelo menos um sensor foi importado), 0 caso contrario
 */
int importsensor(Listasensores *L) {
     setlocale(LC_ALL, "Portuguese");
    setlocale(LC_ALL, "pt_PT.UTF-8");
    pListasensor Ls = L;
    FILE* F = fopen("sensores.txt", "r");
    if (F == NULL) {
        printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
        return 0;
    }

    int COD;
    char NOME[100];
    char Lat[100];  // Aumentado para 17 (16 caracteres + \0)
    char Lon[100];  // Aumentado para 15 (14 caracteres + \0)

    char linha[256];
    while (fgets(linha, sizeof(linha), F)) {
        linha[strcspn(linha, "\n")] = '\0'; // Remove a quebra de linha

        if (strlen(linha) == 0) {
            continue; // Ignora linhas vazias
        }

        // Formato ajustado para ler Latitude (16 caracteres) e Longitude (14 caracteres)
        int camposLidos = sscanf(linha, "%d\t%[^\t]\t%[^\t]\t%[^\n]",&COD, NOME, Lat, Lon);

        if (camposLidos == 4) {
            printf("COD = %d, NOME: [%s], Lat=[%s], Lon=[%s]\n", COD, NOME, Lat, Lon);
            sensor* nsensor = (psensor)malloc(sizeof(sensor));
            nsensor->codSensor = COD;
            strcpy(nsensor->Designacao, NOME);
            strcpy(nsensor->Latitude, Lat);
            strcpy(nsensor->Longitude, Lon);
            Addsensor(Ls, nsensor);
        } else {
            printf("Formato invalido na linha: %s\n", linha);
        }
    }

    fclose(F);
    return 1;
}





//fun�oes para importar
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


/**
 * \brief Cria uma nova lista ligada de donos
 *
 * Cria e retorna uma nova lista ligada "Listadono" vazia, com contador a zero.
 *
 * \return pListadono Ponteiro para a nova lista de donos inicializada
 */
pListadono crialistadono() {

	pListadono L = (pListadono)malloc(sizeof(Listadono));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}

/**
 * \brief Cria uma nova lista ligada de carros
 *
 * Cria e retorna uma nova lista ligada "Listacarro" vazia, com contador a zero.
 *
 * \return pListacarro Ponteiro para a nova lista de carros
 */
pListacarro crialistacarro() {
	pListacarro L = (pListacarro)malloc(sizeof(Listacarro));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}

/**
 * \brief Cria uma nova lista ligada de sensores
 *
 * Cria e retorna uma nova lista ligada "Listasensor" vazia, com contador a zero.
 *
 * \return pListasensor Ponteiro para a nova lista de sensores
 */
pListasensor crialistasensores() {
	pListasensor L = (pListasensor)malloc(sizeof(Listasensor));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}


/**
 * \brief Cria uma nova lista ligada de passagens
 *
 * Cria e retorna uma nova lista ligada "Listapassagem" vazia, com contador a zero.
 *
 * \return pListapassagem Ponteiro para a nova lista de passagens
 */
pListapassagem crialistapasagem() {
	pListapassagem L = (pListapassagem)malloc(sizeof(Listapassagem));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}

/**
 * \brief Cria uma nova marca e insere-a na tabela HASHING
 *
 * Cria uma nova marca com nome e lista de carros associada, e insere-a no in�cio da tabela HASHING.
 *
 * \param has Ponteiro para a estrutura de hashing onde a nova marca ser� inserida
 * \param nome Nome da marca a ser criada
 * \return pmarca Ponteiro para a nova marca criada
 */
pmarca criamarca(HASHING *has ,char *nome) {
    pmarca nova = (pmarca)malloc(sizeof(marca));
    if (nova) {
        strcpy(nova->nome, nome);
        nova->Numcarrototal = 0;
        nova->NUmcarromarca = 0;
        nova->inf = crialistacarro(); // Lista de carros inicializada
        nova->prox = NULL;
    }
    pmarca temp = has->Inicio;
    has->Inicio = nova;
    nova->prox = temp;
    return nova;
}


/**
 * \brief Cria e inicializa uma nova estrutura HASHING
 *
 * Esta fun�ao aloca mem�ria para um hashing e define seu ponteiro de inicio como NULL.
 *
 * \return HASHING* Ponteiro para a nova tabela de hashing inicializada
 */
HASHING *CriarHASHING()
{
    HASHING *Has = (HASHING *)malloc(sizeof(HASHING));
    Has->Inicio = NULL;
    //Has->N_CHAVES = 0;
    //Has->Flag_Destruir = flag;
    return Has;
}


//fun�oes que criam listas
//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------


/**
 * \brief Adiciona um novo dono � lista ligada de donos
 *
 * Adiciona um novo dono a lista ligada "Listadono", inserindo-o no inicio da lista.
 *
 * \param l Ponteiro para a lista de donos
 * \param D Ponteiro para o dono a ser adicionado
 */
void AddDono(Listadono* l, dono* D)
{
	pno nNo = (pno)malloc(sizeof(no));
	nNo->info = D;
	nNo->prox = l->inicio;
	l->inicio = nNo;
	l->numel++;

}

/**
 * \brief Adiciona um novo carro � lista ligada de carros
 *
 * Adiciona um novo carro � lista ligada "Listacarro", inserindo-o no inicio da lista, se for valido.
 *
 * \param lista Ponteiro para a lista de carros
 * \param carro Ponteiro para o carro a ser adicionado
 */
void Addcarro(Listacarro* lista, pcarro carro) {
    if (!lista || !carro) return;

    pnocarro novo = (pnocarro)malloc(sizeof(nocarro));
    if (novo) {
        novo->info = carro;
        novo->prox = lista->inicio;
        lista->inicio = novo;
        lista->numel++;
    }
}

/**
 * \brief Adiciona um novo sensor � lista ligada de sensores
 *
 * Adiciona um novo sensor a lista ligada "Listasensor",adicionando-o no inicio da lista.
 *
 * \param l Ponteiro para a lista de sensores
 * \param D Ponteiro para o sensor a ser adicionado
 */
void Addsensor(Listasensor* l, sensor* D)
{
	pnosensor nNo = (pnosensor)malloc(sizeof(nosensor));
	nNo->info = D;
	nNo->prox = l->inicio;
	l->inicio = nNo;
	l->numel++;

}

/**
 * \brief Adiciona uma nova passagem � lista ligada de passagens
 *
 * Insere uma nova estrutura de passagem no in�cio da lista `Listapassagem`.
 *
 * \param l Ponteiro para a lista de passagens
 * \param D Ponteiro para a estrutura de passagem a ser adicionada
 */
void Addpassagem(Listapassagem* l, passagem* D)
{
	pnopassagem nNo = (pnopassagem)malloc(sizeof(nopassagem));
	nNo->info = D;
	nNo->prox = l->inicio;
	l->inicio = nNo;
	l->numel++;
}


//fun�oes que introduzem um item na sua lista
//----------------------------------------------------
//----------------------------------------------------
//-----------------------------------------------

/**
 * \brief Regista interativamente um novo dono
 *
 * Regista um novo dono interativamente e adiciona-o a lista ligada "Listadono".
 *
 * \param Ld Ponteiro para a lista de donos onde o novo dono ser� inserido
 */
void regist_dono(Listadono* Ld) {
	int opcao = 0;
	pListadono n;
	printf("Gostaria de adicionar mais um dono? 1-sim 0-nao\n");
	scanf("%d", &opcao);
	if (opcao == 1) {
		dono* ndono = (pdono)malloc(n->numel * sizeof(struct no));
		if (ndono == NULL) {
			printf("Ocorreu um erro ao adicionar o dono.\n");
			return;
		}

		printf("Qual o numero de contribuinte?\n");
		scanf("%d", &ndono->numcontibuinte);
		printf("Qual o nome do dono?\n");//nao tenho certeza se devo meter assim o nome temos de ver depois se est� bom
		scanf("%s", ndono->nome);
		printf("Qual o codigo postal?\n");
		scanf("%s", &(ndono->codPostal));
		AddDono(Ld, ndono);
		printf("dono adicionado com sucesso");
	}
	if (opcao == 0) {
		return;
	}
}


/**
 * \brief Lista todos os donos da estrutura Listadono
 *
 * ~Fun��o que apresenta todos os donos no ecr�.
 *
 * \param Ld Ponteiro para a lista de donos a ser listada
 */

void list_dono(Listadono* Ld) {
	int i = 0, skip = 0, continuar;
	pdono linfdono;
	pno ldono = Ld->inicio;
	while (ldono != NULL) {
		linfdono = ldono->info;
		printf("Codigo postal:%s\n", linfdono->codPostal);
		printf("Nome:%s\n", linfdono->nome);
		printf("numero de contribuinte:%d\n", linfdono->numcontibuinte);
		ldono = ldono->prox;
        i++;
			if( skip == 0 && i%100 == 0 ){
                printf("\nmore-1\nexit-2\nskip-3");
                scanf("%d",&continuar);
                if(continuar == 1 ){
                continue;
                }
                else if(continuar == 2){
                    return ;
                }
                else{
                    skip = 1;
                    continue;
                }
            }
	}
}


/**
 * \brief Regista um novo ve�culo e associa-o a um dono e marca
 *
 * Regista um novo veiculo interativamente e associa-o a um dono existente e marca. Pede a: matr�cula, NIF do dono, marca, modelo e ano. Insere no hashing.
 *
 * \param L Ponteiro para a lista de donos
 * \param has Ponteiro para a tabela hash das marcas
 */

void regist_veiculo(Listadono *L,HASHING *has) {
	int opcao = 0, contdono = 0;
	pListadono pL = L;
	pmarca m = has->Inicio;
	pListacarro Lc;
	printf("Gostaria de adicionar um veiculo? 1-Sim | 0-Nao:\n ");
	scanf("%d", &opcao);

	if (opcao == 1) {
		carro* novoCarro = (carro*)malloc(sizeof(carro));
		if (novoCarro == NULL) {
			printf("Ocorreu um erro ao adicionar o veiculo.\n");
			return;
		}

		printf("Insira a matricula do ve�culo:\n");
		scanf("%s", novoCarro->matricula);

		printf("Insira o numero de contribuinte do dono:\n");
		scanf("%d", &contdono);

		pno ldono = pL->inicio;
		while ((ldono != NULL )&&(ldono->info->numcontibuinte != contdono)  ){
			ldono = ldono->prox;
		}

		if (ldono == NULL) {
			printf("dono nao encontrado");
			free(novoCarro);
			return;
		}
		else {
			novoCarro->pdonos = ldono->info;
		}

		printf("Insira a marca do carro:\n");
		scanf("%s", novoCarro->marca);

		printf("Insira o modelo do carro:\n");
		scanf("%s", novoCarro->modelo);

		printf("Insira o ano de producao do carro:\n");
		scanf("%d", &novoCarro->ano);
		m->Numcarrototal++;
		int temp = m->Numcarrototal;
		while (m->prox != NULL && strcmp(novoCarro->marca, m->nome) != 0 ) {
			m->Numcarrototal;
			m = m->prox;
		}
		if (m != NULL && strcmp(novoCarro->marca, m->nome) == 0) {
			Lc = m->inf;
		}
		else {
			pmarca nmarca = criamarca(has, novoCarro->marca);
			m = m->prox;
			Lc = m->inf;
		}
		novoCarro->kilometros = 0;
		novoCarro->tempototal = 0;

		novoCarro->codigo = 0;//m->Numcarrototal;
		m->NUmcarromarca++;

		Addcarro(Lc, novoCarro);
		//m->inf = Lc;
		printf("Veiculo adicionado com sucesso!\n");
	}
	else {
		printf("Veiculo nao adicionado.\n");
		return;
	}
}



void list_veiculo(HASHING *has) {
	pmarca pm = has->Inicio;
	int i = 0, continuar, skip = 0;
	if (pm == NULL) {
		printf("Nenhum veiculo registado.\n");
		return;
	}
	/*
	while (pm != NULL)
    {
        printf("\n marca: [%s]", pm->nome);
        pm = pm->prox;
    }
    printf("\nFIM da lista de marcas...\n\n");
    getchar();
*/

	while (pm != NULL)
    {
		printf("Lista de veiculos da marca [%s]:\n",pm->nome);

        Listacarro* Lc = pm->inf;
        if(Lc)
        {
        pnocarro atual = Lc->inicio;
		printf("Lista de veiculos da marca [%s]:\n",pm->nome);
		while (atual != NULL) {
			carro* c = atual->info;
			printf("\n--------------------------\n");
			printf("Matricula: %s\n", c->matricula);
			printf("Contribuinte do Dono: %d\n", c->pdonos->numcontibuinte);
			printf("Marca: %s\n", c->marca);
			printf("Modelo: %s\n", c->modelo);
			printf("Ano: %d\n", c->ano);
			printf("Codigo: %d", c->codigo);
			atual = atual->prox;
			i++;
			if( skip == 0 && i%100 == 0 ){
                printf("\nmore-1\nexit-2\nskip-3");
                scanf("%d",&continuar);
                if(continuar == 1 ){
                continue;
                }
                else if(continuar == 2){
                    return ;
                }
                else{
                    skip = 1;
                    continue;
                }
            }
		}
        }

		//while(atual == NULL && pm!= NULL) {
       // printf("Nenhum ve�culo desta marca registado.\n");
		//}


		printf("\n-------------------------------------------------------\n");
		pm = pm->prox;
	}
}

//Lista todos os veiculos registados, organizados por marca.


void regist_pass(Listapassagem* Lp, HASHING *has, Listasensor* Ls) {
	passagem* novaPassagem = (passagem*)malloc(sizeof(passagem));
	if (novaPassagem == NULL) {
		printf("Erro ao alocar memoria para a passagem.\n");
		return;
	}

	char matricula[10];
	int codigoSensor;
	float distancia;
	char dataHora[20];
	char marcad[50];
    pmarca pm = has->Inicio;
    printf("Qual a marca do carro?\n");
    scanf("%s",marcad);

    while( pm != NULL && strcmp(marcad,pm->nome ) != 0){
        pm = pm->prox;
    }
    	if (pm == NULL) {
		printf("marca nao encontrado.\n");
		free(novaPassagem);
		return;
	}
        pListacarro Lc = pm->inf;
        printf("Insira a matricula do carro:\n");
        scanf("%s", matricula);
        pnocarro atualCarro = Lc->inicio;
        while (atualCarro != NULL && strcmp(atualCarro->info->matricula, matricula) != 0) {
		atualCarro = atualCarro->prox;
        } // isto vai buscar o carro plea matricula na lista

	if (atualCarro == NULL) {
		printf("Carro nao encontrado.\n");
		free(novaPassagem);
		return;
	} //senao for encontrado diz q n foi encontrado e da return

	novaPassagem->codcarro = atualCarro->info;

	printf("Insira o codigo do sensor:\n");
	scanf("%d", &codigoSensor);

/*	pnosensor atualSensor = Ls->inicio;
	while (atualSensor != NULL && atualSensor->info->codSensor != codigoSensor) {
		atualSensor = atualSensor->prox;
	} //procuyra o sensor pelo codigo

	if (atualSensor == NULL) {
		printf("Sensor n�o encontrado.\n");
		free(novaPassagem);
		return;
	} //se o sensor n for encontrado avisa e retorna*/

	novaPassagem->idsensor = codigoSensor;

	printf("Insira a data e hora da passagem (formato: DD-MM-AAAA HH:MM:SS):\n"); //pede data de passagem
	scanf("%s", dataHora);
	strcpy(novaPassagem->data, dataHora);

	Addpassagem(Lp, novaPassagem);
	printf("Passagem registada com sucesso!\n"); //adicona a passagem e avisa
}
//Regista uma nova passagem de veiculo. Solicita a marca, matricula, ID so sensor, data e hora, e adiciona a lista passagem.

/**
 * \brief Organiza a lista de donos por nome ou n�mero de contribuinte
 *
 *
 * \param Ld Ponteiro para a lista ligada de donos
 */
void organizadonos(Listadono* Ld) {
	if (Ld->inicio == NULL || Ld->inicio->prox == NULL) {
		printf("Poucos ou nenhum dono registrado para ordenar.\n");
		return;
	}
	int trocado, opcao;
	pdono temp;
	pno atual, max = NULL;

    printf("Deseja executar a organizacao por que ordem?\n");
    printf("1. Nome do dono\n");
    printf("2. Numero de contribuinte\n");
    printf("3. Parar organizacao\n");
    printf("Escolha uma opcao: ");
    scanf( "%d", &opcao);

	do {
		trocado = 0;
		atual = Ld->inicio;
		temp = NULL;




		while (atual->prox != NULL) {
            int precisaTrocar = 0;
            if (opcao == 3)
                return;
            else if (opcao == 1 && strcmp(atual->info->nome, atual->prox->info->nome) > 0){
                precisaTrocar = 1;
            }
            else if (opcao == 2 && atual->info->numcontibuinte > atual->prox->info->numcontibuinte){
                precisaTrocar = 1;
            }

                if( precisaTrocar == 1){
                    temp = atual->prox->info;
                    atual->prox->info = atual->info;
                    atual->info = temp;
                    trocado = 1;
                }
           atual = atual->prox;
            }
		max = atual;
	} while (trocado == 1);

}

/**
 * \brief Importa dados de donos, carros e passagens.
 *
 * Esta fun��o interage com o utilizador para importar dados de:
 * - Donos
 * - Carros (dependente dos donos)
 * - Passagens (dependente dos carros)
* 
 *
 * As fun�oes especificas de importa�ao devem estar implementadas:
 * - importdono()
 * - importcarro()
 * - importpassagem()
 * Sensores e dist�ncias n�o s�o importados aqui.
 *
 * \param Ld Ponteiro para a lista de donos.
 * \param has Ponteiro para a tabela hash de marcas e carros.
 * \param Lp Ponteiro para a lista de passagens.
 */
void import(Listadono *Ld, HASHING *has, Listapassagem *Lp) {
		int opcao = 0;

		printf("Deseja importar donos?\n1-Sim\n2-Nao\n");
		scanf("%d", &opcao);

		if (opcao == 1) {
			importdono(Ld);
			printf("deseja importar carros?\n1-Sim\n2-Nao\n");
			scanf("%d", &opcao);

			if (opcao == 1) {
				importcarro(Ld,has);
				printf("deseja importar passagens?\n1-Sim\n2-Nao\n");
				scanf("%d", &opcao);

				if (opcao == 1) {
					importpassagem(Lp,has);
				}
			}
		}
}
/**
 * \brief Organiza os carros por matr�cula, marca ou modelo.
 *
 * Esta fun��o permite ao utilizador escolher um crit�rio para organizar os carros:
 * - Op��o 1: Por matr�cula (ordem alfab�tica global, usando uma lista temporaria)
 * - Op��o 2: Por marca (ordem alfab�tica das marcas na tabela hash)
 * - Op��o 3: Por modelo (ordem alfab�tica global, usando uma lista tempor�ria)
 * - Op��o 4: Terminar (n�o realiza nenhuma organiza��o)
 *
 *
 * \param has Ponteiro para a tabela hash contendo marcas e listas de carros.
 */
void organizacarros(HASHING *has) {
	pmarca nm = has->Inicio;
	pListacarro tempL;
	pmarca tempmarca;
	pListacarro Lc;
	pnocarro nc;
	pcarro tempc;
	char tempn[20];
	int t;
	int troca = 0;
	int opcao;

	printf("Deseja executar a organizacao por que ordem?\n");
    printf("1. Matricula\n");
    printf("2. Marca\n");
    printf("3. Modelo\n");
    printf("4. Parar organizacao\n");
    printf("Escolha uma opcao: ");
	scanf("%d",&opcao);

	if (opcao == 1) {
	    nm = has->Inicio;
        if(nm == NULL){
            printf("\n HASH esta a NULL????");
            return;
        }
        pListacarro tempL = crialistacarro();
        while(nm != NULL){
            Lc = nm->inf;
            nc = Lc->inicio;
            while(nc != NULL){
                pcarro novocarro = nc->info;
                Addcarro(tempL,novocarro);
                nc = nc->prox;
            }
            nm = nm->prox;
        }

        do{
            troca = 0;
            nc = tempL->inicio;
            while(nc->prox != NULL){
              if(strcmp(nc->info->matricula,nc->prox->info->matricula)>0){
                // printf("\n%s\n\n",nc->prox->info->matricula);
                 pcarro tcarro = nc->info;
                 nc->info = nc->prox->info;
                 nc->prox->info =  tcarro;
                // printf("\n%s\n",nc->prox->info->matricula);
                 troca = 1;

              }
                nc = nc->prox;
            }


        }while(troca ==1 );

        nc = tempL->inicio;
        while(nc != NULL){
            printf("\n--------------------------\n");
			printf("Matricula: %s\n", nc->info->matricula);
			printf("Contribuinte do Dono: %d\n", nc->info->pdonos->numcontibuinte);
			printf("Marca: %s\n", nc->info->marca);
			printf("Modelo: %s\n", nc->info->modelo);
			printf("Ano: %d\n", nc->info->ano);
			printf("Codigo: %d", nc->info->codigo);
            nc = nc->prox;
        }
	}
	else if (opcao == 2){

		do {
        nm = has->Inicio;
        if(nm == NULL){
                printf("\n HASH esta a NUL????");
            return;
        }

			troca = 0;
			while (nm->prox != NULL) {
			if(strcmp(nm->nome,nm->prox->nome)>0){
                tempL = nm->inf;
                nm->inf = nm->prox->inf;
                nm->prox->inf = tempL;

                strcpy(tempn,nm->nome);
                strcpy(nm->nome,nm->prox->nome);
                strcpy(nm->prox->nome, tempn);

                t =nm->NUmcarromarca;
                nm->NUmcarromarca = nm->prox->NUmcarromarca;
                nm->prox->NUmcarromarca = t;
                t =nm->Numcarrototal;
                nm->Numcarrototal = nm->prox->Numcarrototal;
                nm->prox->Numcarrototal = t;
                t =nm->numkillmarca;
                nm->numkillmarca = nm->prox->numkillmarca;
                nm->prox->numkillmarca = t;
                troca = 1;

			}
            nm = nm->prox;
            }

		} while (troca == 1);
	}
	else if (opcao == 3){
nm = has->Inicio;
        if(nm == NULL){
            printf("\n HASH esta a NULL????");
            return;
        }
        pListacarro tempL = crialistacarro();
        while(nm != NULL){
            Lc = nm->inf;
            nc = Lc->inicio;
            while(nc != NULL){
                pcarro novocarro = nc->info;
                Addcarro(tempL,novocarro);
                nc = nc->prox;
            }
            nm = nm->prox;
        }

        do{
            troca = 0;
            nc = tempL->inicio;
            while(nc->prox != NULL){
              if(strcmp(nc->info->modelo,nc->prox->info->modelo)>0){
                // printf("\n%s\n\n",nc->prox->info->matricula);
                 pcarro tcarro = nc->info;
                 nc->info = nc->prox->info;
                 nc->prox->info =  tcarro;
                // printf("\n%s\n",nc->prox->info->matricula);
                 troca = 1;

              }
                nc = nc->prox;
            }


        }while(troca ==1 );

        nc = tempL->inicio;
        while(nc != NULL){
            printf("\n--------------------------\n");
			printf("Matricula: %s\n", nc->info->matricula);
			printf("Contribuinte do Dono: %d\n", nc->info->pdonos->numcontibuinte);
			printf("Marca: %s\n", nc->info->marca);
			printf("Modelo: %s\n", nc->info->modelo);
			printf("Ano: %d\n", nc->info->ano);
			printf("Codigo: %d", nc->info->codigo);
            nc = nc->prox;
        }
	}
	else if (opcao == 4){
		return ;
	}

}

//Lista veiculos que circularam num periodo de tempo especifico, ordenados por matricula.

/**
 * \brief Lista ve�culos que circularam num dado periodo
 *
 * Solicita ao utilizador a data e hora inicial e final, e percorre a lista de passagens para identificar os ve�culos distintos que circularam nesse intervalo.
 * Os ve�culos encontrados s�o apresentados por ordem alfab�tica da matricula.
 *
 * A fun��o ignora ve�culos duplicados e exibe todos os dados relevantes do carro e do seu dono.
 *
 * \param pass Ponteiro para a lista de passagens a ser analisada
 */
void listacarroperiodo(pListapassagem pass) {
    int opcao;
    printf("\nDeseja listar os carros que circularam num periodo?\n1 - Sim\n2 - Nao\n");
    scanf("%d", &opcao);

    if (opcao != 1) return;

    char horainicio[30], horafim[30];
    printf("Qual a data inicial (formato: AAAA-MM-DD_HH:MM:SS): ");
    scanf("%s", horainicio);
    printf("Qual a data final (formato: AAAA-MM-DD_HH:MM:SS): ");
    scanf("%s", horafim);

    // Parsing datas com strtok
    char ini[30], fim[30];
    strcpy(ini, horainicio);
    strcpy(fim, horafim);

    char *anoi = strtok(ini, "-");
    char *mesi = strtok(NULL, "-");
    char *diai = strtok(NULL, "_");
    char *horai = strtok(NULL, ":");
    char *mini = strtok(NULL, ":");
    char *segi = strtok(NULL, ":");

    char *anof = strtok(fim, "-");
    char *mesf = strtok(NULL, "-");
    char *diaf = strtok(NULL, "_");
    char *horaf = strtok(NULL, ":");
    char *minf = strtok(NULL, ":");
    char *segf = strtok(NULL, ":");

    float tempo_inicio = calctempo(anoi, mesi, diai, horai, mini, segi);
    float tempo_fim = calctempo(anof, mesf, diaf, horaf, minf, segf);

    // Guardar carros distintos que passaram no per�odo
    pcarro carros[1000];
    int count = 0;

    pnopassagem p = pass->inicio;
    while (p != NULL) {
        char copia[30];
        strcpy(copia, p->info->data);
        char *dia = strtok(copia, "-");
        char *mes = strtok(NULL, "-");
        char *ano = strtok(NULL, " ");
        char *hora = strtok(NULL, ":");
        char *min = strtok(NULL, ":");
        char *seg = strtok(NULL, ".");

        float tempo_passagem = calctempo(ano, mes, dia, hora, min, seg);

        if (tempo_passagem >= tempo_inicio && tempo_passagem <= tempo_fim) {
            // Verifica duplicados
            int duplicado = 0;
            for (int i = 0; i < count; i++) {
                if (carros[i] == p->info->codcarro) {
                    duplicado = 1;
                    break;
                }
            }
            if (!duplicado && count < 1000) {
                carros[count++] = p->info->codcarro;
            }
        }
        p = p->prox;
    }

    // Ordenar por matr�cula
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(carros[i]->matricula, carros[j]->matricula) > 0) {
                pcarro temp = carros[i];
                carros[i] = carros[j];
                carros[j] = temp;
            }
        }
    }

    // Imprimir resultados
    printf("\n=== Veiculos que circularam no periodo ===\n");
    if (count == 0) {
        printf("Nenhum veiculo encontrado no periodo indicado.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("\n--------------------------\n");
            printf("Matricula: %s\n", carros[i]->matricula);
            printf("Contribuinte do Dono: %d\n", carros[i]->pdonos->numcontibuinte);
            printf("Marca: %s\n", carros[i]->marca);
            printf("Modelo: %s\n", carros[i]->modelo);
            printf("Ano: %d\n", carros[i]->ano);
            printf("Codigo: %d\n", carros[i]->codigo);
        }
    }
}

/*Ranking de circula��o. Listagem ordenada pelo total de quil�metros
    que cada ve�culo efectuou na auto-estrada durante determinado per�odo. */

/**
 * \brief Gera um ranking de ve�culos por quilometragem em um per�odo
 *
 * Solicita ao utilizador um intervalo de datas e horas e filtra todas as passagens de ve�culos que ocorreram nesse intervalo. Com base nessas passagens e na matriz de dist�ncias entre sensores, calcula a quilometragem percorrida por cada ve�culo.
 *
 * Em seguida, ordena os ve�culos com base na dist�ncia total percorrida e exibe um ranking com os dados principais de cada um.
 *
 * \param pass Ponteiro para a lista de passagens registadas
 * \param d Ponteiro para a estrutura de dist�ncias entre sensores
 * \param has Ponteiro para a tabela hash com todas as marcas e respetivos carros
 */
void rankveiculos(Listapassagem *pass,distancia *d, HASHING *has) {
	int opcao, troca;
	pListacarro Lc;
	pnocarro nc;
	pmarca nm = has->Inicio;

	printf("Deseja rankear os carros que circularam num periodo pelos kilometros?\n1-Sim\n2-n�o");
	scanf("%d", &opcao);

	if (opcao == 1) {
		int s1, s2;
		pdistancia td= d;
		pListapassagem ppass = pass;
		pListapassagem pLpass = crialistapasagem();
		//pnopassagem pnLpass = (pnopassagem)malloc(sizeof(nopassagem));
		pnopassagem pnLpass ;
        //pnLpass = NULL;
		pnopassagem pnpass = ppass->inicio;
		char horainicio[50];
		char horafim[50];


		printf("entre que datas os carros circularam");
		printf("Qual a data inicial(formato: AAAA-MM-DD_HH:MM:SS)\n");
		scanf("%s",horainicio);
		printf("Qual a data final(formato: AAAA-MM-DD_HH:MM:SS)\n");
		scanf("%s",horafim);

        char *anoi = strtok(horainicio,"-");
       char *mesi = strtok(NULL, "-");
       char *diai = strtok(NULL, "_");
       char *horai =strtok(NULL, ":");
       char *mini = strtok(NULL,":");
       char *segundoi = strtok(NULL, ":");
       char *anof = strtok(horafim,"-");
       char *mesf = strtok(NULL, "-");
       char *diaf = strtok(NULL, "_");
       char *horaf =strtok(NULL, ":");
       char *minf = strtok(NULL,":");
       char *segundof = strtok(NULL, ":");
       float tempoi = calctempo(anoi,mesi,diai,horai,mini,segundoi);
       float tempof = calctempo(anof,mesf,diaf,horaf,minf,segundof);

		while (pnpass != NULL) {
            char copia[30];
            strcpy(copia,pnpass->info->data);
            char *diaa = strtok(copia,"-");
            char *mesa = strtok(NULL, "-");
            char *anoa = strtok(NULL, " ");
            char *horaa =strtok(NULL, ":");
            char *mina = strtok(NULL,":");
            char *segundoa = strtok(NULL, ":");
            //printf("\ndata convertida[%s][%s][%s][%s][%s][%s]",anoa,mesa,diaa,horaa,mina,segundoa);
            float tempoa = calctempo(anoa,mesa,diaa,horaa,mina,segundoa);
           // printf("tempoi : %f\t",tempoi);
            //printf("tempoa : %f\t",tempoa);
           // printf("tempof : %f\n",tempof);
			if (tempoi<tempoa && tempof>tempoa) { // esta passagem est� no per�odo
			    pnLpass = (pnopassagem)malloc(sizeof(nopassagem));
				pnLpass->info = pnpass->info;
                pnLpass->prox = pLpass->inicio;
                pLpass->inicio = pnLpass;
				pLpass->numel= pLpass->numel + 1;
			}
			pnpass = pnpass->prox;

		}
printf("\na lista nova tem %d passagens\n",pLpass->numel);
		pnLpass = pLpass->inicio;

		while(pnLpass != NULL){
            pnopassagem temp = pnLpass;
            pnopassagem temp2 = pnLpass;
            if(pnLpass->info->codcarro->kilometros == 0){
                while(temp2 != NULL){
                   // printf("\ntemp1: %d",temp->info->tiporegist);
                   // printf("\ntemp2: %d",temp2->info->tiporegist);
                        if(temp2->info->codcarro->codigo == temp->info->codcarro->codigo){

                            if(temp->info->tiporegist == 0){

                                temp->info->codcarro->kilometros = temp->info->codcarro->kilometros + d->dist[temp->info->idsensor][temp2->info->idsensor];
                               // printf("o kilometro � %d",temp->info->codcarro->codigo);
                            }
                            temp = temp2;
                        }
                    temp2 =temp2->prox;
                }
		    }
            pnLpass = pnLpass->prox;
		}
        pListacarro tempL = crialistacarro();
        while(nm != NULL){
            Lc = nm->inf;
            nc = Lc->inicio;
            while(nc != NULL){
                if(nc->info->kilometros != 0){
                pcarro novocarro = nc->info;
                Addcarro(tempL,novocarro);
                }
                nc = nc->prox;
            }
            nm = nm->prox;
        }

        do{
            troca = 0;
            nc = tempL->inicio;
            while(nc->prox != NULL){
              if(nc->info->kilometros < nc->prox->info->kilometros){
                // printf("\n%s\n\n",nc->prox->info->matricula);
                 pcarro tcarro = nc->info;
                 nc->info = nc->prox->info;
                 nc->prox->info =  tcarro;
                // printf("\n%s\n",nc->prox->info->matricula);
                 troca = 1;

              }
                nc = nc->prox;
            }


        }while(troca ==1 );

        nc = tempL->inicio;
        while(nc != NULL){
            printf("\n--------------------------\n");
            printf("\nkilometros: %f\n",nc->info->kilometros);
			printf("Matricula: %s\n", nc->info->matricula);
			printf("Contribuinte do Dono: %d\n", nc->info->pdonos->numcontibuinte);
			printf("Marca: %s\n", nc->info->marca);
			printf("Modelo: %s\n", nc->info->modelo);
			printf("Ano: %d\n", nc->info->ano);
			printf("Codigo: %d", nc->info->codigo);
            nc = nc->prox;
        }




	}
}


/*Ranking por marca. Listagem ordenada pelo total de quil�metros que
os ve�culos das diferentes marcas efectuaram na autoestrada durante determinado per�odo. */

/**
 * @brief Gera um ranking das marcas de ve�culos com base nos quilometros percorridos numa autoestrada durante um periodo de tempo especificado.
 *
 * A fun��o pergunta ao utilizador se pretende visualizar o ranking de marcas por quil�metros e, caso afirmativo, solicita um intervalo de datas. 
 * Em seguida, filtra todas as passagens nesse per�odo e calcula os quilmetros percorridos por cada veiculo. 
 * Os valores s�o agregados por marca, e um ranking � exibido por ordem decrescente de quil�metros.
 *
 * @param pass Ponteiro para a lista de passagens registadas.
 * @param d Ponteiro para a estrutura de dist�ncias entre sensores.
 * @param has Ponteiro para a tabela hash que armazena as marcas e os seus ve�culos.
 * 
 * @return Esta fun��o n�o retorna nenhum valor.
 */
void rankmarcas(Listapassagem *pass, distancia *d, HASHING *has) {
    int opcao, troca;
	pListacarro Lc;
	pnocarro nc;
	pmarca nm = has->Inicio;

	printf("Deseja rankear os carros que circularam num periodo pelos kilometros?\n1-Sim\n2-n�o");
	scanf("%d", &opcao);

	if (opcao == 1) {
		int s1, s2;
		pdistancia td= d;
		pListapassagem ppass = pass;
		pListapassagem pLpass = crialistapasagem();
		//pnopassagem pnLpass = (pnopassagem)malloc(sizeof(nopassagem));
		pnopassagem pnLpass ;
        //pnLpass = NULL;
		pnopassagem pnpass = ppass->inicio;
		char horainicio[30];
		char horafim[30];

		printf("entre que datas os carros circularam");
		printf("Qual a data inicial(formato: AAAA-MM-DD_HH:MM:SS)\n");
		scanf("%s", horainicio);
		printf("Qual a data final(formato: AAAA-MM-DD_HH:MM:SS)\n");
		scanf("%s", horafim);

        char *anoi = strtok(horainicio,"-");
       char *mesi = strtok(NULL, "-");
       char *diai = strtok(NULL, "_");
       char *horai =strtok(NULL, ":");
       char *mini = strtok(NULL,":");
       char *segundoi = strtok(NULL, ":");
       char *anof = strtok(horafim,"-");
       char *mesf = strtok(NULL, "-");
       char *diaf = strtok(NULL, "_");
       char *horaf =strtok(NULL, ":");
       char *minf = strtok(NULL,":");
       char *segundof = strtok(NULL, ":");
       float tempoi = calctempo(anoi,mesi,diai,horai,mini,segundoi);
       float tempof = calctempo(anof,mesf,diaf,horaf,minf,segundof);

		while (pnpass != NULL) {
            char copia[30];
            strcpy(copia,pnpass->info->data);
            char *diaa = strtok(copia,"-");
            char *mesa = strtok(NULL, "-");
            char *anoa = strtok(NULL, " ");
            char *horaa =strtok(NULL, ":");
            char *mina = strtok(NULL,":");
            char *segundoa = strtok(NULL, ":");
            //printf("\ndata convertida[%s][%s][%s][%s][%s][%s]",anoa,mesa,diaa,horaa,mina,segundoa);
            float tempoa = calctempo(anoa,mesa,diaa,horaa,mina,segundoa);
           // printf("tempoi : %f\t",tempoi);
            //printf("tempoa : %f\t",tempoa);
           // printf("tempof : %f\n",tempof);
			if (tempoi<tempoa && tempof>tempoa) { // esta passagem est� no per�odo
			    pnLpass = (pnopassagem)malloc(sizeof(nopassagem));
				pnLpass->info = pnpass->info;
                pnLpass->prox = pLpass->inicio;
                pLpass->inicio = pnLpass;
				pLpass->numel= pLpass->numel + 1;
			}
			pnpass = pnpass->prox;

		}
printf("\na lista nova tem %d passagens\n",pLpass->numel);
		pnLpass = pLpass->inicio;

		while(pnLpass != NULL){
            pnopassagem temp = pnLpass;
            pnopassagem temp2 = pnLpass;
            if(pnLpass->info->codcarro->kilometros == 0){
                while(temp2 != NULL){
                   // printf("\ntemp1: %d",temp->info->tiporegist);
                   // printf("\ntemp2: %d",temp2->info->tiporegist);
                        if(temp2->info->codcarro->codigo == temp->info->codcarro->codigo){

                            if(temp->info->tiporegist == 0){

                                temp->info->codcarro->kilometros = temp->info->codcarro->kilometros + d->dist[temp->info->idsensor][temp2->info->idsensor];
                               // printf("o kilometro � %d",temp->info->codcarro->codigo);
                            }
                            temp = temp2;
                        }
                    temp2 =temp2->prox;
                }
		    }
            pnLpass = pnLpass->prox;
		}
        pListacarro tempL = crialistacarro();
        while(nm != NULL){
            Lc = nm->inf;
            nc = Lc->inicio;
            while(nc != NULL){
                if(nc->info->kilometros != 0){
                nm->numkillmarca = nm->numkillmarca + nc->info->kilometros;
                }
                nc = nc->prox;
            }
            nm = nm->prox;
        }



    do{
        troca = 0;
        nm = has->Inicio;
        pmarca maior = has->Inicio;
        if(nm == NULL){
            printf("erro ao procurar marca");
        }

        while(nm !=  NULL){
            if(maior->numkillmarca<nm->numkillmarca){
                maior = nm;
            }

            nm = nm->prox;
        }
        if(maior->numkillmarca == 0){
            return ;
        }
        else if(maior->numkillmarca != 0){
            printf("kilometros:%d",maior->numkillmarca);
            printf("marca:%s\n",maior->nome);
            maior->numkillmarca = 0;
            troca = 1;
            }
    }while(troca == 1);




	}
}

/**
 * @brief Lista os ve�culos que cometeram infra��es de velocidade num determinado per�odo.
 *
 * Ppercorre a lista de passagens para identificar pares de entradas e sa�das dos mesmos ve�culos e calcula a velocidade m�dia entre os sensores.
 * Caso a velocidade m�dia ultrapasse os 120 km/h e o evento ocorra dentro do per�odo definido pelo utilizador, a infra��o � reportada no ecr�.
 *
 * @param pass Ponteiro para a lista de passagens registadas.
 * @param d Ponteiro para a estrutura que cont�m as dist�ncias entre os sensores.
 * @param has Ponteiro para a tabela hash que cont�m as marcas e os seus ve�culos.
 * 
 * 
 * @return Esta fun��o n�o retorna um valor, mas imprime no ecr� as infra��es encontradas e respetiva informa��o detalhada.
 */
void listainfracao(pListapassagem pass, distancia* d, HASHING* has) {
    if (pass == NULL || d == NULL || has == NULL) {
        printf("Erro: Dados inv�lidos.\n");
        return;
    }

    char input_inicio[30], input_fim[30];

    printf("\n--- Listagem de Infracoes ---\n");
    fflush(stdout);

    printf("Periodo a analisar (formato AAAA-MM-DD_HH:MM:SS)\n");
    printf("Data/hora inicial: ");
    scanf("%s", input_inicio);
    printf("Data/hora final: ");
    scanf("%s", input_fim);

    // Preparar datas
    char inicio_parse[30], fim_parse[30];
    strcpy(inicio_parse, input_inicio);
    strcpy(fim_parse, input_fim);

    char* anoi = strtok(inicio_parse, "-");
    char* mesi = strtok(NULL, "-");
    char* diai = strtok(NULL, "_");
    char* horai = strtok(NULL, ":");
    char* mini = strtok(NULL, ":");
    char* segi = strtok(NULL, ":");

    char* anof = strtok(fim_parse, "-");
    char* mesf = strtok(NULL, "-");
    char* diaf = strtok(NULL, "_");
    char* horaf = strtok(NULL, ":");
    char* minf = strtok(NULL, ":");
    char* segf = strtok(NULL, ":");

    float tempo_inicio = calctempo(anoi, mesi, diai, horai, mini, segi);
    float tempo_fim = calctempo(anof, mesf, diaf, horaf, minf, segf);

    printf("\nVeiculos com velocidade media > 120 km/h:\n");
    printf("-------------------------------------------------\n");

    pnopassagem atual = pass->inicio;
    int count = 0;

    while (atual != NULL && atual->info != NULL) {
        if (atual->info->tiporegist != 0) {
            atual = atual->prox;
            continue;
        }

        passagem* entrada = atual->info;

        //Corrigir o ponteiro codcarro se estiver NULL
        if (entrada->codcarro == NULL) {
            marca* m = has->Inicio;
            while (m != NULL && entrada->codcarro == NULL) {
                pnocarro c = m->inf->inicio;
                while (c != NULL) {
                    if (c->info->codigo == entrada->tiporegist) {
                        entrada->codcarro = c->info;
                        break;
                    }
                    c = c->prox;
                }
                m = m->prox;
            }
        }

        pnopassagem prox = atual->prox;
        while (prox != NULL && (
            prox->info == NULL || prox->info->tiporegist != 1)) {
            prox = prox->prox;
        }

        if (prox != NULL && prox->info != NULL) {
            passagem* saida = prox->info;

            // Corrigir codcarro na sa�da tamb�m
            if (saida->codcarro == NULL) {
                marca* m = has->Inicio;
                while (m != NULL && saida->codcarro == NULL) {
                    pnocarro c = m->inf->inicio;
                    while (c != NULL) {
                        if (c->info->codigo == saida->tiporegist) {
                            saida->codcarro = c->info;
                            break;
                        }
                        c = c->prox;
                    }
                    m = m->prox;
                }
            }

            // Validar antes de aceder
            if (entrada->codcarro && saida->codcarro &&
                entrada->codcarro->codigo == saida->codcarro->codigo) {

                // Copiar datas para n�o corromper
                char d1[30], d2[30];
                strcpy(d1, entrada->data);
                strcpy(d2, saida->data);

                char *ae = strtok(d1, "-");
                char *me = strtok(NULL, "-");
                char *de = strtok(NULL, "_");
                char *he = strtok(NULL, ":");
                char *mie = strtok(NULL, ":");
                char *se = strtok(NULL, ":");

                char *as = strtok(d2, "-");
                char *ms = strtok(NULL, "-");
                char *ds = strtok(NULL, "_");
                char *hs = strtok(NULL, ":");
                char *mis = strtok(NULL, ":");
                char *ss = strtok(NULL, ":");

                float t1 = calctempo(ae, me, de, he, mie, se);
                float t2 = calctempo(as, ms, ds, hs, mis, ss);

                if (t1 >= tempo_inicio && t2 <= tempo_fim && t2 > t1) {
                    float tempo_total = t2 - t1;
                    float dist = d->dist[entrada->idsensor][saida->idsensor];
                    float vel = dist / tempo_total;

                    if (vel > 120.0) {
                        printf("Matricula: %s\n", entrada->codcarro->matricula);
                        printf("Marca: %s\n", entrada->codcarro->marca);
                        printf("Modelo: %s\n", entrada->codcarro->modelo);
                        printf("Velocidade media: %.2f km/h\n", vel);
                        printf("Entrada: %s\n", entrada->data);
                        printf("Saida: %s\n", saida->data);
                        printf("-------------------------------------------------\n");
                        count++;
                    }
                }
            }
        }

        atual = atual->prox;
    }

    if (count == 0) {
        printf("Nenhuma infracao encontrada no periodo especificado.\n");
    } else {
        printf("Total de infracoes: %d\n", count);
    }
}

/**
 * @brief Gera um ranking dos ve�culos com mais infra��es de velocidade num dado per�odo.
 *
 * Esta fun��o percorre a lista de passagens, identificando pares de entrada e sa�da para os mesmos ve�culos.
 * Para cada par, calcula a velocidade m�dia. 
 * Se for superior a 120 km/h, conta como uma infra��o. No final, apresenta os ve�culos ordenados pelo n�mero de infra��es.
 *
 * @param pass Ponteiro para a lista de passagens registadas.
 * @param d Ponteiro para a estrutura que cont�m as dist�ncias entre sensores.
 *
 * @return Esta fun��o n�o retorna valores, apenas imprime o ranking no ecr�.
 */

void rankinfracao(pListapassagem pass, distancia* d) {
	if (!pass || !d) {
        printf("Erro: dados invalidos.\n");
        return;
    }

    char input_inicio[30], input_fim[30];
    printf("\n--- Ranking de Infracoes por Veiculo ---\n");
    printf("Periodo a analisar (formato AAAA-MM-DD_HH:MM:SS)\n");
    printf("Data/hora inicial: ");
    scanf("%s", input_inicio);
    printf("Data/hora final: ");
    scanf("%s", input_fim);

    // Separar partes da data de in�cio
    char ini[30], fim[30];
    strcpy(ini, input_inicio);
    strcpy(fim, input_fim);

    char *anoi = strtok(ini, "-");
    char *mesi = strtok(NULL, "-");
    char *diai = strtok(NULL, "_");
    char *horai = strtok(NULL, ":");
    char *mini = strtok(NULL, ":");
    char *segi = strtok(NULL, ":");

    char *anof = strtok(fim, "-");
    char *mesf = strtok(NULL, "-");
    char *diaf = strtok(NULL, "_");
    char *horaf = strtok(NULL, ":");
    char *minf = strtok(NULL, ":");
    char *segf = strtok(NULL, ":");

    float tempo_inicio = calctempo(anoi, mesi, diai, horai, mini, segi);
    float tempo_fim = calctempo(anof, mesf, diaf, horaf, minf, segf);

    pcarro carros[1000];
    int infracoes[1000];
    int total = 0;

    pnopassagem atual = pass->inicio;

    while (atual != NULL) {
        if (!atual->info || atual->info->tiporegist != 0 || !atual->info->codcarro) {
            atual = atual->prox;
            continue;
        }

        passagem* entrada = atual->info;

        pnopassagem p = atual->prox;
        while (p != NULL) {
            if (p->info && p->info->tiporegist == 1 &&
                p->info->codcarro == entrada->codcarro) {

                // Parse entrada
                char d1[30]; strcpy(d1, entrada->data);
                char *diae = strtok(d1, "-");
                char *mese = strtok(NULL, "-");
                char *anoe = strtok(NULL, " ");
                char *horae = strtok(NULL, ":");
                char *mine = strtok(NULL, ":");
                char *sege = strtok(NULL, ".");

                float t1 = calctempo(anoe, mese, diae, horae, mine, sege);

                // Parse saida
                char d2[30]; strcpy(d2, p->info->data);
                char *dias = strtok(d2, "-");
                char *mess = strtok(NULL, "-");
                char *anos = strtok(NULL, " ");
                char *horas = strtok(NULL, ":");
                char *mins = strtok(NULL, ":");
                char *segs = strtok(NULL, ".");

                float t2 = calctempo(anos, mess, dias, horas, mins, segs);

                if (t1 >= tempo_inicio && t2 <= tempo_fim && t2 > t1) {
                    float tempo_total = t2 - t1; // j� em horas
                    int s1 = entrada->idsensor;
                    int s2 = p->info->idsensor;

                    float dist = d->dist[s1][s2];
                    if (dist == 0) dist = d->dist[s2][s1];

                    if (dist > 0) {
                        float velocidade = dist / tempo_total;

                        if (velocidade > 120.0) {
                            int encontrado = 0;
                            for (int i = 0; i < total; i++) {
                                if (carros[i] == entrada->codcarro) {
                                    infracoes[i]++;
                                    encontrado = 1;
                                    break;
                                }
                            }
                            if (!encontrado && total < 1000) {
                                carros[total] = entrada->codcarro;
                                infracoes[total] = 1;
                                total++;
                            }
                        }
                    }
                }

                break; // para no primeiro par entrada/saida v�lido
            }

            p = p->prox;
        }

        atual = atual->prox;
    }

    // Ordenar o ranking
    for (int i = 0; i < total - 1; i++) {
        for (int j = i + 1; j < total; j++) {
            if (infracoes[j] > infracoes[i]) {
                int tmp = infracoes[i];
                infracoes[i] = infracoes[j];
                infracoes[j] = tmp;

                pcarro tmp_c = carros[i];
                carros[i] = carros[j];
                carros[j] = tmp_c;
            }
        }
    }

    // Imprimir resultados
    if (total == 0) {
        printf("Nenhuma infracao encontrada no periodo especificado.\n");
    } else {
        printf("\n====== RANKING DE INFRACOES ======\n");
        for (int i = 0; i < total; i++) {
            printf("Matricula: %s | Marca: %s | Modelo: %s | Infracoes: %d\n",
                   carros[i]->matricula, carros[i]->marca, carros[i]->modelo, infracoes[i]);
            printf("--------------------------------------------\n");
        }
    }
}

/**
 * @brief Calcula a velocidade m�dia de um ve�culo.
 *
 * A fun��o calcula a velocidade m�dia de um carro dividindo o total de quilometros percorridos pelo tempo total registado.
 *
 * @param c Ponteiro para a estrutura do carro contendo os dados de quil�metros e tempo.
 *
 * @return A velocidade m�dia (em km/h). Retorna 0 se o tempo total for zero.
 *
 */
float velocidademedia(carro *c) {
    float media = 0;
    if(c->tempototal != 0){
    media = c->kilometros/c->tempototal;
    }
    printf("\nmedia:%f\n",media);
    return media;
	//velocidade m�dia
}

void marcamedia(Listapassagem *pass, distancia *d, HASHING *has) {
    int opcao, troca;
	pListacarro Lc;
	pnocarro nc;
	pmarca nm = has->Inicio;

	printf("Deseja ver a marca com os carros que circulam a maior velocidade media ?\n1-Sim\n2-n�o");
	scanf("%d", &opcao);

	if (opcao == 1) {
		int s1, s2;
		pdistancia td= d;
		pListapassagem ppass = pass;
		pListapassagem pLpass = crialistapasagem();
		//pnopassagem pnLpass = (pnopassagem)malloc(sizeof(nopassagem));
		pnopassagem pnLpass ;
        //pnLpass = NULL;
		pnopassagem pnpass = ppass->inicio;
		char horainicio[30];
		char horafim[30];

		printf("entre que datas os carros circularam");
		printf("Qual a data inicial(formato: AAAA-MM-DD_HH:MM:SS)\n");
		scanf("%s", horainicio);
		printf("Qual a data final(formato: AAAA-MM-DD_HH:MM:SS)\n");
		scanf("%s", horafim);

        char *anoi = strtok(horainicio,"-");
       char *mesi = strtok(NULL, "-");
       char *diai = strtok(NULL, "_");
       char *horai =strtok(NULL, ":");
       char *mini = strtok(NULL,":");
       char *segundoi = strtok(NULL, ":");
       char *anof = strtok(horafim,"-");
       char *mesf = strtok(NULL, "-");
       char *diaf = strtok(NULL, "_");
       char *horaf =strtok(NULL, ":");
       char *minf = strtok(NULL,":");
       char *segundof = strtok(NULL, ":");
       float tempoi = calctempo(anoi,mesi,diai,horai,mini,segundoi);
       float tempof = calctempo(anof,mesf,diaf,horaf,minf,segundof);

		while (pnpass != NULL) {
            char copia[30];
            strcpy(copia,pnpass->info->data);
            char *diaa = strtok(copia,"-");
            char *mesa = strtok(NULL, "-");
            char *anoa = strtok(NULL, " ");
            char *horaa =strtok(NULL, ":");
            char *mina = strtok(NULL,":");
            char *segundoa = strtok(NULL, ":");

            float tempoa = calctempo(anoa,mesa,diaa,horaa,mina,segundoa);

			if (tempoi<tempoa && tempof>tempoa) { // esta passagem est� no per�odo
			    pnLpass = (pnopassagem)malloc(sizeof(nopassagem));
				pnLpass->info = pnpass->info;
                pnLpass->prox = pLpass->inicio;
                pLpass->inicio = pnLpass;
				pLpass->numel= pLpass->numel + 1;
			}
			pnpass = pnpass->prox;

		}
printf("\na lista nova tem %d passagens\n",pLpass->numel);
		pnLpass = pLpass->inicio;

		while(pnLpass != NULL){
            pnopassagem temp = pnLpass;
            pnopassagem temp2 = pnLpass;
            if(pnLpass->info->codcarro->kilometros == 0){
                while(temp2 != NULL){

                        if(temp2->info->codcarro->codigo == temp->info->codcarro->codigo){

                            if(temp->info->tiporegist == 0){

                                temp->info->codcarro->kilometros = temp->info->codcarro->kilometros + d->dist[temp->info->idsensor][temp2->info->idsensor];

                                char copia1[30], copia2[30];
                                strcpy(copia1,temp->info->data);
                                char *dia1 = strtok(copia1,"-");

                                char *mes1 = strtok(NULL, "-");
                                char *ano1 = strtok(NULL, " ");
                                char *hora1 =strtok(NULL, ":");
                                char *min1 = strtok(NULL,":");
                                char *segundo1 = strtok(NULL, ":");//printf("hora:%s, min:%s, sec: %s",hora1,mes1,dia1);
                                strcpy(copia2,temp2->info->data);
                                char *dia2 = strtok(copia2,"-");
                                char *mes2 = strtok(NULL, "-");
                                char *ano2 = strtok(NULL, " ");
                                char *hora2 =strtok(NULL, ":");
                                char *min2 = strtok(NULL,":");
                                char *segundo2 = strtok(NULL, ":");//printf("hora:%s, min:%s, sec: %s",hora2,mes2,dia2);
                                float tempo1 = calctempo(ano1,mes1,dia1,hora1,min1,segundo1);
                                float tempo2 = calctempo(ano2,mes2,dia2,hora2,min2,segundo2);
                                printf("\ntempo1: %f\n",tempo1);
                                printf("\ntempo2: %f\n",tempo2);
                                temp->info->codcarro->tempototal= temp->info->codcarro->tempototal + (tempo2 - tempo1);
                                printf("\ntempo %f\n",temp->info->codcarro->tempototal);
                            }
                            temp = temp2;
                        }
                    temp2 =temp2->prox;
                }
		    }
            pnLpass = pnLpass->prox;
		}
        pListacarro tempL = crialistacarro();
        while(nm != NULL){
            Lc = nm->inf;
            nc = Lc->inicio;
            while(nc != NULL){
                if(nc->info->tempototal != 0){
                nm->numkillmarca = nm->numkillmarca + velocidademedia(nc->info);
                printf("%f",nm->numkillmarca);

                }
                nc = nc->prox;
            }
            nm->numkillmarca/nm->NUmcarromarca;
            nm = nm->prox;
        }

        nm = has->Inicio;
        pmarca maior = has->Inicio;
        if(nm == NULL){
            printf("erro ao procurar marca");
        }

        while(nm !=  NULL){
            if(maior->numkillmarca<nm->numkillmarca){
                maior = nm;
            }

            nm = nm->prox;
        }
        if(maior->numkillmarca == 0){
            return ;
        }
        else if(maior->numkillmarca != 0){
            printf("media:%d",maior->numkillmarca);
            printf("marca:%s\n",maior->nome);
            }





	}
	// Qual a marca dos carros que circulam a maior velocidade m�dia?
}

/**
 * @brief Encontra e exibe o condutor (dono) que circula � maior velocidade m�dia.
 *
 * Esta fun�ao percorre todos os carros registados na estrutura de hashing de marcas e calcula a velocidade m�dia de cada carro com base na dist�ncia percorrida e tempo total.
 * Compara as velocidades medias e identifica o condutor com maior valor.
 *
 * @param has Ponteiro para a estrutura de hashing contendo as marcas e respetivos carros.
 */
void condutorediamax(HASHING *has) {
    /*
        Esta fun��o n�o est� terminada pois n�o tenho certeza como fa�o a parte de velocidade m�dia*/
    pmarca p = has->Inicio;
    pnocarro pnmax = p->inf->inicio;
    while(p!= NULL){
        pListacarro pc = p->inf;
        if(pc == NULL){
                printf("d");
            return;
        }
        pnocarro pnc = pc->inicio;
            while(pnc != NULL){

                if(pnc->info->tempototal == 0){

                }
                else if(pnmax->info->tempototal == 0){
                    pnmax = pnc;
                }
                else if(pnmax->info->kilometros/pnmax->info->tempototal< pnc->info->kilometros/pnc->info->tempototal){
                    pnmax = pnc;

                }
            pnc = pnc->prox;

            }

        p = p->prox;
    }
    if(pnmax->info->tempototal == 0){
        printf("\n\nnenhum condutor tem passagens suficientes para calcular a velocidade");
        return ;
    }
    printf("/nO condutor %s codigo:%d tem a maior velocidade media",pnmax->info->pdonos->nome, pnmax->info->pdonos->numcontibuinte);
	//Qual o condutor(dono) que circula a maior velocidade m�dia ?
}

/**
 * @brief Calcula e exibe a velocidade m�dia de todos os condutores com um determinado c�digo postal.
 *
 * Esta fun��o percorre a lista de donos e filtra os que t�m o c�digo postal introduzido. 
 * Para cada um, verifica os carros associados e as respetivas passagens de entrada e sa�da, somando a dist�ncia percorrida e o tempo total, de forma a calcular a velocidade m�dia de cada condutor.
 *
 * @param pass Lista de passagens efetuadas pelos carros.
 * @param donos Lista de donos registados no sistema.
 * @param has Ponteiro para a estrutura de hashing contendo as marcas e carros.
 * @param d Ponteiro para a estrutura com as dist�ncias entre sensores.
 */
void condutorpostal(pListapassagem pass, pListadono donos, HASHING *has, distancia* d) {
	//Qual a velocidade m�dia dos condutores com c�digo postal X
	char codPostal[9];
    printf("Introduza o codigo postal a filtrar: ");
    scanf("%s", codPostal);

    pno no_atual_dono = donos->inicio;

    while (no_atual_dono != NULL) {
        pdono dno = no_atual_dono->info;

        if (strcmp(dno->codPostal, codPostal) == 0) {
            float soma_dist = 0.0;
            float soma_tempo = 0.0;
            int encontrou = 0;

            marca* m = has->Inicio;
            while (m != NULL) {
                pnocarro carro_atual = m->inf->inicio;
                while (carro_atual != NULL) {
                    pcarro c = carro_atual->info;

                    if (c->pdonos->numcontibuinte == dno->numcontibuinte) {
                        pnopassagem p = pass->inicio;
                        passagem *anterior = NULL;

                        while (p != NULL) {
                            if (p->info->codcarro == c) {
                                if (anterior != NULL && p->info->tiporegist == 0 && anterior->tiporegist == 1) {
                                    char d1[30], d2[30];
                                    strcpy(d1, anterior->data);
                                    strcpy(d2, p->info->data);

                                    char dia1[3], mes1[3], ano1[5], hora1[3], min1[3], seg1[3];
                                    char dia2[3], mes2[3], ano2[5], hora2[3], min2[3], seg2[3];

                                    sscanf(d1, "%2[^-]-%2[^-]-%4[^ ] %2[^:]:%2[^:]:%2s", dia1, mes1, ano1, hora1, min1, seg1);
                                    sscanf(d2, "%2[^-]-%2[^-]-%4[^ ] %2[^:]:%2[^:]:%2s", dia2, mes2, ano2, hora2, min2, seg2);

                                    float t1 = calctempo(ano1, mes1, dia1, hora1, min1, seg1);
                                    float t2 = calctempo(ano2, mes2, dia2, hora2, min2, seg2);
                                    float horas = t2 - t1;

                                    float dist = d->dist[anterior->idsensor][p->info->idsensor];
                                    soma_tempo += horas;
                                    soma_dist += dist;
                                    encontrou = 1;
                                }
                                anterior = p->info;
                            }
                            p = p->prox;
                        }
                    }

                    carro_atual = carro_atual->prox;
                }
                m = m->prox;
            }

            if (encontrou && soma_tempo > 0) {
                float vm = soma_dist / soma_tempo;
                printf("Condutor: %s | Velocidade Media: %.2f km/h\n", dno->nome, vm);
            } else {
                printf("Condutor: %s | Sem passagens validas.\n", dno->nome);
            }
        }

        no_atual_dono = no_atual_dono->prox;
    }
}

/**
 * @brief Determina e exibe a marca de autom�vel mais comum no sistema.
 *
 * A fun��o percorre todas as marcas registadas na estrutura de hashing e conta quantos carros existem em cada uma.
 * Depois, identifica a marca com o maior n�mero de carros registados.
 *
 * @param has Ponteiro para a estrutura de hashing que cont�m as marcas e respetivos carros.
 */
void marcapopular(HASHING *has) {
	pmarca pm = has->Inicio;
	if(pm == NULL){
        printf("nao existe nenhuma marca");
        return;
	}

	while(pm != NULL){

        pListacarro LC = pm->inf;
        pnocarro nc = LC->inicio;
        while(nc != NULL){

            pm->NUmcarromarca= pm->NUmcarromarca + 1;
            nc = nc->prox;
        }
        pm = pm->prox;
	}
	pm = has->Inicio;
	pmarca ppm = pm;


	while (ppm->prox != NULL) {
		if (pm->NUmcarromarca < ppm->NUmcarromarca) {
			pm = ppm;
		}
		printf("\n%s",ppm->nome);
    ppm = ppm->prox;
	}

	printf("\nMarca mais popular: %s", pm->nome);
	//Determinar qual a marca de autom�vel mais comum?
}
/**
 * @brief Exporta os dados das passagens para um ficheiro CSV.
 *
 * A fun��o percorre a lista de passagens e escreve os dados principais de cada passagem (matr�cula, marca, modelo, c�digo do carro, n�mero de contribuinte
 * do dono, nome do dono, data/hora e ID do sensor) no ficheiro chamado "base_dados.csv".
 *
 * @param Lp Ponteiro para a lista de passagens.
 * @param has Ponteiro para a estrutura de hashing que cont�m as marcas (n�o utilizado diretamente na fun��o, mas pode ser necess�rio para valida��es futuras).
 */
void exportarXl(Listapassagem* Lp, HASHING* has) {
    if (Lp == NULL || Lp->inicio == NULL) {
        printf("ERRO: Lista de passagens vazia ou invalida!\n");
        return;
    }

	FILE* f = fopen("base_dados.csv", "w");
	if (f == NULL) {
		printf("Erro ao abrir o ficheiro CSV para escrita.\n");
		return;
	}


	fprintf(f, "Matricula;Marca;Modelo;CodigoCarro;ContribuinteDono;NomeDono;DataHora;IDSensor\n");

	pnopassagem atual = Lp->inicio;
	while (atual != NULL) {
		if (atual->info->codcarro && atual->info->codcarro->pdonos) {
			fprintf(f, "%s;%s;%s;%d;%d;%s;%s;%d\n",
				atual->info->codcarro->matricula,
				atual->info->codcarro->marca,
				atual->info->codcarro->modelo,
				atual->info->codcarro->codigo,
				atual->info->codcarro->pdonos->numcontibuinte,
				atual->info->codcarro->pdonos->nome,
				atual->info->data,
				atual->info->idsensor);
		}
		atual = atual->prox;
	}

	fclose(f);
	printf("Dados exportados com sucesso para base_dados.csv\n");
}

/**
 * @brief Exporta os dados dos sensores, dist�ncias, carros, donos e passagens para um ficheiro XML.
 *
 * A fun��o percorre todas as listas fornecidas (sensores, dist�ncias, ve�culos, donos e passagens), e escreve essa informa��o formatada no ficheiro "base_dados.xml".
 *
 * @param Lp Ponteiro para a lista de passagens.
 * @param has Ponteiro para a estrutura de hashing com as marcas e carros.
 * @param Ls Ponteiro para a lista de sensores.
 * @param d Ponteiro para a estrutura que cont�m as dist�ncias entre sensores.
 * @param Ld Ponteiro para a lista de donos dos ve�culos.
 */
void exportarXML(Listapassagem* Lp, HASHING* has, Listasensor* Ls,distancia *d, Listadono *Ld) {
    FILE* f = fopen("base_dados.xml", "w");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro XML para escrita.\n");
        return;
    }

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<BaseDeDados>\n");

    pnosensor sensorat = Ls->inicio;

        while(sensorat){
        if (sensorat) {
            fprintf(f, "  <Sensor>\n");
            fprintf(f, "    <codSensor>%d<codSensor>\n", sensorat->info->codSensor);
            fprintf(f, "    <Designacao>%s<Designacao>\n", sensorat->info->Designacao);
            fprintf(f, "    <Latitude>%s<Latitude>\n", sensorat->info->Latitude);
            fprintf(f, "    <Longitude>%s<Longitude>\n", sensorat->info->Longitude);
            fprintf(f, "  </Sensor>\n");
        }
        sensorat = sensorat->prox;
    }

    fprintf(f,"\n");

    int i = 0, j = 0;

    while(i<10){
        j = 0;
        while(j<10){
            if (j<10) {
            fprintf(f, "  <distancia>\n");
            fprintf(f, "    <codSensor1>%d<codSensor1>\n", d->cod1[i]);
            fprintf(f, "    <codSensor2>%d<codSensor2>\n", d->cod2[j]);
            fprintf(f, "    <distancia>%f<distancia>\n", d->dist[i][j]);
            fprintf(f, "  <distancia>\n");
            }
        j++;
        }
    i++;
    }

    fprintf(f,"\n");
    pmarca p = has->Inicio;


    while(p){
        pListacarro lp = p->inf;
        pnocarro np = lp->inicio;
        while(np){
        if (np) {
            fprintf(f, "  <carro>\n");
            fprintf(f, "    <matricula>%s<matricula>\n", np->info->matricula);
            fprintf(f, "    <marca>%s<marca>\n", np->info->marca);
            fprintf(f, "    <modelo>%s<modelo>\n", np->info->modelo);
            fprintf(f, "    <ano>%d<ano>\n", np->info->ano);
            fprintf(f, "    <dono>%d<dono>\n", np->info->pdonos->numcontibuinte);
            fprintf(f, "    <codVeiculo>%d<codVeiculo>\n", np->info->codigo);
            fprintf(f, "  <carro>\n");
        }
        np = np->prox;
    }
    p = p->prox;
    }

    fprintf(f,"\n");

    pno n = Ld->inicio;

        while (n != NULL) {
        if (n) {
            fprintf(f, "  <Dono>\n");
            fprintf(f, "    <numContribuinte>%d<numContribuinte>\n", n->info->numcontibuinte);
            fprintf(f, "    <nome>%s<nome>\n", n->info->nome);
            fprintf(f, "    <codPostal>%s<codPostal>\n", n->info->codPostal);
            fprintf(f, "  <Dono>\n");
        }
        n = n->prox;
    }

    fprintf(f,"\n");

    pnopassagem atual = Lp->inicio;
    while (atual != NULL) {
        if (atual->info->codcarro && atual->info->codcarro->pdonos) {
            fprintf(f, "  <Passagem>\n");
            fprintf(f, "    <idSensor>%d<idSensor>\n", atual->info->idsensor);
            fprintf(f, "    <codVeiculo>%d<codVeiculo>\n", atual->info->codcarro->codigo);
            fprintf(f, "    <data>%s<data>\n", atual->info->data);
            fprintf(f, "    <Tipo Registo>%d<Tipo Registo>\n", atual->info->tiporegist);
            fprintf(f, "  </Passagem>\n");
        }
        atual = atual->prox;
    }


    fprintf(f, "</BaseDeDados>\n");
    fclose(f);
    printf("Dados exportados com sucesso para base_dados.xml\n");
}

/**
 * @brief Calcula a dist�ncia total percorrida por cada carro com base nas suas passagens.
 *
 * A fun��o percorre a lista de passagens e acumula a dist�ncia total percorrida por cada carro, utilizando os sensores de entrada e sa�da e a matriz de dist�ncias fornecida.
 *
 *
 * @param p Ponteiro para a lista de passagens.
 * @param d Ponteiro para a estrutura que cont�m as dist�ncias entre sensores.
 */
void calcvelocidade(Listapassagem *p, distancia *d){
    pnopassagem np = p->inicio;
    pnopassagem npa;
    pnopassagem npf;
    while(np != NULL){
        npa = np;
        npf= np;
        if(np->info->codcarro->kilometros == 0){
            while (npf!= NULL){
                if(npa->info->codcarro->codigo == npf->info->codcarro->codigo){
                    npa->info->codcarro->kilometros = npa->info->codcarro->kilometros + d->dist[npa->info->idsensor][npf->info->idsensor];

                    npa = npf;
                }
                npf = npf->prox;
            }
        }
        np = np->prox;
    }
    printf("    \n %d\n%f",npa->info->codcarro->codigo, npa->info->codcarro->kilometros);
}

/**
 * @brief Calcula o tempo total em horas a partir de uma data e hora espec�ficas.
 *
 * A fun��o converte uma data e hora no formato (ano, m�s, dia, hora, minuto, segundo)
 * em tempo total em horas desde o "ano 0". 
 * Esta fun��o � usada para comparar diferen�as de tempoentre passagens de ve�culos.
 *
 * @param ano Cadeia de caracteres representando o ano (ex: "2024").
 * @param mes Cadeia de caracteres representando o m�s (ex: "05").
 * @param dia Cadeia de caracteres representando o dia (ex: "21").
 * @param hora Cadeia de caracteres representando a hora (ex: "14").
 * @param minut Cadeia de caracteres representando os minutos (ex: "35").
 * @param sec Cadeia de caracteres representando os segundos (ex: "20").
 * 
 * @return Tempo total em horas (float) desde o ano 0 at� � data/hora especificada.
 */
float calctempo(char *ano,char *mes,char *dia,char *hora,char *minut,char *sec){
    float tempo = 0;
    tempo = tempo + (float)atoi(ano)*365*24*3600;
    int mesp = atoi(mes);
    if(mesp == 1 || mesp == 3 || mesp ==  5  || mesp == 7 || mesp ==  8 || mesp  == 10  || mesp == 12){
      tempo = tempo + 31*24*3600;

    }
    else if(mesp == 4 || mesp == 6 || mesp ==  9  || mesp == 11){
      tempo = tempo +30*24*3600;

    }
    else{
        tempo = tempo + 28*24*3600;
    }
    tempo = tempo + (float)atoi(dia)*24*3600;
    /*printf("\ntempo%f\n",tempo);
    printf("hora: %d\n", atoi(hora));*/
    tempo = tempo + (float)atoi(hora)*3600;
    tempo = tempo + (float)atoi(minut)*60;
    tempo = tempo + (float)atoi(sec);
    //printf("tempo%f\n\n",tempo);
    return tempo/3600;

}


/**
 * @brief Reinicia o contador de quil�metros por marca.
 *
 * Esta fun��o percorre todas as marcas presentes na estrutura de hashing e redefine o valor do campo `numkillmarca` para 0.
 *
 * @param has Ponteiro para a estrutura de hashing que contem as marcas.
 */
void resetmarca(HASHING *has){
    pmarca m = has->Inicio;
    while( m != NULL){
        m->numkillmarca = 0;
        m = m->prox;
    }
}

/**
 * @brief Planeamento para calcular velocidades m�dias por ve�culo com base nas passagens e dist�ncias.
 *
 * Esta fun��o visa percorrer a lista de passagens e calcular a velocidade m�dia dos ve�culos ao comparar pares de passagens sucessivas do mesmo carro.
 *
 * @param Lp Ponteiro para a lista de passagens.
 * @param d Ponteiro para a estrutura com dist�ncias entre sensores.
 */

void velocidade(Listapassagem *Lp, distancia *d){
    pnopassagem np = Lp->inicio;
        while(np != NULL){
               pnopassagem npa = np;
               pnopassagem npf = np;
               while(npf!= NULL){
                if(npf->info->codcarro->codigo == npa->info->codcarro->codigo ){
                 //   npa->info->codcarro->kilometros =
                }
               }
            np->prox;
        }
}

//------------------------------------------------------------------//
//fun��es relacionadas � memoria

/**
 * @brief Calcula a mem�ria total ocupada por uma lista de donos.
 *
 * Soma a mem�ria estatica da estrutura da lista e de cada n� individual.
 *
 * @param Ld Ponteiro para a lista de donos.
 * 
 * @return Tamanho em bytes da mem�ria ocupada.
 */
size_t calcularMemoriaListadono(pListadono Ld) {
    if (Ld == NULL) return 0;
    size_t total = sizeof(Listadono);
    pno atual = Ld->inicio;
    while (atual != NULL) {
        total += sizeof(no) + sizeof(dono);
        atual = atual->prox;
    }
    return total;
}

/**
 * @brief Calcula a mem�ria total ocupada por uma lista de carros.
 *
 * Soma a memoria est�tica da estrutura da lista e de cada carro e n�.
 *
 * @param Lc Ponteiro para a lista de carros.
 * 
 * @return Tamanho em bytes da mem�ria ocupada.
 */
size_t calcularMemoriaListacarro(pListacarro Lc) {
    if (Lc == NULL) return 0;
    size_t total = sizeof(Listacarro);
    pnocarro atual = Lc->inicio;
    while (atual != NULL) {
        total += sizeof(nocarro) + sizeof(carro);
        atual = atual->prox;
    }
    return total;
}
/**
 * @brief Calcula a mem�ria total ocupada pela estrutura de hashing de marcas.
 *
 * Percorre todas as marcas, somando sua mem�ria e a das listas de carros associadas.
 *
 * @param has Ponteiro para a estrutura de hashing.
 * 
 * @return Tamanho em bytes da mem�ria ocupada.
 */
size_t calcularMemoriaHashing(HASHING *has) {
    if (has == NULL) return 0;
    size_t total = sizeof(HASHING);
    pmarca atual = has->Inicio;
    while (atual != NULL) {
        total += sizeof(marca) + calcularMemoriaListacarro(atual->inf);
        atual = atual->prox;
    }
    return total;
}

/**
 * @brief Calcula a mem�ria total ocupada pela lista de passagens.
 *
 * @param Lp Ponteiro para a lista de passagens.
 * @return Tamanho em bytes da mem�ria ocupada.
 */
size_t calcularMemoriaListapassagem(pListapassagem Lp) {
    if (Lp == NULL) return 0;
    size_t total = sizeof(Listapassagem);
    pnopassagem atual = Lp->inicio;
    while (atual != NULL) {
        total += sizeof(nopassagem) + sizeof(passagem);
        atual = atual->prox;
    }
    return total;
}
/**
 * @brief Calcula a mem�ria total ocupada pela lista de sensores.
 *
 * @param Ls Ponteiro para a lista de sensores.
 * @return Tamanho em bytes da mem�ria ocupada.
 */
size_t calcularMemoriaListasensor(pListasensor Ls) {
    if (Ls == NULL) return 0;
    size_t total = sizeof(Listasensor);
    pnosensor atual = Ls->inicio;
    while (atual != NULL) {
        total += sizeof(nosensor) + sizeof(sensor);
        atual = atual->prox;
    }
    return total;
}

/**
 * @brief Retorna a mem�ria ocupada pela estrutura de dist�ncias.
 *
 * @param d Ponteiro para a estrutura de dist�ncias.
 * @return Tamanho em bytes da mem�ria ocupada.
 */
size_t calcularMemoriaDistancia(pdistancia d) {
    return (d != NULL) ? sizeof(distancias) : 0;
}

/**
 * @brief Calcula e imprime a mem�ria total ocupada por todas as estruturas principais.
 *
 * Soma a memoria ocupada por listas de donos, ve�culos, sensores, passagens e a matriz de distancias.
 *
 * @param Ld Ponteiro para a lista de donos.
 * @param has Ponteiro para a tabela de hashing das marcas.
 * @param Lp Ponteiro para a lista de passagens.
 * @param Ls Ponteiro para a lista de sensores.
 * @param d Ponteiro para a estrutura de dist�ncias.
 */
void memoriateste(Listadono *Ld, HASHING *has, Listapassagem *Lp, Listasensor *Ls, pdistancia d) {
    size_t total = 0;

    // Mem�ria das listas de donos
    total += calcularMemoriaListadono(Ld);

    // Mem�ria do hashing (marcas e carros)
    total += calcularMemoriaHashing(has);

    // Mem�ria das passagens
    total += calcularMemoriaListapassagem(Lp);

    // Mem�ria dos sensores
    total += calcularMemoriaListasensor(Ls);

    // Mem�ria da matriz de dist�ncias
    total += calcularMemoriaDistancia(d);

    printf("\nMemoria total ocupada: %zu bytes\n", total);
    printf("\nMemoria total ocupada: %zu Mbytes\n", total/1024/1024);
}
