# Konzept für eine robustere Bunderkennung

## Ausgangslage

Im vollständigen Sensormodus scannt `firmwareCtl` nacheinander alle Bundkontakte
und liest dabei für jede Saite den zugehörigen Eingang. Aus allen aktiven
Kontakten wird derzeit der höchste Bund als `strPrs` gewählt. Erst wenn derselbe
Wert `strBncs` Scans lang anliegt und seit dem letzten ausgegebenen Ereignis
mindestens `fretMaskT` Millisekunden vergangen sind, wird die Änderung
weitergegeben.

Ein normal gegriffener Ton oberhalb des ersten Bundes kann mechanisch zwei
benachbarte Kontakte erzeugen:

- den tatsächlich gegriffenen Bund `F`, und
- den darunterliegenden Bund `F - 1`.

Beim Greifen und Loslassen entstehen diese Kontakte jedoch nicht gleichzeitig.
Die Zwischenzustände `nur F`, `nur F - 1` und `kein Kontakt` können deshalb als
scheinbar gültige Zustände lange genug anliegen, um trotz Debounce und Maske ein
falsches Ereignis auszulösen.

## Bewertung der Zwei-Kontakt-Idee

Zwei **benachbarte** Kontakte sind ein wesentlich stärkeres Plausibilitätssignal
als ein einzelner Kontakt. Die Idee kann daher insbesondere beim Umgreifen
Fehlinterpretationen des tieferen Bundes unterdrücken. Eine globale Regel
„gedrückt nur bei mindestens zwei Kontakten“ ist aber nicht ausreichend:

1. **Bund 1 ist konstruktiv ein Sonderfall.** Dort steht kein tieferer
   Bundkontakt zur Bestätigung zur Verfügung. Ein striktes Zwei-Kontakt-Kriterium
   würde Bund 1 unspielbar machen.
2. **Ein einzelner Kontakt am höheren Bund ist nicht zwingend falsch.** Je nach
   Fingerposition, Saitenlage, Verschmutzung oder Kontaktwiderstand kann der
   zweite Kontakt zeitweise fehlen. Eine harte Regel würde dann gültige Griffe
   übersehen und sich langsamer anfühlen.
3. **Beliebige zwei Kontakte reichen nicht.** Nur das Paar `(F - 1, F)` bestätigt
   `F`. Zwei nicht benachbarte Kontakte können durch mehrere Finger, Nebenschlüsse
   oder Scanartefakte entstehen.
4. **Beim Loslassen hilft das Kriterium allein nicht.** Das bestätigte Paar zerfällt
   zwangsläufig zuerst zu einem Einzelkontakt. Würde dieser sofort als anderer
   Bund behandelt, bleibt das bisherige Fehltracking erhalten; würde er sofort
   als „losgelassen“ behandelt, entstehen vorzeitige Note-Offs.
5. **Die aktuelle Information wird bereits erfasst, aber verworfen.** Der Scanner
   hält die gesamte Matrix in `frtState`; `updateFretDebounce()` reduziert sie
   anschließend auf den höchsten Kontakt und eine Kontaktanzahl. Für ein
   Nachbarschaftskriterium muss deshalb kein Pin oder Scanablauf geändert werden.

**Empfehlung:** Das benachbarte Kontaktpaar als schnelle Bestätigung verwenden,
aber nicht als absolute Voraussetzung. Es sollte Teil eines zustandsbehafteten,
asymmetrischen Filters sein: bestätigte Paare dürfen schneller übernommen
werden; Einzelkontakte werden als Übergang behandelt und erst nach einer kurzen,
gemessenen Frist als Fallback akzeptiert.

## Vorgeschlagene Zustandsmaschine pro Saite

Die Zustandsmaschine arbeitet nach einem vollständigen Matrixscan und getrennt
von MIDI-, Audio-, Kickup- und Sequenceraktionen. Sie liefert weiterhin nur den
bisherigen logischen Wert `0..nFrets`; nachgelagerte Ereignisse und Protokolle
bleiben unverändert.

### Beobachtung aus einem Scan

Für jede Saite werden abgeleitet:

- `highest`: höchster aktiver Kontakt oder `0`,
- `hasPair`: `highest > 1` und die Kontakte `highest` sowie `highest - 1` sind
  aktiv,
- `contactCount`: Zahl aktiver Kontakte,
- optional eine Bitmaske aller Kontakte für Diagnose und Tests.

Die vorhandene Abbildung ist zu beachten: Matrixindex `f` wird aktuell als
Bundnummer `f + 1` ausgegeben. Das Paar für den ausgegebenen Bund `F` liegt somit
an den Matrixindizes `F - 1` und `F - 2`.

### Zustände

| Zustand | Bedeutung |
| --- | --- |
| `RELEASED` | Kein bestätigter Griff. |
| `CANDIDATE(F)` | Ein noch nicht bestätigter Einzelkontakt oder ein instabiler Übergang. |
| `PRESSED(F)` | Stabil bestätigter, nach außen gemeldeter Griff. |
| `RELEASING(F)` | Das bestätigte Paar ist zerfallen; kurze Haltephase gegen falschen Bundwechsel beziehungsweise vorzeitiges Loslassen. |

### Übergänge

1. **Schnelles Greifen oberhalb Bund 1:** In `RELEASED` oder `CANDIDATE`
   bestätigt das benachbarte Paar `(F - 1, F)` den Bund `F` nach einer kleinen
   Anzahl aufeinanderfolgender identischer Scans. Diese Schwelle sollte deutlich
   kleiner als der heutige allgemeine Debounce sein, muss aber am Instrument
   gemessen werden.
2. **Bund 1:** Ein Einzelkontakt an Bund 1 wird über einen eigenen, zeitbasierten
   oder scanbasierten Debounce bestätigt.
3. **Fallback für unvollständige Paare:** Ein stabiler Einzelkontakt oberhalb
   Bund 1 darf nach einer längeren Frist bestätigt werden. Dadurch bleibt das
   System bei schwachem oder fehlendem Zweitkontakt spielbar.
4. **Umgreifen:** Liegt in `PRESSED(oldF)` ein vollständiges neues Paar für
   `newF` vor, kann direkt auf `newF` gewechselt werden. Ein einzelner Kontakt
   neben `oldF` ändert die ausgegebene Note zunächst nicht, sondern startet oder
   erneuert nur `CANDIDATE(newF)`.
5. **Loslassen:** Zerfällt das Paar, bleibt `oldF` zunächst in `RELEASING(oldF)`
   logisch aktiv. Erst eine kurze stabile Nullphase erzeugt Release. Entsteht
   währenddessen ein bestätigtes neues Paar, erfolgt ein direkter Bundwechsel
   ohne zwischenzeitliches Release.
6. **Mehrdeutige Matrix:** Bei mehr als zwei Kontakten wird das höchste
   benachbarte Paar als Kandidat gewählt. Existiert kein benachbartes Paar, gilt
   die Beobachtung als unsicher und wird nur über den langsamen Fallback
   übernommen. Ob Mehrfingergriffe auf derselben Saite für Step-Edit weiterhin
   separat benötigt werden, muss vor der Implementierung geprüft werden.

## Warum Zeit statt ausschließlich Scananzahl erwägen

`readFretboard()` läuft im Hauptloop, dessen Frequenz durch andere Aufgaben
schwanken kann. Eine feste Anzahl gleicher Scans entspricht daher nicht zwingend
einer konstanten Latenz. Für musikalisch wahrnehmbare Übergänge sind Zeitgrenzen
in Mikro- oder Millisekunden leichter zu messen und zu begründen. Alternativ muss
die tatsächliche Scanrate unter Worst-Case-Last bestimmt werden, bevor neue
Scan-Schwellen gewählt werden.

Die bestehenden Werte `strBncs = 500` Samples und `fretMaskT = 50 ms` sollten
nicht blind übernommen oder geändert werden. Die Paarbestätigung, der
Einzelkontakt-Fallback und die Release-Haltezeit benötigen getrennte, anhand von
Messdaten festgelegte Parameter. Insbesondere darf die bestehende 50-ms-Maske
einen schnellen, eindeutig bestätigten Bundwechsel nicht unnötig blockieren.

## Implementierung in kleinen Schritten

1. **Nur instrumentieren:** Pro Scan Zeitstempel, Rohkontaktmaske, aktuell
   ausgegebenen Bund und Ereignisgrund in einen begrenzten Debug-Puffer schreiben.
   Keine seriellen Prints im zeitkritischen Scanpfad; den Puffer später gedrosselt
   ausgeben.
2. **Reale Übergänge aufzeichnen:** Je Saite Bund 1, mehrere mittlere und hohe
   Bünde, langsames und schnelles Greifen, Loslassen, Slides und schnelle
   Bundwechsel erfassen. Dabei besonders die Dauer der Einzelkontaktphasen
   bestimmen.
3. **Filter als reine Logik isolieren:** Die Auswertung einer Kontaktmaske in
   einer kleinen zustandsbehafteten Komponente implementieren. Der bestehende
   Hardware-Scan bleibt unverändert. Hostseitige Tests speisen aufgezeichnete
   Maskenfolgen ein.
4. **Schattenbetrieb:** Alten und neuen Filter parallel berechnen, Aktionen aber
   weiterhin nur aus dem alten Filter auslösen. Abweichungen protokollieren.
5. **Gezielter Hardwareversuch:** Erst nach Auswertung der Logs den neuen Filter
   für Ereignisse aktivieren. MIDI, Audio, Kickup und Step-Edit einzeln prüfen.
6. **Parameter abstimmen:** Fast-Path, Fallback und Releasezeit anhand der
   schlechtesten realen Saite/Bund-Kombination wählen, nicht anhand eines
   synthetischen Idealverlaufs.

## Minimale Testmatrix

Für jede Saite und mindestens Bund 1, 2, einen mittleren sowie den höchsten
erfassten Bund:

- einzelnes Greifen und Loslassen, langsam und schnell,
- Wechsel `F -> F + 1`, `F + 1 -> F` und Sprung über mehrere Bünde,
- Slide mit durchgehendem Saitenkontakt,
- sehr kurzer Tap und wiederholte schnelle Taps,
- absichtlich nur ein Bundkontakt,
- Kontaktfolge `F - 1`, dann Paar; beim Loslassen Paar, dann `F - 1`, dann null,
- Kontaktfolge `F`, dann Paar; beim Loslassen Paar, dann `F`, dann null,
- mehr als zwei gleichzeitige Kontakte sowie nicht benachbarte Kontakte,
- mehrere Saiten gleichzeitig,
- Betrieb mit LED-Update, Sequencer und MIDI, um variable Looplast abzudecken.

Zu messen sind mindestens False Note-On, False Note-Off, falsche Bundnummer,
verlorene Griffe sowie Latenz von erstem Kontakt bis zum Ereignis. Als
Abnahmekriterium sollte die neue Logik bei gleicher Testsequenz weniger
Fehlereignisse erzeugen, ohne die gemessene typische Anschlagslatenz zu erhöhen.

## Auswirkungen und Grenzen

- **Betroffenes Board:** ausschließlich `firmwareCtl` (Teensy 4.1).
- **Pins und elektrischer Scan:** Das Konzept benötigt keine Änderung.
- **Serielle Protokolle:** Keine Änderung; nach außen bleibt der logische
  Griffzustand gleich codiert.
- **Realtime-Audio und Audio-Routing:** Keine Änderung.
- **Kickup:** Keine Timingänderung, aber verbesserte Erkennung verändert
  naturgemäß, welches Griffereignis einen Kick auslöst; deshalb sicher testen.
- **Ungewissheit:** Ob wirklich jeder reale Griff ab Bund 2 zuverlässig das
  benachbarte Paar bildet, ist aus dem Code nicht belegbar. Diese Annahme muss mit
  Rohdaten aller Saiten und relevanten Bünde validiert werden.

## Implementierungsstand

Der zustandsbehaftete Filter ist als hardwareunabhängige Logik in
`software/firmwareCtl/FretDetection.h` umgesetzt. Im vollständigen Sensormodus
wertet `firmwareCtl` nach jedem unveränderten Matrixscan die Kontaktmaske aus und
verwendet den höchsten benachbarten Kontakt als schnelle Bestätigung. Bund 1,
einzelne höhere Kontakte und die Nullphase besitzen getrennte Zeitgrenzen. Der
vereinfachte Sensormodus verwendet weiterhin den bisherigen scanbasierten
Debounce und die 50-ms-Ereignismaske.

Die Startwerte betragen 4 ms für ein Paar, 12 ms für Bund 1, 30 ms für den
Einzelkontakt-Fallback und 12 ms für das Loslassen. Diese Werte sind ausdrücklich
vorläufig. Vor dem Einsatz am Instrument müssen sie mit der oben beschriebenen
Testmatrix und aufgezeichneten Rohkontakten validiert und gegebenenfalls
angepasst werden.

Die reine Filterlogik wird hostseitig durch `tests/fret_detection_test.cpp`
geprüft. Der Test deckt Paarbestätigung, zerfallende Paare, Loslassen, Bund 1,
Einzelkontakt-Fallback, direkte Bundwechsel, mehrdeutige Kontaktmasken und den
Überlauf von `millis()` ab.
