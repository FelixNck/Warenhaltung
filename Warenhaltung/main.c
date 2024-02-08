#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HALLE_20 18000//vorhandene Anzahl Positions IDs für 20cm hohe Fächer
#define HALLE_40 54000
#define PORTA_20 34800
#define PORTA_40 6000
#define PORTA_80 7800


// StartIDs zum Zaehlen; Halle = Lager 1; PW = Lager 2
#define START_ID_HALLE_20CM 12000000
#define START_ID_HALLE_40CM 14000000
#define START_ID_PORTA_20CM 22000000
#define START_ID_PORTA_40CM 2400000
#define START_ID_PORTA_80CM 2800000

// Maximale Anzahl von Artikeln
#define MAX_ARTIKEL 400

int menue();
int neuen_artikel_anlegen();
int artikel_bearbeiten();
int details_waehlen_bearbeitung(struct ArtikelTyp* artikel);
int artikel_loeschen();
int artikel_erfassen();
int artikel_einlagern_nach_nummer(int eingabeNummer, int lager, struct Artikel artikel);
// artikel_einlagern_nach_name(char eingabeName[], int lager);
int lagere_artikel_an_positions_ids_Halle(struct Artikel *artikel);
int lagere_artikel_an_positions_ids_Porta(struct Artikel *artikel);
int print_zugeordnete_ids(struct Artikel *artikel);
int vorhandene_artikel_ansehen();
int lager_aktualisieren(struct ArtikelTyp *artikel);
void bs_loeschen(); // Funktion zum leeren der Konsolenausgabe
void strtrim(char* str);    // Leerzeichen am Anfang und Ende einer Eingabe ignorieren/wegschneiden

struct PositionsID {
	int id;
	int resthoehe;
	int positions_id_voll;
	int artikelnummer; // Artikelnummer an dieser Position
};

struct PositionsID *ptr_positions_id;

struct ArtikelTyp {
	char name[100];
	int art_nummer;
	double preis;
	double hoehe;
	double breite;
	double tiefe;
	int lager;
};

struct Artikel {
	struct ArtikelTyp *typ;
	int inventarnummer;
	struct PositionsID positions[60]; // Pointer auf PositionsID
	int anzahl_positions_ids;
};

struct Lager {
	struct ArtikelTyp artikel_liste[MAX_ARTIKEL];
	int anzahl_artikel;
};

// Liste für bereits angelegte Artikel
// Kann das hier nicht nur ein int artikel_liste[MAX_ARTIKEL] sein?
struct ArtikelTyp artikel_liste[MAX_ARTIKEL];

struct Lager halle_lager;
struct Lager porta_lager;

//Enumeration, gibt HALLE Wert 1 und PW Wert 2
enum LagerTyp {
	HALLE = 1,  //verderblich
	PORTA_WESTFALICA = 2    //nicht verderblich
};

// Anzahl der bereits vorhandenen Artikel
int anzahl_artikel = 0;

int inventarnummerZaehler = 1000;
struct PositionsID belegte_ids_halle_20[HALLE_20]; // Array für belegte IDs im Halle-Lager mit 20cm Höhe
struct PositionsID belegte_ids_halle_40[HALLE_40]; // Array für belegte IDs im Halle-Lager mit 40cm Höhe
struct PositionsID belegte_ids_porta_20[PORTA_20]; // Array für belegte IDs im Porta-Lager mit 20cm Höhe
struct PositionsID belegte_ids_porta_40[PORTA_40]; // Array für belegte IDs im Porta-Lager mit 40cm Höhe
struct PositionsID belegte_ids_porta_80[PORTA_80]; // Array für belegte IDs im Porta-Lager mit 80cm Höhe

int main(void) {
	int auswahl;

	// Initialisierung der Lager
	halle_lager.anzahl_artikel = 0;
	porta_lager.anzahl_artikel = 0;

	while (1) {
		menue();

		scanf("%d", &auswahl);

		switch (auswahl) {
		case 1:
			neuen_artikel_anlegen();
			break;
		case 2:
			artikel_bearbeiten();
			break;
		case 3:
			artikel_loeschen();
			break;
		case 4:
			artikel_erfassen();
			break;
		case 7:
			vorhandene_artikel_ansehen();
			break;
		default:
			printf("\nFalsche Eingabe. Waehle eine der oben aufgelisteten Moeglichkeiten!\n");
		}
	}

	return 0;
}

// Hier wird das Menue ausgegeben
int menue() {
	bs_loeschen();
	printf("  Halle\t\t\tPorta Westfalica");
	printf("\n  %d Artikel\t\t%d Artikel", halle_lager.anzahl_artikel, porta_lager.anzahl_artikel);
	printf("\n  Belegung:\t\tBelegung:");
	printf("\n  20cm: 40 %%            20cm: 5 %%");
	printf("\n  40cm: 14 %%            40cm: 51 %%");
	printf("\n  80cm: 79 %%");
	printf("\n");
	printf("\nWarenausgang");
	printf("\nHalle: 5 Artikel      Porta Westfalica: 6 Artikel");
	printf("\n");
	printf("\n(1)   Neuen Artikel anlegen");
	printf("\n(2)   Artikel bearbeiten");
	printf("\n(3)   Artikel loeschen");
	printf("\n(4)   Artikel erfassen");
	printf("\n(5)   Artikel entfernen");
	printf("\n(6)   Artikel umraeumen");
	printf("\n(7)   Alle Artikel ansehen");
	printf("\n(8)   Lagerbestand zufaellig befuellen");
	printf("\n(9)   Zufaellige Bestellung erzeugen");
	printf("\n(10)  Manuelle Bestellung erzeugen");
	printf("\n(11)  Erfasste Bestellungen versenden");
	printf("\n(12)  Erfasste Bestellungen abbrechen");
	printf("\n");
	printf("\nWaehle eine Auswahlmoeglichkeit:");
	printf("\n");

	return 0;
}

int neuen_artikel_anlegen() {
	struct ArtikelTyp artikeltyp;
	int lagerwahl = -1;
	int wahl = 0;
	int i;

	bs_loeschen();
	printf("Neuen Artikel anlegen");

	printf("\n");
	printf("\nArtikelname (max. 100 Zeichen): ");
	// Leere den Eingabepuffer nach der vorherigen Eingabe
	while (getchar() != '\n');  // verhindert, dass leere Zeile eingelesen wird
	fgets(artikeltyp.name, sizeof(artikeltyp.name), stdin); // kann Leerzeichen/ganze Zeile einlesen
	// Entferne das Zeilenumbruchzeichen am Ende der Eingabe
	artikeltyp.name[strcspn(artikeltyp.name, "\n")] = '\0';

	strtrim(artikeltyp.name); //löscht Leerzeichen hinter/vor Name
	// Überprüfen, ob der Artikelname bereits existiert
	for (i = 0; i < anzahl_artikel; i++) {
		if (strcmp(artikeltyp.name, artikel_liste[i].name) == 0) {
			printf("\nFehler: Ein Artikel mit diesem Namen existiert bereits. Druecke Enter, um zurueck in das Menue zu kommen.\n");
			while (getchar() != '\n');
			getchar();
			return 0; // Rückkehr, da Fehler aufgetreten ist
		}
	}

	printf("\nArtikel Nummer: ");
	scanf("%d", &artikeltyp.art_nummer);

	// Überprüfen, ob die Artikelnummer bereits existiert
	for (i = 0; i < anzahl_artikel; i++) {
		if (artikeltyp.art_nummer == artikel_liste[i].art_nummer) {
			printf("\nFehler: Artikel mit dieser Nummer existiert bereits. Druecke Enter, um zurueck in das Menue zu kommen.\n");
			while (getchar() != '\n');
			getchar();
			return 0; // Rückkehr, da Fehler aufgetreten ist
		}
	}


	printf("\nPreis (in EUR): ");
	scanf("%lf", &artikeltyp.preis);

	do {
		printf("\nHoehe (in cm; wenn verderblich <= 40, sonst <= 80): ");
		scanf("%lf", &artikeltyp.hoehe);
		if (artikeltyp.hoehe > 80)
			printf("Fehler: Falsche Hoehe! Geben Sie bitte eine gueltige Hoehe ein!\n");
	} while (artikeltyp.hoehe > 80);

	do {
		printf("\nBreite (in cm, <= 300): ");
		scanf("%lf", &artikeltyp.breite);
		if (artikeltyp.breite > 300)
			printf("Fehler: Die Breite darf nicht groesser als 300 cm sein. Geben Sie bitte einen gueltigen Wert ein!\n");
	} while (artikeltyp.breite > 300);

	do {
		printf("\nTiefe (in cm, <= 120): ");
		scanf("%lf", &artikeltyp.tiefe);
		if (artikeltyp.tiefe > 120)
			printf("Fehler: Die Tiefe darf nicht groesser als 120 cm sein. Geben Sie bitte einen gueltigen Wert ein!\n");
	} while (artikeltyp.tiefe > 120);


	do {
		printf("\nWeisen Sie dem Artikel nun noch zu, ob er verderblich ist oder nicht.\nVerderblich: (1)\nNicht verderblich: (2)\n");
		scanf("%d", &lagerwahl);
		if (artikeltyp.hoehe > 40 && lagerwahl == HALLE) {
			printf("Fehler: Artikel mit einer Hoehe ueber 40 cm koennen nicht in Halle an der Saale eingelagert werden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!\n");
			getchar(); // Eingabepuffer leeren
			getchar(); // Warten, bis der Benutzer fortfährt
			return 0;
		}
		else if (lagerwahl != HALLE && lagerwahl != PORTA_WESTFALICA) {
			printf("Fehler: Ungueltige Eingabe. Bitte waehlen Sie eine der angegebenen Optionen.\n");
		}
	} while (lagerwahl != HALLE && lagerwahl != PORTA_WESTFALICA);

	switch (lagerwahl) {
	case HALLE:
		// Artikel ist verderblich, Lagerort entsprechend festlegen und verarbeiten
		halle_lager.artikel_liste[halle_lager.anzahl_artikel] = artikeltyp;
		halle_lager.anzahl_artikel++;
		artikeltyp.lager = 1;
		printf("\nDer Artikel wurde erfolgreich in Halle an der Saale angelegt.\nDruecken Sie Enter, um fortzufahren.\n");
		break;
	case PORTA_WESTFALICA:
		// Artikel ist nicht verderblich, Lagerort entsprechend festlegen und verarbeiten
		porta_lager.artikel_liste[porta_lager.anzahl_artikel] = artikeltyp;
		porta_lager.anzahl_artikel++;
		artikeltyp.lager = 2;
		printf("\nDer Artikel wurde erfolgreich in Porta Westfalica angelegt.\nDruecken Sie Enter, um fortzufahren.\n");
		break;
	default:
		printf("\nUngueltige Eingabe. Bitte waehlen Sie eine der angegebenen Optionen.");
		break;
	}

	while (getchar() != '\n');
	getchar();

	bs_loeschen();

	// Vielleicht auch in artikel_anzeigen() Funktion bauen?
	printf("Artikel wurde erfolgreich angelegt. Druecke Enter, um zum Menue zu kommen!");
	printf("\n");
	printf("\nArtikel Name: %s", artikeltyp.name);
	printf("\nArtikel Nummer: %d", artikeltyp.art_nummer);
	printf("\nArtikel Preis: %.2lf", artikeltyp.preis);
	printf("\nArtikel Hoehe (in cm): %.2lf", artikeltyp.hoehe);
	printf("\nArtikel Breite (in cm): %.2lf", artikeltyp.breite);
	printf("\nArtikel Tiefe (in cm): %.2lf", artikeltyp.tiefe);
	// Lagerort anzeigen lassen

	// Hinzufügen des neuen Artikels zur Liste
	artikel_liste[anzahl_artikel] = artikeltyp;
	anzahl_artikel++;

	while (getchar() != '\n');
	getchar();

	return 0;
}



int artikel_bearbeiten() {
	int auswahl;
	char suchname[100];
	int suchnummer;
	int gefunden = 0;
	int i;

	bs_loeschen();
	printf("Artikel bearbeiten");
	printf("\n");
	printf("\nMoechten Sie den Artikel anhand der Artikelnummer (1) oder des Namens (2) suchen? ");
	scanf("%d", &auswahl);

	switch (auswahl) {
	case 1:
		printf("\nBitte geben Sie die Artikelnummer ein: ");
		scanf("%d", &suchnummer);
		for (i = 0; i < anzahl_artikel; i++) {
			if (artikel_liste[i].art_nummer == suchnummer) {
				// Artikel gefunden
				printf("\nArtikel gefunden:\n");
				printf("\nArtikel Name: %s", artikel_liste[i].name);
				printf("\nArtikel Nummer: %d", artikel_liste[i].art_nummer);
				printf("\nArtikel Preis: %.2lf", artikel_liste[i].preis);
				printf("\nArtikel Hoehe (in cm): %.2lf", artikel_liste[i].hoehe);
				printf("\nArtikel Breite (in cm): %.2lf", artikel_liste[i].breite);
				printf("\nArtikel Tiefe (in cm): %.2lf", artikel_liste[i].tiefe);
				printf("\n");

				// Code für Änderung Details des Artikels
				details_waehlen_bearbeitung(&artikel_liste[i]); // Details waehlen ausgelagert, um richtiges Speichern zu ermoeglichen und fuer Uebersichtlichkeit
				gefunden = 1;
				break;
			}
		}
		if (!gefunden) {
			printf("\nEin Artikel mit dieser Nummer wurde nicht gefunden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!"); //vielleicht zu Auswahl Name oder Nummer suchen zurueckkehren??
			while (getchar() != '\n');
			getchar();
		}
		break;
	case 2:
		printf("\nBitte geben Sie den Artikelnamen ein: ");
		getchar(); // Leertaste nach vorheriger Eingabe abfangen
		fgets(suchname, sizeof(suchname), stdin); // Einlesen des Namens mit Leerzeichen
		suchname[strcspn(suchname, "\n")] = 0; // Entfernen des Zeilenumbruchs
		strtrim(suchname);
		for (i = 0; i < anzahl_artikel; i++) {
			if (strcmp(artikel_liste[i].name, suchname) == 0) {
				// Artikel gefunden
				printf("\nArtikel gefunden:\n");
				printf("\nArtikel Name: %s", artikel_liste[i].name);
				printf("\nArtikel Nummer: %d", artikel_liste[i].art_nummer);
				printf("\nArtikel Preis: %.2lf", artikel_liste[i].preis);
				printf("\nArtikel Hoehe (in cm): %.2lf", artikel_liste[i].hoehe);
				printf("\nArtikel Breite (in cm): %.2lf", artikel_liste[i].breite);
				printf("\nArtikel Tiefe (in cm): %.2lf", artikel_liste[i].tiefe);
				printf("\n");

				// Code für die Änderung Details Artikel
				details_waehlen_bearbeitung(&artikel_liste[i]);
				gefunden = 1;
				break;
			}
		}
		if (!gefunden) {
			printf("\nEin Artikel mit diesem Namen wurde nicht gefunden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");  //vielleicht zu Auswahl Name oder Nummer suchen zurueckkehren??
			while (getchar() != '\n');
			getchar();
		}
		break;
	default:
		printf("\nUngueltige Auswahl.");
		break;
	}

	return 0;
}

int details_waehlen_bearbeitung(struct ArtikelTyp* artikel) {
	int weitere_aenderungen = 1;
	int auswahl;
	struct ArtikelTyp temp_artikel = *artikel; // Temporäre Kopie des Artikels
	int i;

	do {
		printf("\nWelches Detail moechten Sie bearbeiten?");
		printf("\n(1) Name");
		printf("\n(2) Artikelnummer");
		printf("\n(3) Preis");
		printf("\n(4) Hoehe");
		printf("\n(5) Breite");
		printf("\n(6) Tiefe");
		printf("\n");

		scanf("%d", &auswahl);
		switch (auswahl) {
		case 1:
			printf("\nBitte geben Sie den neuen Namen ein: ");
			getchar(); // Leertaste nach vorheriger Eingabe abfangen
			fgets(temp_artikel.name, sizeof(temp_artikel.name), stdin); // Einlesen des Namens mit Leerzeichen
			temp_artikel.name[strcspn(temp_artikel.name, "\n")] = 0; // Entfernen des Zeilenumbruchs
			strtrim(temp_artikel.name);
			// Überprüfung, ob der Name bereits existiert
			for (i = 0; i < anzahl_artikel; i++) {
				if (strcmp(temp_artikel.name, artikel_liste[i].name) == 0 && temp_artikel.art_nummer != artikel_liste[i].art_nummer) {
					printf("\nFehler: Der eingegebene Name existiert bereits in einem anderen Artikel.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
					while (getchar() != '\n');
					getchar();
					return 1;
				}
			}
			break;
		case 2:
			printf("\nBitte geben Sie die neue Artikelnummer ein: ");
			scanf("%d", &temp_artikel.art_nummer);
			// Überprüfung, ob die Artikelnummer bereits existiert
			for (i = 0; i < anzahl_artikel; i++) {
				if (temp_artikel.art_nummer == artikel_liste[i].art_nummer && strcmp(temp_artikel.name, artikel_liste[i].name) != 0) {
					printf("\nFehler: Die eingegebene Artikelnummer existiert bereits.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
					while (getchar() != '\n');
					getchar();
					return 1;
				}
			}
			break;
		case 3:
			printf("\nBitte geben Sie den neuen Preis ein: ");
			scanf("%lf", &temp_artikel.preis);
			break;
		case 4:
			printf("\nBitte geben Sie die neue Hoehe ein: ");
			scanf("%lf", &temp_artikel.hoehe);
			break;
		case 5:
			printf("\nBitte geben Sie die neue Breite ein: ");
			scanf("%lf", &temp_artikel.breite);
			break;
		case 6:
			printf("\nBitte geben Sie die neue Tiefe ein: ");
			scanf("%lf", &temp_artikel.tiefe);
			break;
		default:
			printf("\nUngueltige Auswahl.");
			break;
		}

		printf("\nArtikel Name: %s", temp_artikel.name);
		printf("\nArtikel Nummer: %d", temp_artikel.art_nummer);
		printf("\nArtikel Preis: %.2lf", temp_artikel.preis);
		printf("\nArtikel Hoehe (in cm): %.2lf", temp_artikel.hoehe);
		printf("\nArtikel Breite (in cm): %.2lf", temp_artikel.breite);
		printf("\nArtikel Tiefe (in cm): %.2lf", temp_artikel.tiefe);
		printf("\n");

		printf("\nMoechten Sie weitere Details bearbeiten? (1 = Ja, 0 = Nein): ");
		scanf("%d", &weitere_aenderungen);
	} while (weitere_aenderungen);

	printf("\nMoechten Sie diese Aenderungen speichern? (1 = Ja, 0 = Nein): ");
	scanf("%d", &auswahl);
	if (auswahl == 1) {
		printf("\nDie Aenderungen wurden erfolgreich gespeichert.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
		while (getchar() != '\n');
		getchar();
		*artikel = temp_artikel; // Aktualisierung des Originalartikels mit den Änderungen aus der temporären Kopie
		lager_aktualisieren(artikel); // Funktion zur Aktualisierung der Änderungen im Lager 
	}

	return 0;
}

int artikel_loeschen() {
	int artikel_nummer;
	int i;

	bs_loeschen();

	printf("Geben Sie die Artikelnummer des zu loeschenden Artikels ein: ");
	scanf("%d", &artikel_nummer);

	// Sucht nach dem Artikel in den Lagerlisten und löscht ihn, wenn j
	for (i = 0; i < halle_lager.anzahl_artikel; i++) {
		if (halle_lager.artikel_liste[i].art_nummer == artikel_nummer) {
			// Artikel gefunden, zeigt die Details an
			printf("\nArtikel Details:\n");
			printf("Name: %s\n", halle_lager.artikel_liste[i].name);
			printf("Artikelnummer: %d\n", halle_lager.artikel_liste[i].art_nummer);
			printf("Preis: %.2f EUR\n", halle_lager.artikel_liste[i].preis);
			printf("Hoehe: %.2f cm\n", halle_lager.artikel_liste[i].hoehe);
			printf("Breite: %.2f cm\n", halle_lager.artikel_liste[i].breite);
			printf("Tiefe: %.2f cm\n", halle_lager.artikel_liste[i].tiefe);

			// Bestätigung vom Benutzer einholen
			char antwort;
			printf("Moechten Sie diesen Artikel wirklich loeschen? (j/n): ");
			scanf(" %c", &antwort);

			if (antwort == 'j' || antwort == 'J') {
				// Artikel löschen aus Lager in Halle
				for (int j = i; j < halle_lager.anzahl_artikel - 1; j++) {
					halle_lager.artikel_liste[j] = halle_lager.artikel_liste[j + 1];
				}
				halle_lager.anzahl_artikel--;

				// Artikel aus Artikelliste löschen
				for (int k = 0; k < anzahl_artikel; k++) {
					if (artikel_liste[k].art_nummer == artikel_nummer) {
						for (int l = k; l < anzahl_artikel - 1; l++) {
							artikel_liste[l] = artikel_liste[l + 1];
						}
						anzahl_artikel--;
						break;
					}
				}

				printf("Artikel erfolgreich geloescht.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return 1;
			}
			else {
				printf("Loeschvorgang abgebrochen.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return 0;
			}
		}
	}

	for (i = 0; i < porta_lager.anzahl_artikel; i++) {
		if (porta_lager.artikel_liste[i].art_nummer == artikel_nummer) {
			// Artikel gefunden, zeigt die Details an
			printf("\nArtikel Details:\n");
			printf("Name: %s\n", porta_lager.artikel_liste[i].name);
			printf("Artikelnummer: %d\n", porta_lager.artikel_liste[i].art_nummer);
			printf("Preis: %.2f EUR\n", porta_lager.artikel_liste[i].preis);
			printf("Hoehe: %.2f cm\n", porta_lager.artikel_liste[i].hoehe);
			printf("Breite: %.2f cm\n", porta_lager.artikel_liste[i].breite);
			printf("Tiefe: %.2f cm\n", porta_lager.artikel_liste[i].tiefe);

			// Bestätigung vom Benutzer einholen
			char antwort;
			printf("Moechten Sie diesen Artikel wirklich loeschen? (j/n): ");
			scanf(" %c", &antwort);

			if (antwort == 'j' || antwort == 'J') {
				// Artikel löschen aus Lager PW
				for (int j = i; j < porta_lager.anzahl_artikel - 1; j++) {
					porta_lager.artikel_liste[j] = porta_lager.artikel_liste[j + 1];
				}
				porta_lager.anzahl_artikel--;

				// Artikel aus Artikelliste (wo alle Artikel drin stehen) löschen
				for (int k = 0; k < anzahl_artikel; k++) {
					if (artikel_liste[k].art_nummer == artikel_nummer) {
						for (int l = k; l < anzahl_artikel - 1; l++) {
							artikel_liste[l] = artikel_liste[l + 1];
						}
						anzahl_artikel--;
						break;
					}
				}

				printf("Artikel erfolgreich geloescht.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return 1;
			}
			else {
				printf("Loeschvorgang abgebrochen.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return 0;
			}
		}
	}

	// Artikel nicht gefunden
	printf("Artikel mit der Nummer %d wurde nicht gefunden.\n", artikel_nummer);
	return 0;
}


// Funktion zum Anzeigen der vorhandenen Artikel und gleichzeitigen Aktualisierung des Lagerbestands
int vorhandene_artikel_ansehen() {
	int lagerwahl = 0;
	int i;

	bs_loeschen();
	printf("Welches Lager moechten Sie anzeigen?");
	printf("\n(1) HALLE Lager");
	printf("\n(2) PORTA WESTFALICA Lager");
	printf("\n");

	scanf("%d", &lagerwahl);

	switch (lagerwahl) {
	case HALLE:
		printf("\nArtikel im HALLE Lager:\n\n");
		for (i = 0; i < halle_lager.anzahl_artikel; i++) {
			struct ArtikelTyp artikeltyp = halle_lager.artikel_liste[i];
			printf("\nArtikel Name: %s", artikeltyp.name);
			printf("\nArtikel Nummer: %d", artikeltyp.art_nummer);
			printf("\nArtikel Preis: %.2lf", artikeltyp.preis);
			printf("\nArtikel Hoehe (in cm): %.2lf", artikeltyp.hoehe);
			printf("\nArtikel Breite (in cm): %.2lf", artikeltyp.breite);
			printf("\nArtikel Tiefe (in cm): %.2lf", artikeltyp.tiefe);
			printf("\n");


		}
		break;
	case PORTA_WESTFALICA:
		printf("\nArtikel im PORTA WESTFALICA Lager:\n\n");
		for (i = 0; i < porta_lager.anzahl_artikel; i++) {
			struct ArtikelTyp artikeltyp = porta_lager.artikel_liste[i];
			printf("\nArtikel Name: %s", artikeltyp.name);
			printf("\nArtikel Nummer: %d", artikeltyp.art_nummer);
			printf("\nArtikel Preis: %.2lf", artikeltyp.preis);
			printf("\nArtikel Hoehe (in cm): %.2lf", artikeltyp.hoehe);
			printf("\nArtikel Breite (in cm): %.2lf", artikeltyp.breite);
			printf("\nArtikel Tiefe (in cm): %.2lf", artikeltyp.tiefe);
			printf("\n");
		}
		break;
	default:
		printf("\nUngueltige Eingabe. Bitte waehlen Sie eine der angegebenen Optionen.");
		break;
	}

	printf("\n\nDruecke Enter, um zum Hauptmenue zurueckzukehren.");
	while (getchar() != '\n');
	getchar();
}

// Aktualisierung der Änderungen am Artikel im Lager
int lager_aktualisieren(struct ArtikelTyp* artikel) {
	int i;
	for (i = 0; i < anzahl_artikel; i++) {
		if (halle_lager.artikel_liste[i].art_nummer == artikel->art_nummer) {
			halle_lager.artikel_liste[i] = *artikel;
			break;
		}
	}

	for (i = 0; i < anzahl_artikel; i++) {
		if (porta_lager.artikel_liste[i].art_nummer == artikel->art_nummer) {
			porta_lager.artikel_liste[i] = *artikel;
			break;
		}
	}
}

// Funktion zum Suchen, Anzeigen und einlagern (dazu ists noch nicht gekommen) des Artikels
int artikel_erfassen() {
	int gefunden = 0;
	int eingabeNummer;
	char eingabeName[100];
	char wahl;
	int i;


	do {
		gefunden = 0; // Zurücksetzen des gefunden-Flags

		bs_loeschen();

		printf("Geben Sie die Artikelnummer oder den Artikelnamen ein: ");
		if (scanf("%d", &eingabeNummer) == 1) { // Eingabe als Nummer
			for (i = 0; i < anzahl_artikel; i++) {
				if (artikel_liste[i].art_nummer == eingabeNummer) {
					gefunden = 1;
					printf("\nArtikel Typ gefunden:");
					printf("\nName: %s", artikel_liste[i].name);
					printf("\nArtikelnummer: %d", artikel_liste[i].art_nummer);
					printf("\nPreis (in EUR): %.2lf", artikel_liste[i].preis);
					printf("\nHoehe (in cm): %.2lf", artikel_liste[i].hoehe);
					printf("\nBreite (in cm): %.2lf", artikel_liste[i].breite);
					printf("\nTiefe (in cm): %.2lf", artikel_liste[i].tiefe);
					printf("\nLager: %d", artikel_liste[i].lager);
					printf("\nMoechten Sie zum Menue zurueckkehren (0), nach einem anderen Artikel suchen (1) oder den Artikel einlagern (2)?: ");
					scanf(" %c", &wahl);
					if (wahl == '0') {
						printf("\nZurueck zum Menue...");
						return 0;
					}
					else if (wahl == '2') {

						struct ArtikelTyp *artikel_typ_pntr;
						artikel_typ_pntr = &artikel_liste[i];

						struct Artikel artikel;
						artikel.typ = artikel_typ_pntr;
						artikel.inventarnummer = generiere_inventarnummer(); // Hier wird die Inventarnummer generiert und zugewiesen

						artikel_einlagern_nach_nummer(eingabeNummer, artikel_liste[i].lager, artikel);
						break;
					}
				}
			}
		}
		else { // Eingabe als Name
			getchar();
			fgets(eingabeName, sizeof(eingabeName), stdin); // Einlesen mit Leerzeichen moeglich/ganze Zeile
			eingabeName[strcspn(eingabeName, "\n")] = 0; // Entfernen der Zeilenumbruchzeichen (\n)
			strtrim(eingabeName); // Leerzeichen am Anfang und Ende entfernen

			for (i = 0; i < anzahl_artikel; i++) {
				if (strcmp(artikel_liste[i].name, eingabeName) == 0) {
					gefunden = 1;
					printf("\nArtikel Typ gefunden:");
					printf("\nName: %s", artikel_liste[i].name);
					printf("\nNummer: %d", artikel_liste[i].art_nummer);
					printf("\nPreis (in EUR): %.2lf", artikel_liste[i].preis);
					printf("\nHoehe (in cm): %.2lf", artikel_liste[i].hoehe);
					printf("\nBreite (in cm): %.2lf", artikel_liste[i].breite);
					printf("\nTiefe (in cm): %.2lf", artikel_liste[i].tiefe);
					printf("\nLager: %d", artikel_liste[i].lager);
					printf("\nMoechten Sie zum Menue zurueckkehren (0), nach einem anderen Artikel suchen (1) oder den Artikel einlagern (2)?: ");
					scanf(" %c", &wahl);
					if (wahl == '0') {
						printf("\nZurueck zum Menue...");
						return 0;
					}
					else if (wahl == '2') {
						//artikel_einlagern_nach_name(eingabeName, artikel_liste[i].lager);
						return 1;
					}
					break;
				}
			}
		}

		if (!gefunden) {
			printf("\nArtikel nicht gefunden.\n");
		}
		printf("Moechten Sie zum Menue zurueckkehren (0) oder einen neuen Artikel suchen (1) ?: ");
		scanf(" %c", &wahl);
	} while (wahl != '0');

	return 0;
}

int generiere_inventarnummer() {
	// Inkrementiere den Zähler und gib die neue Inventarnummer zurück
	return ++inventarnummerZaehler;
}

int artikel_einlagern_nach_nummer(int eingabeNummer, int lager, struct Artikel artikel) {
	int i;

	struct Artikel *artikel_pntr;
	artikel_pntr = &artikel;

	// Überprüfen des Lagers und Berechnungen, vielleicht als separate Funktionen nochmal
	if (lager == 1) {
		// Aktionen für Lager 1 (HALLE)
		if (lagere_artikel_an_positions_ids_Halle(artikel_pntr) != -1) {
			print_zugeordnete_ids(artikel_pntr); // Ausgabe der zugeordneten IDs
			printf("Artikel mit der Nummer %d wurde erfolgreich im Halle Lager eingelagert.\n", eingabeNummer);
			
			// Zeigt Positions IDs der Artikel an, nur zum überprüfen
			int l;
			printf("Artikel: %s\n", artikel.typ->name);
			printf("AnzahlPositionsIDs: %d\n", artikel.anzahl_positions_ids);
			printf("PositionsIDs:");
			for (l = 0; l < 60; l++) {
				printf("%d, ", artikel.positions[l].id);
			}
			while (getchar() != '\n');
			getchar();
			
		}
		else {
			printf("Artikel mit der Nummer %d konnte nicht eingelagert werden. Keine passende Position gefunden.\n", eingabeNummer);
		}
		printf("Druecken Sie Enter, um weitere Aktionen auszufuehren!\n");
		while (getchar() != '\n');
		getchar();
	}
	else if (lager == 2) {
		// Aktionen für Lager 2 (PW)
		if (lagere_artikel_an_positions_ids_Porta(artikel_pntr) != -1) {
			print_zugeordnete_ids(artikel_pntr); // Ausgabe der zugeordneten IDs
			printf("Artikel mit der Nummer %d wurde erfolgreich im Porta Lager eingelagert.\n", eingabeNummer);
		}
		else {
			printf("Artikel mit der Nummer %d konnte nicht eingelagert werden. Keine passende Position gefunden.\n", eingabeNummer);
		}
		printf("Druecken Sie Enter, um weitere Aktionen auszufuehren!\n");
		while (getchar() != '\n');
		getchar();
	}

	return 1;
}

/*int artikel_einlagern_nach_name(char eingabeName[], int lager) {
	// Überprüfen des Lagers und dann Berechnungen
	if (lager == 1) {
		// Aktionen für Lager 1 (HALLE)
		printf("Artikel mit dem Namen %s wird im Halle Lager eingelagert.\nDruecken Sie Enter, um weitere Aktionen auszufuehren!\n", eingabeName);
		while (getchar() != '\n');
		getchar();
	}
	else if (lager == 2) {
		// Aktionen für Lager 2 (PW) einzeln, weil es noch 80cm gibt
		printf("Artikel mit dem Namen %s wird im Porta Westfalica Lager eingelagert.\nDruecken Sie Enter, um weitere Aktionen auszufuehren!\n", eingabeName);
		while (getchar() != '\n');
		getchar();
	}
	else {
		// Fehlermeldung, mehr oder weniger sinnlos
		printf("Unbekanntes Lager fuer Artikel mit dem Namen %s.\nDruecken Sie Enter, um weitere Aktionen auszufuehren!\n", eingabeName);
		while (getchar() != '\n');
		getchar();
	}

	return 1;
}*/

int lagere_artikel_an_positions_ids_Halle(struct Artikel *artikel) {
	int i;
	int start_id;
	int aktuelle_id;
	int max_anzahl_halle_20 = HALLE_20 + 12000000;
	int max_anzahl_halle_40 = HALLE_40 + 14000000;

	// Zugriff auf Daten des Artikels
	double hoehe = artikel->typ->hoehe;
	double breite = artikel->typ->breite;
	double tiefe = artikel->typ->tiefe;
	int artikelnummer = artikel->typ->art_nummer;

	int lager_halle_20_cm_voll = 0; // Flag, ob Lagerplatz mit 20cm hohen Fächern voll ist
	int ids_von_artikel_belegt = 0; // Anzahl der vom Artikel belegten IDs

	// Berechnung der Anzahl der vom Artikel belegten IDs basierend auf Breite und Tiefe
	ids_von_artikel_belegt = (int)(breite / 10); // Breite belegt breite/10 IDs
	if (tiefe > 60) {
		ids_von_artikel_belegt *= 2; // Tiefe > 60 belegt das doppelte an IDs
	}

	artikel->anzahl_positions_ids = 0;

	if (hoehe <= 20) {
		start_id = START_ID_HALLE_20CM;

		// Durchlaufen aller möglichen IDs für Halle 20cm Lagerplatz 
		for (aktuelle_id = start_id; aktuelle_id < max_anzahl_halle_20; aktuelle_id++) {
			int available = 0;
			i = aktuelle_id - START_ID_HALLE_20CM;
			// Überprüfen, ob aktuelle ID schon in der Liste belegte_ids_halle_20 vorhanden ist
			// Wenn an der Stelle i in belegte_ids_halle_20 noch kein Element vorhanden ist, wir die ID als verfügbar angesehen
			if (belegte_ids_halle_20[i].id == NULL) {
				available = 1;
			}
			else {
				// Vergleich, ob die aktuelle_id in belegte_ids_halle_20 vorhanden ist
				if (belegte_ids_halle_20[i].id == aktuelle_id) {
					// Vergleich, ob die Artikelnummer der PositionsID an der Stelle i in belegte_ids_halle_20 gleich der Artikel Nummer von dem ArtikelTyp des Artikels ist
					// Wenn nicht, fange mit der nächsten ID neu an
					if (belegte_ids_halle_20[i].artikelnummer == artikel->typ->art_nummer) {
						// Überprüft, ob PositionsID den Flag "voll" gesetzt hat
						// Wenn ja, fange mit der nächsten ID neu an
						if (belegte_ids_halle_20[i].positions_id_voll == 1) {
							continue;
						}
						// Überprüft, ob der Artikel von der Höhe noch in die PositionsID passt
						// Wenn ja, dann ist diese ID Potentiell möglich
						else if (belegte_ids_halle_20[i].resthoehe >= hoehe) {
							available = 1;
						}
					}
					else {
						continue;
					}
				}
			}
			// aktuelle ID ist potentiell möglich
			// überprüfen, ob die nachfolgenden IDs auch noch frei sind (wenn Artikel mehrere belegt)
			if (available == 1) {
				// Der ID Index entspricht der ID Nummer, ohne die Lagererkennung (12000000)
				int aktuelle_id_index = aktuelle_id - 12000000;
				int letzte_belegte_id = aktuelle_id_index + ids_von_artikel_belegt;
				int id_index_zaheler;
				// Durchlaufen für alle vom Artikel benötigten Positions IDs
				for (id_index_zaheler = aktuelle_id_index; id_index_zaheler <= letzte_belegte_id; id_index_zaheler++) {
					// Steht an der Stelle im Array überhaupt schon was?
					if (belegte_ids_halle_20[id_index_zaheler].id == NULL) {
						continue;
					}
					// Überprüfen, ob die Artikelnummer an dieser Stelle gleich der Artikelnummer des einzulagernden Artikels ist
					else if (belegte_ids_halle_20[id_index_zaheler].artikelnummer == artikel->typ->art_nummer) {
						// Überprüfen, ob das Fach als "voll" makiert ist, wenn nicht, dann ist diese ID nicht verfügbar, es muss also eine andere ID gesucht werden
						if (belegte_ids_halle_20[id_index_zaheler].positions_id_voll == 1) {
							available = 0;
						}
						// Überprüfen, ob der einzulagernde Artikel in die Resthöhe des Fachs passt, wenn ja, dann durchlaufe alles auch für die nächsten benötigten IDs
						else if (belegte_ids_halle_20[id_index_zaheler].resthoehe >= artikel->typ->hoehe) {
							continue;
						}
					}
					// ID nicht verfügbar, suche andere ID
					else {
						available = 0;
						break;
					}
				}

				// Aktuelle ID (und nachfolgende) sind möglich, artikel kann an die Stelle eingelagert werden
				if (available == 1) {
					// Für jede vom Artikel belegte ID einen struct anlegen
					for (aktuelle_id_index; aktuelle_id_index < letzte_belegte_id; aktuelle_id_index++) {
						double resthoehe;
						// Hier wird die richtige ID wieder mit der Lagerkennung zusammengebaut
						aktuelle_id = aktuelle_id_index + 12000000;

						// Überprüft, ob die aktuelle ID schon als PositionsID erstellt ist
						// Wenn nicht (== NULL), dann erstelle eine neue PositionsID
						if (belegte_ids_halle_20[aktuelle_id_index].id == NULL) {
							struct PositionsID positions_id;
							positions_id.id = aktuelle_id;
							positions_id.artikelnummer = artikel->typ->art_nummer;
							positions_id.positions_id_voll = 0;

							// Berechnung der Resthöhe
							double resthoehe = 20 - artikel->typ->hoehe;
							positions_id.resthoehe = resthoehe;

							// Wenn Resthöhe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}

							// PositionsID als belegt Kennzeichnen
							belegte_ids_halle_20[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die für diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
						else {
							struct PositionsID positions_id = belegte_ids_halle_20[aktuelle_id_index];

							// Wenn Resthöhe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							double resthoehe = positions_id.resthoehe - artikel->typ->hoehe;
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}
							positions_id.resthoehe = resthoehe;

							// PositionsID als belegt Kennzeichnen
							belegte_ids_halle_20[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die für diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
					}
					return 0;
				}
			}
		}
		lager_halle_20_cm_voll = 1;
	}
	else if (((hoehe > 20) && (hoehe <= 40)) || lager_halle_20_cm_voll == 1) {
		start_id = START_ID_HALLE_40CM;

		// Durchlaufen aller möglichen IDs für Halle 40cm Lagerplatz 
		for (aktuelle_id = start_id; aktuelle_id < max_anzahl_halle_40; aktuelle_id++) {
			int available = 0;
			int i = aktuelle_id - START_ID_HALLE_40CM;
			// Überprüfen, ob aktuelle ID schon in der Liste belegte_ids_halle_40 vorhanden ist
			// Wenn an der Stelle i in belegte_ids_halle_40 noch kein Element vorhanden ist, wir die ID als verfügbar angesehen
			if (belegte_ids_halle_40[i].id == NULL) {
				available = 1;
			}
			else {
				// Vergleich, ob die aktuelle_id in belegte_ids_halle_40 vorhanden ist
				if (belegte_ids_halle_40[i].id == aktuelle_id) {
					// Vergleich, ob die Artikelnummer der PositionsID an der Stelle i in belegte_ids_halle_40 gleich der Artikel Nummer von dem ArtikelTyp des Artikels ist
					// Wenn nicht, fange mit der nächsten ID neu an
					if (belegte_ids_halle_40[i].artikelnummer == artikel->typ->art_nummer) {
						// Überprüft, ob PositionsID den Flag "voll" gesetzt hat
						// Wenn ja, fange mit der nächsten ID neu an
						if (belegte_ids_halle_40[i].positions_id_voll == 1) {
							continue;
						}
						// Überprüft, ob der Artikel von der Höhe noch in die PositionsID passt
						// Wenn ja, dann ist diese ID Potentiell möglich
						else if (belegte_ids_halle_40[i].resthoehe >= hoehe) {
							available = 1;
						}
					}
					else {
						continue;
					}
				}
			}
			// aktuelle ID ist potentiell möglich
			// überprüfen, ob die nachfolgenden IDs auch noch frei sind (wenn Artikel mehrere belegt)
			if (available == 1) {
				// Der ID Index entspricht der ID Nummer, ohne die Lagererkennung (14000000)
				int aktuelle_id_index = aktuelle_id - 14000000;
				int letzte_belegte_id = aktuelle_id_index + ids_von_artikel_belegt;
				int id_index_zaheler;
				// Durchlaufen für alle vom Artikel benötigten Positions IDs
				for (id_index_zaheler = aktuelle_id_index; id_index_zaheler <= letzte_belegte_id; id_index_zaheler++) {
					// Steht an der Stelle im Array überhaupt schon was?
					if (belegte_ids_halle_40[id_index_zaheler].id == NULL) {
						continue;
					}
					// Überprüfen, ob die Artikelnummer an dieser Stelle gleich der Artikelnummer des einzulagernden Artikels ist
					else if (belegte_ids_halle_40[id_index_zaheler].artikelnummer == artikel->typ->art_nummer) {
						// Überprüfen, ob das Fach als "voll" makiert ist, wenn nicht, dann ist diese ID nicht verfügbar, es muss also eine andere ID gesucht werden
						if (belegte_ids_halle_40[id_index_zaheler].positions_id_voll == 1) {
							available = 0;
						}
						// Überprüfen, ob der einzulagernde Artikel in die Resthöhe des Fachs passt, wenn ja, dann durchlaufe alles auch für die nächsten benötigten IDs
						else if (belegte_ids_halle_40[id_index_zaheler].resthoehe >= artikel->typ->hoehe) {
							continue;
						}
					}
					// ID nicht verfügbar, suche andere ID
					else {
						available = 0;
						break;
					}
				}

				// Aktuelle ID (und nachfolgende) sind möglich, artikel kann an die Stelle eingelagert werden
				if (available == 1) {
					// Für jede vom Artikel belegte ID einen struct anlegen
					for (aktuelle_id_index; aktuelle_id_index < letzte_belegte_id; aktuelle_id_index++) {
						double resthoehe;
						// Hier wird die richtige ID wieder mit der Lagerkennung zusammengebaut
						aktuelle_id = aktuelle_id_index + 14000000;

						// Überprüft, ob die aktuelle ID schon als PositionsID erstellt ist
						// Wenn nicht (== NULL), dann erstelle eine neue PositionsID
						if (belegte_ids_halle_40[aktuelle_id_index].id == NULL) {
							struct PositionsID positions_id;
							positions_id.id = aktuelle_id;
							positions_id.artikelnummer = artikel->typ->art_nummer;
							positions_id.positions_id_voll = 0;

							// Berechnung der Resthöhe
							double resthoehe = 40 - artikel->typ->hoehe;
							positions_id.resthoehe = resthoehe;

							// Wenn Resthöhe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}

							// PositionsID als belegt Kennzeichnen
							belegte_ids_halle_40[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die für diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
						else {
							struct PositionsID positions_id = belegte_ids_halle_40[aktuelle_id_index];

							// Wenn Resthöhe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							double resthoehe = positions_id.resthoehe - artikel->typ->hoehe;
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}
							positions_id.resthoehe = resthoehe;
							// Anzahl der PositionsIDs, die für diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// PositionsID als belegt Kennzeichnen
							belegte_ids_halle_40[aktuelle_id_index] = positions_id;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
					}
					return 0;
				}
			}
		}
	}
	// Rückgabe eines ungültigen Wertes, falls keine passende Position gefunden wurde
	return -1;
}

// Muss noch gemacht werden
int lagere_artikel_an_positions_ids_Porta(struct Artikel *artikel) {
	int i;
	int start_id;
	int aktuelle_id;
	int max_anzahl_porta_20 = PORTA_20 + 22000000;
	int max_anzahl_porta_40 = PORTA_40 + 2400000;
	int max_anzahl_porta_80 = PORTA_80 + 2800000;

	// Zugriff auf Daten des Artikels
	double hoehe = artikel->typ->hoehe;
	double breite = artikel->typ->breite;
	double tiefe = artikel->typ->tiefe;
	int artikelnummer = artikel->typ->art_nummer;

	int lager_porta_20_cm_voll = 0; // Flag, ob Lagerplatz mit 20cm hohen Fächern voll ist
	int lager_porta_40_cm_voll = 0; // Flag, ob Lagerplatz mit 40cm hohen Fächern voll ist

	int ids_von_artikel_belegt = 0; // Anzahl der belegten IDs

	// Berechnung der Anzahl der belegten IDs basierend auf Breite und Tiefe
	ids_von_artikel_belegt = (int)(breite / 10); // Breite belegt breite/10 IDs
	if (tiefe > 60) {
		ids_von_artikel_belegt *= 2; // Tiefe > 60 belegt das doppelte an IDs
	}

	artikel->anzahl_positions_ids = 0;

	if (hoehe <= 20) {
		start_id = START_ID_PORTA_20CM;

		// Durchlaufen aller möglichen IDs für Porta 20cm Lagerplatz 
		for (aktuelle_id = start_id; aktuelle_id < max_anzahl_porta_20; aktuelle_id++) {
			int available = 0;
			i = aktuelle_id - START_ID_PORTA_20CM;
			// Überprüfen, ob aktuelle ID schon in der Liste belegte_ids_porta_20 vorhanden ist
			// Wenn an der Stelle i in belegte_ids_porta_20 noch kein Element vorhanden ist, wir die ID als verfügbar angesehen
			if (belegte_ids_porta_20[i].id == NULL) {
				available = 1;
			}
			else {
				// Vergleich, ob die aktuelle_id in belegte_ids_porta_20 vorhanden ist
				if (belegte_ids_porta_20[i].id == aktuelle_id) {
					// Vergleich, ob die Artikelnummer der PositionsID an der Stelle i in belegte_ids_porta_20 gleich der Artikel Nummer von dem ArtikelTyp des Artikels ist
					// Wenn nicht, fange mit der nächsten ID neu an
					if (belegte_ids_porta_20[i].artikelnummer == artikel->typ->art_nummer) {
						// Überprüft, ob PositionsID den Flag "voll" gesetzt hat
						// Wenn ja, fange mit der nächsten ID neu an
						if (belegte_ids_porta_20[i].positions_id_voll == 1) {
							continue;
						}
						// Überprüft, ob der Artikel von der Höhe noch in die PositionsID passt
						// Wenn ja, dann ist diese ID Potentiell möglich
						else if (belegte_ids_porta_20[i].resthoehe >= hoehe) {
							available = 1;
						}
					}
					else {
						continue;
					}
				}
			}
			// aktuelle ID ist potentiell möglich
			// überprüfen, ob die nachfolgenden IDs auch noch frei sind (wenn Artikel mehrere belegt)
			if (available == 1) {
				// Der ID Index entspricht der ID Nummer, ohne die Lagererkennung (12000000)
				int aktuelle_id_index = aktuelle_id - 22000000;
				int letzte_belegte_id = aktuelle_id_index + ids_von_artikel_belegt;
				int id_index_zaheler;
				// Durchlaufen für alle vom Artikel benötigten Positions IDs
				for (id_index_zaheler = aktuelle_id_index; id_index_zaheler <= letzte_belegte_id; id_index_zaheler++) {
					// Steht an der Stelle im Array überhaupt schon was?
					if (belegte_ids_porta_20[id_index_zaheler].id == NULL) {
						continue;
					}
					// Überprüfen, ob die Artikelnummer an dieser Stelle gleich der Artikelnummer des einzulagernden Artikels ist
					else if (belegte_ids_porta_20[id_index_zaheler].artikelnummer == artikel->typ->art_nummer) {
						// Überprüfen, ob das Fach als "voll" makiert ist, wenn nicht, dann ist diese ID nicht verfügbar, es muss also eine andere ID gesucht werden
						if (belegte_ids_porta_20[id_index_zaheler].positions_id_voll == 1) {
							available = 0;
						}
						// Überprüfen, ob der einzulagernde Artikel in die Resthöhe des Fachs passt, wenn ja, dann durchlaufe alles auch für die nächsten benötigten IDs
						else if (belegte_ids_porta_20[id_index_zaheler].resthoehe >= artikel->typ->hoehe) {
							continue;
						}
					}
					// ID nicht verfügbar, suche andere ID
					else {
						available = 0;
						break;
					}
				}

				// Aktuelle ID (und nachfolgende) sind möglich, artikel kann an die Stelle eingelagert werden
				if (available == 1) {
					// Für jede vom Artikel belegte ID einen struct anlegen
					for (aktuelle_id_index; aktuelle_id_index < letzte_belegte_id; aktuelle_id_index++) {
						double resthoehe;
						// Hier wird die richtige ID wieder mit der Lagerkennung zusammengebaut
						aktuelle_id = aktuelle_id_index + 22000000;

						// Überprüft, ob die aktuelle ID schon als PositionsID erstellt ist
						// Wenn nicht (== NULL), dann erstelle eine neue PositionsID
						if (belegte_ids_porta_20[aktuelle_id_index].id == NULL) {
							struct PositionsID positions_id;
							positions_id.id = aktuelle_id;
							positions_id.artikelnummer = artikel->typ->art_nummer;
							positions_id.positions_id_voll = 0;

							// Berechnung der Resthöhe
							double resthoehe = 20 - artikel->typ->hoehe;
							positions_id.resthoehe = resthoehe;

							// Wenn Resthöhe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_20[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die für diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
						else {
							struct PositionsID positions_id = belegte_ids_porta_20[aktuelle_id_index];

							// Wenn Resthöhe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							double resthoehe = positions_id.resthoehe - artikel->typ->hoehe;
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}
							positions_id.resthoehe = resthoehe;

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_20[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die für diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
					}
					return 0;
				}
			}
		}
		lager_porta_20_cm_voll = 1;
	}
	else if (((hoehe > 20) && (hoehe <= 40)) || lager_porta_20_cm_voll == 1) {
		start_id = START_ID_PORTA_40CM;

		// Durchlaufen aller möglichen IDs für Porta 40cm Lagerplatz 
		for (aktuelle_id = start_id; aktuelle_id < max_anzahl_porta_40; aktuelle_id++) {
			int available = 0;
			i = aktuelle_id - START_ID_PORTA_40CM;
			// Überprüfen, ob aktuelle ID schon in der Liste belegte_ids_porta_40 vorhanden ist
			// Wenn an der Stelle i in belegte_ids_porta_40 noch kein Element vorhanden ist, wir die ID als verfügbar angesehen
			if (belegte_ids_porta_40[i].id == NULL) {
				available = 1;
			}
			else {
				// Vergleich, ob die aktuelle_id in belegte_ids_porta_40 vorhanden ist
				if (belegte_ids_porta_40[i].id == aktuelle_id) {
					// Vergleich, ob die Artikelnummer der PositionsID an der Stelle i in belegte_ids_porta_40 gleich der Artikel Nummer von dem ArtikelTyp des Artikels ist
					// Wenn nicht, fange mit der nächsten ID neu an
					if (belegte_ids_porta_40[i].artikelnummer == artikel->typ->art_nummer) {
						// Überprüft, ob PositionsID den Flag "voll" gesetzt hat
						// Wenn ja, fange mit der nächsten ID neu an
						if (belegte_ids_porta_40[i].positions_id_voll == 1) {
							continue;
						}
						// Überprüft, ob der Artikel von der Höhe noch in die PositionsID passt
						// Wenn ja, dann ist diese ID Potentiell möglich
						else if (belegte_ids_porta_40[i].resthoehe >= hoehe) {
							available = 1;
						}
					}
					else {
						continue;
					}
				}
			}
			// aktuelle ID ist potentiell möglich
			// überprüfen, ob die nachfolgenden IDs auch noch frei sind (wenn Artikel mehrere belegt)
			if (available == 1) {
				// Der ID Index entspricht der ID Nummer, ohne die Lagererkennung (24000000)
				int aktuelle_id_index = aktuelle_id - 24000000;
				int letzte_belegte_id = aktuelle_id_index + ids_von_artikel_belegt;
				int id_index_zaheler;
				// Durchlaufen für alle vom Artikel benötigten Positions IDs
				for (id_index_zaheler = aktuelle_id_index; id_index_zaheler <= letzte_belegte_id; id_index_zaheler++) {
					// Steht an der Stelle im Array überhaupt schon was?
					if (belegte_ids_porta_40[id_index_zaheler].id == NULL) {
						continue;
					}
					// Überprüfen, ob die Artikelnummer an dieser Stelle gleich der Artikelnummer des einzulagernden Artikels ist
					else if (belegte_ids_porta_40[id_index_zaheler].artikelnummer == artikel->typ->art_nummer) {
						// Überprüfen, ob das Fach als "voll" makiert ist, wenn nicht, dann ist diese ID nicht verfügbar, es muss also eine andere ID gesucht werden
						if (belegte_ids_porta_40[id_index_zaheler].positions_id_voll == 1) {
							available = 0;
						}
						// Überprüfen, ob der einzulagernde Artikel in die Resthöhe des Fachs passt, wenn ja, dann durchlaufe alles auch für die nächsten benötigten IDs
						else if (belegte_ids_porta_40[id_index_zaheler].resthoehe >= artikel->typ->hoehe) {
							continue;
						}
					}
					// ID nicht verfügbar, suche andere ID
					else {
						available = 0;
						break;
					}
				}

				// Aktuelle ID (und nachfolgende) sind möglich, artikel kann an die Stelle eingelagert werden
				if (available == 1) {
					// Für jede vom Artikel belegte ID einen struct anlegen
					for (aktuelle_id_index; aktuelle_id_index < letzte_belegte_id; aktuelle_id_index++) {
						double resthoehe;
						// Hier wird die richtige ID wieder mit der Lagerkennung zusammengebaut
						aktuelle_id = aktuelle_id_index + 24000000;

						// Überprüft, ob die aktuelle ID schon als PositionsID erstellt ist
						// Wenn nicht (== NULL), dann erstelle eine neue PositionsID
						if (belegte_ids_porta_40[aktuelle_id_index].id == NULL) {
							struct PositionsID positions_id;
							positions_id.id = aktuelle_id;
							positions_id.artikelnummer = artikel->typ->art_nummer;
							positions_id.positions_id_voll = 0;

							// Berechnung der Resthöhe
							double resthoehe = 20 - artikel->typ->hoehe;
							positions_id.resthoehe = resthoehe;

							// Wenn Resthöhe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_40[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die für diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
						else {
							struct PositionsID positions_id = belegte_ids_porta_40[aktuelle_id_index];

							// Wenn Resthöhe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							double resthoehe = positions_id.resthoehe - artikel->typ->hoehe;
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}
							positions_id.resthoehe = resthoehe;

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_40[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die für diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
					}
					return 0;
				}
			}
		}
		lager_porta_40_cm_voll = 1;
	}
	else if (((hoehe > 40) && (hoehe <= 80)) || lager_porta_40_cm_voll == 1) {
		start_id = START_ID_PORTA_80CM;

		// Durchlaufen aller möglichen IDs für Porta 80cm Lagerplatz 
		for (aktuelle_id = start_id; aktuelle_id < max_anzahl_porta_80; aktuelle_id++) {
			int available = 0;
			i = aktuelle_id - START_ID_PORTA_80CM;
			// Überprüfen, ob aktuelle ID schon in der Liste belegte_ids_porta_80 vorhanden ist
			// Wenn an der Stelle i in belegte_ids_porta_80 noch kein Element vorhanden ist, wir die ID als verfügbar angesehen
			if (belegte_ids_porta_80[i].id == NULL) {
				available = 1;
			}
			else {
				// Vergleich, ob die aktuelle_id in belegte_ids_porta_80 vorhanden ist
				if (belegte_ids_porta_80[i].id == aktuelle_id) {
					// Vergleich, ob die Artikelnummer der PositionsID an der Stelle i in belegte_ids_porta_80 gleich der Artikel Nummer von dem ArtikelTyp des Artikels ist
					// Wenn nicht, fange mit der nächsten ID neu an
					if (belegte_ids_porta_80[i].artikelnummer == artikel->typ->art_nummer) {
						// Überprüft, ob PositionsID den Flag "voll" gesetzt hat
						// Wenn ja, fange mit der nächsten ID neu an
						if (belegte_ids_porta_80[i].positions_id_voll == 1) {
							continue;
						}
						// Überprüft, ob der Artikel von der Höhe noch in die PositionsID passt
						// Wenn ja, dann ist diese ID Potentiell möglich
						else if (belegte_ids_porta_80[i].resthoehe >= hoehe) {
							available = 1;
						}
					}
					else {
						continue;
					}
				}
			}
			// aktuelle ID ist potentiell möglich
			// überprüfen, ob die nachfolgenden IDs auch noch frei sind (wenn Artikel mehrere belegt)
			if (available == 1) {
				// Der ID Index entspricht der ID Nummer, ohne die Lagererkennung (28000000)
				int aktuelle_id_index = aktuelle_id - 28000000;
				int letzte_belegte_id = aktuelle_id_index + ids_von_artikel_belegt;
				int id_index_zaheler;
				// Durchlaufen für alle vom Artikel benötigten Positions IDs
				for (id_index_zaheler = aktuelle_id_index; id_index_zaheler <= letzte_belegte_id; id_index_zaheler++) {
					// Steht an der Stelle im Array überhaupt schon was?
					if (belegte_ids_porta_80[id_index_zaheler].id == NULL) {
						continue;
					}
					// Überprüfen, ob die Artikelnummer an dieser Stelle gleich der Artikelnummer des einzulagernden Artikels ist
					else if (belegte_ids_porta_80[id_index_zaheler].artikelnummer == artikel->typ->art_nummer) {
						// Überprüfen, ob das Fach als "voll" makiert ist, wenn nicht, dann ist diese ID nicht verfügbar, es muss also eine andere ID gesucht werden
						if (belegte_ids_porta_80[id_index_zaheler].positions_id_voll == 1) {
							available = 0;
						}
						// Überprüfen, ob der einzulagernde Artikel in die Resthöhe des Fachs passt, wenn ja, dann durchlaufe alles auch für die nächsten benötigten IDs
						else if (belegte_ids_porta_80[id_index_zaheler].resthoehe >= artikel->typ->hoehe) {
							continue;
						}
					}
					// ID nicht verfügbar, suche andere ID
					else {
						available = 0;
						break;
					}
				}

				// Aktuelle ID (und nachfolgende) sind möglich, artikel kann an die Stelle eingelagert werden
				if (available == 1) {
					// Für jede vom Artikel belegte ID einen struct anlegen
					for (aktuelle_id_index; aktuelle_id_index < letzte_belegte_id; aktuelle_id_index++) {
						double resthoehe;
						// Hier wird die richtige ID wieder mit der Lagerkennung zusammengebaut
						aktuelle_id = aktuelle_id_index + 28000000;

						// Überprüft, ob die aktuelle ID schon als PositionsID erstellt ist
						// Wenn nicht (== NULL), dann erstelle eine neue PositionsID
						if (belegte_ids_porta_80[aktuelle_id_index].id == NULL) {
							struct PositionsID positions_id;
							positions_id.id = aktuelle_id;
							positions_id.artikelnummer = artikel->typ->art_nummer;
							positions_id.positions_id_voll = 0;

							// Berechnung der Resthöhe
							double resthoehe = 20 - artikel->typ->hoehe;
							positions_id.resthoehe = resthoehe;

							// Wenn Resthöhe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_80[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die für diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
						else {
							struct PositionsID positions_id = belegte_ids_porta_80[aktuelle_id_index];

							// Wenn Resthöhe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							double resthoehe = positions_id.resthoehe - artikel->typ->hoehe;
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}
							positions_id.resthoehe = resthoehe;

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_80[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die für diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
					}
					return 0;
				}
			}
		}
	}
	// Rückgabe eines ungültigen Wertes, falls keine passende Position gefunden wurde
	return -1;
}


// Funktion zur Ausgabe aller zugeordneten IDs
int print_zugeordnete_ids(struct Artikel *artikel) {
	int i;
	printf("Eingelagerter Artikel:\n");
	printf("Artikelnummer: %d\n", artikel->typ->art_nummer);
	printf("Inventarnummer: %d\n", artikel->inventarnummer);
	printf("Anzahl der zugeordneten IDs: %d\n", artikel->anzahl_positions_ids);

	printf("Zugeordnete IDs:\n");
	for (i = 0; i < artikel->anzahl_positions_ids; i++) {	//gibt falschen wert aus und demnach auch keine IDs
		printf("ID %d: %d\n", i + 1, artikel->positions[i].id);
	}
}

void bs_loeschen(void) {
	system("CLS");   // MS-DOS-Kommando
}

void strtrim(char* str) {
	int start = 0, end;

	// Leerzeichen am Anfang entfernen
	while (isspace(str[start])) {
		start++;
	}

	// Wenn die Zeichenkette nur aus Leerzeichen besteht
	if (str[start] == '\0') {
		*str = '\0';
		return;
	}

	// Leerzeichen am Ende finden
	end = strlen(str) - 1;
	while (end >= start && isspace(str[end])) {
		end--;
	}

	// Nullterminator setzen
	str[end + 1] = '\0';

	// Zeichenkette verschieben, wenn nötig
	if (start > 0) {
		memmove(str, str + start, end - start + 2);
	}
}