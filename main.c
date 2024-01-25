#include <stdio.h>
#include <stdlib.h>

struct ArtikelTyp {
	char name[100];
	int art_nummer;
	int preis;
	int hoehe;
	int breite;
	int tiefe;
};

struct Aritkel {
	int artikel_typ_nummer;
	int inventarnummer;
	int positions_id;
	int stueck;
};

int main() {
	struct ArtikelTyp art_typ;

	printf("\nNeuen Artikel anlegen");

	printf("\n");
	printf("\nArtikelname (max. 100 Zeichen):");
	scanf(" %[^\n]s", art_typ.name);
	getchar();

	printf("\nArtikel Nummer:");
	do {
		scanf("%4d", &art_typ.art_nummer);
	} while (getchar() != '\n');

	printf("\nPreis (in EUR):");
	do {
		scanf("%4d", &art_typ.preis);
	} while (getchar() != '\n');

	printf("\nHoehe (in cm):");
	do {
		scanf("%4d", &art_typ.hoehe);
	} while (getchar() != '\n');

	printf("\nBreite (in cm):");
	do {
		scanf("%4d", &art_typ.breite);
	} while (getchar() != '\n');

	printf("\nTiefe (in cm):");
	do {
		scanf("%4d", &art_typ.tiefe);
	} while (getchar() != '\n');

	printf("\nArtikel Name: %s", art_typ.name);
	printf("\nArtikel Nummer: %d", art_typ.art_nummer);
	printf("\nArtikel Preis: %d", art_typ.preis);
	printf("\nArtikel Hoehe (in cm): %d", art_typ.hoehe);
	printf("\nArtikel Breite (in cm): %d", art_typ.breite);
	printf("\nArtikel Tiefe (in cm): %d", art_typ.tiefe);

	getchar();

	return 0;
}