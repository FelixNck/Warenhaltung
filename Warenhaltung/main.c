#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
double berechne_belegung_von(int);
int neuen_artikel_typ_anlegen();
int artikel_typ_bearbeiten();
int details_waehlen_bearbeitung(struct ArtikelTyp* artikel);
int artikel_typ_loeschen();
int artikel_erfassen();
int artikel_einlagern_nach_nummer(int eingabeNummer, int lager, struct Artikel artikel);
int lagere_artikel_an_positions_ids_Halle(struct Artikel *artikel);
int lagere_artikel_an_positions_ids_Porta(struct Artikel *artikel);
int print_zugeordnete_ids(struct Artikel *artikel);
int artikel_aus_lager_entfernen();
int erste_ziffer(int zahl);
int zweite_ziffer(int zahl);
int vorhandene_artikel_typen_ansehen();
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
	struct ArtikelTyp artikel_liste[MAX_ARTIKEL_TYP]; // Artikel, die in diesem Lager gelagert sind
	int anzahl_artikel_typen;
};

// Liste fuer bereits angelegte ArtikelTypen
struct ArtikelTyp artikel_liste[MAX_ARTIKEL_TYP];

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
			artikel_erfassen();
			break;
		case 5:
			artikel_aus_lager_entfernen();
			break;
		case 7:
			vorhandene_artikel_typen_ansehen();
			break;
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
	printf("\n%d Artikel\t\t%d Artikel", halle_lager.anzahl_artikel_typen, porta_lager.anzahl_artikel_typen);
	printf("\nBelegung:\t\tBelegung:");
	printf("\n20cm: %.2lf %%\t\t20cm: %.2lf %%", berechne_belegung_von(0), berechne_belegung_von(1));
	printf("\n40cm: %.2lf %%\t\t40cm: %.2lf %%", berechne_belegung_von(2), berechne_belegung_von(3));
	printf("\n80cm: %.2lf %%", berechne_belegung_von(4));
	printf("\n");
	printf("\nWarenausgang");
	printf("\nHalle: 5 Artikel      Porta Westfalica: 6 Artikel");
	printf("\n");
	printf("\n(1)   Neuen Artikel anlegen");
	printf("\n(2)   Artikel bearbeiten");
	printf("\n(3)   Artikel Typ loeschen");
	printf("\n(4)   Artikel erfassen");
	printf("\n(5)   Artikel entfernen"); // Vielleicht muss das auch nicht im Menue angeboten werden 
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

// Anlegen eines neuen ArtikelTyps
int neuen_artikel_typ_anlegen() {
	struct ArtikelTyp artikeltyp;
	artikeltyp.artikel_davon_im_lager = 0;
	int lagerwahl = -1;
	int wahl = 0;
	int i;

	bs_loeschen();
	printf("Neuen Artikel Typ anlegen");

	printf("\n");

	// Artikelname eingeben; Ueberpruefen, ob Name schon existiert
	printf("\nArtikelname (max. 100 Zeichen): ");
	// Leert Eingabepuffer nach vorherigen Eingabe
	while (getchar() != '\n');  // Verhindert, dass leere Zeile eingelesen wird
	do {
		fgets(artikeltyp.name, sizeof(artikeltyp.name), stdin); // Kann Leerzeichen/ganze Zeile einlesen
		// Entfernt Zeilenumbruchzeichen am Ende der Eingabe
		artikeltyp.name[strcspn(artikeltyp.name, "\n")] = '\0';

		strtrim(artikeltyp.name); // Loescht Leerzeichen hinter/vor Name
		// Ueberpruefen, ob Artikelname bereits existiert
		int name_existiert = 0;
		for (i = 0; i < anzahl_artikel_typen; i++) {
			if (strcmp(artikeltyp.name, artikel_liste[i].name) == 0) {
				printf("\nFehler: Ein Artikel mit diesem Namen existiert bereits.\nBitte geben Sie einen neuen Namen ein:\n");
				name_existiert = 1;
				break;
			}
		}
		if (!name_existiert) // Wenn Name eindeutig ist, bricht Schleife ab
			break;
	} while (1);

	// Eingabe Artikelnummer; Ueberpruefen, ob schon existent und ob Ganzzahl
	while (1) {
		printf("\nArtikel Nummer (Ganzzahl): ");
		if (scanf("%d", &artikeltyp.art_nummer) != 1 || getchar() != '\n') {
			printf("Fehler: Bitte geben Sie eine gueltige Ganzzahl fuer die Artikelnummer ein.\n");
			while (getchar() != '\n'); 
			continue; 
		}

		// Ueberpruefen, ob Artikelnummer bereits existiert
		int nummer_existiert = 0;
		for (i = 0; i < anzahl_artikel_typen; i++) {
			if (artikeltyp.art_nummer == artikel_liste[i].art_nummer) {
				printf("\nFehler: Artikel mit dieser Nummer existiert bereits. Bitte geben Sie eine andere Nummer ein.\n");
				nummer_existiert = 1;
				break; // Bricht Schleife ab, da Nummer schon existent
			}
		}

		if (!nummer_existiert) // Wenn eingegebene Nummer eindeutig ist
			break;
	}

	printf("\nPreis (in EUR): ");
	scanf("%lf", &artikeltyp.preis);

	// Artikel Hoehe muss > 5 und <= 80 cm sein
	do {
		printf("\nHoehe (in cm; > 5, wenn verderblich <= 40, sonst <= 80): ");
		scanf("%lf", &artikeltyp.hoehe);
		if (artikeltyp.hoehe <= 5 || artikeltyp.hoehe > 80)
			printf("Fehler: Die Hoehe muss groesser als 5 cm und kleiner oder gleich 80 cm sein. Bitte geben Sie einen gueltigen Wert ein!\n");
	} while (artikeltyp.hoehe <= 5 || artikeltyp.hoehe > 80);

	// Artikel Breite darf nicht groesser als 300cm sein
	do {
		printf("\nBreite (in cm; > 5, <= 300): ");
		scanf("%lf", &artikeltyp.breite);
		if (artikeltyp.breite <= 5 || artikeltyp.breite > 300)
			printf("Fehler: Die Breite muss groesser als 5 cm und kleiner oder gleich 300 cm sein. Geben Sie bitte einen gueltigen Wert ein!\n");
	} while (artikeltyp.breite <= 5 || artikeltyp.breite > 300);

	// Artikel Tiefe darf nicht groesser als 120cm sein
	do {
		printf("\nTiefe (in cm, <= 120): ");
		scanf("%lf", &artikeltyp.tiefe);
		if (artikeltyp.tiefe > 120)
			printf("Fehler: Die Tiefe darf nicht groesser als 120 cm sein. Geben Sie bitte einen gueltigen Wert ein!\n");
	} while (artikeltyp.tiefe > 120);

	// Abfrage, ob Artikel verderblich oder nicht (Entscheidung in welches Lager der Artikel geht)
	do {
		printf("\nWeisen Sie dem Artikel nun noch zu, ob er verderblich ist oder nicht.\nVerderblich: (1)\nNicht verderblich: (2)\n");
		scanf("%d", &lagerwahl);
		if (artikeltyp.hoehe > 40 && lagerwahl == HALLE) {
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
		halle_lager.artikel_liste[halle_lager.anzahl_artikel_typen] = artikeltyp;
		halle_lager.anzahl_artikel_typen++;
		artikeltyp.lager = 1;
		printf("\nDer Artikel wurde erfolgreich in Halle an der Saale angelegt.\nDruecken Sie Enter, um fortzufahren.\n");
		break;
	case PORTA_WESTFALICA:
		// Artikel ist nicht verderblich, Lagerort entsprechend festlegen und verarbeiten
		porta_lager.artikel_liste[porta_lager.anzahl_artikel_typen] = artikeltyp;
		porta_lager.anzahl_artikel_typen++;
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
	printf("Artikel wurde erfolgreich angelegt. Druecke Enter, um zum Menue zu kommen!");
	printf("\n");
	printf("\nArtikel Name: %s", artikeltyp.name);
	printf("\nArtikel Nummer: %d", artikeltyp.art_nummer);
	printf("\nArtikel Preis: %.2lf", artikeltyp.preis);
	printf("\nArtikel Hoehe (in cm): %.2lf", artikeltyp.hoehe);
	printf("\nArtikel Breite (in cm): %.2lf", artikeltyp.breite);
	printf("\nArtikel Tiefe (in cm): %.2lf", artikeltyp.tiefe);

	// Hinzufuegen des neuen Artikels zur Liste
	artikel_liste[anzahl_artikel_typen] = artikeltyp;
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
			if (artikel_liste[i].art_nummer == suchnummer) {
				printf("\nArtikel gefunden:\n");
				printf("\nArtikel Name: %s", artikel_liste[i].name);
				printf("\nArtikel Nummer: %d", artikel_liste[i].art_nummer);
				printf("\nArtikel Preis: %.2lf", artikel_liste[i].preis);
				printf("\nArtikel Hoehe (in cm): %.2lf", artikel_liste[i].hoehe);
				printf("\nArtikel Breite (in cm): %.2lf", artikel_liste[i].breite);
				printf("\nArtikel Tiefe (in cm): %.2lf", artikel_liste[i].tiefe);
				printf("\n");

				// Code für Aenderung Details des Artikels
				details_waehlen_bearbeitung(&artikel_liste[i]);
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
			if (strcmp(artikel_liste[i].name, suchname) == 0) {
				printf("\nArtikel gefunden:\n");
				printf("\nArtikel Name: %s", artikel_liste[i].name);
				printf("\nArtikel Nummer: %d", artikel_liste[i].art_nummer);
				printf("\nArtikel Preis: %.2lf", artikel_liste[i].preis);
				printf("\nArtikel Hoehe (in cm): %.2lf", artikel_liste[i].hoehe);
				printf("\nArtikel Breite (in cm): %.2lf", artikel_liste[i].breite);
				printf("\nArtikel Tiefe (in cm): %.2lf", artikel_liste[i].tiefe);
				printf("\n");

				// Code für die Aenderung Details Artikel
				details_waehlen_bearbeitung(&artikel_liste[i]);
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
			getchar(); // Leertaste nach vorheriger Eingabe abfangen
			fgets(temp_artikel.name, sizeof(temp_artikel.name), stdin); // Einlesen des Namens mit Leerzeichen
			temp_artikel.name[strcspn(temp_artikel.name, "\n")] = 0; // Entfernen des Zeilenumbruchs
			strtrim(temp_artikel.name);
			// Ueberpruefen, ob der Name bereits existiert
			for (i = 0; i < anzahl_artikel_typen; i++) {
				if (strcmp(temp_artikel.name, artikel_liste[i].name) == 0 && temp_artikel.art_nummer != artikel_liste[i].art_nummer) {
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
	int artikel_nummer;
	int i, j, k, l;

	bs_loeschen();

	printf("Geben Sie die Artikelnummer des zu loeschenden Artikels ein: ");
	scanf("%d", &artikel_nummer);

	// Sucht nach dem Artikel in den Lagerlisten und loescht ihn, wenn j
	for (i = 0; i < halle_lager.anzahl_artikel_typen; i++) {
		if (halle_lager.artikel_liste[i].art_nummer == artikel_nummer) {
			// Artikel gefunden, zeigt die Details an
			printf("\nArtikel Details:\n");
			printf("Name: %s\n", halle_lager.artikel_liste[i].name);
			printf("Artikelnummer: %d\n", halle_lager.artikel_liste[i].art_nummer);
			printf("Preis: %.2f EUR\n", halle_lager.artikel_liste[i].preis);
			printf("Hoehe: %.2f cm\n", halle_lager.artikel_liste[i].hoehe);
			printf("Breite: %.2f cm\n", halle_lager.artikel_liste[i].breite);
			printf("Tiefe: %.2f cm\n", halle_lager.artikel_liste[i].tiefe);

			// Bestaetigung vom Benutzer einholen
			int antwort;
			printf("Moechten Sie diesen Artikel wirklich loeschen? (1 = Ja, 0 = Nein): ");
			scanf("%d", &antwort);

			if (antwort == 1) {
				// Artikel loeschen aus Lager in Halle
				for (j = i; j < halle_lager.anzahl_artikel_typen - 1; j++) {
					halle_lager.artikel_liste[j] = halle_lager.artikel_liste[j + 1];
				}
				halle_lager.anzahl_artikel_typen--;

				// Artikel aus Artikelliste loeschen
				for (k = 0; k < anzahl_artikel_typen; k++) {
					if (artikel_liste[k].art_nummer == artikel_nummer) {
						for (l = k; l < anzahl_artikel_typen - 1; l++) {
							artikel_liste[l] = artikel_liste[l + 1];
						}
						anzahl_artikel_typen--;
						break;
					}
				}
				/* Loeschen aus Lager in Halle 20
				for (j = 0; j < HALLE_20; j++) {
					if (belegte_ids_halle_20[j].artikelnummer == artikel_nummer) {
						// Gefundene Positions-ID loeschen
						belegte_ids_halle_20[j].artikelnummer = 0;
						belegte_ids_halle_20[j].positions_id_voll = 0;
						belegte_ids_halle_20[j].resthoehe = 20;
					}
				}

				// Loeschen aus Lager in Halle 40
				for (j = 0; j < HALLE_40; j++) {
					if (belegte_ids_halle_40[j].artikelnummer == artikel_nummer) {
						// Gefundene Positions-ID loeschen
						belegte_ids_halle_40[j].artikelnummer = 0;
						belegte_ids_halle_40[j].positions_id_voll = 0;
						belegte_ids_halle_40[j].resthoehe = 40;
					}
				}*/

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

	for (i = 0; i < porta_lager.anzahl_artikel_typen; i++) {
		if (porta_lager.artikel_liste[i].art_nummer == artikel_nummer) {
			// Artikel gefunden, zeigt die Details an
			printf("\nArtikel Details:\n");
			printf("Name: %s\n", porta_lager.artikel_liste[i].name);
			printf("Artikelnummer: %d\n", porta_lager.artikel_liste[i].art_nummer);
			printf("Preis: %.2f EUR\n", porta_lager.artikel_liste[i].preis);
			printf("Hoehe: %.2f cm\n", porta_lager.artikel_liste[i].hoehe);
			printf("Breite: %.2f cm\n", porta_lager.artikel_liste[i].breite);
			printf("Tiefe: %.2f cm\n", porta_lager.artikel_liste[i].tiefe);

			// Bestaetigung vom Benutzer einholen
			int antwort;
			printf("Moechten Sie diesen Artikel wirklich loeschen? (1 = Ja, 0 = Nein): ");
			scanf("%d", &antwort);

			if (antwort == 1) {
				// Artikel loeschen aus Lager PW
				for (int j = i; j < porta_lager.anzahl_artikel_typen - 1; j++) {
					porta_lager.artikel_liste[j] = porta_lager.artikel_liste[j + 1];
				}
				porta_lager.anzahl_artikel_typen--;

				// Artikel aus Artikelliste (wo alle Artikel drin stehen) loeschen
				for (int k = 0; k < anzahl_artikel_typen; k++) {
					if (artikel_liste[k].art_nummer == artikel_nummer) {
						for (int l = k; l < anzahl_artikel_typen - 1; l++) {
							artikel_liste[l] = artikel_liste[l + 1];
						}
						anzahl_artikel_typen--;
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
		for (i = 0; i < porta_lager.anzahl_artikel_typen; i++) {
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

// Aktualisierung der Aenderungen am Artikel im Lager
int lager_aktualisieren(struct ArtikelTyp *artikel) {
	int i;
	for (i = 0; i < anzahl_artikel_typen; i++) {
		if (halle_lager.artikel_liste[i].art_nummer == artikel->art_nummer) {
			halle_lager.artikel_liste[i] = *artikel;
			break;
		}
	}

	for (i = 0; i < anzahl_artikel_typen; i++) {
		if (porta_lager.artikel_liste[i].art_nummer == artikel->art_nummer) {
			porta_lager.artikel_liste[i] = *artikel;
			break;
		}
	}
}

// Funktion zum Suchen, Anzeigen und einlagern des Artikels
int artikel_erfassen() {
	int gefunden = 0;
	int eingabeNummer;
	char eingabeName[100];
	char wahl;
	int i;


	do {
		gefunden = 0; // Zuruecksetzen des gefunden-Flags

		bs_loeschen();

		printf("Geben Sie die Artikelnummer ein: ");
		if (scanf("%d", &eingabeNummer) == 1) {
			for (i = 0; i < anzahl_artikel_typen; i++) {
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
					printf("\nMoechten Sie zum Menue zurueckkehren (0), nach einem anderen Artikel suchen (1) oder den Artikel einlagern (2)?:\n");
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

		if (!gefunden) {
			printf("\nArtikel nicht gefunden.\n");
		}
		printf("Moechten Sie zum Menue zurueckkehren (0) oder einen neuen Artikel suchen (1) ?: ");
		scanf(" %c", &wahl);
	} while (wahl != '0');

	return 0;
}

int generiere_inventarnummer() {
	// Inkrementiere Zaehler und gib neue Inventarnummer zurück
	return ++inventarnummerZaehler;
}

// Artikel anhand der Artikelnummer einlagern
int artikel_einlagern_nach_nummer(int eingabeNummer, int lager, struct Artikel artikel) {
	int i, j;

	struct Artikel *artikel_pntr;
	artikel_pntr = &artikel;

	// Ueberpruefen des Lagers und Berechnungen, vielleicht als separate Funktionen nochmal
	if (lager == 1) {
		// Aktionen für Lager 1 (HALLE)
		if (lagere_artikel_an_positions_ids_Halle(artikel_pntr) != -1) {
			print_zugeordnete_ids(artikel_pntr); // Ausgabe der zugeordneten IDs
			artikel_pntr->typ->artikel_davon_im_lager++;
			for (j = 0; j < MAX_ARTIKEL; j++) {
				if (gelagerte_artikel_liste[j].inventarnummer == 0) {
					gelagerte_artikel_liste[j] = *artikel_pntr;
				}
			}
			printf("\nArtikel mit der Nummer %d wurde erfolgreich im Halle Lager eingelagert.\n", eingabeNummer);
			printf("\nArtikel von Typ %s im Lager: %d\n", artikel_pntr->typ->name, artikel_pntr->typ->artikel_davon_im_lager);
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
			artikel_pntr->typ->artikel_davon_im_lager++;
			for (j = 0; j < MAX_ARTIKEL; j++) {
				if (gelagerte_artikel_liste[j].inventarnummer == 0) {
					gelagerte_artikel_liste[j] = *artikel_pntr;
				}
			}
			printf("Artikel mit der Nummer %d wurde erfolgreich im Porta Lager eingelagert.\n", eingabeNummer);
			printf("\nArtikel von Typ %s im Lager: %d\n", artikel_pntr->typ->name, artikel_pntr->typ->artikel_davon_im_lager);
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
	else if (((hoehe > 20) && (hoehe <= 40)) || lager_halle_20_cm_voll == 1) {
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
							// Anzahl der PositionsIDs, die fuer diesen Artikel verwendet wurden
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
	else if (((hoehe > 20) && (hoehe <= 40)) || lager_porta_20_cm_voll == 1) {
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
	else if (((hoehe > 40) && (hoehe <= 80)) || lager_porta_40_cm_voll == 1) {
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
	printf("Eingelagerter Artikel:\n");	// noch Name hinzufuegen?
	printf("Artikelnummer: %d\n", artikel->typ->art_nummer);
	printf("Inventarnummer: %d\n", artikel->inventarnummer);
	printf("Anzahl der zugeordneten IDs: %d\n", artikel->anzahl_positions_ids);

	printf("Zugeordnete IDs:\n");
	for (i = 0; i < artikel->anzahl_positions_ids; i++) {
		printf("ID %d: %d\n", i + 1, artikel->positions[i].id);
	}
}


int artikel_aus_lager_entfernen() {
	int inventarnummer;
	int i, j, k;
	int artikel_gefunden = 0;
	int lagerort_des_artikel = 0;
	int hoehe_des_artikels = 0;
	int belegte_positions_ids[60];
	struct Artikel* gefundener_artikel = 0;
	int erste_ziffer_1_gefunden = 0; // Flagge fuer Finden ID mit erster Ziffer 1
	int erste_ziffer_2_gefunden = 0;

	bs_loeschen();

	printf("Geben Sie die Inventarnummer des zu suchenden Artikels ein: ");
	scanf("%d", &inventarnummer);


	printf("Gelagerter Artikel!!!: %d \n", gelagerte_artikel_liste[0].inventarnummer); // !!! Hier wird immer 1001 ausgegeben; muss an jeweilige Stelle im Array; dadurch werden alle anderen Artikel nicht gefunden
	while (getchar() != '\n');
	getchar();

	// Durchsuchen aller Artikel und ueberpruefen, ob Inventarnummer existiert
	for (i = 0; i < MAX_ARTIKEL; i++) {
		// Wenn Inventarnummer gefunden wurde, nehme den Artikel, dessen Lagerort und Hoehe
		if (gelagerte_artikel_liste[i].inventarnummer == inventarnummer) {
			gefundener_artikel = &gelagerte_artikel_liste[i];
			lagerort_des_artikel = gelagerte_artikel_liste[i].typ->lager;
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

				// Isoliere die erste Ziffer der Zahl
				int erste_zahl = erste_ziffer(belegte_positions_ids[0]);	//Test, erste Id ueberpruefen reicht

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
				continue;
			}
		}
	}
	else {
		printf("Artikel nicht gefunden. ");
	}


	if (erste_ziffer_1_gefunden) {
		// Aktionen ausfuehren, wenn eine ID mit erster Ziffer 1 gefunden wurde (Halle) -> auf zweite Ziffern pruefen
		printf("Eine ID mit erster Ziffer 1 wurde gefunden.\n");
		// Isoliere die zweite Ziffer der Zahl
		int zweite_zahl = zweite_ziffer(belegte_positions_ids[0]);
		printf("Die zweite Ziffer ist: %d\n", zweite_zahl); //Test; ab hier dann Ueberpruefung, welche belegte_id_halle (20 oder 40) angeschaut werden soll (wenn zweite ziffer 2 -> 20, wenn 4 -> 40)

		if (zweite_zahl == 2) {
			// Benutzerbestaetigung für das Entfernen des Artikels		!!!kann man vielleicht statt fuenf Mal einmal irgendwo einbinden??
			int antwort;
			printf("Moechten Sie diesen Artikel wirklich entfernen? (1 = Ja, 0 = Nein): ");
			scanf("%d", &antwort);

			if (antwort == 0) {
				printf("Entfernungsprozess abgebrochen.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return; // Zurueck zum Menue
			}
			else if (antwort != 1) {
				printf("Ungueltige Eingabe.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return; // Zurueck zum Menue
			}

			// Durchsuchen von belegte_ids_halle_20[], um die entsprechende Position zu finden und deren Eigenschaften zu aendern
			for (k = 0; k < HALLE_20; k++) {
				for (j = 0; j < 60; j++) {
					if (belegte_ids_halle_20[k].id == belegte_positions_ids[j]) {
						printf("ID %d gefunden in Position %d\n", belegte_ids_halle_20[k].id, k);
						// Entfernen der ID, indem sie auf 0 gesetzt wird
						belegte_ids_halle_20[k].id = 0;
						printf("ID %d gefunden in Position %d\n", belegte_ids_halle_20[k].id, k);
						while (getchar() != '\n');
						getchar();

						// Berechnen der neuen Resthoehe
						belegte_ids_halle_20[k].resthoehe += hoehe_des_artikels;
						printf("Neue Resthoehe fuer Position %d: %d\n", k, belegte_ids_halle_20[k].resthoehe);

						// Ueberpruefen, ob Resthoehe 20 betraegt und die Artikelnummer loeschen
						if (belegte_ids_halle_20[k].resthoehe == 20) {
							belegte_ids_halle_20[k].artikelnummer = 0;	
							printf("Artikelnummer fuer %d: %d", k, belegte_ids_halle_20[k].artikelnummer);
						}

						belegte_ids_halle_20[k].positions_id_voll = 0;
						printf("\nPositions Ids an Stelle %d voll = %d", k, belegte_ids_halle_20[k].positions_id_voll);

						gelagerte_artikel_liste[k].inventarnummer = 0; //geht das so?
						printf("\nInventarnummer %d", gelagerte_artikel_liste[k].inventarnummer);
						// Setzt Anzahl der Positions-IDs auf 0
						gelagerte_artikel_liste[k].anzahl_positions_ids = 0;
						printf("\nArtikel mit der Inventarnummer %d erfolgreich entfernt.\n", inventarnummer);

						printf("Anzahl Artikel im Lager %d", gelagerte_artikel_liste[k].typ->artikel_davon_im_lager);
						
						

						while (getchar() != '\n');
						getchar();
						break;
					
					}
				}
			}	
		} 
		else if (zweite_zahl == 4) {
			int antwort;
			printf("Moechten Sie diesen Artikel wirklich entfernen? (1 = Ja, 0 = Nein): ");
			scanf("%d", &antwort);

			if (antwort == 0) {
				printf("Entfernungsprozess abgebrochen.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return; // Zurueck zum Menue
			}
			else if (antwort != 1) {
				printf("Ungueltige Eingabe.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return; // Zurueck zum Menue
			}

			for (k = 0; k < HALLE_40; k++) {
				for (j = 0; j < 60; j++) {
					if (belegte_ids_halle_40[k].id == belegte_positions_ids[j]) {
						belegte_ids_halle_40[k].id = 0;
						belegte_ids_halle_40[k].resthoehe += hoehe_des_artikels;
						// Ueberpruefen, ob Resthoehe 40 betraegt und die Artikelnummer loeschen
						if (belegte_ids_halle_40[k].resthoehe == 40) {
							belegte_ids_halle_40[k].artikelnummer = 0;
						}
						belegte_ids_halle_40[k].positions_id_voll = 0;
						gelagerte_artikel_liste[k].inventarnummer = 0;
						gelagerte_artikel_liste[k].anzahl_positions_ids = 0;
						
						break;
					}
				}
			}
		}	gelagerte_artikel_liste[i].typ->artikel_davon_im_lager--;
			printf("Anzahl Artikel im Lager %d", gelagerte_artikel_liste[k].typ->artikel_davon_im_lager);
			printf("\nArtikel erfolgreich aus Lager entfernt.\nDruecken Sie Enter, um zum Menue zurueckzukehren!"); // Test
			while (getchar() != '\n');
			getchar();
	}

	if (erste_ziffer_2_gefunden) {
		// Aktionen ausfuehren, wenn ID mit erster Ziffer 2 gefunden wurde (Porta) -> jetzt auf zweite Ziffern pruefen
		printf("Eine ID mit erster Ziffer 2 wurde gefunden.\n");
		// Isoliere die zweite Ziffer der Zahl
		int zweite_zahl = zweite_ziffer(belegte_positions_ids[0]);
		printf("Die zweite Ziffer ist: %d\n", zweite_zahl); //Test; ab hier dann Ueberpruefung, welche belegte_id_porta (20, 40 oder 80) angeschaut werden soll

		if (zweite_zahl == 2) {
			// Benutzerbestaetigung für das Entfernen des Artikels
			int antwort;
			printf("Moechten Sie diesen Artikel wirklich entfernen? (1 = Ja, 0 = Nein): ");
			scanf("%d", &antwort);

			if (antwort == 0) {
				printf("Entfernungsprozess abgebrochen.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return; // Zurueck zum Menue
			}
			else if (antwort != 1) {
				printf("Ungueltige Eingabe.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return; // Zurueck zum Menue
			}

			// Durchsuchen von belegte_ids_porta_20[], um die entsprechende Position zu finden und deren Eigenschaften zu aendern
			for (k = 0; k < PORTA_20; k++) {
				for (j = 0; j < 60; j++) {
					if (belegte_ids_porta_20[k].id == belegte_positions_ids[j]) {
						belegte_ids_porta_20[k].id = 0;
						belegte_ids_porta_20[k].resthoehe += hoehe_des_artikels;
						// Ueberpruefen, ob Resthoehe 20 betraegt und die Artikelnummer loeschen
						if (belegte_ids_porta_20[k].resthoehe == 20) {
							belegte_ids_porta_20[k].artikelnummer = 0;
						}
						belegte_ids_porta_20[k].positions_id_voll = 0;
						gelagerte_artikel_liste[k].inventarnummer = 0;
						gelagerte_artikel_liste[k].anzahl_positions_ids = 0;

						break;
					}
				}
			}
		}
		else if (zweite_zahl == 4) {
			// Benutzerbestaetigung für das Entfernen des Artikels
			int antwort;
			printf("Moechten Sie diesen Artikel wirklich entfernen? (1 = Ja, 0 = Nein): ");
			scanf("%d", &antwort);

			if (antwort == 0) {
				printf("Entfernungsprozess abgebrochen.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return; // Zurueck zum Menue
			}
			else if (antwort != 1) {
				printf("Ungueltige Eingabe.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return; // Zurueck zum Menue
			}

			for (k = 0; k < PORTA_40; k++) {
				for (j = 0; j < 60; j++) {
					if (belegte_ids_porta_40[k].id == belegte_positions_ids[j]) {
						belegte_ids_porta_40[k].id = 0;
						belegte_ids_porta_40[k].resthoehe += hoehe_des_artikels;
						// Ueberpruefen, ob Resthoehe 40 betraegt und die Artikelnummer loeschen
						if (belegte_ids_porta_40[k].resthoehe == 40) {
							belegte_ids_porta_40[k].artikelnummer = 0;
						}
						belegte_ids_porta_40[k].positions_id_voll = 0;
						gelagerte_artikel_liste[k].inventarnummer = 0;
						gelagerte_artikel_liste[k].anzahl_positions_ids = 0;

						break;
					}
				}
			}
		}
		else if (zweite_zahl == 8) {
			// Benutzerbestaetigung für das Entfernen des Artikels
			int antwort;
			printf("Moechten Sie diesen Artikel wirklich entfernen? (1 = Ja, 0 = Nein): ");
			scanf("%d", &antwort);

			if (antwort == 0) {
				printf("Entfernungsprozess abgebrochen.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return; // Zurueck zum Menue
			}
			else if (antwort != 1) {
				printf("Ungueltige Eingabe.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
				while (getchar() != '\n');
				getchar();
				return; // Zurueck zum Menue
			}

			for (k = 0; k < PORTA_80; k++) {
				for (j = 0; j < 60; j++) {
					if (belegte_ids_porta_80[k].id == belegte_positions_ids[j]) {
						belegte_ids_porta_80[k].id = 0;
						belegte_ids_porta_80[k].resthoehe += hoehe_des_artikels;
						// Ueberpruefen, ob Resthoehe 80 betraegt und die Artikelnummer loeschen
						if (belegte_ids_porta_80[k].resthoehe == 40) {
							belegte_ids_porta_80[k].artikelnummer = 0;
						}
						belegte_ids_porta_80[k].positions_id_voll = 0;
						gelagerte_artikel_liste[k].inventarnummer = 0;
						gelagerte_artikel_liste[k].anzahl_positions_ids = 0;

						break;
					}
				}
			}
		}	gelagerte_artikel_liste[i].typ->artikel_davon_im_lager--;
			printf("Anzahl Artikel im Lager %d", gelagerte_artikel_liste[k].typ->artikel_davon_im_lager);
			printf("\nArtikel erfolgreich aus Lager entfernt.\nDruecken Sie Enter, um zum Menue zurueckzukehren!"); // Test
			while (getchar() != '\n');
			getchar();
	}


		// Aktueller Stand: wir haben jetzt alle vom Artikel belegten IDs (Nummern davon) in belegte_positions_ids
		// Weiteres Vorgehen:
		// - checken in welchem Lager der Artikel liegt (lagerort_des_artikel) und machen daran fest, welche belegte_ids_[LAGERORT] wir durchsuchen müssen
		//		- z.B. für einen 20cm Artikel in Halle -> lagerort_des_artikel = 1
		//											   -> belegte_positions_ids[] = [12000000, 12000001]
		//											   -> anhand der PositionsID.id können wir herausfinden, in welchem Lager er liegt (12000000 oder 14000000)
		//			- belegte_ids_halle_20[], ob IDs da drin liegen 
		//			- für jede Positions ID eine neue Resthöhe berechnen und dran schreiben, positions_id_voll flag auf 0 setzen und wenn PositionsID leer (also Resthöhe ist 20, 40 oder 80) ist, dann muss auch die Artikelnummer gelöscht werden (auf 0 setzen?)
		//			  und aus belegte_ids_[LAGER] Liste entfernen
		// - Artikel aus gelagerte_artikel_liste[] raus nehmen
		// - vom Artikel Inventarnummer auf 0 setzen und positions[] leeren (und eventuell noch anzahl_position_ids auf 0 setzen)
		// - An Artikeltyp artikel_davon_im_lager - 1 setzen

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