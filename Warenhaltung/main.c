#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HALLE_20 18000 //vorhandene Anzahl Positions IDs für 20cm hohe Fächer
#define HALLE_40 54000
#define PORTA_20 34800
#define PORTA_40 6000
#define PORTA_80 7800

// Maximale Anzahl von Artikeln
#define MAX_ARTIKEL 200

int menue();
int neuen_artikel_anlegen();
void bs_loeschen();



/* Beispiel

// id_20 => 2018000
// id_40 => 4054000
// id_80 => 807800

// resthöhe
    // Minimale Höhe für einen Artikel festlegen?
       // (minimal 5cm und Maximal 80cm)
       // (Wenn ein Fach eine Resthöhe von z.B. 5cm hat, dann wird es als "voll" angesehen)

// Beispiel
// Artikel (20x30x120)
// (Höhe, Breite, Tiefe)

struct PositionsID {
    int id;
    int resthöhe;
    int positions_id_voll;
};


int belegte_id_halle_20[18000] = [2000001, 2000002, 2000003, ..., 2000245];

if (hohe <= 20) {
    int id = 2000001;
    max_anzahl = HALLE_20 + 2000000;
    anzahl_belegter_ids = breite / 10;
        for (int id; id <= max_anzahl; id++) {
            if id in belegte_id_halle_20{
                break;
            }
            else
                if artikel_name == aktueller_artikel_name
                    if PositionsID[id].resthöhe >= artikel_höhe
                        // einlagern
                    else
                        // nächste ID
                else
                    // nächste ID
            else // ID noch nicht belegt
                //artikel einlagern und resthöhe berechnen
    }
}


*/

struct ArtikelTyp {
    char name[100];
    int art_nummer;
    double preis;
    double hoehe;
    double breite;
    double tiefe;
};

// Liste für bereits angelegte Artikel
struct ArtikelTyp artikel_liste[MAX_ARTIKEL];

// Anzahl der bereits vorhandenen Artikel
int anzahl_artikel = 0;

// Funktion zur Überprüfung der Artikel
int artikel_existiert_bereits(const char* name, int art_nummer) {
    int i;
    for (i = 0; i < anzahl_artikel; i++) {
        if (strcmp(artikel_liste[i].name, name) == 0 && artikel_liste[i].art_nummer == art_nummer) {
            return 1; // Artikel bereits vorhanden
        }
    }
    return 0; // Artikel nicht vorhanden
}

//Enumeration, gibt VERDERBLICH Wert 1 und NICHT_VERDERBLICH Wert 2
enum LagerTyp {
    VERDERBLICH = 1,
    NICHT_VERDERBLICH = 2
};

/*
struct Artikel {
    int artikel_typ_nummer = 12345
    int inventarnummer = 9785
    int positions_id = [3, 4]
};

*/

// Hier geht der richtige Code los

int main(void) {
    int auswahl;

    while (1) {
        menue();

        scanf("%d", &auswahl);

        switch (auswahl) {
        case 1:
            neuen_artikel_anlegen();
            break;
        default:
            printf("\nFalsche eingabe. Waehle eine der oben aufgelisteten Moeglichkeiten!\n");
        } // Ende des switch
    } // Ende der while-Schleife

    return 0;
}

int menue() { //hier wird das Menue ausgegeben
    bs_loeschen();
    printf("Halle                 Porta Westfalica");
    printf("\nArtikel:              Artikel:");
    printf("\nBelegung:             Belegung:");
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
    printf("\n(7)   Artikel ansehen");
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
    int janein = 0;
    int lagerwahl = 0;
    int wahl = 0;

    bs_loeschen();
    printf("Neuen Artikel anlegen?");
    printf("\n(1) Ja");
    printf("\n(0) Nein");
    printf("\n");

    scanf("%d", &janein);

    if (janein == 0) {
        return 0;
    }

    bs_loeschen();
    printf("Neuen Artikel anlegen");

    printf("\n");
    printf("\nArtikelname (max. 100 Zeichen):");

    // input für artikelname auslesen
    scanf("%s", artikeltyp.name);

    // Überprüfen, ob der Artikelname bereits existiert
    for (int i = 0; i < anzahl_artikel; i++) {
        if (strcmp(artikeltyp.name, artikel_liste[i].name) == 0) {
            printf("\nFehler: Ein Artikel mit diesem Namen existiert bereits.\n");
            return 0; // Rückkehr, da Fehler aufgetreten ist
        }
    }

    printf("\nArtikel Nummer:");
    // input für artikel nummer auslesen
    scanf("%d", &artikeltyp.art_nummer);

    // Überprüfen, ob die Artikelnummer bereits existiert
    for (int i = 0; i < anzahl_artikel; i++) {
        if (artikeltyp.art_nummer == artikel_liste[i].art_nummer) {
            printf("\nFehler: Artikel mit dieser Nummer existiert bereits.\n");
            return 0; // Rückkehr, da Fehler aufgetreten ist
        }
    }


    printf("\nPreis (in EUR):");
    // input für artikel preis auslesen
    scanf("%lf", &artikeltyp.preis);

    printf("\nHoehe (in cm):");
    // input für hoehe auslesen
    scanf("%lf", &artikeltyp.hoehe);

    printf("\nBreite (in cm):");
    // input für breite auslesen
    scanf("%lf", &artikeltyp.breite);

    printf("\nTiefe (in cm):");
    // input für tiefe auslesen
    scanf("%lf", &artikeltyp.tiefe);


    printf("\nWeisen Sie dem Artikel nun noch zu, ob er verderblich ist oder nicht.\nVerderblich: (1)\nNicht verderblich: (2)");
    // Eingabe für die Art des Artikels (verderblich oder nicht)
    scanf("%d", &lagerwahl);

    switch (lagerwahl) {
    case VERDERBLICH:
        // Artikel ist verderblich, Lagerort entsprechend festlegen und verarbeiten
        printf("\nDer Artikel ist als verderblich markiert. Er wird in Halle an der Saale eingelagert.\nDruecken Sie Enter, um den angelegten Artikel anzuzeigen.\n");
        // Weitere Verarbeitung für das verderbliche Lager hier einfügen
        break;
    case NICHT_VERDERBLICH:
        // Artikel ist nicht verderblich, Lagerort entsprechend festlegen und verarbeiten
        printf("\nDer Artikel ist als nicht verderblich markiert. Er wird in Porta Westfalica eingelagert.\nDruecken Sie Enter, um den angelegten Artikel anzuzeigen.\n");
        // Weitere Verarbeitung für das nicht-verderbliche Lager hier einfügen
        break;
    default:
        printf("\nUngültige Eingabe. Bitte wählen Sie eine der angegebenen Optionen.");
        break;
    }


    getchar();
    getchar();

    bs_loeschen();

    // anzeigen des erstellten Artikels so umbauen, dass User Inputs richtig ausgegeben werden
    // Vielleicht auch in artikel_anzeigen() Funktion bauen?
    printf("Artikel wurde erfolgreich angelegt. Druecke Enter, um zum Menue zu kommen!");
    printf("\n");
    printf("\nArtikel Name: %s", artikeltyp.name);
    printf("\nArtikel Nummer: %d", artikeltyp.art_nummer);
    printf("\nArtikel Preis: %.2lf", artikeltyp.preis);
    printf("\nArtikel Hoehe (in cm): %.2lf", artikeltyp.hoehe);
    printf("\nArtikel Breite (in cm): %.2lf", artikeltyp.breite);
    printf("\nArtikel Tiefe (in cm): %.2lf", artikeltyp.tiefe);

    // Hinzufügen des neuen Artikels zur Liste
    artikel_liste[anzahl_artikel] = artikeltyp;
    anzahl_artikel++;

    getchar();

    return 0;
}

void bs_loeschen(void)
{
    system("CLS");   // MS-DOS-Kommando
}

