#include "header.h"
#include "func.h"





distancias calcDistancia{
void carregar_distancias(double matriz[SAMPLE_SIZE][SAMPLE_SIZE]) {
	// Inicializa a matriz com zeros
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		for (int j = 0; j < SAMPLE_SIZE; j++) {
			matriz[i][j] = 0.0;
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
		cod1[no1 - 1] = no1;
		cod2[no2 - 1] = no2;
		matriz[cod1 - 1][cod2 - 1] = mdist;
		matriz[cod2 - 1][cod1 - 1] = mdist; // Matriz simétrica
	
	}

	fclose(arquivo);
}
}

int importdono(Listadono *ld) {
	//char token;
	const char r[5]="\n";//não sei se este nome é bom
	FILE* F = fopen("donos.txt", "r");
	if (F == NULL) {
		printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
		return 0;
	}		

	//token = strtok(str1, r);
	int COD;
	char NOME[100];
	char CP[10];
	while (!feof(F))
	{
		fscanf(F, "%d\t%[^\t]\t%[^\t]\t%[^\t]\t", &COD, NOME, CP);
		printf("COD = %d, NOMe: [%s], CP=[%s]\n", COD, NOME, CP);
		dono* ndono = (pdonos)malloc(sizeof(dono));
		ndono->numcontibuinte = COD;
		strcpy(ndono->nome, NOME);
		strcpy(ndono->codPostal, CP);
		AddDono(ld, ndono);
		/*
		dono* ndono = (pdonos)malloc(n->numel * sizeof(struct no));

		ndono->numcontibuinte = atoi(token);
		token = strtok(NULL, r);
		ndono->nome = token;
		token = strtok(NULL, r);
		ndono->codPostal = token;
		token = strtok(NULL, r);

		AddDono(ld, ndono);
		*/
	}
	fclose(F);
	return 1;
}

void importcarro() {
	FILE* ficheiro = fopen("carros.txt", "r");
	if (ficheiro == NULL) {
		printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
		return NULL;
	}
}

void importpassagem() {
	FILE* ficheiro = fopen("passagem.txt", "r");
	if (ficheiro == NULL) {
		printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
		return NULL;
	}
}

void importsensor() {
	FILE* ficheiro = fopen("sensores.txt", "r");
	if (ficheiro == NULL) {
		printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
		return NULL;
	}
}

void importdistancia() {
	FILE* ficheiro = fopen("distancias.txt", "r");
	if (ficheiro == NULL) {
		printf("\nErro ao abrir o ficheiro para leitura!!!!\n");
		return NULL;
	}
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

pListadistancia crialistadistancias() {
	pListadistancia L = (pListadistancia)malloc(sizeof(Listadistancia));
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
	L->prox = NULL;
	L->nome = nome;
	L->inf = crialistacarro();
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

void AddDono(distancia* D)
{
	pnodistancia nNo = (pnodistancia)malloc(sizeof(nodistancia));
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

void regist_veiculo(Listadono L) {
	int opcao = 0, contdono = 0;
	pListadono pL = L;
	pmarca m;
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

		while (strcmp(novoCarro->marca, m->nome) != 0 && m != NULL) {
			m = m->prox;
		}

		if (strcmp(novoCarro->marca, m->nome) == 0) {
			Lc = m->inf;
		}
		else {
			marca nmarca = criamarca(novoCarro->marca);
			addmarca(m, nmarca);
			m = m->prox;
		}
		Lc = m->inf;
		Addcarro(Lc, novoCarro);
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
			printf("Contribuinte do Dono: %d\n", c->numContribuinteDono);
			printf("Marca: %s\n", c->marca);
			printf("Modelo: %s\n", c->modelo);
			printf("Ano: %d\n", c->ano);
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

	novaPassagem->pveiculo = atualCarro->info;

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

	printf("Insira a data e hora da passagem (formato: AAAA-MM-DD_HH:MM):\n"); //pede data de passagem
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
	pno atual, temp,max = NULL;

	do {
		trocado = 0;
		atual = Ld->inicio;
		temp = NULL;

		printf("Deseja executar a organização por que ordem?\n 1- Nome do dono\n 2- Número de contribuinte\n 3- Parar organização");
			scanf(%d,opcao); 

		while (atual->prox != NULL && atual->prox =! max) {

			int precisaTrocar = 0;
			if (opcao == 3)
				return ;
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

	void import() {
		int opcao = 0;

		printf("deseja importar donos?\n1-Sim\n2-Não");
		scanf("%d", &opcao);

		if (opcao == 1) {
			importdono();
			printf("deseja importar carros?\n1-Sim\n2-Não");
			scanf("%d", &opcao);

			if (opcao == 1) {
				importcarro();
				printf("deseja importar passagens?\n1-Sim\n2-Não");
				scanf("%d", &opcao);

				if (opcao == 1) {
					importpassagem();
				}
			}
		}
		//funcao que permite importar as listas(exceto sensores e distancias)
		//pode ser feita fazendo varias mini funçoes para importar cada e usar esta para chamar elas
	}

void 
void memoria() {
	//Determinar a memoria ocupada por toda a estrutura de dados
}

void organizacarros() {
	//Listagem com a matrícula, marca e modelo (Ordenado por cada um destes atributos) de todos os veículos registados.
}

void listacarroperiodo() {
	//Listagem ordenada por matrícula dos veículos que circularam autoestrada durante o período X.
}

void rankveiculos() {
	/*Ranking de circulação. Listagem ordenada pelo total de quilómetros
	que cada veículo efectuou na auto-estrada durante determinado período. */
}

void rankmarcas() {
	/*Ranking por marca. Listagem ordenada pelo total de quilómetros que
	os veículos das diferentes marcas efectuaram na autoestrada durante determinado período. */
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

void marcapopular() {
	//Determinar qual a marca de automóvel mais comum?
}

void exportarXl() {
	//Exportar toda a Base de Dados para o formato *.csv, para ser lido em Excel
}

void exportarXML() {
	//Exportar toda a Base de Dados para o formato *.xml
}