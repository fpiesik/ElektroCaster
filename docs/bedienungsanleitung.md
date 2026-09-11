# ElektroCaster – Bedienungsanleitung

> **Stand und Geltungsbereich:** Diese Anleitung beschreibt ausschließlich das Verhalten, das sich aus dem aktuellen Firmware-Quellcode ableiten lässt. Die Bedienelemente sind im Code nur teilweise beschriftet; deshalb werden sie hier zusätzlich mit ihren Firmware-Indizes bezeichnet. Aussagen mit **„unklar“**, **„derzeit ohne Wirkung“** oder **„zu prüfen“** sind bewusst keine Vermutungen über die Hardware.

## 1. Sicherheit und wichtige Einschränkungen

- Die ElektroCaster ist ein Prototyp mit sechs **Kickup-Magneten** und sechs elektromagnetischen **MulEBow-Spulen**. Aktoren nur mit korrekt dimensionierter Versorgung und Treiberhardware betreiben. Vor Arbeiten an Saiten, Steg, Spulen oder Verkabelung die Aktorversorgung trennen.
- Kickup kann eine Saite selbsttätig anschlagen. Hände und lose Gegenstände aus dem Bewegungsbereich halten.
- MulEBow erzeugt ein elektromagnetisches Dauersignal. Bei ungewöhnlicher Erwärmung oder Geräuschentwicklung sofort ausschalten.
- Die Firmware zeigt keine aus dem Code erkennbare Fehler- oder Schutzmeldung am OLED für SD-, Audio- oder Kommunikationsfehler an.
- Die reale Anordnung, Beschriftung, Schalterpolarität und Saitennummerierung sind nicht vollständig dokumentiert. Vor einem Bühnen- oder Lautstärkebetrieb alle Zuordnungen bei kleiner Lautstärke prüfen.
- Die Firmware verwendet intern die **Saitenindizes 0 bis 5**. OLED-Werte werden meist in umgekehrter räumlicher Reihenfolge ausgegeben. Diese Anleitung spricht deshalb neutral von „Saite 0 … 5“ und nicht von hoher/tiefer E-Saite.

## 2. Systemüberblick

Die ElektroCaster kann zunächst wie eine normale Gitarre gespielt werden. Zusätzlich verarbeitet sie jede Saite getrennt und bietet:

- ein berührungsempfindliches Griffbrett mit 21 erfassten Bundpositionen,
- 150 RGB-LEDs, entsprechend 25 LED-Positionen je Saite,
- hexaphonische Filter-, Hüllkurven- und Ringmodulationsverarbeitung,
- Kickup zum mechanischen Anschlagen einzelner Saiten,
- MulEBow zum elektromagnetischen Anregen gegriffener Saiten,
- USB-MIDI-Ausgabe und USB-MIDI-Eingang,
- drei parallele generische Sequencer-Instanzen mit je sechs Patterns,
- einen alternativen Saiten-Arpeggiator mit sechs Patterns,
- 100 Song-Speicherplätze auf SD-Karte,
- Skalen-, MIDI- und Sequencer-Anzeigen auf dem LED-Griffbrett.

Beim Start lädt der Hauptcontroller **Song 0**, anschließend die separat gespeicherte Saitenstimmung und die Saitenpegel. Der interne Standardwert ist 90 BPM; gestartet wird die Clock erst über die Wiedergabetaste oder einen entsprechenden Steuerablauf.

## 3. Bedienelemente und Bezeichnungen

### 3.1 Übersicht

| Bedienelement | Anzahl | Bezeichnung in dieser Anleitung |
| --- | ---: | --- |
| Schieberegler | 10 | Fader 1 … 10 |
| Potentiometer | 7 | Poti 1 … 7 |
| Joystick | 1, zweiachsig | Joystick X/Y |
| Drehencoder | 8 | Display-Encoder, sechs Saiten-Encoder, Sammel-Encoder |
| 12-Stufen-Drehschalter | 3 | Pattern-Schalter 1 … 3 |
| digitale Eingänge | 19 | Display-Taster, Shift, drei Aktor/Auto-Schalter, drei Transporttaster, Poti-Schalter, Speichertaster, Poti-Encoder-Taster, zwei Griffbrett-Schalter, sechs Saiten-Encoder-Taster |

Die Zuordnung „Fader/Poti“ folgt den Kommentaren im Controllercode. Ein analoger Eingang ist dort als „Poti 7“ geführt; die übrigen sechs als Poti 1, 2, 3, 5 und 6 sowie ein weiterer, nur durch seine Funktion erkennbarer LFO-Regler. Eine physische Beschriftung für „Poti 4“ ist aus dem Code nicht sicher rekonstruierbar.

### 3.2 Encoder

- **Display-Encoder:** Der einzelne Encoder neben dem OLED. Drehen verändert je nach Ansicht einen Wert oder die gewählte Funktionszeile. Drücken schaltet zwischen diesen beiden Ebenen um.
- **Saiten-Encoder 0 … 5:** Je ein Encoder pro Saite. Sie ändern den aktuell gewählten Saitenparameter. Ihre Anzeige auf dem OLED erfolgt von links nach rechts in der Reihenfolge Saite 5 … 0.
- **Sammel-Encoder:** Ändert den aktuellen Parameter grundsätzlich für alle sechs Saiten. Im Step-Edit-Modus kann er stattdessen den gewählten Step-Parameter bearbeiten.
- Die HID-Firmware teilt Encoderbewegungen durch vier. Eine wahrnehmbare Rasterbewegung entspricht daher erst nach vier Rohschritten einer Änderung.

### 3.3 Display-Encoder bedienen

In den Skalen-, Sequencer- und Arpeggiatoransichten gilt grundsätzlich:

1. **Display-Taster nicht gedrückt:** Drehen ändert den Wert der markierten Zeile.
2. **Display-Taster gedrückt:** Drehen verschiebt die Auswahlmarkierung zwischen den Zeilen.

Im String-Setup wechselt das Drehen des Display-Encoders direkt zwischen Stimmung, Saitenpegel und BPM; der Tastenzustand wird dort nicht ausgewertet.

## 4. Globale Schalter und Taster

### 4.1 Shift-Taster

Der Taster unter dem Display-Encoder setzt den globalen `shift`-Zustand. Shift verändert folgende Funktionen:

- zusammen mit **MulEBow-Schalter**: interne/externe Clock auswählen,
- zusammen mit **Kickup-Schalter**: Griffbrett-Vollerkennung/vereinfachte Erkennung auswählen,
- beim Bearbeiten eines Sequencer-Steps: den Step löschen bzw. mit dem Display-Encoder alle Steps der gewählten Saite(n) ändern,
- beim Songwechsel: vor dem Wechsel Song 0 statt des aktuell gewählten Slots speichern,
- zusammen mit Speichertaster und gedrücktem Display-Taster: Standard-Song nur im Arbeitsspeicher herstellen.

Der Code wertet einen Zustand aus, nicht ausdrücklich eine Haltezeit. Ob der physische Taster als momentaner Taster oder rastender Schalter ausgeführt ist, muss am Instrument geprüft werden.

### 4.2 Dreifach-Schaltergruppe

| Schalter | Ohne Shift | Mit Shift |
| --- | --- | --- |
| links | Automatik für Instanz 1: generischen Saitensequencer oder, bei gewähltem Arpeggiator, Arpeggiator ein/aus | keine zusätzliche Sonderfunktion |
| Mitte | MulEBow ein/aus | setzt gleichzeitig `extClk`: aus = interne Clock, ein = externe USB-MIDI-Clock |
| rechts | Kickup ein/aus | setzt gleichzeitig den Griffbrett-Sensormodus: aus = Bundposition erkennen, ein = nur gedrückt/nicht gedrückt erkennen |

**Wichtig:** Mit Shift werden MulEBow beziehungsweise Kickup trotzdem ebenfalls geschaltet. Die Clock- beziehungsweise Sensormoduswahl ist keine getrennte, „stumme“ Zweitfunktion.

### 4.3 Dreifach-Tastergruppe

| Taster | Funktion |
| --- | --- |
| rechts | plant die Synchronisierung aller drei Sequencer-Instanzen und des Arpeggiators auf den nächsten Taktwechsel |
| Mitte | schaltet Start/Stop um; Start sendet USB-MIDI Start, danach 24 Clock-Ticks pro Viertelnote, Stop sendet USB-MIDI Stop und All Notes Off |
| links | wählt zyklisch Sequencer-Instanz 1, 2, 3 und schaltet auf deren Bedienmodus |

### 4.4 Griffbrett-Schalter

- **Edit/Play-Schalter (`fbrdMode`):**
  - `0` = Spielen. Berührte Bünde erzeugen je nach Automatikzustand Hüllkurven-, MIDI- und Kickup-Ereignisse; das LED-Griffbrett zeigt die Skalen-/Notenansicht.
  - `1` = Step-Edit. Die ersten 16 Bundpositionen dienen zum Setzen und Bearbeiten von Sequencer-Steps; das LED-Griffbrett zeigt das Raster. Außer im alternativen Arpeggiatorbetrieb wird der Audio-Hauptausgang dabei stummgeschaltet.
- **Sequencer-Ansicht halten (`fbrdSeqVHld`):** Zeigt das Sequencer- oder Arpeggiatorraster auch im Spielmodus. Der Kommentar im Code bezeichnet diese Ansicht ausdrücklich als **nicht editierbar**.

### 4.5 Poti-Schalter und Encoder-Taster

Der **Poti-Schalter** wählt die Funktion der sechs Saiten-Encoder-Taster:

- Stellung `1`: Ein Saiten-Encoder-Taster wählt direkt eine Encoderfunktion. Wegen der umgekehrten Zuordnung wählen die sechs Taster die Funktionsindizes 5 … 0. Beim generischen Sequencer existieren nur fünf gültige Encoderfunktionen; der zusätzliche Index 5 wird zwar gesetzt, hat aber keine implementierte Wirkung. Beim Arpeggiator gilt dasselbe für den zusätzlichen Index 5.
- Stellung `0`: Ein Saiten-Encoder-Taster führt die aktuell im OLED unter `btn` gewählte Saitenfunktion aus.

Der separate **Poti-Encoder-Taster** führt die aktuell gewählte generische Saitentaster-Funktion gleichzeitig für alle sechs Saiten aus. Er ruft auch im Arpeggiatorbetrieb die generische Sequencerfunktion auf, nicht die Arpeggiatorfunktion.

## 5. Fader, Potis und Joystick

Alle analogen Werte werden intern auf ungefähr 0 … 1 normiert.

| Regler | Funktion | Ausgabebereich / Hinweis |
| --- | --- | --- |
| Fader 1 | USB-MIDI CC 3, Kanal 1 | 0 … 127 |
| Fader 2 | USB-MIDI CC 4, Kanal 1 | 0 … 127 |
| Fader 3 | USB-MIDI CC 5, Kanal 1 | 0 … 127 |
| Fader 4 | USB-MIDI CC 6, Kanal 1 | 0 … 127 |
| Fader 5 | Filter-Hüllkurven-Decay | skaliert bis ca. 5000 ms |
| Fader 6 | Amplituden-Hüllkurven-Decay | skaliert bis ca. 5000 ms |
| Fader 7 | Filter-Hüllkurven-Attack | skaliert bis ca. 1000 ms |
| Fader 8 | Amplituden-Hüllkurven-Attack | skaliert bis ca. 1000 ms |
| Fader 9 | Anteil der Filterhüllkurve | Überblendung zwischen konstantem und Hüllkurven-Steuersignal |
| Fader 10 | Anteil der Amplitudenhüllkurve | Überblendung zwischen trockenem Ringmodulationssignal und Hüllkurvensignal |
| Joystick X | USB-MIDI CC 9, Kanal 1 | 0 … 127 |
| Joystick Y | USB-MIDI CC 10 **und** Channel Aftertouch, Kanal 1 | beide 0 … 127 |
| Poti 1 | Audio-Gesamtlautstärke | nichtlinear; Controller begrenzt den übertragenen Maximalwert zur Clipping-Vermeidung |
| Poti 2 | Filterresonanz | Audio-Skalierung etwa 0 … 7 |
| Poti 3 | Filter-Oktavsteuerung | Audio-Skalierung etwa 0 … 7 |
| Poti 4 (im Code nicht ausdrücklich kommentiert) | LFO-Tonhöhenoffset | etwa −12 … +12 Halbtöne |
| Poti 5 | USB-MIDI CC 7, Kanal 1 | 0 … 127 |
| Poti 6 | LFO-/Ringmodulations-Tiefe | Amplitude steigt, Offset sinkt entsprechend |
| Poti 7 | LFO-Oktavlage | intern ungefähr −5 … +4 Oktaven |

Beim manuellen Greifen hält die Firmware die Amplituden- und Filterhüllkurve für die jeweilige Saite auf Sustain, selbst wenn der Decay-Regler kürzer eingestellt ist. Beim Loslassen oder bei automatischer Triggerung gilt wieder die normale Decay-/Sustain-Logik.

## 6. String Setup und Stimmhilfe

### 6.1 Aufrufen

String Setup ist Betriebsmodus 0. Er wird über **Pattern-Schalter 2 oder 3, Stellung 6** (`val == 6`) aufgerufen. Pattern-Schalter 1 wechselt ab Stellung 6 stattdessen in den Arpeggiatorbetrieb.

### 6.2 Parameter

Der Display-Encoder wählt eine von drei markierten OLED-Zeilen:

1. **Stimmung:** Saiten-Encoder verschieben die Sollstimmung der jeweiligen Saite in MIDI-Halbtönen. Das OLED zeigt nur den Notennamen ohne Oktave.
2. **Saitenpegel:** Bereich 0 … 50; der Wert wird als Verstärkung von 0,00 … 1,00 an den Audio-Controller übertragen.
3. **BPM:** Bereich 1 … 250; jeder Saiten-Encoder verändert denselben globalen Tempowert.

Der Sammel-Encoder transponiert im String Setup stets alle sechs Stimmungen zugleich, unabhängig von der aktuell markierten OLED-Zeile.

Beim Verlassen des String Setup werden Stimmung und Saitenpegel separat gespeichert. Beim Systemstart werden sie nach Song 0 geladen und überschreiben damit die im Song enthaltenen entsprechenden Werte.

### 6.3 LED-Stimmhilfe

Im String Setup:

- markiert LED-Position 0 die eingestellte Leersaite in deren Tonfarbe,
- dient Position 12 als mittige Referenz,
- wird bei ausreichendem Eingangspegel die erkannte Tonhöhe farbig angezeigt,
- zeigt eine benachbarte LED an, ob die erkannte Tonhöhe oberhalb oder unterhalb des Zielbereichs liegt.

Die Richtung „zu hoch/zu tief“ relativ zur physischen Halsorientierung ist aus dem Code allein nicht sicher benennbar. Die Tonhöhenerkennung übernimmt nur Messwerte mit hoher Analysewahrscheinlichkeit; bei leiser oder instabiler Saite kann die Anzeige stehenbleiben.

## 7. Skalen- und Spielansicht

Die Skalenansicht erscheint in den Sequencer-Modi, wenn der Griffbrettschalter auf Spielen steht und „Sequencer-Ansicht halten“ aus ist.

### 7.1 Einstellbare Felder

Mit gedrücktem Display-Taster wählt man eines von sechs Feldern; ohne gedrückten Taster ändert man dessen Wert:

| OLED-Feld | Funktion | Werte |
| --- | --- | --- |
| Grundton | tonales Zentrum | C, C#, D, D#, E, F, F#, G, G#, A, A#, B |
| Skala | Skalentyp | off, root, pentatonic, major, minor, hrm minor, mel minor, altered, whole, wholeHalf |
| Stufe | Modus/Rotation innerhalb der Skala | abhängig von der Skala |
| `clr` | Skalenfarbe | 0 = weiß, 1 = Tonklassenfarben |
| `sng` | Songslot | 0 … 99 |
| `src` | Quelle der zusätzlich hervorgehobenen Noten | 0 = empfangene MIDI-/Saitennoten; 1 … 3 = aktive Noten der Sequencer-Instanz 1 … 3 |

**Skala `off`:** Sie besitzt laut Datenstruktur null Stufen. Die Stufenberechnung verwendet trotzdem Modulo mit dieser Anzahl. Diese Kombination ist daher als fehleranfällig einzustufen und sollte bis zur Korrektur nicht über die Stufenfunktion bedient werden.

### 7.2 Songwechsel über das OLED

Beim Drehen des Songfeldes geschieht vor jedem Wechsel:

1. ohne Shift: aktueller Songslot wird gespeichert,
2. mit Shift: Slot 0 wird gespeichert,
3. danach wird der neu gewählte Slot geladen.

Das Wechseln des Songfeldes ist daher **nicht nur Lesen**, sondern kann Daten überschreiben. Vor dem Experimentieren eine Sicherung der SD-Karte anlegen.

### 7.3 LED-Anzeige

- Die Skala wird über alle sichtbaren Bünde eingezeichnet.
- Tonklassen können farbig oder monochrom erscheinen.
- `src 0` überlagert aktive USB-MIDI-Noten:
  - Kanäle 1 … 6 werden jeweils einer Saite zugeordnet,
  - Kanal 15 hebt eine Tonklasse auf allen passenden Bünden hervor,
  - Kanal 16 hebt die exakte MIDI-Note hervor.
- `src 1 … 3` überlagert die aktuell ausgegebenen Tonklassen der jeweiligen Sequencer-Instanz.
- Die letzten drei LED-Positionen bilden zusätzlich eine Pattern-Anzeige für die drei Instanzen.

## 8. Generischer Sequencer

### 8.1 Aufbau

Es existieren drei Instanzen. Jede besitzt:

- sechs Patterns,
- sechs Saitenspuren,
- maximal 16 Steps je Spur,
- unabhängige Länge, Zeitteilung, Startversatz, Sync-/Patternsteuerung und MIDI-Kanal je Spur,
- pro Step: Ton/Skalenstufe, Oktave, Velocity und drei CC-Werte.

Die linke Transporttaste wählt Instanz 1 … 3. Die Pattern-Drehschalter 1 … 3 gehören den entsprechenden Instanzen.

### 8.2 Pattern-Drehschalter

- Stellungen 0 … 5 wählen Pattern 1 … 6 der betreffenden Instanz.
- Bei laufender Clock wird der Wechsel auf die nächste Viertelnote gelegt; bei gestoppter Clock erfolgt er unmittelbar.
- Mit Shift wird das gewählte Pattern nur als Edit-Pattern eingestellt, nicht als laufendes Pattern geplant.
- Stellungen 9 … 11 können die Instanz mit einer anderen Instanz synchronisieren. Die genaue physische Drehrichtung und sinnvolle Selbstsynchronisationsfälle müssen am Instrument geprüft werden.
- Bei Pattern-Schalter 2 und 3 ruft Stellung 6 String Setup auf.

Pattern-Schalter 1 hat zusätzlich eine Sonderrolle: Stellungen 6 … 11 wählen den Arpeggiator und dessen sechs Patterns; der Wechsel wird wie ein Patternwechsel quantisiert.

### 8.3 Spurparameter über Saiten-Encoder

Im Step-Edit-/Rastermodus wählt das OLED unter `enc` eine der folgenden Funktionen:

| Anzeige | Bedeutung | Bereich |
| --- | --- | ---: |
| `len` | Schleifenlänge der Spur | 1 … 16 Steps |
| `tmDv` | Tickabstand pro Step | `4`, `2`, `1`, `.75`, `/2`, `/3`, `/4`, `/6`, `/8`, `/12`, `/16`, `/24`, `/32`, `/48` |
| `offSt` | Startversatz im 16-Step-Raster | 0 … 16 |
| `sync` | Patternnummer, die eine Kanal-0-Steuerspur anfordern kann | 0 … 6; 0 = aus, 1 … 6 = Pattern 1 … 6 |
| `chn` | USB-MIDI-Kanal | 0 … 16; Kanal 0 ist interne Steuerung statt MIDI-Ausgabe |

Die Bezeichnungen der Zeitteilungen sind direkt aus dem Code übernommen. Intern beziehen sie sich auf eine 24-PPQN-Clock; beispielsweise entspricht `1` 96 Ticks, `/4` 24 Ticks und `/16` 6 Ticks.

### 8.4 Step setzen und bearbeiten

1. Griffbrett auf **Edit** stellen.
2. Auf einer Saite Bund 1 … 16 drücken. Dadurch wird der entsprechende Step umgeschaltet.
3. Mit gedrücktem Bund:
   - Display-Encoder drehen, um den unter `str` gewählten Parameter zu ändern, oder
   - Saiten-Encoder verwenden. Im Step-Modus sind die sechs Encoder den sechs Step-Parametern zugeordnet.
4. Shift + Bundkontakt löscht den Step.
5. Shift + Display-Encoder ändert den gewählten Parameter an allen 16 Steps der gerade gehaltenen Saite(n).

Mehrere gleichzeitig erkannte Bundkontakte auf einer Saite bewirken, dass Änderungen über Saiten-/Sammel-Encoder auf alle Steps der ausgewählten Saite(n) angewandt werden. Diese Mehrfachkontaktlogik ist hardwareabhängig.

### 8.5 Step-Parameter (`str`)

| Anzeige | Bedeutung | Bereich / Ausgabe |
| --- | --- | --- |
| `pStp` | Tonklasse oder Skalenstufe | 0 … 11 chromatisch; bei `sclQ` Index in der gewählten Skala |
| `oct` | Oktavindex | 0 … 9; OLED zeigt 1 … 10 |
| `vel` | Velocity | 0 … 50, für MIDI auf 0 … 127 skaliert |
| `c10` | interner Name; gesendet wird CC 2 | 0 … 99 |
| `c11` | interner Name; gesendet wird CC 3 | 0 … 99 |
| `c12` | interner Name; gesendet wird CC 4 | 0 … 99 |

Die OLED-Namen `c10`, `c11`, `c12` stimmen nicht mit den tatsächlich gesendeten Controller-Nummern 2, 3 und 4 überein.

Ist der Spurkanal `0`, werden die drei CC-Felder nicht gesendet, sondern steuern intern:

- erstes CC-Feld: Grundton (`Wert − 1`),
- zweites CC-Feld: Skala (`Wert − 1`),
- drittes CC-Feld: Skalenstufe (`Wert − 1`).

Ein CC-/Steuerwert von 0 führt keine Aktion aus. Bei Kanal 0 und aktivem `sync` kann ein gesetzter Step außerdem einen Patternwechsel anfordern.

### 8.6 Saitentaster-Funktionen (`btn`)

| Anzeige | Wirkung beim Tastendruck |
| --- | --- |
| `mute` | Spur stumm/aktiv umschalten |
| `rnd` | soll eine Spur randomisieren; im aktuellen Code ist die Zielanzahl jedoch immer 0, sodass die aktiven Steps der Spur gelöscht werden |
| `sclQ` | Skalenquantisierung für die Spur ein/aus |

Bei `sclQ = 0` ist `pStp` eine chromatische Tonklasse. Bei `sclQ = 1` ist es eine Stufe der aktuell ausgewählten, rotierten Skala.

### 8.7 Pattern kopieren

Beim Drücken des Display-Encoders merkt sich die Firmware die aktuell laufenden Patterns. Wird anschließend ein anderes Edit-Pattern gewählt und der Display-Encoder losgelassen, kopiert sie das zuvor laufende Pattern in das gewählte Edit-Pattern. Dieser Vorgang betrifft alle drei Instanzen, bei denen Edit- und zuvor aktives Pattern voneinander abweichen. Es gibt keine Bestätigungsabfrage.

## 9. Saiten-Arpeggiator

### 9.1 Aktivieren

1. Pattern-Schalter 1 auf Stellung 6 … 11 drehen; damit wird eines der sechs Arpeggiator-Patterns gewählt.
2. Linken Schalter der Dreifachgruppe einschalten, damit die Automatik läuft.
3. Im Spielmodus Saiten an gewünschten Bünden greifen.

Der Arpeggiator baut seine Folge dynamisch aus den aktuell gedrückten Saiten. Er sendet pro ausgelöstem Saitenschritt MIDI, triggert die Audiohüllkurve und – falls freigegeben – Kickup.

### 9.2 Globale Arpeggiatorfunktionen

Unter `str`:

| Anzeige | Reihenfolge |
| --- | --- |
| `simple` | konfigurierte/gedrückte Reihenfolge vorwärts |
| `back` | Reihenfolge rückwärts |
| `mirror` | vorwärts, dann rückwärts einschließlich der Randwerte |
| `rev mirror` | rückwärts, dann vorwärts einschließlich der Randwerte |

Unter `btn`:

- `mute`: Saitenspur umschalten,
- `randomise`: im aktuellen Code **ohne Wirkung**.

Unter `enc`:

| Anzeige | Funktion | Bereich |
| --- | --- | ---: |
| `steps` | Anzahl Steps der Saite in der Arpeggiofolge | 0 … 64 |
| `tmDv` | Dauer dieser Saite | `1`, `1.5`, `2`, `3`, `4`, `6`, `8`, `12`, `16`, `24`, `32`, `48` |
| `repeat` | Anzahl Trigger pro Step; die Step-Länge bleibt unverändert | 1 … 6 |
| `order` | feste Reihenfolge/Gruppe | 0 … 6 |
| `chn` | USB-MIDI-Kanal | 0 … 16 |

`order = 0` ordnet Saiten nach der Reihenfolge ihres Greifens ein. Gleiche positive `order`-Werte gruppieren Saiten; sie werden gemeinsam ausgelöst. `steps = 0` nimmt eine Saite aus der getakteten Folge. Eine solche Saite kann bei aktivem Arpeggiator weiterhin manuell triggern.

## 10. Spielen, Griffbrettsensing und Aktoren

### 10.1 Vollständige Bunderkennung

Im Sensormodus `0` erkennt die Firmware pro Saite den höchsten gleichzeitig kontaktierten Bund. Im Spielmodus erzeugt ein neuer Kontakt:

- eine Audio-Hüllkurve, sofern keine Sequencer-/Arpeggiatorautomatik die manuelle Auslösung unterdrückt,
- eine USB-MIDI-Note mit Velocity 127,
- optional einen Kickup-Impuls,
- eine Meldung an den Audio-Controller über Tonhöhe und Greifzustand.

Die manuelle USB-MIDI-Note ist `Saitenstimmung + Bundnummer + 12`. Beim Wechsel oder Loslassen wird die vorherige Note der Saite beendet.

### 10.2 Vereinfachte Erkennung

Im Sensormodus `1` erkennt die Firmware nur, ob eine Saite irgendeinen Bund berührt. Da keine sichere Bundnummer vorliegt, werden die normalen MIDI-Notenereignisse für Griffpositionen unterdrückt. Der Modus ist über Shift + Kickup-Schalter erreichbar.

### 10.3 Saiten halten

Wird im Spielmodus bei aktivem Shift eine Saite gegriffen, setzt die Firmware für diese Saite `hold`. Nach Loslassen der Saite wird `hold` wieder gelöscht. Im generischen Sequencer kann eine gehaltene Saite ihre zuletzt bekannte Griffposition als MIDI-Note verwenden. Das ist ein momentaner, an Shift und aktuellen Kontakt gebundener Zustand, kein dauerhaft verriegeltes Hold.

### 10.4 Kickup

- Der rechte Schalter der Dreifachgruppe gibt Kickup frei.
- Manuelle Griffereignisse, Saitensequencer, Arpeggiator und eingehende USB-MIDI-Noten auf Kanälen 1 … 6 können Kickup auslösen.
- MIDI-Kanal 1 triggert intern Saite 5, Kanal 2 Saite 4, …, Kanal 6 Saite 0; die Notennummer wird dabei nicht ausgewertet.
- Gleichzeitig angeforderte Anschläge werden nacheinander abgearbeitet, um Stromspitzen zu reduzieren.

### 10.5 MulEBow

- Der mittlere Schalter der Dreifachgruppe schaltet MulEBow ein/aus.
- Bei eingeschaltetem MulEBow erzeugt der Audio-Controller nur für als gegriffen gemeldete Saiten ein Spulensignal in der gemeldeten Tonhöhe.
- Beim Loslassen wird das Signal der Saite abgeschaltet.
- Der globale `bowMode` ist im Code vorhanden, aber kein aktuelles Bedienelement sendet eine Änderung. Deshalb ist außer Ein/Aus kein bedienbarer MulEBow-Modus dokumentierbar.

## 11. Clock und Synchronisation

### 11.1 Interne Clock

- Tempo: 1 … 250 BPM, Standard 90 BPM.
- Auflösung: 24 PPQN.
- Start/Stop: mittlerer Transporttaster.
- Start, Clock und Stop werden als USB-MIDI-Realtime-Nachrichten ausgegeben.

### 11.2 Externe Clock

1. Shift aktivieren.
2. MulEBow-Schalter einschalten, um `extClk = 1` zu setzen.
3. Clock mit dem mittleren Transporttaster freigeben.
4. USB-MIDI Clock zuführen.

USB-MIDI Start setzt den internen Zähler zurück; USB-MIDI Stop setzt ihn zurück, löscht empfangene Notenanzeigen und sendet All Notes Off. Der externe Start/Stop-Handler setzt `clckOn` jedoch nicht selbst. Die lokale Transportfreigabe bleibt daher erforderlich.

## 12. MIDI-Funktionen

### 12.1 Ausgehend über USB-MIDI

- manuelle Griffbrettnoten auf dem pro Spur gewählten Kanal,
- Sequencer-Noten der Instanzen 2 und 3,
- Instanz 1 als Saitensequencer-/Arpeggiatorsteuerung,
- Sequencer-CC 2, 3 und 4,
- Fader-CC 3 … 6 auf Kanal 1,
- Joystick-CC 9/10 und Aftertouch auf Kanal 1,
- Poti-CC 7 auf Kanal 1,
- Program Change beim Laden der Songs 1 … 99: Programmnummer = Songslot − 1,
- interne Start/Clock/Stop-Nachrichten.

Der Hardware-MIDI-Port wird initialisiert, aber im Hauptloop nicht gelesen und die Sendefunktionen verwenden USB-MIDI. Eine aktuelle Bedienfunktion für DIN-/Hardware-MIDI lässt sich daher nicht bestätigen.

### 12.2 Eingehend über USB-MIDI

- Note On/Off wird für die LED-Anzeige gespeichert.
- Note On auf Kanal 1 … 6 triggert zusätzlich Audiohüllkurve und Kickup der zugeordneten Saite. Die Notenhöhe spielt dabei keine Rolle.
- Clock, Start und Stop steuern bei aktiviertem externem Clockmodus den Taktablauf.
- CC 10 wählt den Grundton modulo 12.
- CC 11 wählt Skala (`Wert modulo 16`, sofern gültig) und Skalenstufe (`Wert / 16`).
- CC 12 wählt die Skalenstufe modulo Anzahl der Skalenstufen.
- CC 13 steuert den LFO-Tonhöhenoffset.

Auch hier gilt: CC 11/12 zusammen mit Skala `off` kann wegen der Stufenanzahl 0 zu ungültigen Berechnungen führen.

## 13. Songs und SD-Karte

### 13.1 Gespeicherte Daten

Ein Song enthält nach aktuellem Dateiformat:

- Grundton, Skala, Skalenstufe, Farbmodus, BPM und LED-Quelle,
- alle drei Sequencer-Instanzen mit sechs Patterns, Spurparametern, Steps, Stepwerten und Skalenquantisierung,
- Stimmung und Saitenpegel,
- alle sechs Arpeggiator-Patterns einschließlich globaler Funktionswahl und Saitenparameter.

Die Dateien heißen `SONG00.ECS` bis `SONG99.ECS`. Beim Schreiben wird zunächst eine temporäre Datei erstellt, wieder eingelesen und per CRC geprüft, bevor sie den eigentlichen Slot ersetzt.

### 13.2 Speichertaster

Der eingebettete Taster nahe der Potis reagiert beim Drücken:

| Display-Taster | Shift | Aktion |
| --- | --- | --- |
| nicht gedrückt | aus | ausgewählten Songslot laden |
| nicht gedrückt | ein | aktuellen Zustand in **Slot 0** speichern |
| gedrückt | aus | **Slot 0** laden |
| gedrückt | ein | Standard-Song im Arbeitsspeicher herstellen, noch nicht speichern |

Das Speichern des aktuell ausgewählten Slots erfolgt dagegen automatisch, wenn im OLED das Songfeld **ohne Shift** weitergedreht wird.

### 13.3 Fehlerfall und Standard-Song

Fehlt eine Songdatei, ist sie beschädigt, hat eine falsche Version/Größe/Prüfsumme oder enthält ungültige Werte, lädt die Firmware stattdessen den Standard-Song. Der Fehlerstatus ist nur über die Debug-Schnittstelle sichtbar, nicht über das OLED.

Der Standard-Song setzt unter anderem 90 BPM, C-Dur, farbige Skala, Sequencerlänge 16, Zeitteilung `/16`, leere Steps und unverstummte Arpeggiatorspuren.

## 14. OLED- und LED-Anzeigen lesen

- Das OLED zeigt jeweils nur die kontextabhängigen Parameter; eine permanente Statuszeile für Clock, Kickup, MulEBow oder externe Clock existiert nicht.
- Ein **gefüllter** abgerundeter Rahmen markiert in Sequencer/Arpeggiator die aktuell veränderbare Zeile; ein **leerer** Rahmen markiert die Zeile, während der Display-Taster zur Funktionsauswahl gedrückt ist.
- Im Step-Edit zeigt die rechte OLED-Hälfte Tonname/Oktave, Velocity, drei CC-Werte und `sclQ` des zuletzt ausgewählten Steps.
- Im Sequencerraster markieren schwache Farben das Raster, Tonfarben gesetzte Steps, Grau Kanal 0, gedämpfte Farben stummgeschaltete Spuren und eine helle Überlagerung die aktuelle Cursorposition.
- Die LED-Anzeige blendet abklingend aus, statt jeden Pixel sofort hart abzuschalten.

## 15. Bekannte derzeitige Grenzen

Folgende Punkte sind direkt aus dem aktiven Code erkennbar und sollten bei der Bedienung berücksichtigt werden:

1. String-Arpeggiator-Modus 1 als eigener `opMode` zeichnet derzeit weder OLED noch LEDs; praktisch wird der Arpeggiator über Pattern-Schalter 1 innerhalb von Sequencer-Instanz 1 bedient.
2. Arpeggiator-`randomise` ist nicht implementiert.
3. Generisches `rnd` löscht nach aktueller Implementierung die Spur, statt ein Zufallsmuster zu erzeugen.
4. Die Skala `off` ist mit Step-/Stufenoperationen nicht robust kombinierbar.
5. Die drei Pattern-Drehschalter teilen sich Wertebereiche mit Sonderfunktionen (Arpeggiator, Instanzsync, String Setup). Vor Live-Betrieb die physischen Rastpositionen markieren.
6. Songwechsel und Patternkopieren können ohne Bestätigung Daten überschreiben.
7. Die Firmware initialisiert die SD-Karte vor weiteren Hauptcontroller-Initialisierungen und beendet `setup()` bei Fehlschlag vorzeitig. Ohne funktionsfähige SD-Karte ist daher kein normaler Betrieb zu erwarten.
8. Die physische Saitenreihenfolge, Steckerbelegung und Reglerbeschriftung müssen am konkreten Aufbau verifiziert werden.

## 16. Empfohlener erster Funktionstest

1. Aktorversorgung zunächst ausgeschaltet lassen; Audiopegel niedrig einstellen.
2. SD-Karte einsetzen und Instrument einschalten. Prüfen, ob OLED und LED-Griffbrett reagieren.
3. String Setup über Pattern-Schalter 2 oder 3, Stellung 6, öffnen; Saitenstimmung und Pegel kontrollieren.
4. In die Skalen-/Spielansicht wechseln und Grundton, Skala und LED-Quelle prüfen.
5. Jede Saite an mehreren Bünden greifen und USB-MIDI sowie LED-Reaktion beobachten.
6. Interne Clock starten, ein leeres Testpattern auswählen und einzelne Steps programmieren.
7. Erst danach Kickup mit strombegrenzter Aktorversorgung testen.
8. MulEBow zuletzt bei kurzer Einschaltdauer und Überwachung der Spulentemperatur testen.
9. Vor Songwechseln die SD-Karte sichern und die ungewöhnlichen Speicher-/Ladekombinationen aus Abschnitt 13.2 beachten.
