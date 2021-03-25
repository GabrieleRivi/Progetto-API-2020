# Progetto-API-2020
## Presentazione del progetto: edU o ed con Undo multipli
Il progetto consiste nell’implementare un semplice editor di testi. L’editor considera un documento come una sequenza di righe, di dimensione arbitraria, numerate a partire da uno. L’interfaccia dell’editor è costituita da comandi testuali, terminati da un carattere di “a capo”. I comandi possono essere seguiti da una porzione di
testo, costituita da una o più righe, terminata da un carattere . (punto) che compare come unico carattere sulla riga successiva. I comandi sono costituiti da un’unica lettera, preceduta opzionalmente da uno o due numeri interi.

_Esempio comandi:_
* (ind1,ind2)c: Cambia il testo presente alle righe comprese tra ind1 ed ind2 (estremi inclusi);
* (ind1,ind2)d: Cancella le righe comprese tra ind1 ed ind2 (estremi inclusi);
* (ind1,ind2)p: Stampa le righe tra ind1 ed ind2, inclusi;
* (numero)u: Effettua l’annullamento (undo) di un numero di comandi (c o d) pari a quello specificato tra parentesi tonde;
* (numero)r: Annulla l’effetto di undo per un numero di comandi pari a numero a partire dalla versione corrente (funzione di redo);


## Esempio 

 1,2c  
 prima riga  
 seconda riga  
 .  
2,3c  
nuova seconda riga  
terza riga  
.  
1,3p  
1,1c  
nuova prima riga  
.  
1,2p  
2,2d  
4,5p  
1,3p  
4,5d  
1,4p  
3u  
1,6p  
1r  
1,3p  
q  

La precedente sequenza di comandi dà origine al seguente output da parte
dell’editor:  
prima riga  
nuova seconda riga  
terza riga  
nuova prima riga  
nuova seconda riga  
.  
.  
nuova prima riga  
terza riga  
.  
nuova prima riga  
terza riga  
.  
.  
prima riga  
nuova seconda riga  
terza riga  
.  
.  
.  
nuova prima riga  
nuova seconda riga  
terza riga  

## Strategia d'implementazione
Il programma è composto principalmente da tre componenti, tutti liste concatenate:
* Testo (lista principale del programma)
* Indietro (lista di supporto per tenere traccia degli Undo)
* Avanti (lista di supporto per tenere traccia dei Redo)
Ogni funzione implementata codifica una delle funzionalità espresse dai comandi c,d,p,u ed r.

Una volta preso l'input da terminale, il testo viene composto da nodi della lista principale che rappresentano ognuno una stringa. Ogni volta che si esegue un comando semplice di tipo c o d, viene istanziato anche un nodo della lista Indietro affinchè sia possibile risalire cronologicamente a quel comando ed eseguirne l'Undo, se ciò venisse richiesto. Una volta eseguito l'undo di un particolare comando, il nodo di riferimento a quel comando passa dalla lista Indietro a quella Avanti, se poi fosse richiesto di eseguirne il Redo. 
