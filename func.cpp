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

//fun�oes que criam listas
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

	//adiciona uma nova marca � lista eu fa�o depois j� sei como vou fazer-Afonso
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

//fun�oes que introduzem um item na sua lista
//----------------------------------------------------
//----------------------------------------------------
//-----------------------------------------------
	
void regist_dono(Listadono *Ld) {
	int opcao=0;
	pListadono n;
	printf("Gostaria de adicionar mais um dono? 1-sim 0-n�o");
	scanf("%d", &opcao);
	if (opcao == 1) {
		dono *ndono =(pdonos)malloc(n->numel * sizeof(struct no));
		if (ndono == NULL) {
			printf("Ocorreu um erro ao adicionar o dono.\n");
			return;
		}

		printf("Qual o n�mero de contribuinte?\n");
		scanf("%d", &ndono->numcontibuinte);
		printf("Qual o nome do dono?\n");//nao tenho certeza se devo meter assim o nome temos de ver depois se est� bom
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
	//fun��o que apresenta no ecr� todos os donos
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

		printf("Insira a matricula do ve�culo:\n");
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
		printf("Ve�culo adicionado com sucesso!\n");
	}
	else {
		printf("Veiculo nao adicionado.\n");
	}
}


void list_veiculo(Listacarro* Lc) {
	pnocarro atual = Lc->inicio;

	if (atual == NULL) {
		printf("Nenhum ve�culo registrado.\n");
		return;
	}

	printf("Lista de ve�culos:\n");
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
	//Listagem (ordenada pelo n�mero de contribuinte) com o respetivo n�mero e nome de todos os condutores.
}

void memoria() {
	//Determinar a memoria ocupada por toda a estrutura de dados
}

void organizacarros() {
	//Listagem com a matr�cula, marca e modelo (Ordenado por cada um destes atributos) de todos os ve�culos registados.
}

void listacarroperiodo() {
	//Listagem ordenada por matr�cula dos ve�culos que circularam autoestrada durante o per�odo X.
}

void rankveiculos() {
	/*Ranking de circula��o. Listagem ordenada pelo total de quil�metros
	que cada ve�culo efectuou na auto-estrada durante determinado per�odo. */
}

void rankmarcas() {
	/*Ranking por marca. Listagem ordenada pelo total de quil�metros que
	os ve�culos das diferentes marcas efectuaram na autoestrada durante determinado per�odo. */
}

void listainfracao() {
	/*Listagem de infra��es. Cont�m a matr�cula dos ve�culos que
	circularam com velocidade m�dia superior a 120 km/h durante determinado per�odo*/
}

void rankinfracao() {
	//Ranking de infra��es p / ve�culo.Listagem com o ve�culo e respectivo
	//n�mero total de infra��es de velocidade ocorridas durante determinado per�odo. 
}

void velocidademedia() {
	//velocidade m�dia
}

void marcamedia() {
	// Qual a marca dos carros que circulam a maior velocidade m�dia?
}

void condutorediamax() {
	//Qual o condutor(dono) que circula a maior velocidade m�dia ?
}

void condutorpostal(){
	//Qual a velocidade m�dia dos condutores com c�digo postal X?
}

void marcapopular() {
	//Determinar qual a marca de autom�vel mais comum?
}

void exportarXl() {
	//Exportar toda a Base de Dados para o formato *.csv, para ser lido em Excel
}

void exportarXML() {
	//Exportar toda a Base de Dados para o formato *.xml
}