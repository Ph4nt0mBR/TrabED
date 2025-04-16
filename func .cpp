#include "header.h"


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
		scanf("%d", &(ndono->codPostal));
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
		printf("%d\n", linfdono->codPostal);
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


void regist_pass(Listapassagem* Lp, marca* Lm, Listasensor* Ls) {
	passagem* novaPassagem = (passagem*)malloc(sizeof(passagem));
	if (novaPassagem == NULL) {
		printf("Erro ao alocar memória para a passagem.\n");
		return;
	}

	char matricula[10],marca[10];
	int codigoSensor;
	float distancia;
	char dataHora[20];

	printf("Insira a matrícula do carro:\n");
	scanf("%s", marca);

	pmarca atualmarca;
	while (atualmarca != NULL && strcmp(atualmarca->nome, marca) != 0) {
		atualmarca = atualmarca->prox;
	} // isto vai buscar a marca na lista

	if (atualmarca == NULL) {
		printf("Marca não encontrada.\n");
		free(novaPassagem);
		return;
	} //senao for encontrada diz q n foi encontrado e da return

	pListacarro Lc = atualmarca->info;
	
	printf("Insira a matrícula do carro:\n");
	scanf("%s", matricula);

	pnocarro atualCarro = Lc->inicio;
	while (atualCarro != NULL && strcmp(atualCarro->info->matricula, matricula) != 0) {
		atualCarro = atualCarro->prox;
	} // isto vai buscar o carro pela matricula na lista

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

	//falta as distancias !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	Addpassagem(Lp, novaPassagem);
	printf("Passagem registrada com sucesso!\n"); //adicona a passagem e avisa
}


void organizadonos() {
	// Verifica se a lista esta vazia
	if (Ld->inicio == NULL) {
		printf("Nenhum dono registrado.\n");
		return;
	}

	
	pdono* donos = (pdono*)malloc(Ld->numel * sizeof(pdono));
	if (donos == NULL) {
		printf("Erro ao alocar memória.\n");
		return;
	} // Aloca array para armazenar ponteiros para donos

	
	int i = 0;
	pno atual = Ld->inicio;
	while (atual != NULL) {
		donos[i++] = atual->info;
		atual = atual->prox;
	} //preenche o array com os ponteiros dos donos da lista

	//ordena por nome
	for (int x = 0; x < Ld->numel - 1; x++) {
		for (int y = 0; y < Ld->numel - x - 1; y++) {
			if (strcmp(donos[y]->nome, donos[y + 1]->nome) > 0) {
				pdono temp = donos[y];
				donos[y] = donos[y + 1];
				donos[y + 1] = temp;
			}
		}
	}

	//mostra lista ordenada opr nome
	printf("\n--- Donos ordenados por nome ---\n");
	for (i = 0; i < Ld->numel; i++) {
		printf("Nome: %s | NIF: %d | Código Postal: %d\n", donos[i]->nome, donos[i]->numcontibuinte, donos[i]->codPostal);
	}

	//ordena por n de contribuinte
	for (int x = 0; x < Ld->numel - 1; x++) {
		for (int y = 0; y < Ld->numel - x - 1; y++) {
			if (donos[y]->numcontibuinte > donos[y + 1]->numcontibuinte) {
				pdono temp = donos[y];
				donos[y] = donos[y + 1];
				donos[y + 1] = temp;
			}
		}
	}

	//mostra lista ordenada por contribuinte
	printf("\n--- Donos ordenados por número de contribuinte ---\n");
	for (i = 0; i < Ld->numel; i++) {
		printf("NIF: %d | Nome: %s\n", donos[i]->numcontibuinte, donos[i]->nome);
	}

	free(donos);
}

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