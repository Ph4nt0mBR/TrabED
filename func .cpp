#include "header.h"
#include "func.h"
#include<locale>




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

	// Lê cada linha e preenche a matriz
	int no1, no2;

	float distancia;

	while (fscanf(F, "%d %d %f", &no1, &no2, &distancia) == 3) {
		if (no1 < 1 || no1 > 10 || no2 < 1 || no2 > 10) {
			fprintf(stderr, "Nó inválido no arquivo: %d %d\n", no1, no2);
			continue;
		}

		nd->cod1[no1 - 1] = no1;
		nd->cod2[no2 - 1] = no2;
		nd->dist[no1 - 1][no2 - 1] = distancia;
		nd->dist[no2 - 1][no1 - 1] = distancia; // Matriz simétrica
        printf("%f\n",nd->dist[no1-1][no2-1]);
	}
	fclose(F);
    return nd;
}
/*Lê distâncias entre pares de nós do arquivo "distancias.txt" e armazena-as numa matriz 10x10 dentro da struct "distancias".
Cada linha do arquivo deve conter: <no1> <no2> <distancia>. Os nós devem estar no intervalo [1, 10]. A distância é registada nos dois sentidos.*/



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

        // Tentativa 2: Formato com espaços (se falhou com tabs)
        if (campos_lidos != 3) {
            campos_lidos = sscanf(buffer, "%d %99[^\n] %9s", &COD, NOME, CP);
        }

        if (campos_lidos == 3) {
            // Verifica dados numéricos
            if (COD <= 0) {
                printf("Linha %d: Código inválido (%d)\n", line, COD);
                problemas++;
                continue;
            }

            // Cria e adiciona o dono
            pdono ndono = (pdono)malloc(sizeof(dono));
            if (!ndono) {
                printf("Erro de memória na linha %d\n", line);
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
            printf("ERRO - Linha %d: Formato inválido\n", line);
            printf("Conteúdo: [%s]\n\n", buffer);
            problemas++;
        }
    }

    fclose(F);

    printf("\nRelatório Final:\n");
    printf("Total de linhas: %d\n", line);
    printf("Registros importados: %d\n", count);
    printf("Problemas detectados: %d\n\n", problemas);

    return (count > 0) ? 1 : 0;
}
/*Lê dados de donos a partir do arquivo "donos.txt" e adiciona-os à lista ligada "Listadono". Cada linha deve conter: <código> <nome> <código postal>, separados por tabulação ou espaço.
Ignora ou reporta linhas mal formatadas ou com dados inválidos.*/



int importcarro(Listadono *L, HASHING *has)
{
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
            printf("Erro de alocação de memória.\n");
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
            printf("Dono '%d' não encontrado. Carro '%s' ignorado.\n", ndono, Mat);
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
/*Lê dados de carros do arquivo "carros.txt" e associa-os ao respetivo dono na lista "Listadono" e à marca na struct hashing.
Cada linha deve conter: <matrícula> <marca> <modelo> <ano> <NIF do dono> <código>, separados por tabulações.*/


int importpassagem(Listapassagem *L, HASHING *has) {
    FILE* F = fopen("passagem.txt", "r");
    if (F == NULL) {
        printf("\nErro ao abrir o ficheiro!\n");
        return 0;
    }

    int Id, COD, regist;
    char Data[20];

    int contador =0;
    while (fscanf(F, "%d\t%d\t%[^\t]\t%d", &Id, &COD, Data, &regist) == 4) {
        //printf("Processando: ID=%d, COD=%d, Data=%s, Regist=%d\n", Id, COD, Data, regist);
        contador++;
if (contador%100000 == 0)
    printf("\n+ %d passagens lidas",contador);
        passagem* npass = (passagem*)malloc(sizeof(passagem));
        if (!npass) {
            printf("Erro de alocação de memória\n");
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
            printf("Carro COD %d nao encontrado! Registro ignorado.\n", COD);
            free(npass);
            continue;  // Continua para próximo registro ao invés de retornar
        }

    npass->codcarro = plc->info;
    Addpassagem(L,npass);
    }

    fclose(F);
    printf("Importação concluída com sucesso!\n");
    return 1;
}
/*Lê dados de passagens do arquivo "passagem.txt" e associa cada uma ao carro correspondente na estrutura "HASHING".
Cada linha deve conter: <ID sensor> <código do carro> <data> <tipo de registo>, separados por tabulações.*/


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
            printf("Formato inválido na linha: %s\n", linha);
        }
    }

    fclose(F);
    return 1;
}
/*Lê dados dos sensores do arquivo "sensores.txt" e adiciona-os à lista "Listasensores".
Cada linha deve conter: <código> <designação> <latitude> <longitude>, separados por tabulações.*/





//funçoes para importar
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
pListadono crialistadono() {

	pListadono L = (pListadono)malloc(sizeof(Listadono));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}
//Cria e retorna uma nova lista ligada "Listadono" vazia, com contador a zero.

pListacarro crialistacarro() {
	pListacarro L = (pListacarro)malloc(sizeof(Listacarro));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}
//Cria e retorna uma nova lista ligada "Listacarro" vazia, com contador a zero.

pListasensor crialistasensores() {
	pListasensor L = (pListasensor)malloc(sizeof(Listasensor));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}
//Cria e retorna uma nova lista ligada "Listasensor" vazia, com contador a zero.


pListapassagem crialistapasagem() {
	pListapassagem L = (pListapassagem)malloc(sizeof(Listapassagem));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}
//Cria e retorna uma nova lista ligada "Listapassagem" vazia, com contador a zero.

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
//Cria uma nova marca com nome e lista de carros associada, e insere-a no início da tabela HASHING.


HASHING *CriarHASHING()
{
    HASHING *Has = (HASHING *)malloc(sizeof(HASHING));
    Has->Inicio = NULL;
    //Has->N_CHAVES = 0;
    //Has->Flag_Destruir = flag;
    return Has;
}
//funçoes que criam listas
//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------

void AddDono(Listadono* l, dono* D)
{
	pno nNo = (pno)malloc(sizeof(no));
	nNo->info = D;
	nNo->prox = l->inicio;
	l->inicio = nNo;
	l->numel++;

}
//Adiciona um novo dono a lista ligada "Listadono", inserindo-o no inicio da lista.

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
//Adiciona um novo carro à lista ligada "Listacarro", inserindo-o no inicio da lista, se for valido.


void Addsensor(Listasensor* l, sensor* D)
{
	pnosensor nNo = (pnosensor)malloc(sizeof(nosensor));
	nNo->info = D;
	nNo->prox = l->inicio;
	l->inicio = nNo;
	l->numel++;

}
//Adiciona um novo sensor a lista ligada "Listasensor",adicionando-o no inicio da lista.

void Addpassagem(Listapassagem* l, passagem* D)
{
	pnopassagem nNo = (pnopassagem)malloc(sizeof(nopassagem));
	nNo->info = D;
	nNo->prox = l->inicio;
	l->inicio = nNo;
	l->numel++;
}

//funçoes que introduzem um item na sua lista
//----------------------------------------------------
//----------------------------------------------------
//-----------------------------------------------

void regist_dono(Listadono* Ld) {
	int opcao = 0;
	pListadono n;
	printf("Gostaria de adicionar mais um dono? 1-sim 0-não\n");
	scanf("%d", &opcao);
	if (opcao == 1) {
		dono* ndono = (pdono)malloc(n->numel * sizeof(struct no));
		if (ndono == NULL) {
			printf("Ocorreu um erro ao adicionar o dono.\n");
			return;
		}

		printf("Qual o número de contribuinte?\n");
		scanf("%d", &ndono->numcontibuinte);
		printf("Qual o nome do dono?\n");//nao tenho certeza se devo meter assim o nome temos de ver depois se está bom
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

void list_dono(Listadono* Ld) {
	//função que apresenta no ecrã todos os donos
	int i = 0, skip = 0, continuar;
	pdono linfdono;
	pno ldono = Ld->inicio;
	while (ldono != NULL) {
		linfdono = ldono->info;
		printf("Código postal:%s\n", linfdono->codPostal);
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

		printf("Insira a matricula do veículo:\n");
		scanf("%s", novoCarro->matricula);

		printf("Insira o nmero de contribuinte do dono:\n");
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
		printf("Veículo adicionado com sucesso!\n");
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
		printf("Nenhum veiculo registrado.\n");
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
       // printf("Nenhum veículo desta marca registado.\n");
		//}


		printf("\n-------------------------------------------------------\n");
		pm = pm->prox;
	}
}


void regist_pass(Listapassagem* Lp, HASHING *has, Listasensor* Ls) {
	passagem* novaPassagem = (passagem*)malloc(sizeof(passagem));
	if (novaPassagem == NULL) {
		printf("Erro ao alocar memória para a passagem.\n");
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
		printf("Sensor não encontrado.\n");
		free(novaPassagem);
		return;
	} //se o sensor n for encontrado avisa e retorna*/

	novaPassagem->idsensor = codigoSensor;

	printf("Insira a data e hora da passagem (formato: DD-MM-AAAA HH:MM:SS):\n"); //pede data de passagem
	scanf("%s", dataHora);
	strcpy(novaPassagem->data, dataHora);

	Addpassagem(Lp, novaPassagem);
	printf("Passagem registrada com sucesso!\n"); //adicona a passagem e avisa
}


void organizadonos(Listadono* Ld) {
	if (Ld->inicio == NULL || Ld->inicio->prox == NULL) {
		printf("Poucos ou nenhum dono registrado para ordenar.\n");
		return;
	}
	int trocado, opcao;
	pdono temp;
	pno atual, max = NULL;

    printf("Deseja executar a organizacao por que ordem?\n 1- Nome do dono\n 2- Numero de contribuinte\n 3- Parar organizacao\n");
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
		//funcao que permite importar as listas(exceto sensores e distancias)
		//sao usadas varias mini funçoes para importar cada um
}

void organizacarros(HASHING *has) {
    //falta fazer a 1 e 3 organização
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

	printf("Organizar carros por:\n1-matricula\n2-marca\n3-modelo\n4-Sair");
	printf("\n <aviso: organizar por matricula ou modelo automaticamente lista os carros, as alteracoes seram revertidas depois da \tlistagem>");
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

void listacarroperiodo(Listapassagem *pass) {
	int opcao;
	printf("/nDeseja listar os carros que circularam num periodo?\n1-Sim\n2-não");
	scanf("%d", &opcao);

	if (opcao == 1) {
		pListapassagem ppass = pass;
		pnopassagem pnpass = ppass->inicio;
		char horafim[100];
		char horainicio[100];

		printf("entre que datas os carros circularam");
		printf("Qual a data incial(formato: AAAA-MM-DD_HH:MM:SS)");
		scanf("%s", horainicio);
		printf("Qual a data final(formato: AAAA-MM-DD_HH:MM:SS)");
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
       /* printf("tempoi : %f\t",tempoi);
        printf("tempoa : %f\t",tempoa);
        printf("tempof : %f\n",tempof);*/
			if(tempoi<tempoa && tempoa<tempof){
				printf("\n--------------------------\n");
				printf("Matricula: %s\n", pnpass->info->codcarro->matricula);
				printf("Contribuinte do Dono: %d\n", pnpass->info->codcarro->pdonos->numcontibuinte);
				printf("Marca: %s\n", pnpass->info->codcarro->marca);
				printf("Modelo: %s\n", pnpass->info->codcarro->modelo);
				printf("Ano: %d\n", pnpass->info->codcarro->ano);
				printf("Codigo: %d\b", pnpass->info->codcarro->codigo);
			}
			pnpass = pnpass->prox;
		}

	}
}

void rankveiculos(Listapassagem *pass,distancia *d) {
	int opcao, troca;

	printf("Deseja rankear os carros que circularam num periodo pelos kilometros?\n1-Sim\n2-não");
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
		char horafim[100];
		char horainicio[100];

		printf("entre que datas os carros circularam");
/*		printf("Qual a data incial(formato: AAAA-MM-DD_HH:MM:SS)");
		scanf("%s", horainicio);
		printf("Qual a data final(formato: AAAA-MM-DD_HH:MM:SS)");
		scanf("%s", horafim);
*/    strcpy(horainicio,"2000-01-01_12:12:12");
    strcpy(horafim,"2006-01-01_12:12:12");
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
            printf("\ndata convertida[%s][%s][%s][%s][%s][%s]",anoa,mesa,diaa,horaa,mina,segundoa);
            float tempoa = calctempo(anoa,mesa,diaa,horaa,mina,segundoa);
            printf("tempoi : %f\t",tempoi);
            printf("tempoa : %f\t",tempoa);
            printf("tempof : %f\n",tempof);
			if (tempoi<tempoa && tempof>tempoa) { // esta passagem está no período
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
                               // printf("o kilometro é %d",temp->info->codcarro->codigo);
                            }
                            temp = temp2;
                        }
                    temp2 =temp2->prox;
                }
		    }
            pnLpass = pnLpass->prox;
		}


		do{
            pnLpass = pLpass->inicio;
            int troca = 0;
            while(pnLpass != NULL){
                if(pnLpass->prox != NULL && pnLpass->info->codcarro->kilometros < pnLpass->prox->info->codcarro->kilometros){

//                    ppassagem ptemp = (ppassagem)malloc(sizeof(passagem));
                    ppassagem ptemp = pnLpass->info;
                    printf("\no carro temporario %d\n",ptemp->codcarro->codigo);
                    printf("no caro antes de temp %d\n ",pnLpass->prox->info->codcarro->codigo);
                    pnLpass->info = pnLpass->prox->info;
                    pnLpass->prox->info = ptemp;
                    printf("\no carro depois de temp %d\n",pnLpass->prox->info->codcarro->codigo);

                    troca = 1;

                }
                pnLpass = pnLpass->prox;
            }
		}while(troca == 1);

        pnLpass = pLpass->inicio;
        printf("%d",pLpass->inicio->info->codcarro->codigo);

		while(pnLpass != NULL){
                if(pnLpass->info->codcarro->kilometros != 0){
                printf("\n------------------------------\n");
                printf("percorreu %f kilometros",pnLpass->info->codcarro->kilometros);
                printf("Matricula: %s\n", pnLpass->info->codcarro->matricula);
				printf("Contribuinte do Dono: %d\n", pnLpass->info->codcarro->pdonos->numcontibuinte);
				printf("Marca: %s\n", pnLpass->info->codcarro->marca);
				printf("Modelo: %s\n", pnLpass->info->codcarro->modelo);
				printf("Ano: %d\n", pnLpass->info->codcarro->ano);
				printf("Codigo: %d\b", pnLpass->info->codcarro->codigo);
				pnLpass->info->codcarro->kilometros = 0;
                }
            pnLpass = pnLpass->prox;
		}
	}
	/*Ranking de circulação. Listagem ordenada pelo total de quilómetros
	que cada veículo efectuou na auto-estrada durante determinado período. */
}


void rankmarcas(Listapassagem *pass, distancia *d, HASHING *has) {
	int opcao, troca;
	pmarca pm = has->Inicio;
	pListacarro pl;
	pnocarro pn;
	printf("\n\nDeseja rankear as marcas num periodo pelos kilometros?\n1-Sim\n2-não");
	scanf("%d", &opcao);

	if (opcao == 1) {
		int s1, s2;
		pdistancia td = d;
		pListapassagem ppass = pass;
		pListapassagem pLpass = (pListapassagem)malloc(sizeof(Listapassagem));
		pnopassagem pnLpass = pLpass->inicio;
		pnopassagem pnpass = ppass->inicio;
		char horafim[100];
		char horainicio[100];

		printf("entre que datas os carros circularam");
		printf("Qual a data incial(formato: AAAA-MM-DD_HH:MM:SS)");
		scanf("%s", horainicio);
		printf("Qual a data final(formato: AAAA-MM-DD_HH:MM:SS)");
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
            char *anoa = strtok(pnpass->info->data,"-");
            char *mesa = strtok(NULL, "-");
            char *diaa = strtok(NULL, " ");
            char *horaa =strtok(NULL, ":");
            char *mina = strtok(NULL,":");
            char *segundoa = strtok(NULL, ":");
            float tempoa = calctempo(anoa,mesa,diaa,horaa,mina,segundoa);
            printf("tempoi : %f\t",tempoi);
            printf("tempoa : %f\t",tempoa);
            printf("tempof : %f\n",tempof);
			if (tempoi<tempoa && tempof>tempoa) {
				pnLpass->info = pnpass->info;

				pnopassagem Lpass =(pnopassagem)malloc(sizeof(nopassagem));

				pnLpass->prox = Lpass;
				pLpass->numel= pLpass->numel + 1;
			}
			pnpass = pnpass->prox;
			printf("o");
		}

		pnLpass = pLpass->inicio;
		while (pnLpass != NULL) {
			pnopassagem p = pnLpass->prox;

			while (p->prox != NULL) {
				if (p->prox->info->codcarro == pnLpass->info->codcarro) {
					pnopassagem delp = p;
					p = p->prox;
					pnopassagem ptemp = p;
					s1 = pnLpass->info->idsensor;
					s2 = p->info->idsensor;
					pnLpass->info->codcarro->kilometros = pnLpass->info->codcarro->kilometros + td->dist[s1][s2];
					p = p->prox;
					free(ptemp->info);
					free(ptemp);
					delp->prox = p;
				}
				else {
					p = p->prox;
				}
			}
			pnLpass = pnLpass->prox;
		}


		while (pm != NULL) {
			pl = pm->inf;
			pn = pl->inicio;

			while (pn != NULL) {
				pm->numkillmarca = pm->numkillmarca + pn->info->kilometros;
				pn = pn->prox;
			}
			pm = pm->prox;
		}

		pm = has->Inicio;

		do{
			pmarca antpm = pm;
			pm = pm->prox;
			pmarca ppm = pm->prox;
			pmarca temppm;
			troca = 0;

			if (pm->numkillmarca > antpm->numkillmarca) {
				antpm->prox = pm->prox;
				pm->prox = antpm;
				troca = 1;
			}
			while (ppm != NULL) {


				if (pm->numkillmarca < ppm->numkillmarca) {

					antpm->prox = ppm;//antpm->pm->ppm->
					pm = ppm->prox;
					ppm->prox = pm;//antpm->ppm->pm->
					troca = 1;
				}


			}
		} while (troca == 1);
		/*Ranking por marca. Listagem ordenada pelo total de quilómetros que
		os veículos das diferentes marcas efectuaram na autoestrada durante determinado período. */
	}
}

void listainfracao(Listapassagem* pass, distancia* d) {
    if (pass == NULL || d == NULL) {
        printf("Erro: Dados inválidos.\n");
        return;
    }

    char inicio[20], fim[20];
    printf("\n--- Listagem de Infrações ---\n");
    printf("Período a analisar (formato AAAA-MM-DD_HH:MM:SS)\n");
    printf("Data/hora inicial: ");
    scanf("%19s", inicio);
    printf("Data/hora final: ");
    scanf("%19s", fim);

   //data inicio
    char* anoi = strtok(inicio, "-");
    char* mesi = strtok(NULL, "-");
    char* diai = strtok(NULL, "_");
    char* horai = strtok(NULL, ":");
    char* mini = strtok(NULL, ":");
    char* segundoi = strtok(NULL, ":");

    //data fim
    char* anof = strtok(fim, "-");
    char* mesf = strtok(NULL, "-");
    char* diaf = strtok(NULL, "_");
    char* horaf = strtok(NULL, ":");
    char* minf = strtok(NULL, ":");
    char* segundof = strtok(NULL, ":");

    float tempo_inicio = calctempo(anoi, mesi, diai, horai, mini, segundoi);
    float tempo_fim = calctempo(anof, mesf, diaf, horaf, minf, segundof);

    //lista temporaria de passagems
    pListapassagem tempList = crialistapasagem();
    pnopassagem atual = pass->inicio;

    while (atual != NULL) {
        char data[20];
        strcpy(data, atual->info->data);

        char* ano = strtok(data, "-");
        char* mes = strtok(NULL, "-");
        char* dia = strtok(NULL, "_");
        char* hora = strtok(NULL, ":");
        char* min = strtok(NULL, ":");
        char* segundo = strtok(NULL, ":");

        float tempo_atual = calctempo(ano, mes, dia, hora, min, segundo);

        if (tempo_atual >= tempo_inicio && tempo_atual <= tempo_fim) {

            //adiciona a lista temporaria
            pnopassagem novo = (pnopassagem)malloc(sizeof(nopassagem));
            novo->info = atual->info;
            novo->prox = tempList->inicio;
            tempList->inicio = novo;
            tempList->numel++;
        }
        atual = atual->prox;
    }

    printf("\nVeiculos com velocidade media > 120 km/h:\n");
    printf("----------------------------------------\n");

    pnopassagem p = tempList->inicio;
    int count = 0;

    while (p != NULL) {
        pnopassagem prox = p->prox;

        //encontra passagem da saida
        while (prox != NULL && prox->info->codcarro != p->info->codcarro) {
            prox = prox->prox;
        }

        if (prox != NULL && p->info->tiporegist == 0 && prox->info->tiporegist == 1) {
            //calcular velocidade
            float distancia = d->dist[p->info->idsensor][prox->info->idsensor];

            // Converter tempos para horas
            char data_entrada[20], data_saida[20];
            strcpy(data_entrada, p->info->data);
            strcpy(data_saida, prox->info->data);

            char* anoe = strtok(data_entrada, "-");
            char* mese = strtok(NULL, "-");
            char* diae = strtok(NULL, "_");
            char* horae = strtok(NULL, ":");
            char* mine = strtok(NULL, ":");
            char* sege = strtok(NULL, ":");

            char* anos = strtok(data_saida, "-");
            char* mess = strtok(NULL, "-");
            char* dias = strtok(NULL, "_");
            char* horas = strtok(NULL, ":");
            char* mins = strtok(NULL, ":");
            char* segs = strtok(NULL, ":");

            float tempo_entrada = calctempo(anoe, mese, diae, horae, mine, sege);
            float tempo_saida = calctempo(anos, mess, dias, horas, mins, segs);
            float tempo_total = tempo_saida - tempo_entrada;

            if (tempo_total > 0) {
                float velocidade = distancia / tempo_total;

                if (velocidade > 120) {
                    printf("Matricula: %s\n", p->info->codcarro->matricula);
                    printf("Marca: %s\n", p->info->codcarro->marca);
                    printf("Modelo: %s\n", p->info->codcarro->modelo);
                    printf("Velocidade media: %.2f km/h\n", velocidade);
                    printf("Data/hora entrada: %s\n", p->info->data);
                    printf("Data/hora saida: %s\n", prox->info->data);
                    printf("----------------------------------------\n");
                    count++;
                }
            }
        }
        p = p->prox;
    }

    if (count == 0) {
        printf("Nenhuma infracao encontrada no período especificado.\n");
    }
    else {
        printf("Total de infracoes: %d\n", count);
    }

    //free a memoriar temporaria
    pnopassagem temp = tempList->inicio;
    while (temp != NULL) {
        pnopassagem next = temp->prox;
        free(temp);
        temp = next;
    }
    free(tempList);
}




void rankinfracao() {
	//Ranking de infrações p / veículo.Listagem com o veículo e respectivo
	//número total de infrações de velocidade ocorridas durante determinado período.
}

void velocidademedia() {

	//velocidade média
}

void marcamedia() {
	// Qual a marca dos carros que circulam a maior velocidade média?
}

void condutorediamax(HASHING *has) {
    /*
        Esta função não está terminada pois não tenho certeza como faço a parte de velocidade média*/
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
	//Qual o condutor(dono) que circula a maior velocidade média ?
}

void condutorpostal() {
	//Qual a velocidade média dos condutores com código postal X?
}

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
	//Determinar qual a marca de automóvel mais comum?
}

void exportarXl(Listapassagem* Lp, HASHING* has) {
    if (Lp == NULL || Lp->inicio == NULL) {
        printf("ERRO: Lista de passagens vazia ou inválida!\n");
        return;
    }

	FILE* f = fopen("base_dados.csv", "w");
	if (f == NULL) {
		printf("Erro ao abrir o ficheiro CSV para escrita.\n");
		return;
	}


	fprintf(f, "Matricula,Marca,Modelo,CodigoCarro,ContribuinteDono,NomeDono,DataHora,IDSensor\n");

	pnopassagem atual = Lp->inicio;
	while (atual != NULL) {
		if (atual->info->codcarro && atual->info->codcarro->pdonos) {
			fprintf(f, "%s,%s,%s,%d,%d,%s,%s,%d\n",
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

void exportarXML(Listapassagem* Lp, HASHING* has, Listasensor* Ls) {
    FILE* f = fopen("base_dados.xml", "w");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro XML para escrita.\n");
        return;
    }

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<BaseDeDados>\n");

    pnopassagem atual = Lp->inicio;
    while (atual != NULL) {
        if (atual->info->codcarro && atual->info->codcarro->pdonos) {
            fprintf(f, "  <Passagem>\n");
            fprintf(f, "    <Matricula>%s</Matricula>\n", atual->info->codcarro->matricula);
            fprintf(f, "    <Marca>%s</Marca>\n", atual->info->codcarro->marca);
            fprintf(f, "    <Modelo>%s</Modelo>\n", atual->info->codcarro->modelo);
            fprintf(f, "    <CodigoCarro>%d</CodigoCarro>\n", atual->info->codcarro->codigo);
            fprintf(f, "    <ContribuinteDono>%d</ContribuinteDono>\n", atual->info->codcarro->pdonos->numcontibuinte);
            fprintf(f, "    <NomeDono>%s</NomeDono>\n", atual->info->codcarro->pdonos->nome);
            fprintf(f, "    <DataHora>%s</DataHora>\n", atual->info->data);
            fprintf(f, "    <IDSensor>%d</IDSensor>\n", atual->info->idsensor);
            fprintf(f, "  </Passagem>\n");
        }
        atual = atual->prox;
    }

    fprintf(f, "</BaseDeDados>\n");
    fclose(f);
    printf("Dados exportados com sucesso para base_dados.xml\n");
}

void calcvelociade(Listapassagem *p, distancia *d){
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
    tempo = tempo + (float)atoi(hora)*3600;
    tempo = tempo + (float)atoi(minut)*60;
    tempo = tempo + (float)atoi(sec);
    return tempo/3600;

}

void resetveiculo(Listapassagem *pass,distancia *d){
    pnopassagem nop = pass->inicio;
    while(nop != NULL){
        nop->info->codcarro->kilometros = 0;
        nop->info->codcarro->tempototal = 0;
        nop = nop->prox;
    }
}

void resetmarca(HASHING *has){
    pmarca m = has->Inicio;
    while( m != NULL){
        m->numkillmarca = 0;
        m = m->prox;
    }
}

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
//funções relacionadas á memoria
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

size_t calcularMemoriaDistancia(pdistancia d) {
    return (d != NULL) ? sizeof(distancias) : 0;
}

// Função principal que soma toda a memória
void memoriateste(Listadono *Ld, HASHING *has, Listapassagem *Lp, Listasensor *Ls, pdistancia d) {
    size_t total = 0;

    // Memória das listas de donos
    total += calcularMemoriaListadono(Ld);

    // Memória do hashing (marcas e carros)
    total += calcularMemoriaHashing(has);

    // Memória das passagens
    total += calcularMemoriaListapassagem(Lp);

    // Memória dos sensores
    total += calcularMemoriaListasensor(Ls);

    // Memória da matriz de distâncias
    total += calcularMemoriaDistancia(d);

    printf("\nMemoria total ocupada: %zu bytes\n", total);
    printf("\nMemoria total ocupada: %zu Mbytes\n", total/1024/1024);
}
