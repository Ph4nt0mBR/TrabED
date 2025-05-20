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
}


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

int importpassagem(Listapassagem *L, HASHING *has) {
    FILE* F = fopen("passagem.txt", "r");
    if (F == NULL) {
        printf("\nErro ao abrir o ficheiro!\n");
        return 0;
    }

    int Id, COD, regist;
    char Data[20];

    while (fscanf(F, "%d\t%d\t%[^\t]\t%d", &Id, &COD, Data, &regist) == 4) {
        printf("Processando: ID=%d, COD=%d, Data=%s, Regist=%d\n", Id, COD, Data, regist);

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

        while (pm != NULL && !encontrado) {
            if (pm->inf == NULL || pm->inf->inicio == NULL) {
                pm = pm->prox;
                continue;
            }
            nocarro *current = pm->inf->inicio;
            while (current != NULL && !encontrado) {
                if (current->info != NULL && current->info->codigo == COD) {
                    plc = current;
                    encontrado = 1;
                    printf("Carro COD %d encontrado na marca\n", COD);
                }
                current = current->prox;
            }

            if (!encontrado) pm = pm->prox;
        }
/*
        if (!encontrado) {
            printf("Carro COD %d não encontrado! Registro ignorado.\n", COD);
            free(npass);
            continue;  // Continua para próximo registro ao invés de retornar
        }
*/
        //npass->codcarro = plc->info;
    Addpassagem(L,npass);
    }

    fclose(F);
    printf("Importação concluída com sucesso!\n");
    return 1;
}

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

pListacarro crialistacarro() {
	pListacarro L = (pListacarro)malloc(sizeof(Listacarro));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}

pListasensor crialistasensores() {
	pListasensor L = (pListasensor)malloc(sizeof(Listasensor));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}

pListapassagem crialistapasagem() {
	pListapassagem L = (pListapassagem)malloc(sizeof(Listapassagem));
	L->inicio = NULL;
	L->numel = 0;
	return L;
}

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

void Addsensor(Listasensor* l, sensor* D)
{
	pnosensor nNo = (pnosensor)malloc(sizeof(nosensor));
	nNo->info = D;
	nNo->prox = l->inicio;
	l->inicio = nNo;
	l->numel++;

}

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
	}
	if (opcao == 0) {
		return;
	}
}

void list_dono(Listadono* Ld) {
	//função que apresenta no ecrã todos os donos
	pdono linfdono;
	pno ldono = Ld->inicio;
	while (ldono != NULL) {
		linfdono = ldono->info;
		printf("Código postal:%s\n", linfdono->codPostal);
		printf("Nome:%s\n", linfdono->nome);
		printf("numero de contribuinte:%d\n", linfdono->numcontibuinte);
		ldono = ldono->prox;
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
                printf("\nmore-1\nexit-2\n");
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

	printf("Insira a data e hora da passagem (formato: AAAA-MM-DD_HH:MM:SS):\n"); //pede data de passagem
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

void memoria() {
	//Determinar a memoria ocupada por toda a estrutura de dados !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1












}

void organizacarros(HASHING *has) {
	pmarca nm = has->Inicio;
	pListacarro tempL;
	pmarca tempmarca;
	pListacarro Lc;
	pnocarro nc;
	pcarro tempc;
	char tempn[20];
	int t;
	int troca;
	int opcao;

	printf("Organizar carros por:\n1-matricula\n2-marca\n3-modelo\n4-Sair");
	scanf("%d",&opcao);

	if (opcao == 1) {
		do{
			troca = 0;
			while (nm != NULL) {
				Lc = nm->inf;
				nc = Lc->inicio;

				while (nc != NULL) {
					if (strcmp(nc->info->matricula, nc->prox->info->matricula) > 0) {
						tempc = nc->info;
						nc->info = nc->prox->info;
						nc->prox->info = tempc;
						troca = 1;
					}
					nc = nc->prox;
				}
				nm = nm->prox;
			}
		} while (troca == 1);
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
		do {
			troca = 0;
			while (nm != NULL) {
				Lc = nm->inf;
				nc = Lc->inicio;

				while (nc != NULL) {
					if (strcmp(nc->info->modelo, nc->prox->info->modelo) > 0) {
						tempc = nc->info;
						nc->info = nc->prox->info;
						nc->prox->info = tempc;
						troca = 1;
					}
					nc = nc->prox;
				}
				nm = nm->prox;
			}
		} while (troca == 1);
	}
	else if (opcao == 4){
		return ;
	}

}

void listacarroperiodo(Listapassagem pass) {
	int opcao;
	printf("Deseja listar os carros que circularam num periodo?\n1-Sim\n2-não");
	scanf("%d", &opcao);

	if (opcao == 1) {
		pListapassagem ppass = &pass;
		pnopassagem pnpass = ppass->inicio;
		char horafim[100];
		char horainicio[100];

		printf("entre que datas os carros circularam");
		printf("Qual a data incial(formato: AAAA-MM-DD_HH:MM:SS)");
		scanf("%s", horainicio);
		printf("Qual a data final(formato: AAAA-MM-DD_HH:MM:SS)");
		scanf("%s", horafim);

		while (pnpass != NULL) {
			if (strcmp(pnpass->info->data, horainicio) > 0 && strcmp(pnpass->info->data, horafim) < 0) {
				printf("--------------------------\n");
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

void rankveiculos(Listapassagem pass,distancia d) {
	int opcao, troca;
	printf("Deseja rankear os carros que circularam num periodo pelos kilometros?\n1-Sim\n2-não");
	scanf("%d", &opcao);

	if (opcao == 1) {
		int s1, s2;
		pdistancia td= &d;
		pListapassagem ppass = &pass;
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

		while (pnpass != NULL) {
			if (strcmp(pnpass->info->data, horainicio) > 0 && strcmp(pnpass->info->data, horafim) < 0) {
				pnLpass->info = pnpass->info;
				pnLpass = pnLpass->prox;
				pLpass->numel++;
			}
			pnpass = pnpass->prox;
		}
		//Esta funçao ainda nao foi finalizada atualmente contem a criacao  e destruição de uma lista para guardar todas as passagens neste periodo agora tenho de ver como uso isso e
		pnLpass = pLpass->inicio;
		while (pnLpass != NULL) {
			pnopassagem p = pnLpass->prox;

			while (p->prox != NULL) {
				if (p->prox->info->codcarro == pnLpass->info->codcarro){
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
				else{
					p = p->prox;
				}
			}
			pnLpass = pnLpass->prox;
		}


		do{
			troca = 0;
			pnLpass = pLpass->inicio;
			while (pnLpass != NULL) {
			ppassagem ptemp;
			if (pnLpass->info->codcarro->kilometros > pnLpass->prox->info->codcarro->kilometros) {
				ptemp = pnLpass->info;
				pnLpass->info = pnLpass->prox->info;
				pnLpass->prox->info = ptemp;
				troca = 1;
					}
				}
		} while (troca == 1);

		pnLpass = pLpass->inicio;
		while (pnLpass != NULL) {
				printf("--------------------------\n");
				printf("Matricula: %s\n", pnLpass->info->codcarro->matricula);
				printf("Contribuinte do Dono: %d\n", pnLpass->info->codcarro->pdonos->numcontibuinte);
				printf("Marca: %s\n", pnLpass->info->codcarro->marca);
				printf("Modelo: %s\n", pnLpass->info->codcarro->modelo);
				printf("Ano: %d\n", pnLpass->info->codcarro->ano);
				printf("Codigo: %d\b", pnLpass->info->codcarro->codigo);
				pnLpass = pnLpass->prox;
		}

		pnLpass = pLpass->inicio;
		while (pnLpass != NULL) {
			pnopassagem ptemp;
			ptemp = pnLpass;
			pnLpass = pnLpass->prox;
			free(ptemp->info);
			free(ptemp);
		}
		free(pnLpass);
		//free(pLpass->numel);
		free(pLpass);
	}
	/*Ranking de circulação. Listagem ordenada pelo total de quilómetros
	que cada veículo efectuou na auto-estrada durante determinado período. */
}


void rankmarcas(Listapassagem pass, distancia d, marca m) {
	int opcao, troca;
	pmarca pm = &m;
	pListacarro pl;
	pnocarro pn;
	printf("Deseja rankear os carros que circularam num periodo pelos kilometros?\n1-Sim\n2-não");
	scanf("%d", &opcao);

	if (opcao == 1) {
		int s1, s2;
		pdistancia td = &d;
		pListapassagem ppass = &pass;
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

		while (pnpass != NULL) {
			if (strcmp(pnpass->info->data, horainicio) > 0 && strcmp(pnpass->info->data, horafim) < 0) {
				pnLpass->info = pnpass->info;
				pnLpass = pnLpass->prox;
				pLpass->numel++;
			}
			pnpass = pnpass->prox;
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

		pm = &m;

		do
		{
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

void listainfracao() {
	/*Listagem de infrações. Contém a matrícula dos veículos que
	circularam com velocidade média superior a 120 km/h durante determinado período*/
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

void condutorediamax() {
	//Qual o condutor(dono) que circula a maior velocidade média ?
}

void condutorpostal() {
	//Qual a velocidade média dos condutores com código postal X?
}

void marcapopular(marca m) {
	pmarca pm = &m;
	pmarca ppm = pm->prox;

	while (ppm->prox != NULL) {
		if (pm->NUmcarromarca < ppm->NUmcarromarca) {
			pm = ppm;
		}
	}

	printf("A marca mais popular é %s", pm->nome);
	//Determinar qual a marca de automóvel mais comum?
}

void exportarXl() {
	//Exportar toda a Base de Dados para o formato *.csv, para ser lido em Excel
}

void exportarXML() {
	//Exportar toda a Base de Dados para o formato *.xml
}
