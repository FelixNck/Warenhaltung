#include <stdio.h>
#include <stdlib.h>

#define HALLE_20 18000 //vorhandene Anzahl Positions IDs für 20cm hohe Fächer
#define HALLE_40 54000
#define PORTA_20 34800
#define PORTA_40 6000
#define PORTA_80 7800

int menue();
int neuen_artikel_anlegen();
void bs_loeschen();



// Beispiel

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




struct ArtikelTyp {
    char name[100] = Katzenfutter Bio 100g
    int art_nummer = 12345
    int preis = 10
    int hoehe = 10
    int breite = 20
    int tiefe = 10
};

struct Artikel {
    int artikel_typ_nummer = 12345
    int inventarnummer = 9785
    int positions_id = [3, 4]
};



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

int menue() {
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
    int janein = 0;

    int wahl = 0;

    bs_loeschen();
    printf("(0) Nein");
    printf("\n(1) Ja");
    printf("\nNeuen Artikel anlegen?");
    printf("\n");
    
    scanf("%d", &janein);

    if (janein == 0) {
        return 0;
    }

    bs_loeschen();
    printf("Artikel Name:");
    fflush(stdin);
    wahl = getchar();
    fflush(stdin);

    printf("\nArtikel Nummer:");
    fflush(stdin);
    wahl = getchar();
    fflush(stdin);

    printf("\nArtikel Preis:");
    fflush(stdin);
    wahl = getchar();
    fflush(stdin);

    printf("\nArtikel Hoehe:");
    fflush(stdin);
    wahl = getchar();
    fflush(stdin);

    printf("\nArtikel Breite:");
    fflush(stdin);
    wahl = getchar();
    fflush(stdin);

    printf("\nArtikel Tiefe:");
    fflush(stdin);
    wahl = getchar();
    fflush(stdin);

    return 0;
}

void bs_loeschen(void)
{
    system("CLS");   // MS-DOS-Kommando
}