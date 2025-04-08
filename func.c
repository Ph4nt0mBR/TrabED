#include<header.h>

void lertxt() {
		FILE* f = fopen("data/Donos.txt", "r");
		if (f == NULL) {
			printf("Erro ao abrir Donos.txt\n");
			return;
		}
		//
		while (fscanf(f, "%d %s %s",
			&donos[totalDonos].numContribuinte,
			donos[totalDonos].nome,
			donos[totalDonos].codPostal) != EOF) {
			totalDonos++;
		}

		fclose(f);
		printf("Foram lidos %d donos do ficheiro.\n", totalDonos);
}

void AddDono(Listadono* l, dono* D)
{
	pno nNo = (pno)malloc(sizeof(no));
	nNo->info = D;
	nNo->prox = l->inicio;
	l->inicio = nNo;
	l->numel++;
}

	
void regist_dono(Listadono *Ld) {
	int opcao=0;
	pListadono n;
	printf("Gostaria de adicionar mais um dono? 1-sim 0-não");
	scanf("%d", &opcao);
	if (opcao == 1) {
		dono *ndono =(pdonos)malloc(n->numel * sizeof(struct no));

		printf("Qual o número de contribuinte?\n");
		scanf("%d", &ndono->numcontibuinte);
		printf("Qual o nome do dono?\n");//nao tenho certeza se devo meter assim o nome temos de ver depois se está bom
		scanf("%s", ndono->nome);
		printf("Qual o codigo postal?\n");
		scanf("%d", &(ndono->codPostal));
		AddDono(Ld, ndono);
		/*
		pno nNo = (pno)malloc(sizeof(no));

		nNo->info = ndono;
		nNo->prox = NULL;
		if (n->inicio == NULL){
			n->inicio = nNo;
		}
		while (pont->prox != Null) {
			pont = pont->prox;
		}
		pont->prox = nNo;
		*/
	}
	if (opcao == 0) {
		return ;
	}

}

void list_dono() {
	//fazer lista de donos
}

void regist_veiculo() {
	//Registar veiculo
}

void list_veiculo() {
	//fazer lista de veiculo
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