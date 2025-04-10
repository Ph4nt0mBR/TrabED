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

void addmarca(l
{

	//adiciona uma nova marca á lista eu faço depois já sei como vou fazer-Afonso
}

void Addsensor(Listasensor* l, sensor* D)
{
	pnosensor nNo = (pnosensor)malloc(sizeof(nosensor));
	nNo->info = D;
	nNo->prox = l->inicio;
	l->inicio = nNo;
	l->numel++;

}

void AddDono(Listadistancia* l, distancia* D)
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
	
void regist_dono(Listadono *Ld) {
	int opcao=0;
	pListadono n;
	printf("Gostaria de adicionar mais um dono? 1-sim 0-não");
	scanf("%d", &opcao);
	if (opcao == 1) {
		dono *ndono =(pdonos)malloc(n->numel * sizeof(struct no));
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
		return ;
	}
}

void list_dono(Listadono *Ld) {
	//função que apresenta no ecrã todos os donos
	pdono linfdono;
	pno ldono = Ld->inicio;
	while (ldono != Null) {
		linfdono = ldono->info;
		printf("%d\n",linfdono->codPostal );
		printf("%s\n",linfdono->nome );
		printf("%d\n", linfdono->numcontibuinte);
		ldono=ldono->prox,
	}
}

void regist_veiculo() {
	int opcao = 0;
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
		scanf("%d", &novoCarro->numContribuinteDono);

		printf("Insira a marca do carro:\n");
		scanf("%s", novoCarro->marca);

		printf("Insira o modelo do carro:\n");
		scanf("%s", novoCarro->modelo);

		printf("Insira o ano de producao do carro:\n");
		scanf("%d", &novoCarro->ano);

		while (strcmp(novoCarro->marca, m->nome)!=0 && m!=NULL) {
			m = m->prox;
		}

		if (strcmp(novoCarro->marca, m->nome) == 0) {
			Lc = m->inf;
		}
		else {
			marca nmarca = criamarca(novoCarro->marca);
			addmarca(m,nmarca);
			m->

		}

		Addcarro(Lc, novoCarro);
		printf("Veículo adicionado com sucesso!\n");
	}
	else {
		printf("Veiculo nao adicionado.\n");
	}
}


void list_veiculo(Listacarro* Lc) {
	pnocarro atual = Lc->inicio;

	if (atual == NULL) {
		printf("Nenhum veículo registrado.\n");
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
}


void regist_pass() {
	//registrar passagem
}

void organizadonos() {
	//Listagem (ordenada alfabeticamente) com o nome de todos os condutores
	//Listagem (ordenada pelo número de contribuinte) com o respetivo número e nome de todos os condutores.
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

void condutorpostal(){
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