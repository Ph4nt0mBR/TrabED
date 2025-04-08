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
	printf("Gostaria de adicionar mais um dono? 1-sim 0-n�o");
	scanf("%d", &opcao);
	if (opcao == 1) {
		dono *ndono =(pdonos)malloc(n->numel * sizeof(struct no));

		printf("Qual o n�mero de contribuinte?\n");
		scanf("%d", &ndono->numcontibuinte);
		printf("Qual o nome do dono?\n");//nao tenho certeza se devo meter assim o nome temos de ver depois se est� bom
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