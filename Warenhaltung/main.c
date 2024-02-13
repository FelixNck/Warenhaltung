#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HALLE_20 18000	// Vorhandene Anzahl Positions IDs für 20cm hohe Fächer
#define HALLE_40 54000
#define PORTA_20 34800
#define PORTA_40 6000
#define PORTA_80 7800
#define MAX_ARTIKEL HALLE_20+HALLE_40+PORTA_20+PORTA_40+PORTA_80


// StartIDs zum Zaehlen; Halle = Lager 1; PW = Lager 2
#define START_ID_HALLE_20CM 12000000
#define START_ID_HALLE_40CM 14000000
#define START_ID_PORTA_20CM 22000000
#define START_ID_PORTA_40CM 2400000
#define START_ID_PORTA_80CM 2800000

// Maximale Anzahl von Artikeln
#define MAX_ARTIKEL_TYP 400

int menue();
double berechne_belegung_von(int lager);
int berechne_artikel_im_warenausgang();
int neuen_artikel_typ_anlegen();
int artikel_typ_bearbeiten();
int details_waehlen_bearbeitung(struct ArtikelTyp* artikel);
int artikel_typ_loeschen();
int atikel_manuell_erfassen();
int artikel_erfassen(int artikelnummer, struct ArtikelTyp *artikel_typ_pntr, int ausgabe_aktiviert);
int artikel_einlagern_nach_nummer(int eingabeNummer, int lager, struct Artikel artikel, int ausgabe_aktiviert);
int lagere_artikel_an_positions_ids_Halle(struct Artikel *artikel);
int lagere_artikel_an_positions_ids_Porta(struct Artikel *artikel);
int print_zugeordnete_ids(struct Artikel *artikel);
int artikel_aus_lager_entfernen(int inventarnummer);
int erste_ziffer(int zahl);
int zweite_ziffer(int zahl);
int vorhandene_artikel_typen_ansehen();
int lager_zufaellig_befuellen();
int manuelle_bestellung_anlegen();
int lager_aktualisieren(struct ArtikelTyp *artikel);
void bs_loeschen(); // Funktion zum leeren der Konsolenausgabe
void strtrim(char* str);    // Leerzeichen am Anfang und Ende einer Eingabe ignorieren/wegschneiden

// Kennzeichnet eine 10cm Breite und 60cm Tiefe Position im Lager
struct PositionsID {
	int id;
	int resthoehe; // Restlicher freier Platz an dieser PositionsID (nicht < 5cm)
	int positions_id_voll; // Flag, zum Kennzeichnen, ob PositionsID voll ist
	int artikelnummer; // Artikelnummer des ArtikelTypen an dieser Position
};

// Viele Artikel gehören einem Typen an
struct ArtikelTyp {
	char name[100];
	int art_nummer; // Eindeutige Kennzeichnung des ArtikelTypen
	double preis;
	double hoehe;
	double breite;
	double tiefe;
	int lager;
	int artikel_davon_im_lager;
};

// Ein Artikel verbraucht einen bestimmten Lagerplatz (bestimmte Anzahl an PositionsIDs)
struct Artikel {
	struct ArtikelTyp *typ; // Verlinkung zum ArtikelTypen
	int inventarnummer; // Eindeutige Kennzeichnung des gelagertern Artikels
	struct PositionsID positions[60]; // Verlinkung aller PositionsIDs, die dieser Artikel belegt (60, weil maximal 3 Meter Regale (30 IDs) und doppelseitige Belegung)
	int anzahl_positions_ids; // Anzahl belegter PositionsIDs
};

struct Lager {
	struct ArtikelTyp artikel_typ_liste[MAX_ARTIKEL_TYP]; // Artikel, die in diesem Lager gelagert sind
	int anzahl_artikel_typen;
};

struct ArtikelTypImWarenausgang {
	struct ArtikelTyp* artikel_typ;
	int anzahl_von_diesem_typ_im_warenausgang;
};

// Liste fuer Artikel im Warenausgang
struct ArtikelTypImWarenausgang warenausgang[MAX_ARTIKEL_TYP];

// Liste fuer bereits angelegte ArtikelTypen
struct ArtikelTyp artikel_typ_liste[MAX_ARTIKEL_TYP];

// Liste fuer bereits angelegte Artikel
struct Artikel gelagerte_artikel_liste[MAX_ARTIKEL] = { 0 };

struct Lager halle_lager;
struct Lager porta_lager;

// Enumeration, gibt HALLE Wert 1 und PW Wert 2
enum LagerTyp {
	HALLE = 1,  // Verderblich
	PORTA_WESTFALICA = 2    // Nicht verderblich
};

// Anzahl der bereits vorhandenen ArtikelTypen
int anzahl_artikel_typen = 0;

int inventarnummerZaehler = 1000;
struct PositionsID belegte_ids_halle_20[HALLE_20]; // Liste fuer belegte IDs im Halle-Lager mit 20cm Hoehe
struct PositionsID belegte_ids_halle_40[HALLE_40]; // Liste fuer belegte IDs im Halle-Lager mit 40cm Hoehe
struct PositionsID belegte_ids_porta_20[PORTA_20]; // Liste fuer belegte IDs im Porta-Lager mit 20cm Hoehe
struct PositionsID belegte_ids_porta_40[PORTA_40]; // Liste fuer belegte IDs im Porta-Lager mit 40cm Hoehe
struct PositionsID belegte_ids_porta_80[PORTA_80]; // Liste fuer belegte IDs im Porta-Lager mit 80cm Hoehe

int main(void) {
	int auswahl;

	// Initialisierung der Lager
	halle_lager.anzahl_artikel_typen = 0;
	porta_lager.anzahl_artikel_typen = 0;

	while (1) {
		menue();

		scanf("%d", &auswahl);

		switch (auswahl) {
		case 1:
			neuen_artikel_typ_anlegen();
			break;
		case 2:
			artikel_typ_bearbeiten();
			break;
		case 3:
			artikel_typ_loeschen();
			break;
		case 4:
			atikel_manuell_erfassen();
			break;
		case 6:
			vorhandene_artikel_typen_ansehen();
			break;
		case 7:
			lager_zufaellig_befuellen();
			break;
		case 9:
			manuelle_bestellung_anlegen();
			break;
		case 12:
			exit(1);
		default:
			printf("\nFalsche Eingabe. Druecken Sie Enter und waehlen Sie eine der oben aufgelisteten Moeglichkeiten!\n");
			while (getchar() != '\n');
			getchar();
		}
	}

	return 0;
}

// Hier wird das Menue ausgegeben
int menue() {
	bs_loeschen();
	printf("Halle\t\t\tPorta Westfalica");
	printf("\n%d Artikeltyp(en)\t%d Artikeltyp(en)", halle_lager.anzahl_artikel_typen, porta_lager.anzahl_artikel_typen);
	printf("\nBelegung:\t\tBelegung:");
	printf("\n20cm: %.2lf %%\t\t20cm: %.2lf %%", berechne_belegung_von(0), berechne_belegung_von(1));
	printf("\n40cm: %.2lf %%\t\t40cm: %.2lf %%", berechne_belegung_von(2), berechne_belegung_von(3));
	printf("\n\t\t\t80cm: %.2lf %%", berechne_belegung_von(4));
	printf("\n");
	printf("\nWarenausgang: %d", berechne_artikel_im_warenausgang());
	printf("\n");
	printf("\n(1)   Neuen Artikeltyp anlegen");
	printf("\n(2)   Artikeltyp bearbeiten");
	printf("\n(3)   Artikeltyp loeschen");
	printf("\n(4)   Artikel erfassen");
	printf("\n(5)   Artikel umraeumen");
	printf("\n(6)   Alle Artikeltypen ansehen");
	printf("\n(7)   Lagerbestand zufaellig befuellen");
	printf("\n(8)   Zufaellige Bestellung erzeugen");
	printf("\n(9)   Manuelle Bestellung erzeugen");
	printf("\n(10)  Erfasste Bestellungen versenden");
	printf("\n(11)  Erfasste Bestellungen abbrechen");
	printf("\n(12)  Programm beenden");
	printf("\n");
	printf("\nWaehlen Sie eine Auswahlmoeglichkeit:");
	printf("\n");
	
	return 0;
}

double berechne_belegung_von(int lager) {
	int i;
	double anzahl_belegter_ids = 0;
	double prozent = 0;
	switch (lager) {
	case 0: // Prozentberechnung fuer Halle 20cm
		for (i = 0; i < HALLE_20; i++) {
			if (belegte_ids_halle_20[i].id != NULL) {
				anzahl_belegter_ids++;
			}
		}
		prozent = anzahl_belegter_ids * 100 / HALLE_20;
		break;
	case 1: // Prozentberechnung fuer Porta WF 20cm
		for (i = 0; i < PORTA_20; i++) {
			if (belegte_ids_porta_20[i].id != NULL) {
				anzahl_belegter_ids++;
			}
		}
		prozent = anzahl_belegter_ids * 100 / PORTA_20;
		break;
	case 2: // Prozentberechnung fuer Halle 40cm
		for (i = 0; i < HALLE_40; i++) {
			if (belegte_ids_halle_40[i].id != NULL) {
				anzahl_belegter_ids++;
			}
		}
		prozent = anzahl_belegter_ids * 100 / HALLE_40;
		break;
	case 3: // Prozentberechnung fuer Porta WF 40cm
		for (i = 0; i < PORTA_40; i++) {
			if (belegte_ids_porta_40[i].id != NULL) {
				anzahl_belegter_ids++;
			}
		}
		prozent = anzahl_belegter_ids * 100 / PORTA_40;
		break;
	case 4: // Prozentberechnung fuer Porta WF 80cm
		for (i = 0; i < PORTA_80; i++) {
			if (belegte_ids_porta_80[i].id != NULL) {
				anzahl_belegter_ids++;
			}
		}
		prozent = anzahl_belegter_ids * 100 / PORTA_80;
		break;
	}
	return prozent;
}

int berechne_artikel_im_warenausgang() {
	int i;
	int anzahl_artikel_im_warenausgang = 0;
	for (i = 0; i < MAX_ARTIKEL_TYP; i++) {
		if (warenausgang[i].artikel_typ != NULL) {
			anzahl_artikel_im_warenausgang += warenausgang[i].anzahl_von_diesem_typ_im_warenausgang;
		}
	}
	return anzahl_artikel_im_warenausgang;
}

// Anlegen eines neuen ArtikelTyps
int neuen_artikel_typ_anlegen() {
	struct ArtikelTyp artikeltyp;
	struct ArtikelTyp *artikeltyp_pntr;
	artikeltyp_pntr = &artikeltyp;
	artikeltyp_pntr->artikel_davon_im_lager = 0;
	int lagerwahl = -1;
	int wahl = 0;
	int i;

	bs_loeschen();
	printf("Neuen Artikeltyp anlegen");

	printf("\n");

	// Artikelname eingeben; Ueberpruefen, ob Name schon existiert
	printf("\nArtikelname (max. 100 Zeichen): ");
	// Leert Eingabepuffer nach vorherigen Eingabe
	while (getchar() != '\n');  
	do {
		fgets(artikeltyp_pntr->name, sizeof(artikeltyp_pntr->name), stdin); // Kann Leerzeichen/ganze Zeile einlesen
		artikeltyp_pntr->name[strcspn(artikeltyp_pntr->name, "\n")] = '\0'; // Entfernt Zeilenumbruchzeichen am Ende der Eingabe

		strtrim(artikeltyp_pntr->name); // Loescht Leerzeichen hinter/vor Name
		int name_existiert = 0; // Ueberpruefen, ob Artikelname bereits existiert
		for (i = 0; i < anzahl_artikel_typen; i++) {
			if (strcmp(artikeltyp_pntr->name, artikel_typ_liste[i].name) == 0) {
				printf("\nFehler: Ein Artikel mit diesem Namen existiert bereits.\nBitte geben Sie einen neuen Namen ein:\n");
				name_existiert = 1;
				break;
			}
		}
		// Wenn Name eindeutig ist, bricht Schleife ab
		if (!name_existiert) 
			break;
	} while (1);

	// Eingabe Artikelnummer; Ueberpruefen, ob schon existent, ob Ganzzahl und ob > 0
	while (1) {
		printf("\nArtikel Nummer (Ganzzahl > 0): ");
		if (scanf("%d", &artikeltyp_pntr->art_nummer) != 1 || artikeltyp_pntr->art_nummer <= 0 || getchar() != '\n') {
			printf("Fehler: Bitte geben Sie eine gueltige Ganzzahl groesser als 0 fuer die Artikelnummer ein.\n");
			while (getchar() != '\n');
			continue;
		}

		int nummer_existiert = 0;
		// Ueberpruefen, ob Artikelnummer bereits existiert
		for (i = 0; i < anzahl_artikel_typen; i++) {
			if (artikeltyp_pntr->art_nummer == artikel_typ_liste[i].art_nummer) {
				printf("\nFehler: Artikel mit dieser Nummer existiert bereits. Bitte geben Sie eine andere Nummer ein.\n");
				nummer_existiert = 1;
				break; // Bricht Schleife ab, da Nummer schon existent
			}
		}

		if (!nummer_existiert) // Wenn eingegebene Nummer eindeutig ist
			break;
	}

	// Eingabe Preis in Euro und Ueberpruefung auf Doublewert
	while (1) {
		printf("\nPreis (in EUR): ");
		if (scanf("%lf", &artikeltyp_pntr->preis) != 1 || artikeltyp_pntr->preis <= 0 || getchar() != '\n') {
			printf("Fehler: Bitte geben Sie einen gueltigen Preis als Dezimalzahl groesser als 0 ein.\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}

	// Artikel Hoehe muss > 5 und <= 80 cm sein
	// Eingabe der Höhe als Double
	while (1) {
		printf("\nHoehe (in cm; > 5, wenn verderblich <= 40, sonst <= 80): ");
		if (scanf("%lf", &artikeltyp_pntr->hoehe) != 1 || artikeltyp_pntr->hoehe <= 5 || artikeltyp_pntr->hoehe > 80 || getchar() != '\n') {
			printf("Fehler: Die Hoehe muss groesser als 5 cm und kleiner oder gleich 80 cm sein. Bitte geben Sie einen gueltigen Wert ein!\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}

	// Artikel Breite darf nicht groesser als 300cm sein
	// Eingabe der Breite als Double
	while (1) {
		printf("\nBreite (in cm; > 5, <= 300): ");
		if (scanf("%lf", &artikeltyp_pntr->breite) != 1 || artikeltyp_pntr->breite <= 5 || artikeltyp_pntr->breite > 300 || getchar() != '\n') {
			printf("Fehler: Die Breite muss groesser als 5 cm und kleiner oder gleich 300 cm sein. Bitte geben Sie einen gueltigen Wert ein!\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}

	// Artikel Tiefe darf nicht groesser als 120cm sein
	// Eingabe der Tiefe als Double
	while (1) {
		printf("\nTiefe (in cm, <= 120): ");
		if (scanf("%lf", &artikeltyp_pntr->tiefe) != 1 || artikeltyp_pntr->tiefe > 120 || getchar() != '\n') {
			printf("Fehler: Die Tiefe darf nicht groesser als 120 cm sein. Bitte geben Sie einen gueltigen Wert ein!\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}

	// Abfrage, ob Artikel verderblich oder nicht (Entscheidung in welches Lager der Artikel geht)
	do {
		printf("\nWeisen Sie dem Artikel nun noch zu, ob er verderblich ist oder nicht.\nVerderblich: (1)\nNicht verderblich: (2)\n");
		if (scanf("%d", &lagerwahl) != 1 || (lagerwahl != 1 && lagerwahl != 2)) {
			printf("Fehler: Ungueltige Eingabe. Bitte waehlen Sie eine der angegebenen Optionen (1 oder 2).\n");
			while (getchar() != '\n');
			continue; 
		}

		if (artikeltyp_pntr->hoehe > 40 && lagerwahl == HALLE){
			printf("Fehler: Artikel mit einer Hoehe ueber 40 cm koennen nicht in Halle an der Saale eingelagert werden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!\n");
			while (getchar() != '\n');
			getchar();
			return 0;
		}
		else if (lagerwahl != HALLE && lagerwahl != PORTA_WESTFALICA) {
			printf("Fehler: Ungueltige Eingabe. Bitte waehlen Sie eine der angegebenen Optionen.\n");
		}
	} while (lagerwahl != HALLE && lagerwahl != PORTA_WESTFALICA);

	switch (lagerwahl) {
	case HALLE:
		// Artikel ist verderblich, Lagerort entsprechend festlegen und verarbeiten
		artikeltyp_pntr->lager = 1;
		halle_lager.artikel_typ_liste[halle_lager.anzahl_artikel_typen] = *artikeltyp_pntr;
		halle_lager.anzahl_artikel_typen++;
		//lager_aktualisieren(artikeltyp_pntr);
		printf("\nDer Artikel wurde erfolgreich in Halle an der Saale angelegt.\nDruecken Sie Enter, um fortzufahren.\n");
		break;
	case PORTA_WESTFALICA:
		// Artikel ist nicht verderblich, Lagerort entsprechend festlegen und verarbeiten
		artikeltyp_pntr->lager = 2;
		porta_lager.artikel_typ_liste[porta_lager.anzahl_artikel_typen] = *artikeltyp_pntr;
		porta_lager.anzahl_artikel_typen++;
		//lager_aktualisieren(artikeltyp_pntr);
		printf("\nDer Artikel wurde erfolgreich in Porta Westfalica angelegt.\nDruecken Sie Enter, um fortzufahren.\n");
		break;
	default:
		printf("\nUngueltige Eingabe. Bitte waehlen Sie eine der angegebenen Optionen.");
		break;
	}

	while (getchar() != '\n');
	getchar();

	bs_loeschen();
	printf("Artikel wurde erfolgreich angelegt. Druecke Enter, um zum Menue zu kommen!");
	printf("\n");
	printf("\nArtikel Name: %s", artikeltyp_pntr->name);
	printf("\nArtikel Nummer: %d", artikeltyp_pntr->art_nummer);
	printf("\nArtikel Preis: %.2lf", artikeltyp_pntr->preis);
	printf("\nArtikel Hoehe (in cm): %.2lf", artikeltyp_pntr->hoehe);
	printf("\nArtikel Breite (in cm): %.2lf", artikeltyp_pntr->breite);
	printf("\nArtikel Tiefe (in cm): %.2lf", artikeltyp_pntr->tiefe);

	// Hinzufuegen des neuen Artikels zur Liste
	artikel_typ_liste[anzahl_artikel_typen] = *artikeltyp_pntr;
	anzahl_artikel_typen++;

	while (getchar() != '\n');
	getchar();

	return 0;
}


int artikel_typ_bearbeiten() {
	int auswahl;
	char suchname[100];
	int suchnummer;
	int gefunden = 0;
	int i;

	bs_loeschen();
	printf("Artikel Typ bearbeiten");
	printf("\n");
	printf("\nMoechten Sie den Artikel anhand der Artikelnummer (1) oder des Namens (2) suchen?\n");
	scanf("%d", &auswahl);

	switch (auswahl) {
	case 1:
		printf("\nBitte geben Sie die Artikelnummer ein: ");
		scanf("%d", &suchnummer);
		for (i = 0; i < anzahl_artikel_typen; i++) {
			// Artikel in Artikelliste anhand Artikelnummer finden
			if (artikel_typ_liste[i].art_nummer == suchnummer) {
				printf("\nArtikel gefunden:\n");
				printf("\nArtikel Name: %s", artikel_typ_liste[i].name);
				printf("\nArtikel Nummer: %d", artikel_typ_liste[i].art_nummer);
				printf("\nArtikel Preis: %.2lf", artikel_typ_liste[i].preis);
				printf("\nArtikel Hoehe (in cm): %.2lf", artikel_typ_liste[i].hoehe);
				printf("\nArtikel Breite (in cm): %.2lf", artikel_typ_liste[i].breite);
				printf("\nArtikel Tiefe (in cm): %.2lf", artikel_typ_liste[i].tiefe);
				printf("\n");

				// Code für Aenderung Details des Artikels
				details_waehlen_bearbeitung(&artikel_typ_liste[i]);
				gefunden = 1;
				break;
			}
		}
		if (!gefunden) {
			printf("\nEin Artikel mit dieser Nummer wurde nicht gefunden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
			while (getchar() != '\n');
			getchar();
		}
		break;
	case 2:
		printf("\nBitte geben Sie den Artikelnamen ein: ");
		while (getchar() != '\n');
		getchar();
		fgets(suchname, sizeof(suchname), stdin); // Einlesen des Namens mit Leerzeichen
		suchname[strcspn(suchname, "\n")] = 0; // Entfernen des Zeilenumbruchs
		strtrim(suchname);
		for (i = 0; i < anzahl_artikel_typen; i++) {
			// Artikel in Artikelliste anhand Artikelname finden
			if (strcmp(artikel_typ_liste[i].name, suchname) == 0) {
				printf("\nArtikel gefunden:\n");
				printf("\nArtikel Name: %s", artikel_typ_liste[i].name);
				printf("\nArtikel Nummer: %d", artikel_typ_liste[i].art_nummer);
				printf("\nArtikel Preis: %.2lf", artikel_typ_liste[i].preis);
				printf("\nArtikel Hoehe (in cm): %.2lf", artikel_typ_liste[i].hoehe);
				printf("\nArtikel Breite (in cm): %.2lf", artikel_typ_liste[i].breite);
				printf("\nArtikel Tiefe (in cm): %.2lf", artikel_typ_liste[i].tiefe);
				printf("\n");

				// Code für die Aenderung Details Artikel
				details_waehlen_bearbeitung(&artikel_typ_liste[i]);
				gefunden = 1;
				break;
			}
		}
		if (!gefunden) {
			printf("\nEin Artikel mit diesem Namen wurde nicht gefunden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
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
	struct ArtikelTyp temp_artikel = *artikel; // Temporaere Kopie des Artikels
	int i;

	do {
		printf("\nWelches Detail moechten Sie bearbeiten?");
		printf("\n(1) Name");
		printf("\n(2) Artikelnummer");
		printf("\n(3) Preis");
		printf("\n");

		scanf("%d", &auswahl);
		switch (auswahl) {
		case 1:
			printf("\nBitte geben Sie den neuen Namen ein: ");
			getchar();
			fgets(temp_artikel.name, sizeof(temp_artikel.name), stdin); // Einlesen des Namens mit Leerzeichen
			temp_artikel.name[strcspn(temp_artikel.name, "\n")] = 0; // Entfernen des Zeilenumbruchs
			strtrim(temp_artikel.name);
			// Ueberpruefen, ob der Name bereits existiert
			for (i = 0; i < anzahl_artikel_typen; i++) {
				if (strcmp(temp_artikel.name, artikel_typ_liste[i].name) == 0 && temp_artikel.art_nummer != artikel_typ_liste[i].art_nummer) {
					printf("\nFehler: Der eingegebene Name existiert bereits in einem anderen Artikel.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
					while (getchar() != '\n');
					getchar();
					return 1;
				}
			}
			break;
		case 2:
			printf("\nBitte geben Sie die neue Artikelnummer ein:\n");
			// Ueberpruefen, ob Ganzzahl eingegeben wird
			while (scanf("%d", &temp_artikel.art_nummer) != 1) {
				printf("Fehler: Bitte geben Sie eine Ganzzahl fuer die Artikelnummer ein.\n");
				while (getchar() != '\n');
				printf("\nBitte geben Sie die neue Artikelnummer ein: ");
			}
			// Ueberpruefen, ob die Artikelnummer bereits existiert
			for (i = 0; i < anzahl_artikel_typen; i++) {
				if (temp_artikel.art_nummer == artikel_typ_liste[i].art_nummer && strcmp(temp_artikel.name, artikel_typ_liste[i].name) != 0) {
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
		default:
			printf("\nUngueltige Auswahl.");
			break;
		}

		printf("\nArtikel Name: %s", temp_artikel.name);
		printf("\nArtikel Nummer: %d", temp_artikel.art_nummer);
		printf("\nArtikel Preis: %.2lf", temp_artikel.preis);
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
		*artikel = temp_artikel; // Aktualisierung des Originalartikels mit den Aenderungen aus der temporaeren Kopie
		lager_aktualisieren(artikel); // Funktion zur Aktualisierung der Aenderungen im Lager 
	}

	return 0;
}

int artikel_typ_loeschen() {
	int i, j, k, l, m;
	int artikel_nummer;

	bs_loeschen();

	printf("Geben Sie die Artikelnummer des zu loeschenden Artikels ein: ");
	scanf("%d", &artikel_nummer);

	// Sucht nach dem Artikel in den Lagerlisten und loescht ihn, wenn j
	for (i = 0; i < halle_lager.anzahl_artikel_typen; i++) {
		if (halle_lager.artikel_typ_liste[i].art_nummer == artikel_nummer) {
			// Artikel gefunden, zeigt die Details an
			printf("\nArtikel Details:\n");
			printf("Name: %s\n", halle_lager.artikel_typ_liste[i].name);
			printf("Artikelnummer: %d\n", halle_lager.artikel_typ_liste[i].art_nummer);
			printf("Preis: %.2f EUR\n", halle_lager.artikel_typ_liste[i].preis);
			printf("Hoehe: %.2f cm\n", halle_lager.artikel_typ_liste[i].hoehe);
			printf("Breite: %.2f cm\n", halle_lager.artikel_typ_liste[i].breite);
			printf("Tiefe: %.2f cm\n", halle_lager.artikel_typ_liste[i].tiefe);

			// Bestaetigung vom Benutzer einholen
			int antwort;
			printf("Moechten Sie diesen ArtikelTyp und alle dazugehoerigen Artikel wirklich loeschen? (1 = Ja, 0 = Nein): ");
			scanf("%d", &antwort);

			if (antwort == 1) {
				// Artikel loeschen aus Lager in Halle
				for (j = i; j < halle_lager.anzahl_artikel_typen - 1; j++) {
					halle_lager.artikel_typ_liste[j] = halle_lager.artikel_typ_liste[j + 1];
				}
				halle_lager.anzahl_artikel_typen--;

				// Artikel aus Artikelliste loeschen
				for (k = 0; k < anzahl_artikel_typen; k++) {
					if (artikel_typ_liste[k].art_nummer == artikel_nummer) {
						for (l = k; l < anzahl_artikel_typen - 1; l++) {
							artikel_typ_liste[l] = artikel_typ_liste[l + 1];
						}
						anzahl_artikel_typen--;
						break;
					}
				}
				
				// Durchsuche alle Artikel und loesche alle von diesem ArtikelTyp
				for (m = 0; m < MAX_ARTIKEL; m++) {
					// Ueberspringe Stellen in gelagerte_artikel_liste, wo kein Artikel liegt
					if (gelagerte_artikel_liste[m].typ != NULL) {
						if (gelagerte_artikel_liste[m].typ->art_nummer == halle_lager.artikel_typ_liste[i].art_nummer) {
							if (artikel_aus_lager_entfernen(gelagerte_artikel_liste[m].inventarnummer) == 0) {
								continue;
							}
							else if (artikel_aus_lager_entfernen(gelagerte_artikel_liste[m].inventarnummer) == -1) {
								printf("Artikel nicht gefunden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
								while (getchar() != '\n');
								getchar();
								return;
							}
						}
					}
					else {
						continue;
					}
					
				}

				printf("Artikel erfolgreich geloescht.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return 0;
			}
			else {
				printf("Loeschvorgang abgebrochen.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return 1;
			}
		}
	}

	for (i = 0; i < porta_lager.anzahl_artikel_typen; i++) {
		if (porta_lager.artikel_typ_liste[i].art_nummer == artikel_nummer) {
			// Artikel gefunden, zeigt die Details an
			printf("\nArtikel Details:\n");
			printf("Name: %s\n", porta_lager.artikel_typ_liste[i].name);
			printf("Artikelnummer: %d\n", porta_lager.artikel_typ_liste[i].art_nummer);
			printf("Preis: %.2f EUR\n", porta_lager.artikel_typ_liste[i].preis);
			printf("Hoehe: %.2f cm\n", porta_lager.artikel_typ_liste[i].hoehe);
			printf("Breite: %.2f cm\n", porta_lager.artikel_typ_liste[i].breite);
			printf("Tiefe: %.2f cm\n", porta_lager.artikel_typ_liste[i].tiefe);

			// Bestaetigung vom Benutzer einholen
			int antwort;
			printf("Moechten Sie diesen Artikel wirklich loeschen? (1 = Ja, 0 = Nein): ");
			scanf("%d", &antwort);

			if (antwort == 1) {
				// Artikel loeschen aus Lager PW
				for (int j = i; j < porta_lager.anzahl_artikel_typen - 1; j++) {
					porta_lager.artikel_typ_liste[j] = porta_lager.artikel_typ_liste[j + 1];
				}
				porta_lager.anzahl_artikel_typen--;

				// Artikel aus Artikelliste (wo alle Artikel drin stehen) loeschen
				for (int k = 0; k < anzahl_artikel_typen; k++) {
					if (artikel_typ_liste[k].art_nummer == artikel_nummer) {
						for (int l = k; l < anzahl_artikel_typen - 1; l++) {
							artikel_typ_liste[l] = artikel_typ_liste[l + 1];
						}
						anzahl_artikel_typen--;
						break;
					}
				}

				// Durchsuche alle Artikel und loesche alle von diesem ArtikelTyp
				for (m = 0; m < MAX_ARTIKEL; m++) {
					// Ueberspringe Stellen in gelagerte_artikel_liste, wo kein Artikel liegt
					if (gelagerte_artikel_liste[m].typ != NULL) {
						if (gelagerte_artikel_liste[m].typ->art_nummer == porta_lager.artikel_typ_liste[i].art_nummer) {
							if (artikel_aus_lager_entfernen(gelagerte_artikel_liste[m].inventarnummer) == 0) {
								continue;
							}
							else if (artikel_aus_lager_entfernen(gelagerte_artikel_liste[m].inventarnummer) == -1) {
								printf("Artikel nicht gefunden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
								while (getchar() != '\n');
								getchar();
								return;
							}
						}
					}
					else {
						continue;
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
int vorhandene_artikel_typen_ansehen() {
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
		for (i = 0; i < halle_lager.anzahl_artikel_typen; i++) {
			struct ArtikelTyp *artikeltyp = &halle_lager.artikel_typ_liste[i];
			printf("\nArtikelname: %s", artikeltyp->name);
			printf("\nArtikelnummer: %d", artikeltyp->art_nummer);
			printf("\nArtikelpreis: %.2lf", artikeltyp->preis);
			printf("\nArtikelhoehe (in cm): %.2lf", artikeltyp->hoehe);
			printf("\nArtikelbreite (in cm): %.2lf", artikeltyp->breite);
			printf("\nArtikeltiefe (in cm): %.2lf", artikeltyp->tiefe);
			printf("\nArtikel davon im Lager: %d", artikeltyp->artikel_davon_im_lager);
			printf("\n");


		}
		break;
	case PORTA_WESTFALICA:
		printf("\nArtikel im PORTA WESTFALICA Lager:\n\n");
		for (i = 0; i < porta_lager.anzahl_artikel_typen; i++) {
			struct ArtikelTyp artikeltyp = porta_lager.artikel_typ_liste[i];
			printf("\nArtikelname: %s", artikeltyp.name);
			printf("\nArtikelnummer: %d", artikeltyp.art_nummer);
			printf("\nArtikelpreis: %.2lf", artikeltyp.preis);
			printf("\nArtikelhoehe (in cm): %.2lf", artikeltyp.hoehe);
			printf("\nArtikelbreite (in cm): %.2lf", artikeltyp.breite);
			printf("\nArtikeltiefe (in cm): %.2lf", artikeltyp.tiefe);
			printf("\nArtikel davon im Lager: %d", artikeltyp.artikel_davon_im_lager);
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

int lager_zufaellig_befuellen() {
	int lagerwahl;
	int anzahl_artikel;
	int i; 

	// Inventarnummern ausgeben

	srand(time(NULL));

	bs_loeschen();

	while (1) {
		printf("Welches Lager moechten Sie zufaellig befuellen?\n");
		printf("\n(1) HALLE Lager");
		printf("\n(2) PORTA WESTFALICA Lager\n");
		if (scanf("%d", &lagerwahl) != 1 || (lagerwahl != 1 && lagerwahl != 2)) {
			printf("Fehler: Bitte treffen Sie eine gueltige Auswahl (1 oder 2).\n");
			while (getchar() != '\n');
			continue;
		}
		break;
	}

	// Ueberpruefen, ob Artikeltypen im ausgewaehlten Lager vorhanden sind
	if ((lagerwahl == 1 && halle_lager.anzahl_artikel_typen == 0) ||
		(lagerwahl == 2 && porta_lager.anzahl_artikel_typen == 0)) {
		printf("Fehler: Die Lagerliste enthaelt keine Artikeltypen. Erstellen Sie zuerst einen neuen Artikeltypen.\nDruecken Sie Enter, um zum Menue zurueckzukehren!\n");
		while (getchar() != '\n');
		getchar();
		return -1;
	}

	switch (lagerwahl) {
	case 1:
		printf("Wie viele Artikel moechten Sie in das Lager eingelagern?\n");
		scanf("%d", &anzahl_artikel);
		
		for (i = 0; i < anzahl_artikel; i++) {
			int zufallsindex = rand() % halle_lager.anzahl_artikel_typen;	// Zufaelligen Artikel per Index waehlen
			if (artikel_erfassen(halle_lager.artikel_typ_liste[zufallsindex].art_nummer, &halle_lager.artikel_typ_liste[zufallsindex], 0) == -1) {
				printf("\nArtikel vom Typ %s konnte nicht eingelagert werden. Das Lager ist voll.\nDruecken Sie Enter, um zum Menue zurueckzukehren!\n\n", halle_lager.artikel_typ_liste[zufallsindex].name);
				while (getchar() != '\n');
				getchar();
				return -1;
			}
		}

		break;
	case 2:
		printf("Wie viele Artikel moechten Sie in das Lager eingelagern?\n");
		scanf("%d", &anzahl_artikel);

		for (i = 0; i < anzahl_artikel; i++) {
			int zufallsindex = rand() % porta_lager.anzahl_artikel_typen;	// Zufaelligen Artikel per Index waehlen
			if (artikel_erfassen(porta_lager.artikel_typ_liste[zufallsindex].art_nummer, &porta_lager.artikel_typ_liste[zufallsindex], 0) == -1) {
				printf("\nArtikel vom Typ %s konnte nicht eingelagert werden. Das Lager ist voll.\nDruecken Sie Enter, um zum Menue zurueckzukehren!\n\n", halle_lager.artikel_typ_liste[zufallsindex].name);
				while (getchar() != '\n');
				getchar();
				return -1;
			}
		}

		break;
	default:
		printf("Ungueltige Auswahl. Druecken Sie Enter, um zum Menue zurueckzukehren!\n");
		while (getchar() != '\n');
		getchar();
		return -1; 
	}

	printf("%d Artikel wurden erfolgreich erfasst! Druecken Sie Enter, um zum Menue zurueckzukehren!\n", anzahl_artikel);
	while (getchar() != '\n');
	getchar();

	return 0; 
}

// Manuelles anlegen einer Bestellung
int manuelle_bestellung_anlegen() {
	int i, j;
	int artikel_nummer;
	int anzahl_artikel;
	int artikel_gefunden = 0;
	int artikel_schon_im_warenausgang = 0;
	int weitere_artikel;
	struct ArtikelTyp artikeltyp;
	struct ArtikelTyp* artikeltyp_pntr;
	artikeltyp_pntr = &artikeltyp;
	struct ArtikelTypImWarenausgang artikel_typ_im_warenausgang;
	struct ArtikelTypImWarenausgang* artikel_typ_im_warenausgang_pntr;
	artikel_typ_im_warenausgang_pntr = &artikel_typ_im_warenausgang;

	do {
		artikel_typ_liste[MAX_ARTIKEL_TYP];
		bs_loeschen();
		printf("\nVorhandene Artikeltypen: ");
		for (i = 0; i < MAX_ARTIKEL_TYP; i++) {
			if (artikel_typ_liste[i].art_nummer != 0) {
				if (artikel_typ_liste[i].artikel_davon_im_lager != 0) {
					printf("\nArtikelname: %s", artikel_typ_liste[i].name);
					printf("\nArtikelnummer: %d", artikel_typ_liste[i].art_nummer);
					printf("\nArtikelpreis: %.2lf", artikel_typ_liste[i].preis);
					printf("\nArtikel davon im Lager: %d", artikel_typ_liste[i].artikel_davon_im_lager);
					printf("\n");
				}
			}
		}
		printf("\nWaehlen Sie eine Artikelnummer aus, die Sie bestellen moechten: ");
		scanf("%d", &artikel_nummer);
		bs_loeschen();
		for (i = 0; i < MAX_ARTIKEL_TYP; i++) {
			if (artikel_typ_liste[i].art_nummer == artikel_nummer) {
				artikel_gefunden = 1;
				if (artikel_typ_liste[i].artikel_davon_im_lager != 0) {
					artikeltyp_pntr = &artikel_typ_liste[i];
					printf("\nArtikelname: %s", artikeltyp_pntr->name);
					printf("\nArtikelnummer: %d", artikeltyp_pntr->art_nummer);
					printf("\nArtikelpreis: %.2lf", artikeltyp_pntr->preis);
					printf("\nArtikel davon im Lager: %d", artikeltyp_pntr->artikel_davon_im_lager);
					printf("\n");
				}
				else {
					printf("\nEs gibt keine Artikel von dem gewaehlten Typ im Lager. Druecken Sie Enter, um zum Menue zurueckzukehren!\n");
					while (getchar() != '\n');
					getchar();
					return -1;
				}
			}
		}

		if (artikel_gefunden == 0) {
			printf("\nDer gesuchte Artikeltyp exisitert nicht. Druecken Sie Enter, um zum Menue zurueckzukehren!\n");
			while (getchar() != '\n');
			getchar();
			return -1;
		}

		do {
			printf("\nWaehlen Sie eine Anzahl die bestellt werden soll: ");
			scanf("%d", &anzahl_artikel);
			if (anzahl_artikel > artikeltyp_pntr->artikel_davon_im_lager) {
				printf("\nEs gibt nicht genuegend Artikel davon im Lager! Druecken Sie Enter, um erneut die Anzahl einzugeben!");
				while (getchar() != '\n');
				getchar();
			}
		} while (anzahl_artikel > artikeltyp_pntr->artikel_davon_im_lager);

		artikel_typ_im_warenausgang_pntr->artikel_typ = artikeltyp_pntr;

		for (j = 0; j < MAX_ARTIKEL_TYP; j++) {
			if (warenausgang[j].artikel_typ != NULL) {
				if (warenausgang[j].artikel_typ->art_nummer == artikel_typ_im_warenausgang_pntr->artikel_typ->art_nummer) {
					artikel_schon_im_warenausgang = 1;
					if (warenausgang[j].anzahl_von_diesem_typ_im_warenausgang + anzahl_artikel > artikeltyp_pntr->artikel_davon_im_lager) {
						printf("\nEs gibt nicht genuegend Artikel davon im Lager! Artikel kann nicht mehr so oft bestellt werden!\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
						while (getchar() != '\n');
						getchar();
						return -1;
					}
					warenausgang[j].anzahl_von_diesem_typ_im_warenausgang += anzahl_artikel;
				}
			}
		}

		// Artikeltyp ist noch nicht im Warenausgang
		if (artikel_schon_im_warenausgang == 0) {
			for (j = 0; j < MAX_ARTIKEL_TYP; j++) {
				if (warenausgang[j].artikel_typ == NULL) {
					artikel_typ_im_warenausgang_pntr->anzahl_von_diesem_typ_im_warenausgang = anzahl_artikel;
					warenausgang[j] = *artikel_typ_im_warenausgang_pntr;
					break;
				}
			}
		}

		printf("\nArtikel %s wurde dem Warenausgang %d mal hinzugefuegt!", artikeltyp_pntr->name, anzahl_artikel);
		printf("\nMoechten Sie weitere Artikel zu der Bestellung hinzufuegen? (1 = Ja, 0 = Nein)\n");
		scanf("%d", &weitere_artikel);
	} while (weitere_artikel == 1);
}

// Aktualisierung der Aenderungen am Artikel im Lager
int lager_aktualisieren(struct ArtikelTyp *artikel) {
	int i;
	for (i = 0; i < anzahl_artikel_typen; i++) {
		if (halle_lager.artikel_typ_liste[i].art_nummer == artikel->art_nummer) {
			halle_lager.artikel_typ_liste[i] = *artikel;
			break;
		}
	}

	for (i = 0; i < anzahl_artikel_typen; i++) {
		if (porta_lager.artikel_typ_liste[i].art_nummer == artikel->art_nummer) {
			porta_lager.artikel_typ_liste[i] = *artikel;
			break;
		}
	}
}

int atikel_manuell_erfassen() {
	int gefunden = 0;
	int eingabeNummer;
	char wahl;
	int i;


	do {
		gefunden = 0; // Zuruecksetzen des gefunden-Flags

		bs_loeschen();

		printf("Geben Sie die Artikelnummer ein: ");
		if (scanf("%d", &eingabeNummer) == 1) {
			for (i = 0; i < anzahl_artikel_typen; i++) {
				if (artikel_typ_liste[i].art_nummer == eingabeNummer) {
					gefunden = 1;
					printf("\nArtikeltyp gefunden:");
					printf("\nName: %s", artikel_typ_liste[i].name);
					printf("\nArtikelnummer: %d", artikel_typ_liste[i].art_nummer);
					printf("\nPreis (in EUR): %.2lf", artikel_typ_liste[i].preis);
					printf("\nHoehe (in cm): %.2lf", artikel_typ_liste[i].hoehe);
					printf("\nBreite (in cm): %.2lf", artikel_typ_liste[i].breite);
					printf("\nTiefe (in cm): %.2lf", artikel_typ_liste[i].tiefe);
					printf("\nLager: %d", artikel_typ_liste[i].lager);
					printf("\nMoechten Sie zum Menue zurueckkehren (0), nach einem anderen Artikel suchen (1) oder den Artikel einlagern (2)?:\n");
					scanf(" %c", &wahl);
					if (wahl == '0') {
						printf("\nZurueck zum Menue...");
						return 0;
					}
					else if (wahl == '2') {
						if (artikel_erfassen(eingabeNummer, &artikel_typ_liste[i], 1) == -1) {
							printf("\nArtikel konnte nicht eingelagert werden. Keine passende Position gefunden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!\n\n");
							while (getchar() != '\n');
							getchar();
							return -1;
						}
					}
				}
			}
		}

		if (!gefunden) {
			printf("\nArtikel nicht gefunden. Druecken Sie Enter, um zum Menue zurueckzukehren!\n\n");
			while (getchar() != '\n');
			getchar();
			return -1;
		}
		printf("Moechten Sie zum Menue zurueckkehren (0) oder einen neuen Artikel suchen (1) ?: ");
		scanf(" %c", &wahl);
	} while (wahl != '0');

	return 0;
}

// Funktion zum Suchen, Anzeigen und einlagern des Artikels
int artikel_erfassen(int artikelnummer, struct ArtikelTyp *artikel_typ_pntr, int ausgabe_aktiviert) {
	// struct ArtikelTyp *artikel_typ_pntr;
	// artikel_typ_pntr = &artikel_typ_liste[i];

	struct Artikel artikel;
	artikel.typ = artikel_typ_pntr;
	artikel.inventarnummer = generiere_inventarnummer(); // Hier wird die Inventarnummer generiert und zugewiesen;

	if (artikel_einlagern_nach_nummer(artikelnummer, artikel_typ_pntr->lager, artikel, ausgabe_aktiviert) == -1) {
		return -1;
	}
	return 0;
}

int generiere_inventarnummer() {
	// Inkrementiere Zaehler und gib neue Inventarnummer zurück
	return ++inventarnummerZaehler;
}

// Artikel anhand der Artikelnummer einlagern
int artikel_einlagern_nach_nummer(int artikelnummer, int lager, struct Artikel artikel, int ausgabe_aktiviert) {
	int i, j;

	struct Artikel *artikel_pntr;
	artikel_pntr = &artikel;

	// Ueberpruefen des Lagers und Berechnungen, vielleicht als separate Funktionen nochmal
	if (lager == 1) {
		// Aktionen für Lager 1 (HALLE)
		if (lagere_artikel_an_positions_ids_Halle(artikel_pntr) != -1) {
			if (ausgabe_aktiviert == 1) {
				print_zugeordnete_ids(artikel_pntr); // Ausgabe der zugeordneten IDs
			}
			artikel_pntr->typ->artikel_davon_im_lager++;
			lager_aktualisieren(artikel_pntr->typ);
			for (j = 0; j < MAX_ARTIKEL; j++) {
				if (gelagerte_artikel_liste[j].inventarnummer == 0) {
					gelagerte_artikel_liste[j] = *artikel_pntr;
					break;
				}
			}
			if (ausgabe_aktiviert == 1) {
				printf("\nArtikel mit der Nummer %d wurde erfolgreich im Halle Lager eingelagert.\n", artikelnummer);
				printf("\nArtikel von Typ %s im Lager: %d\n", artikel_pntr->typ->name, artikel_pntr->typ->artikel_davon_im_lager);
			}
		}
		else {
			if (ausgabe_aktiviert == 1) {
				printf("Artikel mit der Nummer %d konnte nicht eingelagert werden. Keine passende Position gefunden.\n", artikelnummer);
			}
			return -1;
		}
		if (ausgabe_aktiviert == 1) {
			printf("Druecken Sie Enter, um weitere Aktionen auszufuehren!\n");
			while (getchar() != '\n');
			getchar();
		}
	}
	else if (lager == 2) {
		// Aktionen für Lager 2 (PW)
		if (lagere_artikel_an_positions_ids_Porta(artikel_pntr) != -1) {
			if (ausgabe_aktiviert == 1) {
				print_zugeordnete_ids(artikel_pntr); // Ausgabe der zugeordneten IDs
			}
			artikel_pntr->typ->artikel_davon_im_lager++;
			lager_aktualisieren(artikel_pntr->typ);
			for (j = 0; j < MAX_ARTIKEL; j++) {
				if (gelagerte_artikel_liste[j].inventarnummer == 0) {
					gelagerte_artikel_liste[j] = *artikel_pntr;
					break;
				}
			}
			if (ausgabe_aktiviert == 1) {
				printf("\nArtikel mit der Nummer %d wurde erfolgreich im Halle Lager eingelagert.\n", artikelnummer);
				printf("\nArtikel von Typ %s im Lager: %d\n", artikel_pntr->typ->name, artikel_pntr->typ->artikel_davon_im_lager);
			}
		}
		else {
			if (ausgabe_aktiviert == 1) {
				printf("Artikel mit der Nummer %d konnte nicht eingelagert werden. Keine passende Position gefunden.\n", artikelnummer);
			}
			return -1;
		}
		if (ausgabe_aktiviert == 1) {
			printf("Druecken Sie Enter, um weitere Aktionen auszufuehren!\n");
			while (getchar() != '\n');
			getchar();
		}
	}

	return 1;
}

// Artikel in Halle einlagern
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

	int lager_halle_20_cm_voll = 0; // Flag, ob Lagerplatz mit 20cm hohen Faechern voll ist
	int ids_von_artikel_belegt = 0; // Anzahl der vom Artikel belegten IDs

	// Berechnung der Anzahl der vom Artikel belegten IDs basierend auf Breite und Tiefe
	ids_von_artikel_belegt = (int)(breite / 10); // Breite belegt breite/10 IDs
	// Aufrunden der Breite
	if ((breite > 5 && breite < 10) || (breite > 10 && breite < 20) || (breite > 20 && breite < 30) || (breite > 30 && breite < 40) || (breite > 40 && breite < 50) || (breite > 50 && breite < 60)) {
		ids_von_artikel_belegt += 1;
	}
	if (tiefe > 60) {
		ids_von_artikel_belegt *= 2; // Tiefe > 60 belegt das doppelte an IDs
	}

	artikel->anzahl_positions_ids = 0;

	if (hoehe <= 20) {
		start_id = START_ID_HALLE_20CM;

		// Durchlaufen aller moeglichen IDs für Halle 20cm Lagerplatz 
		for (aktuelle_id = start_id; aktuelle_id < max_anzahl_halle_20; aktuelle_id++) {
			int available = 0;
			i = aktuelle_id - START_ID_HALLE_20CM;
			// Ueberpruefen, ob aktuelle ID schon in der Liste belegte_ids_halle_20 vorhanden ist
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
						// Ueberpruefen, ob PositionsID den Flag "voll" gesetzt hat
						// Wenn ja, fange mit der nächsten ID neu an
						if (belegte_ids_halle_20[i].positions_id_voll == 1) {
							continue;
						}
						// Ueberpruefen, ob der Artikel von der Hoehe noch in die PositionsID passt
						// Wenn ja, dann ist diese ID Potentiell moeglich
						else if (belegte_ids_halle_20[i].resthoehe >= hoehe) {
							available = 1;
						}
					}
					else {
						continue;
					}
				}
			}
			// aktuelle ID ist potentiell moeglich
			// Ueberpruefen, ob die nachfolgenden IDs auch noch frei sind (wenn Artikel mehrere belegt)
			if (available == 1) {
				// Der ID Index entspricht der ID Nummer, ohne die Lagererkennung (12000000)
				int aktuelle_id_index = aktuelle_id - 12000000;
				int letzte_belegte_id = aktuelle_id_index + ids_von_artikel_belegt;
				int id_index_zaheler;
				// Durchlaufen für alle vom Artikel benoetigten Positions IDs
				for (id_index_zaheler = aktuelle_id_index; id_index_zaheler <= letzte_belegte_id; id_index_zaheler++) {
					// Steht an der Stelle im Array ueberhaupt schon was?
					if (belegte_ids_halle_20[id_index_zaheler].id == NULL) {
						continue;
					}
					// Ueberpruefen, ob die Artikelnummer an dieser Stelle gleich der Artikelnummer des einzulagernden Artikels ist
					else if (belegte_ids_halle_20[id_index_zaheler].artikelnummer == artikel->typ->art_nummer) {
						// Ueberpruefen, ob das Fach als "voll" makiert ist, wenn nicht, dann ist diese ID nicht verfuegbar, es muss also eine andere ID gesucht werden
						if (belegte_ids_halle_20[id_index_zaheler].positions_id_voll == 1) {
							available = 0;
						}
						// Ueberpruefen, ob der einzulagernde Artikel in die Resthoehe des Fachs passt, wenn ja, dann durchlaufe alles auch fuer die nächsten benoetigten IDs
						else if (belegte_ids_halle_20[id_index_zaheler].resthoehe >= artikel->typ->hoehe) {
							continue;
						}
					}
					// ID nicht verfuegbar, suche andere ID
					else {
						available = 0;
						break;
					}
				}

				// Aktuelle ID (und nachfolgende) sind moeglich, Artikel kann an die Stelle eingelagert werden
				if (available == 1) {
					// Für jede vom Artikel belegte ID einen struct anlegen
					for (aktuelle_id_index; aktuelle_id_index < letzte_belegte_id; aktuelle_id_index++) {
						double resthoehe;
						// Hier wird die richtige ID wieder mit der Lagerkennung zusammengebaut
						aktuelle_id = aktuelle_id_index + 12000000;

						// Ueberpruefen, ob die aktuelle ID schon als PositionsID erstellt ist
						// Wenn nicht (== NULL), dann erstelle eine neue PositionsID
						if (belegte_ids_halle_20[aktuelle_id_index].id == NULL) {
							struct PositionsID positions_id;
							positions_id.id = aktuelle_id;
							positions_id.artikelnummer = artikel->typ->art_nummer;
							positions_id.positions_id_voll = 0;

							// Berechnung der Resthoehe
							double resthoehe = 20 - artikel->typ->hoehe;
							positions_id.resthoehe = resthoehe;

							// Wenn Resthoehe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}

							// PositionsID als belegt Kennzeichnen
							belegte_ids_halle_20[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
						else {
							struct PositionsID positions_id = belegte_ids_halle_20[aktuelle_id_index];

							// Wenn Resthoehe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							double resthoehe = positions_id.resthoehe - artikel->typ->hoehe;
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}
							positions_id.resthoehe = resthoehe;

							// PositionsID als belegt Kennzeichnen
							belegte_ids_halle_20[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
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
	if (((hoehe > 20) && (hoehe <= 40)) || lager_halle_20_cm_voll == 1) {
		start_id = START_ID_HALLE_40CM;

		// Durchlaufen aller moeglichen IDs fuer Halle 40cm Lagerplatz 
		for (aktuelle_id = start_id; aktuelle_id < max_anzahl_halle_40; aktuelle_id++) {
			int available = 0;
			int i = aktuelle_id - START_ID_HALLE_40CM;
			// Ueberpruefen, ob aktuelle ID schon in der Liste belegte_ids_halle_40 vorhanden ist
			// Wenn an der Stelle i in belegte_ids_halle_40 noch kein Element vorhanden ist, wir die ID als verfuegbar angesehen
			if (belegte_ids_halle_40[i].id == NULL) {
				available = 1;
			}
			else {
				// Vergleich, ob die aktuelle_id in belegte_ids_halle_40 vorhanden ist
				if (belegte_ids_halle_40[i].id == aktuelle_id) {
					// Vergleich, ob die Artikelnummer der PositionsID an der Stelle i in belegte_ids_halle_40 gleich der Artikel Nummer von dem ArtikelTyp des Artikels ist
					// Wenn nicht, fange mit der naechsten ID neu an
					if (belegte_ids_halle_40[i].artikelnummer == artikel->typ->art_nummer) {
						// Ueberpruefen, ob PositionsID den Flag "voll" gesetzt hat
						// Wenn ja, fange mit der naechsten ID neu an
						if (belegte_ids_halle_40[i].positions_id_voll == 1) {
							continue;
						}
						// Ueberpruefen, ob der Artikel von der Hoehe noch in die PositionsID passt
						// Wenn ja, dann ist diese ID Potentiell moeglich
						else if (belegte_ids_halle_40[i].resthoehe >= hoehe) {
							available = 1;
						}
					}
					else {
						continue;
					}
				}
			}
			// aktuelle ID ist potentiell moeglich
			// Ueberpruefen, ob die nachfolgenden IDs auch noch frei sind (wenn Artikel mehrere belegt)
			if (available == 1) {
				// Der ID Index entspricht der ID Nummer, ohne die Lagererkennung (14000000)
				int aktuelle_id_index = aktuelle_id - 14000000;
				int letzte_belegte_id = aktuelle_id_index + ids_von_artikel_belegt;
				int id_index_zaheler;
				// Durchlaufen fuer alle vom Artikel benoetigten Positions IDs
				for (id_index_zaheler = aktuelle_id_index; id_index_zaheler <= letzte_belegte_id; id_index_zaheler++) {
					// Steht an der Stelle im Array ueberhaupt schon was?
					if (belegte_ids_halle_40[id_index_zaheler].id == NULL) {
						continue;
					}
					// Ueberpruefen, ob die Artikelnummer an dieser Stelle gleich der Artikelnummer des einzulagernden Artikels ist
					else if (belegte_ids_halle_40[id_index_zaheler].artikelnummer == artikel->typ->art_nummer) {
						// Ueberpruefen, ob das Fach als "voll" markiert ist, wenn nicht, dann ist diese ID nicht verfuegbar, es muss also eine andere ID gesucht werden
						if (belegte_ids_halle_40[id_index_zaheler].positions_id_voll == 1) {
							available = 0;
						}
						// Ueberpruefen, ob der einzulagernde Artikel in die Resthoehe des Fachs passt, wenn ja, dann durchlaufe alles auch fuer die nächsten benoetigten IDs
						else if (belegte_ids_halle_40[id_index_zaheler].resthoehe >= artikel->typ->hoehe) {
							continue;
						}
					}
					// ID nicht verfuegbar, suche andere ID
					else {
						available = 0;
						break;
					}
				}

				// Aktuelle ID (und nachfolgende) sind moeglich, artikel kann an die Stelle eingelagert werden
				if (available == 1) {
					// Für jede vom Artikel belegte ID einen struct anlegen
					for (aktuelle_id_index; aktuelle_id_index < letzte_belegte_id; aktuelle_id_index++) {
						double resthoehe;
						// Hier wird die richtige ID wieder mit der Lagerkennung zusammengebaut
						aktuelle_id = aktuelle_id_index + 14000000;

						// Ueberpruefen, ob aktuelle ID schon als PositionsID erstellt ist
						// Wenn nicht (== NULL), dann erstelle eine neue PositionsID
						if (belegte_ids_halle_40[aktuelle_id_index].id == NULL) {
							struct PositionsID positions_id;
							positions_id.id = aktuelle_id;
							positions_id.artikelnummer = artikel->typ->art_nummer;
							positions_id.positions_id_voll = 0;

							// Berechnung der Resthoehe
							double resthoehe = 40 - artikel->typ->hoehe;
							positions_id.resthoehe = resthoehe;

							// Wenn Resthoehe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}

							// PositionsID als belegt Kennzeichnen
							belegte_ids_halle_40[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
						else {
							struct PositionsID positions_id = belegte_ids_halle_40[aktuelle_id_index];

							// Wenn Resthoehe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							double resthoehe = positions_id.resthoehe - artikel->typ->hoehe;
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}
							positions_id.resthoehe = resthoehe;
							// PositionsID als belegt Kennzeichnen
							belegte_ids_halle_40[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
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
	// Rueckgabe eines ungueltigen Wertes, falls keine passende Position gefunden wurde
	return -1;
}

// Artikel in Porta WF einlagern
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

	int lager_porta_20_cm_voll = 0; // Flag, ob Lagerplatz mit 20cm hohen Faechern voll ist
	int lager_porta_40_cm_voll = 0; // Flag, ob Lagerplatz mit 40cm hohen Faechern voll ist

	int ids_von_artikel_belegt = 0; // Anzahl der belegten IDs

	// Berechnung der Anzahl der belegten IDs basierend auf Breite und Tiefe
	ids_von_artikel_belegt = (int)(breite / 10); // Breite belegt breite/10 IDs
	// Aufrunden der Breite
	if ((breite > 5 && breite < 10) || (breite > 10 && breite < 20) || (breite > 20 && breite < 30) || (breite > 30 && breite < 40) || (breite > 40 && breite < 50) || (breite > 50 && breite < 60)) {
		ids_von_artikel_belegt += 1;
	}
	if (tiefe > 60) {
		ids_von_artikel_belegt *= 2; // Tiefe > 60 belegt das doppelte an IDs
	}

	artikel->anzahl_positions_ids = 0;

	if (hoehe <= 20) {
		start_id = START_ID_PORTA_20CM;

		// Durchlaufen aller moeglichen IDs für Porta 20cm Lagerplatz 
		for (aktuelle_id = start_id; aktuelle_id < max_anzahl_porta_20; aktuelle_id++) {
			int available = 0;
			i = aktuelle_id - START_ID_PORTA_20CM;
			// Ueberpruefen, ob aktuelle ID schon in der Liste belegte_ids_porta_20 vorhanden ist
			// Wenn an der Stelle i in belegte_ids_porta_20 noch kein Element vorhanden ist, wir die ID als verfuegbar angesehen
			if (belegte_ids_porta_20[i].id == NULL) {
				available = 1;
			}
			else {
				// Vergleich, ob die aktuelle_id in belegte_ids_porta_20 vorhanden ist
				if (belegte_ids_porta_20[i].id == aktuelle_id) {
					// Vergleich, ob die Artikelnummer der PositionsID an der Stelle i in belegte_ids_porta_20 gleich der Artikel Nummer von dem ArtikelTyp des Artikels ist
					// Wenn nicht, fange mit der naechsten ID neu an
					if (belegte_ids_porta_20[i].artikelnummer == artikel->typ->art_nummer) {
						// Ueberpruefen, ob PositionsID den Flag "voll" gesetzt hat
						// Wenn ja, fange mit der naechsten ID neu an
						if (belegte_ids_porta_20[i].positions_id_voll == 1) {
							continue;
						}
						// Ueberpruefen, ob der Artikel von der Hoehe noch in die PositionsID passt
						// Wenn ja, dann ist diese ID Potentiell moeglich
						else if (belegte_ids_porta_20[i].resthoehe >= hoehe) {
							available = 1;
						}
					}
					else {
						continue;
					}
				}
			}
			// aktuelle ID ist potentiell moeglich
			// UEberpruefen, ob die nachfolgenden IDs auch noch frei sind (wenn Artikel mehrere belegt)
			if (available == 1) {
				// Der ID Index entspricht der ID Nummer, ohne die Lagererkennung (12000000)
				int aktuelle_id_index = aktuelle_id - 22000000;
				int letzte_belegte_id = aktuelle_id_index + ids_von_artikel_belegt;
				int id_index_zaheler;
				// Durchlaufen fuer alle vom Artikel benoetigten Positions IDs
				for (id_index_zaheler = aktuelle_id_index; id_index_zaheler <= letzte_belegte_id; id_index_zaheler++) {
					// Steht an der Stelle im Array ueberhaupt schon was?
					if (belegte_ids_porta_20[id_index_zaheler].id == NULL) {
						continue;
					}
					// Ueberpruefen, ob die Artikelnummer an dieser Stelle gleich der Artikelnummer des einzulagernden Artikels ist
					else if (belegte_ids_porta_20[id_index_zaheler].artikelnummer == artikel->typ->art_nummer) {
						// Ueberpruefen, ob das Fach als "voll" makiert ist, wenn nicht, dann ist diese ID nicht verfuegbar, es muss also eine andere ID gesucht werden
						if (belegte_ids_porta_20[id_index_zaheler].positions_id_voll == 1) {
							available = 0;
						}
						// Ueberpruefen, ob der einzulagernde Artikel in die Resthoehe des Fachs passt, wenn ja, dann durchlaufe alles auch fuer die naechsten benoetigten IDs
						else if (belegte_ids_porta_20[id_index_zaheler].resthoehe >= artikel->typ->hoehe) {
							continue;
						}
					}
					// ID nicht verfuegbar, suche andere ID
					else {
						available = 0;
						break;
					}
				}

				// Aktuelle ID (und nachfolgende) sind moeglich, Artikel kann an die Stelle eingelagert werden
				if (available == 1) {
					// Fuer jede vom Artikel belegte ID einen struct anlegen
					for (aktuelle_id_index; aktuelle_id_index < letzte_belegte_id; aktuelle_id_index++) {
						double resthoehe;
						// Hier wird die richtige ID wieder mit der Lagerkennung zusammengebaut
						aktuelle_id = aktuelle_id_index + 22000000;

						// Ueberpruefen, ob die aktuelle ID schon als PositionsID erstellt ist
						// Wenn nicht (== NULL), dann erstelle eine neue PositionsID
						if (belegte_ids_porta_20[aktuelle_id_index].id == NULL) {
							struct PositionsID positions_id;
							positions_id.id = aktuelle_id;
							positions_id.artikelnummer = artikel->typ->art_nummer;
							positions_id.positions_id_voll = 0;

							// Berechnung der Resthoehe
							double resthoehe = 20 - artikel->typ->hoehe;
							positions_id.resthoehe = resthoehe;

							// Wenn Resthoehe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_20[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
						else {
							struct PositionsID positions_id = belegte_ids_porta_20[aktuelle_id_index];

							// Wenn Resthoehe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							double resthoehe = positions_id.resthoehe - artikel->typ->hoehe;
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}
							positions_id.resthoehe = resthoehe;

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_20[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
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
	if (((hoehe > 20) && (hoehe <= 40)) || lager_porta_20_cm_voll == 1) {
		start_id = START_ID_PORTA_40CM;

		// Durchlaufen aller moeglichen IDs für Porta 40cm Lagerplatz 
		for (aktuelle_id = start_id; aktuelle_id < max_anzahl_porta_40; aktuelle_id++) {
			int available = 0;
			i = aktuelle_id - START_ID_PORTA_40CM;
			// Ueberpruefen, ob aktuelle ID schon in der Liste belegte_ids_porta_40 vorhanden ist
			// Wenn an der Stelle i in belegte_ids_porta_40 noch kein Element vorhanden ist, wir die ID als verfuegbar angesehen
			if (belegte_ids_porta_40[i].id == NULL) {
				available = 1;
			}
			else {
				// Vergleich, ob die aktuelle_id in belegte_ids_porta_40 vorhanden ist
				if (belegte_ids_porta_40[i].id == aktuelle_id) {
					// Vergleich, ob die Artikelnummer der PositionsID an der Stelle i in belegte_ids_porta_40 gleich der Artikel Nummer von dem ArtikelTyp des Artikels ist
					// Wenn nicht, fange mit der naechsten ID neu an
					if (belegte_ids_porta_40[i].artikelnummer == artikel->typ->art_nummer) {
						// Ueberprueft, ob PositionsID den Flag "voll" gesetzt hat
						// Wenn ja, fange mit der naechsten ID neu an
						if (belegte_ids_porta_40[i].positions_id_voll == 1) {
							continue;
						}
						// Ueberprueft, ob der Artikel von der Hoehe noch in die PositionsID passt
						// Wenn ja, dann ist diese ID Potentiell moeglich
						else if (belegte_ids_porta_40[i].resthoehe >= hoehe) {
							available = 1;
						}
					}
					else {
						continue;
					}
				}
			}
			// Aktuelle ID ist potentiell moeglich
			// Ueberpruefen, ob die nachfolgenden IDs auch noch frei sind (wenn Artikel mehrere belegt)
			if (available == 1) {
				// Der ID Index entspricht der ID Nummer, ohne die Lagererkennung (24000000)
				int aktuelle_id_index = aktuelle_id - 2400000;
				int letzte_belegte_id = aktuelle_id_index + ids_von_artikel_belegt;
				int id_index_zaheler;
				// Durchlaufen fuer alle vom Artikel benoetigten Positions IDs
				for (id_index_zaheler = aktuelle_id_index; id_index_zaheler <= letzte_belegte_id; id_index_zaheler++) {
					// Steht an der Stelle im Array ueberhaupt schon was?
					if (belegte_ids_porta_40[id_index_zaheler].id == NULL) {
						continue;
					}
					// Ueberpruefen, ob die Artikelnummer an dieser Stelle gleich der Artikelnummer des einzulagernden Artikels ist
					else if (belegte_ids_porta_40[id_index_zaheler].artikelnummer == artikel->typ->art_nummer) {
						// Ueberpruefen, ob das Fach als "voll" makiert ist, wenn nicht, dann ist diese ID nicht verfuegbar, es muss also eine andere ID gesucht werden
						if (belegte_ids_porta_40[id_index_zaheler].positions_id_voll == 1) {
							available = 0;
						}
						// Ueberpruefen, ob der einzulagernde Artikel in die Resthoehe des Fachs passt, wenn ja, dann durchlaufe alles auch fuer die naechsten benoetigten IDs
						else if (belegte_ids_porta_40[id_index_zaheler].resthoehe >= artikel->typ->hoehe) {
							continue;
						}
					}
					// ID nicht verfuegbar, suche andere ID
					else {
						available = 0;
						break;
					}
				}

				// Aktuelle ID (und nachfolgende) sind moeglich, Artikel kann an die Stelle eingelagert werden
				if (available == 1) {
					// Fuer jede vom Artikel belegte ID einen struct anlegen
					for (aktuelle_id_index; aktuelle_id_index < letzte_belegte_id; aktuelle_id_index++) {
						double resthoehe;
						// Hier wird die richtige ID wieder mit der Lagerkennung zusammengebaut
						aktuelle_id = aktuelle_id_index + 2400000;

						// Ueberprueft, ob die aktuelle ID schon als PositionsID erstellt ist
						// Wenn nicht (== NULL), dann erstelle eine neue PositionsID
						if (belegte_ids_porta_40[aktuelle_id_index].id == NULL) {
							struct PositionsID positions_id;
							positions_id.id = aktuelle_id;
							positions_id.artikelnummer = artikel->typ->art_nummer;
							positions_id.positions_id_voll = 0;

							// Berechnung der Resthoehe
							double resthoehe = 20 - artikel->typ->hoehe;
							positions_id.resthoehe = resthoehe;

							// Wenn Resthoehe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_40[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
						else {
							struct PositionsID positions_id = belegte_ids_porta_40[aktuelle_id_index];

							// Wenn Resthoehe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							double resthoehe = positions_id.resthoehe - artikel->typ->hoehe;
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}
							positions_id.resthoehe = resthoehe;

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_40[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
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
	if (((hoehe > 40) && (hoehe <= 80)) || lager_porta_40_cm_voll == 1) {
		start_id = START_ID_PORTA_80CM;

		// Durchlaufen aller moeglichen IDs für Porta 80cm Lagerplatz 
		for (aktuelle_id = start_id; aktuelle_id < max_anzahl_porta_80; aktuelle_id++) {
			int available = 0;
			i = aktuelle_id - START_ID_PORTA_80CM;
			// Ueberpruefen, ob aktuelle ID schon in der Liste belegte_ids_porta_80 vorhanden ist
			// Wenn an der Stelle i in belegte_ids_porta_80 noch kein Element vorhanden ist, wir die ID als verfuegbar angesehen
			if (belegte_ids_porta_80[i].id == NULL) {
				available = 1;
			}
			else {
				// Vergleich, ob die aktuelle_id in belegte_ids_porta_80 vorhanden ist
				if (belegte_ids_porta_80[i].id == aktuelle_id) {
					// Vergleich, ob die Artikelnummer der PositionsID an der Stelle i in belegte_ids_porta_80 gleich der Artikel Nummer von dem ArtikelTyp des Artikels ist
					// Wenn nicht, fange mit der naechsten ID neu an
					if (belegte_ids_porta_80[i].artikelnummer == artikel->typ->art_nummer) {
						// Ueberprueft, ob PositionsID den Flag "voll" gesetzt hat
						// Wenn ja, fange mit der naechsten ID neu an
						if (belegte_ids_porta_80[i].positions_id_voll == 1) {
							continue;
						}
						// Ueberprueft, ob der Artikel von der Hoehe noch in die PositionsID passt
						// Wenn ja, dann ist diese ID potentiell moeglich
						else if (belegte_ids_porta_80[i].resthoehe >= hoehe) {
							available = 1;
						}
					}
					else {
						continue;
					}
				}
			}
			// Aktuelle ID ist potentiell moeglich
			// Ueberpruefen, ob die nachfolgenden IDs auch noch frei sind (wenn Artikel mehrere belegt)
			if (available == 1) {
				// Der ID Index entspricht der ID Nummer, ohne die Lagererkennung (28000000)
				int aktuelle_id_index = aktuelle_id - 2800000;
				int letzte_belegte_id = aktuelle_id_index + ids_von_artikel_belegt;
				int id_index_zaheler;
				// Durchlaufen fuer alle vom Artikel benötigten Positions IDs
				for (id_index_zaheler = aktuelle_id_index; id_index_zaheler <= letzte_belegte_id; id_index_zaheler++) {
					// Steht an der Stelle im Array ueberhaupt schon was?
					if (belegte_ids_porta_80[id_index_zaheler].id == NULL) {
						continue;
					}
					// Ueberpruefen, ob die Artikelnummer an dieser Stelle gleich der Artikelnummer des einzulagernden Artikels ist
					else if (belegte_ids_porta_80[id_index_zaheler].artikelnummer == artikel->typ->art_nummer) {
						// Ueberpruefen, ob das Fach als "voll" markiert ist, wenn nicht, dann ist diese ID nicht verfuegbar, es muss also eine andere ID gesucht werden
						if (belegte_ids_porta_80[id_index_zaheler].positions_id_voll == 1) {
							available = 0;
						}
						// Ueberpruefen, ob der einzulagernde Artikel in die Resthoehe des Fachs passt, wenn ja, dann durchlaufe alles auch fuer die naechsten benoetigten IDs
						else if (belegte_ids_porta_80[id_index_zaheler].resthoehe >= artikel->typ->hoehe) {
							continue;
						}
					}
					// ID nicht verfuegbar, suche andere ID
					else {
						available = 0;
						break;
					}
				}

				// Aktuelle ID (und nachfolgende) sind moeglich, Artikel kann an die Stelle eingelagert werden
				if (available == 1) {
					// Fuer jede vom Artikel belegte ID einen struct anlegen
					for (aktuelle_id_index; aktuelle_id_index < letzte_belegte_id; aktuelle_id_index++) {
						double resthoehe;
						// Hier wird die richtige ID wieder mit der Lagerkennung zusammengebaut
						aktuelle_id = aktuelle_id_index + 28000000;

						// Ueberprueft, ob die aktuelle ID schon als PositionsID erstellt ist
						// Wenn nicht (== NULL), dann erstelle eine neue PositionsID
						if (belegte_ids_porta_80[aktuelle_id_index].id == NULL) {
							struct PositionsID positions_id;
							positions_id.id = aktuelle_id;
							positions_id.artikelnummer = artikel->typ->art_nummer;
							positions_id.positions_id_voll = 0;

							// Berechnung der Resthoehe
							double resthoehe = 20 - artikel->typ->hoehe;
							positions_id.resthoehe = resthoehe;

							// Wenn Resthoehe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_80[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
							artikel->anzahl_positions_ids++;
							// Zuweisung der PositionsID an den Artikel
							artikel->positions[artikel->anzahl_positions_ids - 1] = positions_id;
						}
						else {
							struct PositionsID positions_id = belegte_ids_porta_80[aktuelle_id_index];

							// Wenn Resthoehe kleiner gleich 5 cm ist, markiere Positions-ID als voll
							double resthoehe = positions_id.resthoehe - artikel->typ->hoehe;
							if (resthoehe <= 5) {
								positions_id.positions_id_voll = 1;
							}
							positions_id.resthoehe = resthoehe;

							// PositionsID als belegt Kennzeichnen
							belegte_ids_porta_80[aktuelle_id_index] = positions_id;
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
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
	// Rueckgabe eines ungueltigen Wertes, falls keine passende Position gefunden wurde
	return -1;
}


// Funktion zur Ausgabe aller dem Artikel zugeordneten IDs
int print_zugeordnete_ids(struct Artikel *artikel) {
	int i;
	bs_loeschen();
	printf("Eingelagerter Artikel:\n");
	printf("Artikelname: %s\n", artikel->typ->name);
	printf("Artikelnummer: %d\n", artikel->typ->art_nummer);
	printf("Inventarnummer: %d\n", artikel->inventarnummer);
	printf("Anzahl der zugeordneten IDs: %d\n", artikel->anzahl_positions_ids);

	printf("Zugeordnete IDs:\n");
	for (i = 0; i < artikel->anzahl_positions_ids; i++) {
		printf("ID %d: %d\n", i + 1, artikel->positions[i].id);
	}
}

// Einen Artikel aus einem Lager entfernen
int artikel_aus_lager_entfernen(int inventarnummer) {
	int i, j, k, l;
	int artikel_gefunden = 0;
	int hoehe_des_artikels = 0;
	int belegte_positions_ids[60];
	struct Artikel* gefundener_artikel = 0;
	int erste_ziffer_1_gefunden = 0;
	int erste_ziffer_2_gefunden = 0;

	// Durchsuchen aller Artikel und ueberpruefen, ob Inventarnummer existiert
	for (i = 0; i < MAX_ARTIKEL; i++) {

		// Wenn Inventarnummer gefunden wurde, nehme den Artikel, dessen Lagerort und Hoehe
		if (gelagerte_artikel_liste[i].inventarnummer == inventarnummer) {
			gefundener_artikel = &gelagerte_artikel_liste[i];
			hoehe_des_artikels = gelagerte_artikel_liste[i].typ->hoehe;
			artikel_gefunden = 1;
			break;
		}
	}

	// Wenn Artikel gefunden, dann durchlaufe alle PositionsIDs und sammel alle belegten IDs
	if (artikel_gefunden == 1) {
		for (j = 0; j < 60; j++) {
			if (gefundener_artikel->positions[j].id != NULL) {
				belegte_positions_ids[j] = gefundener_artikel->positions[j].id;
			}
			else {
				continue;
			}
		}
		// Isoliere die erste Ziffer der Zahl
		int erste_zahl = erste_ziffer(belegte_positions_ids[0]);

		// Ueberpruefen, ob die erste Ziffer 1 ist
		if (erste_zahl == 1) {
			erste_ziffer_1_gefunden = 1;

		}
		// Wenn erste Ziffer 2 -> Porta Lager
		else if (erste_zahl == 2) {
			erste_ziffer_2_gefunden = 1;
		}
	}
	else {
		return -1;
	}

	// Lagerort ist Halle, weil eine ID mit erster Ziffer 1 gefunden wurde -> auf zweite Ziffern pruefen
	if (erste_ziffer_1_gefunden) {
		
		// Ueberpruefe die zweite Ziffer der Zahl
		int zweite_zahl = zweite_ziffer(belegte_positions_ids[0]);
		
		// Welche belegte_id_halle (20 oder 40) soll angeschaut werden (wenn zweite ziffer 2 -> 20, wenn 4 -> 40)
		if (zweite_zahl == 2) {

			// Durchsuchen von belegte_ids_halle_20[], um die entsprechende Position zu finden und deren Eigenschaften zu aendern
			for (k = 0; k < HALLE_20; k++) {
				// Durchlaufen belegte_positions_ids[]
				for (j = 0; j < 60; j++) {
					if (belegte_ids_halle_20[k].id == belegte_positions_ids[j]) {
						// Berechnen der neuen Resthoehe
						belegte_ids_halle_20[k].resthoehe += hoehe_des_artikels;

						// Ueberpruefen, wenn Resthoehe 20 betraegt dann die Artikelnummer loeschen
						if (belegte_ids_halle_20[k].resthoehe == 20) {
							belegte_ids_halle_20[k].id = 0; // Entfernen der ID aus belegte_ids_halle_20, indem ID von PositionsID auf 0 gesetzt wird
							belegte_ids_halle_20[k].artikelnummer = 0;	
						}
						belegte_ids_halle_20[k].positions_id_voll = 0;

						// Durchlaufen aller Artikel und zu loeschenden Artikel finden, um ihn aus gelagerte_artikel_liste zu entfernen
						for (l = 0; l < MAX_ARTIKEL; l++) {
							if (gelagerte_artikel_liste[l].inventarnummer == gefundener_artikel->inventarnummer) {
								gelagerte_artikel_liste[l].inventarnummer = 0;
								gelagerte_artikel_liste[l].anzahl_positions_ids = 0;
								break;
							}
						}
						break;
					}
				}
			}	
		} 
		// 40cm Positions ID
		else if (zweite_zahl == 4) {
			for (k = 0; k < HALLE_40; k++) {
				for (j = 0; j < 60; j++) {
					if (belegte_ids_halle_40[k].id == belegte_positions_ids[j]) {
						belegte_ids_halle_40[k].resthoehe += hoehe_des_artikels;
						// Ueberpruefen, ob Resthoehe 40 betraegt und die Artikelnummer loeschen
						if (belegte_ids_halle_40[k].resthoehe == 40) {
							belegte_ids_halle_40[k].id = 0;
							belegte_ids_halle_40[k].artikelnummer = 0;
						}
						belegte_ids_halle_40[k].positions_id_voll = 0;

						for (l = 0; l < MAX_ARTIKEL; l++) {
							if (gelagerte_artikel_liste[l].inventarnummer == gefundener_artikel->inventarnummer) {
								gelagerte_artikel_liste[l].inventarnummer = 0;
								gelagerte_artikel_liste[l].anzahl_positions_ids = 0;
								break;
							}
						}
						
						break;
					}
				}
			}
		}	
		gelagerte_artikel_liste[i].typ->artikel_davon_im_lager--;
	}
	// Lagerort ist Porta WF
	if (erste_ziffer_2_gefunden) {
		// Isoliere die zweite Ziffer der Zahl
		int zweite_zahl = zweite_ziffer(belegte_positions_ids[0]);

		if (zweite_zahl == 2) {

			// Durchsuchen von belegte_ids_porta_20[], um die entsprechende Position zu finden und deren Eigenschaften zu aendern
			for (k = 0; k < PORTA_20; k++) {
				for (j = 0; j < 60; j++) {
					if (belegte_ids_porta_20[k].id == belegte_positions_ids[j]) {
						// Berechnen der neuen Resthoehe
						belegte_ids_porta_20[k].resthoehe += hoehe_des_artikels;

						// Ueberpruefen, wenn Resthoehe 20 betraegt dann die Artikelnummer loeschen
						if (belegte_ids_porta_20[k].resthoehe == 20) {
							belegte_ids_porta_20[k].id = 0; // Entfernen der ID aus belegte_ids_porta_20, indem ID von PositionsID auf 0 gesetzt wird
							belegte_ids_porta_20[k].artikelnummer = 0;
						}
						belegte_ids_porta_20[k].positions_id_voll = 0;

						// Durchlaufen aller Artikel und zu loeschenden Artikel finden, um ihn aus gelagerte_artikel_liste zu entfernen
						for (l = 0; l < MAX_ARTIKEL; l++) {
							if (gelagerte_artikel_liste[l].inventarnummer == gefundener_artikel->inventarnummer) {
								gelagerte_artikel_liste[l].inventarnummer = 0;
								gelagerte_artikel_liste[l].anzahl_positions_ids = 0;
								break;
							}
						}
						break;
					}
				}
			}
		}
		else if (zweite_zahl == 4) {
			for (k = 0; k < PORTA_40; k++) {
				for (j = 0; j < 60; j++) {
					if (belegte_ids_porta_40[k].id == belegte_positions_ids[j]) {
						// Berechnen der neuen Resthoehe
						belegte_ids_porta_40[k].resthoehe += hoehe_des_artikels;

						// Ueberpruefen, wenn Resthoehe 20 betraegt dann die Artikelnummer loeschen
						if (belegte_ids_porta_40[k].resthoehe == 40) {
							belegte_ids_porta_40[k].id = 0; // Entfernen der ID aus belegte_ids_porta_40, indem ID von PositionsID auf 0 gesetzt wird
							belegte_ids_porta_40[k].artikelnummer = 0;
						}
						belegte_ids_porta_40[k].positions_id_voll = 0;

						// Durchlaufen aller Artikel und zu loeschenden Artikel finden, um ihn aus gelagerte_artikel_liste zu entfernen
						for (l = 0; l < MAX_ARTIKEL; l++) {
							if (gelagerte_artikel_liste[l].inventarnummer == gefundener_artikel->inventarnummer) {
								gelagerte_artikel_liste[l].inventarnummer = 0;
								gelagerte_artikel_liste[l].anzahl_positions_ids = 0;
								break;
							}
						}
						break;
					}
				}
			}
		}
		else if (zweite_zahl == 8) {
			for (k = 0; k < PORTA_80; k++) {
				for (j = 0; j < 60; j++) {
					if (belegte_ids_porta_80[k].id == belegte_positions_ids[j]) {
						// Berechnen der neuen Resthoehe
						belegte_ids_porta_80[k].resthoehe += hoehe_des_artikels;

						// Ueberpruefen, wenn Resthoehe 20 betraegt dann die Artikelnummer loeschen
						if (belegte_ids_porta_80[k].resthoehe == 80) {
							belegte_ids_porta_80[k].id = 0; // Entfernen der ID aus belegte_ids_porta_80, indem ID von PositionsID auf 0 gesetzt wird
							belegte_ids_porta_80[k].artikelnummer = 0;
						}
						belegte_ids_porta_80[k].positions_id_voll = 0;

						// Durchlaufen aller Artikel und zu loeschenden Artikel finden, um ihn aus gelagerte_artikel_liste zu entfernen
						for (l = 0; l < MAX_ARTIKEL; l++) {
							if (gelagerte_artikel_liste[l].inventarnummer == gefundener_artikel->inventarnummer) {
								gelagerte_artikel_liste[l].inventarnummer = 0;
								gelagerte_artikel_liste[l].anzahl_positions_ids = 0;
								break;
							}
						}
						break;
					}
				}
			}
		}
		gelagerte_artikel_liste[i].typ->artikel_davon_im_lager--;
	}
}

int erste_ziffer(int zahl) {
	while (zahl >= 10) {
		zahl /= 10;
	}
	return zahl;
}

int zweite_ziffer(int zahl) {
	while (zahl >= 100) {
		zahl /= 10; // Teile Zahl durch 10, um letzte Ziffer zu entfernen
	}
	zahl %= 10; // Verbleibende Ziffer ist zweite Ziffer
	return zahl;
}

// Funktion zum Leeren der Konsolenausgabe
void bs_loeschen(void) {
	system("CLS");
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

	// Zeichenkette verschieben, wenn noetig
	if (start > 0) {
		memmove(str, str + start, end - start + 2);
	}
}