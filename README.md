# Sandborn

## 🎮 Overview

**Sandborn** è un platformer 3D single-player ambientato in un mondo ispirato all'Antico Egitto.

Il giocatore controlla un personaggio che deve esplorare un ambiente desertico caratterizzato da rovine, piattaforme e percorsi da sbloccare.

Durante l'esplorazione deve superare ostacoli ambientali, attivare meccanismi, raccogliere chiavi e affrontare nemici che ostacolano la progressione.

Il gameplay combina elementi di **platforming, esplorazione, combattimento e risoluzione di enigmi ambientali**.

---

## 🎯 Project Goals

**Sandborn** è stato sviluppato durante il **Master in Computer Game Development** come progetto dedicato al modulo di **Game Engine Programming**, utilizzando **Unreal Engine 5.8** e **C++**.

L'obiettivo principale era approfondire lo sviluppo di sistemi di gameplay, concentrandosi sull'interazione tra Actor, componenti, eventi e logiche di gioco.

Per questo motivo lo sviluppo si è concentrato soprattutto su:

- Gameplay Programming in C++;
- Actor e Actor Components;
- Collisioni e Overlap Events;
- Event Dispatching attraverso Delegates;
- Pressure Plates e Moving Platforms;
- Sistemi di raccolta e gestione delle chiavi;
- Collectables e gestione della salute;
- Character Movement e Double Jump;
- Enemy AI e Navigation System;
- StateTree per la gestione dei comportamenti dei nemici;
- Animazioni e Animation Notifies;
- Combattimento e gestione della morte;
- Spawn dinamico di Actor;
- Sistemi di salvataggio e progressione.

Aspetti come **grafica, audio e level design** sono stati utilizzati principalmente per costruire un ambiente nel quale sperimentare e integrare le funzionalità tecniche sviluppate.

L'obiettivo era creare sistemi capaci di comunicare tra loro e contribuire alla progressione del gameplay, evitando di realizzare solamente meccaniche indipendenti.

---

## 🎮 Controls

### Player

| Input | Azione |
|---|---|
| `W A S D` | Movimento |
| `Mouse` | Movimento della visuale |
| `Space` | Salto |
| `Space x2` | Double Jump |

### Gameplay Interactions

Le principali interazioni avvengono automaticamente attraverso collisioni e Overlap Events.

- Le monete e le chiavi vengono raccolte entrando in contatto con esse.
- Le Pressure Plates vengono attivate quando un Actor valido entra nella loro area.
- I nemici possono essere colpiti attraverso uno Stomp, saltando sopra di loro.

---

## 🕹️ Gameplay

Il gameplay di **Sandborn** si basa sull'esplorazione, sul platforming e sulla risoluzione di enigmi ambientali.

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

Oltre ai nemici standard, il livello include un **Boss** caratterizzato da più punti salute e da comportamenti differenti in base alla salute rimanente.

Durante lo scontro il Boss può modificare il proprio comportamento, aumentando la pressione sul giocatore nelle fasi più avanzate del combattimento.

### Save and Continue

Il gioco permette di salvare la progressione e riprendere la partita dal Main Menu attraverso l'opzione **Continue**.

Il sistema conserva lo stato del giocatore e diversi elementi della progressione del livello, permettendo di continuare la partita senza dover ricominciare dall'inizio.

### 🏆 Victory

Il progetto prevede un sistema dedicato alla gestione della vittoria, utilizzato per riconoscere il completamento dell'obiettivo finale e concludere la partita.

Una volta raggiunto l'obiettivo conclusivo viene mostrata una schermata dedicata alla vittoria.

### ❌ Defeat

Il giocatore possiede un sistema di salute che viene ridotta quando subisce danni.

Quando la salute raggiunge zero, viene avviata la sequenza di morte del Character.

Il movimento viene disabilitato, viene attivato il Ragdoll e, dopo un breve intervallo, viene mostrata la schermata di **Game Over**.

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
- StateTree;
- Animation Montages e Notifies;
- Spawn dinamico di Actor;
- SaveGame System.

L'implementazione in C++ ha permesso di approfondire il funzionamento di Unreal Engine, mantenendo la possibilità di configurare diversi parametri attraverso l'Editor.

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
- Comunicazione tra oggetti interattivi e sistemi di gameplay;
- Aggiornamento di elementi legati alla progressione del giocatore.

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

## 📸 Video & Media

🎥 **Gameplay Video:**  
https://youtu.be/dwBMpM6Aox8
