#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	printf("ID: %d \n", masina.id);
	printf("nrUsi: %d \n", masina.nrUsi);
	printf("pret: %.2f \n", masina.pret);
	printf("model: %s \n", masina.model);
	printf("numeSofer: %s \n", masina.numeSofer);
	printf("serie: %c \n\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* temp;
	temp = malloc(sizeof(Masina) * ((*nrMasini) + 1));

	for (int i = 0; i < *nrMasini; i++)
	{
		temp[i] = (*masini)[i];

	}
	temp[(*nrMasini)] = masinaNoua;
	(*nrMasini)++;
	free(*masini);
	(*masini) = temp;
}


Masina citireMasinaFisier(FILE* file) {
	char sep[] = ", \n";
	char line[101];

	if (!fgets(line, sizeof(line), file)) {
		printf("Eroare: linie goala sau final de fisier.\n");
		Masina masinaGoala = { 0, 0, 0.0, NULL, NULL, 0 };
		return masinaGoala;
	}

	Masina temp;

	
	char* token = strtok(line, sep);
	if (!token) { printf("Eroare la citirea ID-ului!\n"); return temp; }
	temp.id = atoi(token);

	token = strtok(NULL, sep);
	if (!token) { printf("Eroare la citirea nrUsi!\n"); return temp; }
	temp.nrUsi = atoi(token);

	token = strtok(NULL, sep);
	if (!token) { printf("Eroare la citirea pretului!\n"); return temp; }
	temp.pret = atof(token);

	token = strtok(NULL, sep);
	if (!token) { printf("Eroare la citirea modelului!\n"); return temp; }
	temp.model = (char*)malloc(strlen(token) + 1);
	if (!temp.model) { printf("Eroare la alocarea memoriei pentru model!\n"); return temp; }
	strcpy(temp.model, token);

	token = strtok(NULL, sep);
	if (!token) { printf("Eroare la citirea numelui soferului!\n"); return temp; }
	temp.numeSofer = (char*)malloc(strlen(token) + 1);
	if (!temp.numeSofer) { printf("Eroare la alocarea memoriei pentru numeSofer!\n"); return temp; }
	strcpy(temp.numeSofer, token);

	token = strtok(NULL, sep);
	if (!token) { printf("Eroare la citirea seriei!\n"); return temp; }
	temp.serie = token[0];

	return temp;
}


Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file;
	file= fopen(numeFisier, "r");
	if (!file)
	{
		printf("Fisierul nu s-a putut deschide");
		return NULL;
	}
	Masina* vec = NULL;
	while (!feof(file))
	{
		Masina masinaNoua=citireMasinaFisier(file);
		adaugaMasinaInVector(&vec, nrMasiniCitite, masinaNoua);
	}
	fclose(file);
	return vec;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
	for (int i = 0; i <(*nrMasini); i++)
	{
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	*nrMasini = 0;
	free(*vector);
	*vector == NULL;

}

int main() {
	int nr = 0;
	Masina* vectorMasini = NULL;
	vectorMasini=citireVectorMasiniFisier("masini.txt", &nr);
	afisareVectorMasini(vectorMasini, nr);
	dezalocareVectorMasini(&vectorMasini, &nr);
	afisareVectorMasini(vectorMasini, nr);
	return 0;
}