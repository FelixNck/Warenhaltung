# Warenhaltung

Die Aufgabe enthält optionale Anforderungen. Werden diese erfüllt können Zusatzpunkte gesammelt werden.

Die Firma Katzen-Bedarf OHG vertreibt diverse Waren im Kontext der Hauskatzenhaltung. Zur Warenhaltung hat die Firma zwei Lager. Ein Lager ist in Halle an der Saale und auf verderbliche Artikel ausgerichtet, das andere in Porta Wesfalica für alle anderen Waren.

Es werden in beiden Lagern genormte Industrieregale verwendet. Diese haben eine unterbrechungsfreie Stützbreite von 3m, eine Tiefe von 1,2m und Fachhöhen von 20cm und 40cm. Das Lager in Porta Westfalica hat zudem auch Fachhöhen von 80cm. Die Regalstützen haben eine Breite von 5cm. Jede Regalebene besteht aus einem speziellen Holzverbundstoff und ist 5cm dick. Jedes Regal hat einen Sockel von 10cm Höhe auf dem Waren bereits abgelegt werden dürfen. Die Maximalhöhe jedes Regals darf 2,55m nicht überschreiten. Ein mit Waren im obersten Fach bestücktes Regals darf 2,90m Höhe nicht überschreiten.

Alle Regale in beiden Lagern können von beiden Seiten bestückt werden.

Waren werden nach dem „Amazon-Prinzip“ eingelagert. Dies bedeutet, dass Waren nicht sortiert in das Lager gelegt werden, sondern die nächst-mögliche freie Stelle, welche gerade groß genug für den Artikel ist, verwendet wird.
(Achtung: Dies bedeutet, dass eine Packung Katzenfutter erst dann in ein riesiges Fach gelegt werden soll, wenn alle anderen kleineren Fächer belegt sind.)

Die minimale Breite einer Einlagerungsfläche beträgt 10cm. Auf jeder Regalebene sind deshalb alle 10cm Positions-IDs markiert. Artikel, welche kleiner sind, belegen trotzdem eine komplette 10cm-Fläche.
(Die kleinst-mögliche Lagerfläche ist somit definiert als: 10cm breit, 60cm tief (wegen beidseitiger Bestückung) und 20cm hoch; die größtmögliche Lagerfläche: 300cm breit, 120cm tief und 80cm hoch.)

Somit ist es wichtig, dass jeder Lagermitarbeiter mit Hilfe einer Warenhaltungssoftware angezeigt bekommt, wo ein Artikel im Lager liegt, wenn diese entnommen und versendet werden soll.

In Halle an der Saale gibt es

* 40 Regalreihen, davon
  * 30 in einer Gesamtlänge von 15,20m mit 40cm-Fächern und
  * 10 in einer Gesamtlänge von 9,10m mit 20cm-Fächern.

In Porta Westfalica gibt es

* 30 Regalreihen, davon
  * 10 in einer Gesamtlänge von 15,20m mit
    * zwei 40cm-Fächern unten und
    * dem Rest 20cm und
  * 10 in einer Gesamtlänge von 9,10m mit 80cm-Fächern und
  * 10 in einer Gesamtlänge von 12,15m mit
    * einer Ebene 80cm-Fächern unten,
    * zwei Ebenen 40cm-Fächern und
    * dem Rest 20cm-Fächern

Wenn neue Artikel erfasst werden, werden insbesondere die Maße erfasst. Ein einmal erfasster Artikel ist über seine Artikelnummer eindeutig auffindbar.

Erfasste Artikel können auch verändert werden, zum Beispiel wenn der Hersteller die Verpackungsgröße oder Stückung verändert. - Das Warensystem muss dann zwischen Alt-Waren und Neu-Waren unterscheiden.

Eingelagerte Artikel sind an ihrem Lagerort über eine eindeutige Inventarnummer zu finden. Artikel dürfen umgeräumt werden, dabei ändert sich die Inventarnummer nicht.

Unterschiedliche Artikel dürfen nie unter derselben Positions-ID gelagert werden. Gleichartige Artikel dürfen so gelagert werden. Dafür ist in der Warenhaltung ein durch die Lagermitarbeiter zu setzendes Flag vorgesehen, welches gesetzt wird, sobald die Lagerfläche der gegebenen Positions-ID voll ist bzw. kein weiterer Artikel dazu gestellt werden soll.

Das typische Katzenfutter ist in kleinen 85g-Tütchen verpackt. 10 Tütchen bilden einen Karton von (30 x 15 x 10) cm³. Kunden können auch einzelne Tütchen kaufen, weshalb die Warenhaltung diesen Fall berücksichtigen muss.


## ANFORDERUNGEN AN DIE PRÜFUNGSLEISTUNG

* Ihr Warenhaltung soll eine Pseudo-grafische Ausgabe auf der Konsole haben. Diese soll Texte und Grafikähnliche Objekte immer an der gleichen Stelle anzeigen. (Beispiel weiter unten)

* Es müssen neue Artikel definiert werden können.

* Existierende Artikel müssen verändert werden können.

* Artikel müssen erfasst und eingelagert werden können.

* Artikel müssen aus dem Lager entnommen werden können.

* Artikel müssen umgeräumt werden können.

* Beide Lager sind am Anfang leer.

* Mit einem Tastendruck sollen die Lager mit zufälligen Artikeln bestückt werden, wobei die Belegung nicht statisch im Quellkode definiert werden soll. Die Belegung muss entlang des oben beschriebenen „Amazon-Prinzips“ deterministisch erfolgen. Dazu soll abgefragt werden, wie viele Artikel eingelagert werden sollen.
(Diese Funktion so soll beliebig oft aufrufbar sein, bis das Lager voll ist.)

* Auf Tastendruck soll eine zufällige Kundenbestellung erzeugt werden, welche nur Artikel aus dem Bestand umfasst. Dazu soll abgefragt werden, wie viele Artikel das Lager verlassen sollen.
(Im Sinne der Prüfungsleistung sollen Sie annehmen, dass Kunden nur das bestellen, was tatsächlich da ist. Es gibt keine Wartelisten o.ä.)

* Es sollen auch manuelle Bestellung erfasst werden können.
(Es muss eine Fehlermeldung geben, wenn ein Artikel nicht im Lager vorrätig ist.)

* Bestellungen sollen über beide Lager hinweg möglich sein.
(Ein Kunde soll einen Kratzbaum UND Katzenfutter bestellen können.)

* Bestellungen sind natürlich nur möglich bis die Lager leer sind.

* Bestellungen liegen so lange im Warenausgang, bis die Bestellung per Tastendruck als „versendet“ markiert sind.

* Es muss jederzeit ersichtlich sein, wie die Belegung der beiden Lager ist.

* Es muss jederzeit ersichtlich sein, wie viele verschiedene Artikel erfasst sind.
(Beachten Sie, dass ein veränderter Artikel anders zu behandeln ist, als das Original. Zum Beispiel „Catfood Bauernhof-Mischung“ ist als 10er-Packung á 100g anders zu behandeln als „Catfood Bauernhof-Mischung“ als 10er-Packung á 85g; oft wird dafür intern die Bezeichnung leicht geändert, z.B. „Catfood Bauernhof-Mischung (2024)“, oder die Artikelnummer wird leicht geändert.)

* Optional: Verderbliche Waren im Lager in Halle an der Saale müssen vor Erreichen des MHD aus dem Lager entfernt werden.

* Optional: Kunden dürfen Non-Food-Artikel zurücksenden. Diese Rücksendungen müssen über den Wareneingang wieder eingelagert werden.


### BEISPIELAUSGABE

HAL                             MI
Artikel:   37                   Artikel:   25
Belegung                        Belegung
    20cm:  40%                      20cm:   5%
    40cm:  14%                      40cm:  51%
                                    80cm:  79%
(n)euen Artikel anlegen
Artikel (e)rfassen
Artikel (v)erändern
Artikel (l)öschen
Lagerbestand zufällig be(f)üllen
Lagerbestand man(u)ell befüllen
zufällige (B)estellung erzeugen                  Warenausgang
(m)anuelle Bestellung erzeugen                            HAL:  6 Artikel
erfasste Bestellung (v)ersenden                            MI:  2 Artikel
erfasste Bestellung (a)bbrechen
