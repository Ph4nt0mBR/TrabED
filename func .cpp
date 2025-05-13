#include "header.h"
#include "func.h"





	pdistancia calcDistancia(double matriz[SAMPLE_SIZE][SAMPLE_SIZE]) {
	// Inicializa a matriz com zeros
	pdistancia nd =  (pdistancia)malloc(sizeof(distancias));
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		for (int j = 0; j < SAMPLE_SIZE; j++) {
			nd->dist[i][j] = 0.0;
		}
	}

	// Abre o arquivo
	FILE* arquivo = fopen("distancias.txt", "r");
	if (arquivo == NULL) {
		perror("Erro ao abrir o arquivo");
		exit(EXIT_FAILURE);
	}

	// Lê cada linha e preenche a matriz
	int no1, no2;

	double distancia;

	while (fscanf(arquivo, "%d %d %lf", &no1, &no2, &distancia) == 3) {
		if (no1 < 1 || no1 > 10 || no2 < 1 || no2 > 10) {
			fprintf(stderr, "Nó inválido no arquivo: %d %d\n", no1, no2);
			continue;
		}
		nd->cod1[no1 - 1] = no1;
		nd->cod2[no2 - 1] = no2;
		nd->dist[no1 - 1][no2 - 1] = mdist;
		nd->dist[no2 - 1][no1 - 1] = mdist; // Matriz simétrica

	}

	fclose(arquivo);
}


int importdono(Listadono *ld) {
	//const char r[5]="\n";
	pListadono L = ld;
	FILE* F = fopen("donos.txt", "r");
	if (F == NULL) {
		printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
		return 0;
	}

	int COD;
	char NOME[100];
	char CP[10];
	while (!feof(F))
	{
		fscanf(F, "%d\t%[^\t]\t%[^\t]\t%[^\t]\t", &COD, NOME, CP);
		printf("COD = %d, NOMe: [%s], CP=[%s]\n", COD, NOME, CP);
		dono* ndono = (pdono)malloc(sizeof(dono));
		ndono->numcontibuinte = COD;
		strcpy(ndono->nome, NOME);
		strcpy(ndono->codPostal, CP);
		AddDono(L, ndono);
	}
	fclose(F);
	return 1;
}

int importcarro(Listadono L, marcas nm) {

	pListacarro Lc;
	//const char r[5] = "\n";
	FILE* F = fopen("carros.txt", "r");
	if (F == NULL) {
		printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
		return 0;
	}

	int cod, ano;
	char Marca[100];
	char modelo[100];
	char Mat[10];
	char ndono;

	while (!feof(F))
	{
		pmarca m = nm;
		pno ldono = L->inicio;

		fscanf(F, "%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t", Mat, Marca, modelo, &ano, ndono, &cod);
		printf("COD = %d, NOMe: [%s], CP=[%s]\n", cod, Marca, ndono);
		carro* ncarro = (pcarro)malloc(sizeof(carro));

		strcpy(ncarro->matricula,Mat);
		strcpy(ncarro->marca,Marca);
		strcpy(ncarro->modelo, modelo);
		ncarro->ano = ano;

		while (strcmp(ldono->info->nome, ndono) != 0 && ldono != NULL) {
			ldono = ldono->prox;
		}

		if (ldono == NULL) {
			printf("dono não encontrado");
			free(ncarro);
			return 0;
		}
		else {
			ncarro->pdonos = ldono->info;
		}
		ncarro->codigo = cod;
		ncarro->kilometros = 0;
		ncarro->tempototal = 0;
		while (strcmp(ncarro->marca, m->nome) != 0 && m != NULL) {
			m = m->prox;
		}
		m->Numcarrototal++;
		if (strcmp(ncarro->marca, m->nome) == 0) {
			Lc = m->inf;
		}
		else {
			marca nmarca = criamarca(ncarro->marca);
			addmarca(m, nmarca);
			m = m->prox;
			Lc = m->inf;
		}
		m->NUmcarromarca++;
		Addcarro(Lc, ncarro);
		m->inf = Lc;
	}
	fclose(F);
	return 1;
}

int importpassagem(Listapassagem L,marca m) {
	FILE* F = fopen("distancias.txt", "r");
	pmarca pm;
	pnocarro plc;
	pListapassagem Lp;
	if (F == NULL) {
		printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
		return 0;
	}

	int Id;
	int COD;
	char Data[10];
	int regist;
	while (!feof(F))
	{
		fscanf(F, "%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t", &Id, COD, Data,&regist);
		printf("COD = %d, NOMe: [%s], CP=[%s]\n", Id, COD, Data,regist);
		passagem* npass = (ppassagem)malloc(sizeof(passagem));
		npass->idsensor = Id;

		while (plc->info->codigo != COD && marca != NULL) {
			plc = pm->inf->inicio;
			while (plc->info->codigo != COD && plc != NULL) {
				plc = plc->prox;
			}
			pm = pm->prox;

		}
		if (plc->info->codigo != COD;) {
			printf("esse caro não existe ou aconteceu algum erro");
			free(npass);
			return;
		}
		npass->codcarro = plc->info;
		npass->codcarro = COD;
		strcpy(npass->data, Data);
		npass->tiporegist = regist;
		Addpassagem(Lp, npass);
	}
	fclose(F);
	return 1;
}

void importsensor(Listasensores L) {
	//const char r[5] = "\n";
	pListasensor Ls = L;
	FILE* F = fopen("sensores.txt", "r");
	if (F == NULL) {
		printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
		return 0;
	}

	int COD;
	char NOME[100];
	char Lat[16];
	char Lon[14];
	while (!feof(F))
	{
		fscanf(F, "%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t", &COD, NOME, CP);
		printf("COD = %d, NOMe: [%s], CP=[%s]\n", COD, NOME, Lat, Lon);
		sensor* nsensor = (psensor)malloc(sizeof(sensor));
		nsensor->codSensor = COD;
		strcpy(nsensor->Designacao, NOME);
		strcpy(nsensor->Latitude, Lat);
		strcpy(nsensor->Longitude, Lon);
		Addsensor(Ls, nsensor);
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

pmarca criamarca(char nome) {
	pmarca L = (pmarca)malloc(sizeof(marca));
	L->Numcarrototal = 0;
	L->NUmcarromarca = 0;
	L->prox = NULL;
	L->nome = nome;
	L->inf = crialistacarro();
	return L;
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

marca Addcarro(Listacarro* l, carro* D)
{
	pnocarro nNo = (pnocarro)malloc(sizeof(nocarro));
	nNo->info = D;
	nNo->prox = l->inicio;
	l->inicio = nNo;
	l->numel++;
	return nNo;

}

void addmarca(pmarca l, marca nmarca)
{
	l->prox = nmarca;

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
	printf("Gostaria de adicionar mais um dono? 1-sim 0-não");
	scanf("%d", &opcao);
	if (opcao == 1) {
		dono* ndono = (pdonos)malloc(n->numel * sizeof(struct no));
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
	while (ldono != Null) {
		linfdono = ldono->info;
		printf("%s\n", linfdono->codPostal);
		printf("%s\n", linfdono->nome);
		printf("%d\n", linfdono->numcontibuinte);
		ldono = ldono->prox,
	}
}

void regist_veiculo(Listadono L,marca nm) {
	int opcao = 0, contdono = 0;
	pListadono pL = L;
	pmarca m = nm;
	pListacarro Lc;
	printf("Gostaria de adicionar um veiculo? 1-Sim | 0-Nao: ");
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
		while (strcmp(ldono, contdono) != 0 && ldono != NULL){
			ldono = ldono->prox;
		}

		if (ldono == NULL) {
			printf("dono não encontrado");
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
		while (strcmp(novoCarro->marca, m->nome) != 0 && m != NULL) {
			m->Numcarrototal;
			m = m->prox;
		}

		if (strcmp(novoCarro->marca, m->nome) == 0) {
			Lc = m->inf;
		}
		else {
			marca nmarca = criamarca(novoCarro->marca);
			addmarca(m, nmarca);
			m = m->prox;
			Lc = m->inf;
		}
		novoCarro->kilometros = 0;
		novoCarro->tempototal = 0;

		novoCarro->codigo = m->Numcarrototal;
		m->NUmcarromarca++;
		Addcarro(Lc, novoCarro);
		m->inf = Lc;
		printf("Veículo adicionado com sucesso!\n");
	}
	else {
		printf("Veiculo nao adicionado.\n");
	}
}


void list_veiculo(marca m) {
	pmarca pm= m;

	if (pm == NULL) {
		printf("Nenhum veículo registrado.\n");
		return;
	}

	while (pm != Null) {
		Listacarro* Lc = pm->inf;
		pnocarro atual = Lc->inicio;

		if (atual == NULL) {
			printf("Nenhum veículo desta marca registrado.\n");
			return;
		}

		printf("Lista de veículos:\n");
		while (atual != NULL) {
			carro* c = atual->info;
			printf("--------------------------\n");
			printf("Matricula: %s\n", c->matricula);
			printf("Contribuinte do Dono: %d\n", c->pdonos->numcontibuinte);
			printf("Marca: %s\n", c->marca);
			printf("Modelo: %s\n", c->modelo);
			printf("Ano: %d\n", c->ano);
			printf("Codigo: %d\b", c->codigo);
			atual = atual->prox;
		}
		printf("-------------------------------------------------------\n");
		pm = pm->prox;
	}
}


void regist_pass(Listapassagem* Lp, Listacarro* Lc, Listasensor* Ls) {
	passagem* novaPassagem = (passagem*)malloc(sizeof(passagem));
	if (novaPassagem == NULL) {
		printf("Erro ao alocar memória para a passagem.\n");
		return;
	}

	char matricula[10];
	int codigoSensor;
	float distancia;
	char dataHora[20];

	printf("Insira a matrícula do carro:\n");
	scanf("%s", matricula);

	pnocarro atualCarro = Lc->inicio;
	while (atualCarro != NULL && strcmp(atualCarro->info->matricula, matricula) != 0) {
		atualCarro = atualCarro->prox;
	} // isto vai buscar o carro plea matricula na lista

	if (atualCarro == NULL) {
		printf("Carro não encontrado.\n");
		free(novaPassagem);
		return;
	} //senao for encontrado diz q n foi encontrado e da return

	novaPassagem->codcarro = atualCarro->info;

	printf("Insira o código do sensor:\n");
	scanf("%d", &codigoSensor);

	pnosensor atualSensor = Ls->inicio;
	while (atualSensor != NULL && atualSensor->info->codigo != codigoSensor) {
		atualSensor = atualSensor->prox;
	} //procuyra o sensor pelo codigo

	if (atualSensor == NULL) {
		printf("Sensor não encontrado.\n");
		free(novaPassagem);
		return;
	} //se o sensor n for encontrado avisa e retorna

	novaPassagem->psensor = atualSensor->info;

	printf("Insira a data e hora da passagem (formato: AAAA-MM-DD_HH:MM:SS):\n"); //pede data de passagem
	scanf("%s", dataHora);
	strcpy(novaPassagem->dataHora, dataHora);

	printf("Insira a distância percorrida (km):\n"); //pede a distancia percorrida
	scanf("%f", &distancia);
	novaPassagem->distancia = distancia;

	Addpassagem(Lp, novaPassagem);
	printf("Passagem registrada com sucesso!\n"); //adicona a passagem e avisa
}


void organizadonos(Listadono* Ld) {
	if (Ld->inicio == NULL || Ld->inicio->prox == NULL) {
		printf("Poucos ou nenhum dono registrado para ordenar.\n");
		return;
	}

	int trocado, opcao;
	pno atual, temp, max = NULL;

	do {
		trocado = 0;
		atual = Ld->inicio;
		temp = NULL;

		printf("Deseja executar a organização por que ordem?\n 1- Nome do dono\n 2- Número de contribuinte\n 3- Parar organização");
		scanf(% d, opcao);

		while (atual->prox != NULL && atual->prox = !max) {

			int precisaTrocar = 0;
			if (opcao == 3)
				return;
			else if (opcao == 1 && strcmp(atual->info->nome, atual->prox->info->nome) > 0)
				precisaTrocar = 1;
			else if (opcao == 2 && atual->info->numcontibuinte > atual->prox->info->numcontibuinte)
				precisaTrocar = 1;

			if (precisaTrocar == 1) {
				temp->info = atual->prox->info;
				atual->prox->info = atual->info;
				atual->info = temp->info;

				trocado = 1;
			}
		}
		max = atual;
	} while (trocado == 1);

}

void import(Listadono Ld, marca m, Listapassagem Lp) {
		int opcao = 0;

		printf("deseja importar donos?\n1-Sim\n2-Não");
		scanf("%d", &opcao);

		if (opcao == 1) {
			importdono(Ld);
			printf("deseja importar carros?\n1-Sim\n2-Não");
			scanf("%d", &opcao);

			if (opcao == 1) {
				importcarro(Ld,m);
				printf("deseja importar passagens?\n1-Sim\n2-Não");
				scanf("%d", &opcao);

				if (opcao == 1) {
					importpassagem(Lp);
				}
			}
		}
		//funcao que permite importar as listas(exceto sensores e distancias)
		//sao usadas varias mini funçoes para importar cada um
}

void memoria() {
	//Determinar a memoria ocupada por toda a estrutura de dados !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1












}

void organizacarros(marca m) {
	pmarca nm = m;
	pListacarro Lc;
	pnocarro nc;
	carro tempc;
	int troca;
	int opcao;

	printf("Organizar carros por:\n1-matricula\n2-marca\n3-modelo\n4-Sair");
	scanf("%d",opcao);

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
			troca = 0;
			while (nm != NULL) {
				Lc = nm->inf;
				nc = Lc->inicio;

				while (nc != NULL) {
					if (strcmp(nc->info->marca, nc->prox->info->marca) > 0) {
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
		return 0;
	}

}

void listacarroperiodo(Listapassagem pass) {
	int opcao;
	printf("Deseja listar os carros que circularam num periodo?\n1-Sim\n2-não");
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
		pdistancia td= d;
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
					pnLpass->info->codcarro->kilometros = pnLpass->info->codcarro->kilometros + td->dist(s1)(s2);
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
			pnopassagem ptemp;
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
		free(pLpass->numel);
		free(pLpass);
	}
	/*Ranking de circulação. Listagem ordenada pelo total de quilómetros
	que cada veículo efectuou na auto-estrada durante determinado período. */
}


void rankmarcas(Listapassagem pass, distancia d, marca m) {
	int opcao, troca;
	pmarca pm = m;
	pListacarro pl;
	pnocarro pn;
	printf("Deseja rankear os carros que circularam num periodo pelos kilometros?\n1-Sim\n2-não");
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
					pnLpass->info->codcarro->kilometros = pnLpass->info->codcarro->kilometros + td->dist(s1)(s2);
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

		pm = m;

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
	pmarca pm = m;
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
