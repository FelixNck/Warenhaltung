#include <stdio.h>
#include <stdlib.h>

int menue();
int neuen_artikel_anlegen();
void bs_loeschen();

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