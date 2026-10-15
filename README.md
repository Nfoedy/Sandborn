# Sandborn

## 🌆 Mood and Game Intent

**Sandborn** è un platformer 3D single-player ambientato in un mondo ispirato all'Antico Egitto.

Il gioco segue un personaggio che emerge dalla sabbia e si ritrova all'interno di un ambiente desertico caratterizzato da rovine, strutture antiche e percorsi da esplorare.

L'obiettivo è trasmettere una sensazione di avventura e scoperta, utilizzando il level design e le meccaniche di gameplay per accompagnare il giocatore attraverso ostacoli, enigmi ambientali e incontri con i nemici.

L'ambientazione egizia rappresenta il contesto nel quale sono state integrate le diverse funzionalità sviluppate durante il progetto.

---

## 🎯 Project Goals

**Sandborn** è stato sviluppato durante il **Master in Computer Game Development** come progetto dedicato al modulo di **Game Engine Programming**, utilizzando **Unreal Engine 5.8** e **C++**.

L'obiettivo principale era approfondire lo sviluppo di sistemi di gameplay, concentrandosi sull'interazione tra Actor, componenti, eventi e logiche di gioco.

Per questo motivo lo sviluppo si è concentrato soprattutto su:

- Gameplay Programming in C++;
- Actor e Actor Components;
- Collisioni e Overlap Events;
- Event Dispatching attraverso Delegates;
- Pressure Plates;
- Moving Platforms;
- Sistemi di raccolta e gestione delle chiavi;
- Collectables e gestione della salute;
- Character Movement e Double Jump;
- Enemy AI e Navigation System;
- Animazioni e Animation Notifies;
- Combattimento e gestione della morte;
- Sistemi di salvataggio e progressione.

Aspetti come **grafica, audio e level design** sono stati utilizzati principalmente per costruire un ambiente nel quale sperimentare e integrare le funzionalità tecniche sviluppate.

L'obiettivo non era semplicemente implementare meccaniche indipendenti, ma creare sistemi capaci di comunicare tra loro e contribuire alla progressione del gameplay.

---

## 🎮 Overview

In **Sandborn** il giocatore controlla un personaggio che deve esplorare un livello ambientato nell'Antico Egitto, superando ostacoli ambientali e affrontando diversi nemici.

Durante l'esplorazione è necessario utilizzare piattaforme mobili, attivare meccanismi e raccogliere chiavi per sbloccare nuovi percorsi.

Il giocatore può inoltre raccogliere monete, recuperare salute e utilizzare il doppio salto per raggiungere piattaforme e zone sopraelevate.

Alcuni nemici ostacolano la progressione e possono essere eliminati saltando sopra di loro.

Le diverse meccaniche sono state progettate per interagire tra loro, rendendo l'esplorazione e il completamento degli obiettivi parte dello stesso gameplay loop.

---

## 🎮 Controls

### Player

| Input | Azioni |
|---|---|
| `W A S D` | Movimento |
| `Mouse` | Movimento della visuale |
| `Space` | Salto |
| `Space x2` | Double Jump |

### Gameplay Interactions

Le principali interazioni avvengono automaticamente attraverso le collisioni e gli Overlap Events.

- Le monete vengono raccolte entrando in contatto con esse.
- Le chiavi vengono raccolte automaticamente dal personaggio.
- Le Pressure Plates vengono attivate quando un Actor valido entra nella loro area.
- I nemici possono essere colpiti attraverso uno Stomp, saltando sopra di loro.

---

## 🕹️ Gameplay

Il gameplay di **Sandborn** si basa sull'esplorazione, sul platforming e sulla risoluzione di semplici enigmi ambientali.

Il giocatore deve attraversare il livello utilizzando le meccaniche di movimento e interagendo con diversi sistemi.

### Exploration and Platforming

Il percorso presenta piattaforme, ostacoli e meccanismi che richiedono l'utilizzo del salto e del Double Jump.

Alcune piattaforme si muovono automaticamente, mentre altre devono essere attivate attraverso specifiche Pressure Plates.

### Keys and Progression

Durante l'esplorazione il giocatore deve recuperare alcune chiavi necessarie per sbloccare una porta.

Le chiavi raccolte vengono visualizzate all'interno di specifici Key Holders.

Quando tutte le chiavi richieste sono state raccolte, il sistema permette l'apertura della porta e la prosecuzione del livello.

### Combat

Il giocatore incontra nemici che possono pattugliare determinate zone e inseguirlo quando viene rilevato.

Il sistema di combattimento si basa principalmente sul movimento del Character e sulla possibilità di eliminare i nemici attraverso lo Stomp.

### 🏆 Victory

Il progetto comprende un sistema dedicato alla gestione della vittoria, utilizzato per riconoscere il completamento dell'obiettivo finale e concludere la partita.

### ❌ Defeat

Il giocatore possiede un sistema di salute che viene ridotta quando subisce danni.

Quando la salute raggiunge zero, viene avviata la sequenza di morte del Character.

Il movimento e il salto vengono disabilitati, viene attivato il Ragdoll e, dopo un breve intervallo, la partita viene messa in pausa.

---

## ✨ Main Features

### Pressure Plate System

Le **Pressure Plates** sono Actor interattivi che rilevano la presenza di altri Actor attraverso gli Overlap Events.

Quando un Actor valido entra nella zona di attivazione, la Pressure Plate comunica il cambiamento del proprio stato ai sistemi collegati.

L'implementazione utilizza i **Delegates** per separare la logica di rilevamento da quella degli oggetti che devono reagire all'attivazione.

Questo permette di utilizzare lo stesso sistema per controllare differenti elementi del livello.

---

### Moving Platform System

Il progetto comprende un sistema di piattaforme mobili sviluppato in C++.

Le piattaforme possono spostarsi tra due punti configurabili del livello.

Il movimento viene gestito attraverso una logica dedicata che permette di controllare il comportamento della piattaforma indipendentemente dal meccanismo che la attiva.

Alcune piattaforme sono collegate alle Pressure Plates e possono essere utilizzate solamente dopo l'attivazione del relativo meccanismo.

---

### Automatic Platforms

Oltre alle piattaforme attivabili, sono presenti piattaforme che si muovono automaticamente.

Questi elementi utilizzano una logica di movimento autonoma, senza richiedere necessariamente l'interazione con una Pressure Plate.

La presenza di entrambe le tipologie permette di costruire situazioni di platforming differenti, alternando movimento continuo e interazioni ambientali.

---

### Collectable Key System

Le chiavi sono Actor raccoglibili sviluppati attraverso la classe **ACollectableKey**.

Ogni chiave possiede una rappresentazione visiva e una logica dedicata alla raccolta.

Il sistema comprende:

- Rotazione dell'oggetto;
- Rilevamento del Character;
- Raccolta tramite Overlap;
- Riproduzione di un effetto audio;
- Comunicazione dell'avvenuta raccolta attraverso un Delegate.

La gestione tramite eventi permette ad altri sistemi di reagire quando una chiave viene raccolta, senza inserire tutte le responsabilità all'interno dello stesso Actor.

---

### Key Holder System

Per rappresentare visivamente le chiavi raccolte è stato realizzato un sistema di **Key Holders**.

Ogni Holder può ricevere e visualizzare una chiave associata.

Quando il giocatore raccoglie una determinata chiave, il sistema aggiorna automaticamente l'Holder corrispondente.

La classe **ACollectableKeyHolder** gestisce la rappresentazione dell'oggetto e il relativo comportamento visivo.

In questo modo il giocatore può osservare direttamente la progressione della raccolta delle chiavi.

---

### Key Door System

Il progetto comprende una porta collegata al sistema delle chiavi.

La porta verifica il completamento della raccolta e viene sbloccata quando tutte le chiavi necessarie risultano disponibili.

Durante lo sviluppo è stata utilizzata una configurazione con **tre chiavi**, ciascuna associata al proprio Holder.

Questo sistema permette di collegare la raccolta degli oggetti alla progressione del livello.

---

### Double Jump

Il Character può eseguire un secondo salto mentre si trova in aria.

La funzionalità utilizza il sistema di movimento di Unreal Engine attraverso la configurazione del numero massimo di salti consentiti.

Il Double Jump amplia le possibilità di movimento e permette al giocatore di raggiungere piattaforme e percorsi che non sarebbero accessibili attraverso un salto normale.

---

### Coin Collection System

Durante l'esplorazione il giocatore può raccogliere monete distribuite nel livello.

Le monete vengono raccolte attraverso un sistema di Overlap e aggiornano il contatore del Character.

Ogni **10 monete raccolte**, il giocatore recupera un punto salute, senza superare il valore massimo consentito.

Questo collega il sistema di Collectables alla sopravvivenza del personaggio, incentivando l'esplorazione.

---

### Health System

Il Character possiede un sistema di salute con un valore massimo configurato a **5 HP**.

La salute viene ridotta quando il giocatore riceve danni dai nemici.

Il sistema gestisce:

- Quantità di salute disponibile;
- Danni ricevuti;
- Recupero della salute;
- Controllo del valore massimo;
- Verifica della morte del Character.

Le interazioni con i Collectables vengono inoltre controllate per evitare che un personaggio morto continui a raccogliere oggetti o modificare la propria salute.

---

### Enemy AI

I nemici utilizzano il sistema di navigazione di Unreal Engine per muoversi all'interno del livello.

La logica comprende comportamenti di pattugliamento e inseguimento del giocatore.

Attraverso la **NavMesh**, i nemici possono individuare percorsi validi all'interno dell'ambiente.

Il sistema distingue differenti situazioni di comportamento, permettendo al nemico di reagire alla presenza del Character.

---

### Spider Enemy

Una delle tipologie di nemico implementate nel progetto è rappresentata da un ragno.

Il ragno possiede una propria logica di movimento, combattimento e gestione della morte.

Durante lo sviluppo sono stati affrontati diversi aspetti tecnici, tra cui:

- Movimento attraverso il Navigation System;
- Allineamento del nemico al terreno;
- Pattugliamento;
- Inseguimento del giocatore;
- Riproduzione delle animazioni di attacco;
- Applicazione dei danni;
- Rilevamento dello Stomp;
- Gestione della morte;
- Rilascio di una chiave.

L'obiettivo era realizzare un nemico capace di interagire con il giocatore e con gli altri sistemi di gameplay, anziché rappresentare solamente un ostacolo statico.

---

### Enemy Attack System

L'attacco del ragno utilizza un'animazione dedicata.

Il danno non viene applicato semplicemente quando il giocatore entra in contatto con il nemico, ma viene collegato a un preciso momento dell'animazione attraverso un **Animation Notify**.

Questo permette di sincronizzare il danno con il movimento di attacco.

Quando l'attacco colpisce correttamente il Character, viene sottratto **1 HP**.

---

### Stomp System

Il giocatore può eliminare un ragno saltando sopra di esso.

Il sistema deve distinguere una collisione valida dall'alto da una normale collisione laterale.

Quando viene riconosciuto uno Stomp valido, viene richiamata la logica di danno del nemico.

Questo introduce una meccanica di combattimento legata direttamente al movimento e al platforming.

---

### Enemy Death and Key Drop

Quando un ragno viene eliminato, viene avviata una sequenza di morte dedicata.

Il sistema gestisce l'animazione e l'interruzione del normale comportamento del nemico.

Alcuni ragni possono inoltre rilasciare una chiave attraverso una classe configurabile.

La chiave può successivamente essere raccolta dal giocatore e utilizzata dal sistema di Key Holders.

Questa funzionalità collega direttamente **combattimento, raccolta degli oggetti e progressione del livello**.

---

### Boss Spawner

Il progetto comprende un sistema dedicato all'attivazione di un Boss attraverso una Pressure Plate.

Quando viene attivato il meccanismo, uno Spawner può generare il Boss associato.

La logica prevede un controllo per evitare che lo stesso Boss venga generato ripetutamente.

Il sistema di attivazione e spawning costituisce la base per la realizzazione dello scontro finale.

---

### Save System

Nel progetto è stato introdotto un sistema di salvataggio utilizzando il **SaveGame System** di Unreal Engine.

Il sistema comprende una funzionalità di **Save and Exit**, pensata per salvare i dati necessari alla continuazione della partita.

Tra i dati gestiti o considerati durante lo sviluppo del sistema sono presenti:

- Posizione del Character;
- Rotazione del Character;
- Salute del giocatore;
- Monete raccolte.

La gestione completa del ripristino dello stato del mondo, compresi nemici e oggetti già raccolti, rappresenta un ulteriore aspetto da approfondire.

---

## 🧠 Game Engine Programming

Una parte importante dello sviluppo di **Sandborn** è stata dedicata alla progettazione dei sistemi di gameplay in C++.

L'obiettivo era comprendere come organizzare le responsabilità delle classi e costruire meccaniche riutilizzabili all'interno del livello.

In particolare, il progetto ha richiesto di lavorare con:

- AActor e relativi componenti;
- Classi C++ esposte all'Editor;
- Collision Components;
- Overlap Events;
- Delegates;
- Character Movement Component;
- AI e Navigation System;
- Animation Montages e Notifies;
- Spawn dinamico di Actor;
- SaveGame System.

L'implementazione delle funzionalità in C++ ha permesso di approfondire il funzionamento di Unreal Engine, mantenendo la possibilità di configurare visivamente diversi parametri attraverso l'Editor.

---

## C++ Gameplay Architecture

Uno degli aspetti centrali del progetto è stata la separazione delle responsabilità tra i diversi sistemi.

Per esempio, nel sistema di raccolta delle chiavi:

- **ACollectableKey** gestisce la chiave e la sua raccolta;
- **ACollectableKeyHolder** gestisce la rappresentazione visiva della chiave;
- Il sistema della porta controlla il completamento delle condizioni richieste.

Allo stesso modo, le Pressure Plates si occupano di rilevare l'attivazione, mentre gli Actor collegati gestiscono il comportamento che deve essere eseguito.

Questo approccio permette di evitare che un singolo Actor debba conoscere e gestire tutte le funzionalità del livello.

---

## Why Delegates?

Uno degli argomenti approfonditi durante lo sviluppo è stato l'utilizzo dei **Delegates** per la comunicazione tra Actor.

Sarebbe stato possibile creare collegamenti diretti tra una Pressure Plate e una specifica piattaforma, oppure tra una chiave e il relativo sistema di progressione.

Tuttavia, questa soluzione avrebbe reso le classi maggiormente dipendenti l'una dall'altra.

Attraverso i Delegates, un Actor può comunicare che un determinato evento si è verificato senza dover necessariamente conoscere tutti i sistemi interessati.

Questo approccio è stato utilizzato soprattutto per:

- Attivazione delle Pressure Plates;
- Raccolta delle chiavi;
- Comunicazione tra oggetti interattivi e sistemi di gameplay.

Il risultato è un'organizzazione più modulare, nella quale differenti Actor possono reagire agli stessi eventi mantenendo separate le proprie responsabilità.

---

## 📌 Final Considerations

Lo sviluppo di **Sandborn** mi ha permesso di approfondire diversi aspetti del **Gameplay Programming in Unreal Engine**, lavorando principalmente attraverso il linguaggio C++.

Una delle parti più importanti del progetto è stata l'integrazione tra sistemi differenti.

Le Pressure Plates e le Moving Platforms mi hanno permesso di approfondire la comunicazione tra Actor e la gestione degli eventi.

Il sistema delle chiavi, degli Holders e delle porte ha rappresentato un esercizio pratico per organizzare meccaniche di progressione attraverso classi con responsabilità differenti.

L'implementazione dei ragni ha invece richiesto di affrontare aspetti più complessi, come la navigazione, il comportamento dei nemici, le animazioni, l'applicazione dei danni e la gestione della morte.

Un altro aspetto importante è stato collegare queste funzionalità tra loro: ad esempio, l'eliminazione di un nemico può portare al rilascio di una chiave, che viene successivamente raccolta e utilizzata per sbloccare un nuovo percorso.

Questo mi ha permesso di comprendere meglio come costruire sistemi che non funzionino solamente in modo indipendente, ma che collaborino per creare un'esperienza di gameplay coerente.

**Sandborn** rappresenta quindi un progetto di apprendimento e sperimentazione, attraverso il quale ho potuto consolidare le mie conoscenze di C++, Unreal Engine e organizzazione dei sistemi di gioco.

---

## 🔮 Future Developments

Il progetto è stato sviluppato principalmente con finalità didattiche, concentrandosi sull'implementazione dei sistemi di gameplay.

Alcuni aspetti possono essere ulteriormente approfonditi e completati.

Tra i possibili sviluppi futuri sono presenti:

- **Clone Mechanic:** creazione di una copia del Character controllata attraverso gli stessi input del giocatore, da utilizzare per risolvere enigmi basati sull'attivazione simultanea di più Pressure Plates;
- **Boss Fight:** completamento di uno scontro finale con un nemico caratterizzato da più punti salute e comportamenti differenti;
- **Enemy AI:** miglioramento dei comportamenti di pattugliamento, inseguimento e combattimento;
- **Enemy Death:** perfezionamento della gestione dei cadaveri e della loro permanenza nel livello;
- **Save System:** estensione del salvataggio allo stato completo del mondo, includendo chiavi raccolte, nemici eliminati e progressione;
- **Level Design:** realizzazione di percorsi più articolati e maggiore varietà di enigmi ambientali;
- **Animations:** miglioramento delle animazioni e delle transizioni del Character e dei nemici;
- **User Interface:** perfezionamento delle schermate e degli elementi visivi di gameplay;
- **Audio and Visual Effects:** introduzione di ulteriori effetti per migliorare il feedback delle interazioni.

Questi sviluppi permetterebbero di ampliare l'esperienza di gioco, mantenendo come obiettivo principale l'approfondimento del Gameplay Programming e delle funzionalità offerte da Unreal Engine.

---

## 📸 Video & Media
