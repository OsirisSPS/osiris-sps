Nomenclatura variabili:

- C++: CamelCase
- Javascript: CamelCase
- CSS: Underscore (es. os_my_style)
- XML: Underscore (es. user_id)
- XSLT: Underscore (es. user_id)
- Python, controlli IDE: Hungarian_notation (es. txtLogin)

- Language Keys: Underscore & dots (es. portal.pages.mypage.account_information)
- Opzioni: Underscore & dots (es. ide.my_option)

http://en.wikipedia.org/wiki/CamelCase
http://en.wikipedia.org/wiki/Hungarian_notation

Tags:

// CLODOURGENT: Under fix by Clodo
// RAZORVERYURGENT: Razor changes
// VERYURGENT: Need to fix, high priority
// URGENT: Need to fix

// FORCOMPATIBILITY: Maybe removed or changed, particulary in Razor.

// TOCLEAN: Need to clean, generally code that can be removed.
// TOTEST: Need to test
// TOFIXURGENT: Need to fix prior the next release
// TOFIXHIGH: Need to fix, high priority
// TOFIX: Need to fix
// TOTRANSLATE: Phrase hard written, need to be translated.
// TODO: Low priority.

// Specific:
// OS_TODOCIP: Intepretation primary changes.
// TOFIX_SQLITE: Code specific to SQLite




// TOFIX_VALIDATING: 
/* Molti controlli sono stati commentati. Ad esempio, se un Text è basato su un modello, non viene più caricato un modello per verificare se l'xml è xsd-validato.
Prima c'era un giro "getDependencies", ma ora da un entity non so qual'è l'oggetto da chiedere (prima chiedeva la primaria).
Oltretutto, prima niente veniva rimosso, ora invece potrei rimuovere il Model, e il Text rimarrebbe presente e neanche controllato.
Soluzioni:
- O faccio un giro complesso di dipendenze/invalidazioni,
- O ignoro semplicemente, e i controlli son fatti in fase di uso/visualizzazione <- attuale.

Molti sono semplicemente da rifare nelle "Acceptable".
*/


// TOCLEAN_DATAENTRY

// TOCLEAN_SNAPSHOT_SCORE - Tentativo di abolire il campo snapshot::score. 
   Stato attuale: fatto, da fixare/sistemare ordinamento "reputazioni" in SearchResultsDatabase (creare join) e SearchResultsLucene (leggere score da snapshot::user), pulire.
   
// TOCLEAN_OBJECTID_PRIMARY_FLAGS - I flag di distinzione primaria/revisione nell'ObjectID non vengono più usati. Possono essere cancellati/puliti.
   Stato attuale: fatto, da pulire.

// TOCLEAN_OBJECTID_OBJECTTYPE - Da studiare.
   Da una prima analisi, ora come ora molte "validate" e "assert" avvengono dirette sull'ID (es. il voto verifica che il padre sia un pool).
   L'alternativa sarebbe che caricano l'oggetto, con il rischio che se l'oggetto non c'è non vengano validati. Potrebbe funzionare.

   Il problema grosso è che nell'allineamento ha solo l'ID, chiama una ObjectsSystem::instance()->createObject per crearlo passandogli il type letto dall'id, poi legge i dati.
   Sistemare questo significherebbe passare, dopo l'ID, anche il tipo a parte.

pulire OS_NOOBJECTID

Todo:

- Serializzare "pov" di IObject, compilare con "submit_date_x":
iportaldatabase
objectsiobject
objectsirevisionable

- diretto in object->sign  faccio che lui valorizza POV?



- Dovrò fare tutto il giro di update del db per il CIP
- Prima verifico che và con il flag di sopravvivenza "accetta no firme", poi fare una procedura (no job, tanto è temporaneo) per il re-sign automagico.


- Quando creo una revisione, deve copiare entity_signature
- Se creo una primaria, devo creare ad-hoc la firma in entity_signature
- Quando creo un oggetto, se la entity non è specificata, usare id
Son tutte cose da mettere in un punto preciso, che sia chiamato da tutti, forse la ->create.

- Tutto il giro di exportAuthor, exportBase, exportXML và ricontrollato e sistemato per il OS_TODOCIP.

- Crea gli snapshotobject se è una primaria... fare un giro completamento diverso.


- Dovrò testare bene che se cancello la primaria, la revisione funziona. Anche con i padri.
  Nella IRevisionable::onCreate ad esempio carica la primaria del parent, da sistemare.


Da verificare:
- Perchè objects::user è derivato da IObject e non da IEntry come tutti gli altri? Se lo fosse, IObject e IEntry potrebbero essere unificate.



TOCLEAN:
- Sarebbe bello rinominare ogni "reference" in "entity_id"