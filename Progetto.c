///////////////////////////////////////////////////////////////////////////////////////////////
// AUTHOR: Gabriele Rivi
// PROJECT:         Prova Finale di Algoritmi e Principi dell'Informatica.
//                  Ed o EdU con possibilità di Undo e Redo multipli;
///////////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 1024
typedef struct nodo{ //node for the main list
    char* riga;                       
    struct nodo* next;
}nodo;
typedef nodo* lista;

typedef struct nodo2{ //node for the undo/redo lists
    char comando;
    int numero;
    int cont;
    lista previous; //pointer used to remember the node exactly before the start of the change
    lista saving; //pointer used to save the nodes i am changing in this moment
    lista last; //pointer used for delete functions to remember the last node i delete from the main list
    lista trl; //pointer used to save the tail of the main list in the exact moment of the change
    struct nodo2* next;
}nodo2;
typedef nodo2* suppU, suppR;

lista testo; //head to the list
lista trailer;  //tail to the list
suppU indietro, avanti; //lists for undo and redo
int contatore =0, pulizia = 0;
int totComandi=0, totRedo = 0, totUndo = 0;



void Inserimento (int start, int finish,lista coda, suppU indietro){ //Insert/Change nodes at the end of the list
    
    int numero=finish-start+1;
    char punto[2], stringa[N];
    lista precedente=coda;

    for(int i=0;i<numero;i++){
        lista new = (lista) malloc(sizeof(nodo));
        gets(stringa);
        new->riga= (char*) malloc(strlen(stringa)+1);
        strcpy(new->riga,stringa);
        if(coda != NULL && i == 0)
            coda->next = new;
        if(testo == trailer){
            testo = new;
            precedente = testo;
        }
        else
        {
            precedente->next = new;
            precedente = precedente->next;
        }
        
        if(i == numero-1){
            trailer = new;
            new -> next = NULL;
        }       
    }
    gets(punto);
    contatore = finish;
}

void Modifica(int start, int finish, suppU indietro){ //Insert/Change nodes in a given position of the list
    
    lista punt=testo, new;
    int numero=finish-start+1;
    char punto[2], stringa[N];
    
    for(int k=1; k<start && punt->next != NULL; k++){
        indietro->previous = punt;
        punt=punt->next;
    }
    new = (lista) malloc(sizeof(nodo));
    
    for(int i=0; i<numero; i++){
        if(start +i >contatore){
            Inserimento(start +i, finish, new, indietro);
            return;
        }
        else{
            if(i == 0){
                indietro->saving = punt;
            }
            gets(stringa);
            new->riga= (char*) malloc(strlen(stringa)+1);
            strcpy(new->riga,stringa);
            if(i== 0 && indietro->previous != NULL)
                indietro->previous->next = new;
            else
                if(i== 0 && start == 1){
                    testo = new;
                }
            if(i == numero-1){
                trailer = new;
                new->next = punt->next;
                gets(punto);
                return;
            }
            if(i<= numero-1 && punt->next != NULL){
                new->next = (lista) malloc(sizeof(nodo));
                new=new->next;
            }
            if(punt->next != NULL)  
                punt = punt->next;
        } 
    }
    gets(punto);
}

void Decisore(int start, int finish,suppU indietro){  //support function to decide which function of CHANGE i should call, based on the start position

    if(start <= contatore){
        Modifica(start, finish, indietro);
    }
    else{
        indietro->previous = trailer;
        Inserimento(start, finish, trailer, indietro);
    }    
}

void Stampa(int start, int finish){ //print function

    lista punt=testo;
    int numero=finish-start+1;

    for(int k=1; k<start && punt != NULL; k++)
        punt = punt->next;

    for(int i=0; i<numero; i++){
        if(start == 0 && finish == 0)
            printf(".\n");
        else{
            if(punt != NULL){
                    printf("%s\n", punt->riga);
                    punt=punt->next;        
            }        
            else
                    printf(".\n");
            }
    }
}

void delete(int start, int finish){     //Delete nodes in a given position of the list
    lista punt= testo;

    if (testo == NULL){                      //empty list case;
        indietro->numero = 0;
        return;
    }
    if(start > contatore){                   //delete of non-existing nodes, valid command but without any logical sense;
        indietro->numero = 0;
        return;
    }

    int k;
    for(k=1; k<start-1 && punt->next != NULL; k++){  //to get to the given position
        indietro->previous = punt;
        punt=punt->next;
    }
    
    lista temp=punt;
    if (start != 1){
        indietro->previous = punt;
        punt=punt->next;
        k++;
    }

    if(finish > contatore)                      //check where i should stop deleting nodes
        indietro->numero = contatore - start +1;
    else
        indietro->numero = finish - start +1;
    
    
    if(start <= contatore && start != 1)      //assign proper values to the undo node, responsible for this delete
        indietro->saving = punt;           
    if (start == 1)
        indietro->saving = testo;
    
    if(start == 1 && finish >= contatore){  //delete algorithm
        testo = NULL;
        trailer = testo;
        contatore = 0;
        return;
    }
    else{
        for(int i=0 ; i< indietro-> numero; i++){
            if(punt != NULL){
                if (start == 1 && i == indietro->numero-1){
                    testo = punt->next;
                    indietro->last = punt;
                }
                if(k+i < finish){                   
                    punt=punt->next;
                }
                else{
                    if(start != 1){
                        temp->next = punt->next;
                        indietro->last = punt;  //save the last node that need to be deleted
                        while(temp->next != NULL)
                            temp = temp->next;
                        trailer = temp;
                    }
                }
            }
            else{   
                indietro->last = temp;
                temp->next = NULL;
                trailer = temp;
                return;
            }
        }
    }
    if(contatore >= indietro->numero)       //update counter, which keeps the count of how many nodes i have on the list
        contatore = contatore - indietro->numero;
    
    if(testo == NULL){
        trailer = testo;
    }
}

void undoChange(){      //undo of a command of Insert/Change, i simply swap the content of the list to what i saved in the undo node responsible for this command
    lista temp; 
    if(indietro->previous == NULL){
        temp = testo;
        testo = indietro->saving;
        indietro->saving = temp;
    }   
    else{
        temp = indietro->previous->next;
        indietro->previous->next = indietro->saving;
        indietro->saving = temp;
    }
    temp = trailer;                 //reinstate the tail of the main list at what it was when i executed the command that i am undoing
    trailer = indietro->trl;
    indietro->trl = temp;

    int t= contatore;               //reinstate the counter of the nodes of the main list at what it was when i executed the command that i am undoing
    contatore = indietro->cont;
    indietro->cont = t; 
}

void redoChange(){  //redo of a command of Insert/Change i simply swap the content of the list to what i saved in the redo node responsible for this command
    lista temp;
    if(avanti->previous == NULL){
        temp = testo;
        testo = avanti->saving;
        avanti->saving = temp;
    }
    else{
        temp = avanti->previous->next;
        avanti->previous->next = avanti->saving;
        avanti->saving = temp;
    }
    temp = trailer;                 //reinstate the tail of the main list at what it was when i executed the command that i am redoing
    trailer = avanti->trl;
    avanti->trl = temp;

    int t= contatore;               //reinstate the counter of the nodes of the main list at what it was when i executed the command that i am redoing
    contatore = avanti->cont;
    avanti->cont = t;
    
}

void undoDelete(){  //undo of a command of Delete, i reinstate the nodes that i cut from the main list that i saved on the undo node
    lista punt;
    if(indietro->saving == NULL)            //cases without sense
        return;

    if(indietro->previous != NULL){
        indietro->previous->next = indietro ->saving;
    }
    else{
        testo = (lista) malloc(sizeof(nodo));       //case of delete on the head of the list
        testo = indietro->saving;
    }
    punt = trailer;                 //reinstate the tail of the main list at what it was when i executed the command that i am undoing
    trailer = indietro->trl;
    indietro->trl = punt;

    int t=contatore;                //reinstate the counter of the nodes of the main list at what it was when i executed the command that i am undoing
    contatore = indietro->cont;
    indietro->cont = t;
}   

void redoDelete(){      //redo of a command of Delete, so i cut back the node i reinstated with an undoDelete
    lista punt,temp;                
    if(avanti->numero != 0){
        if(avanti->previous != NULL){
            if(avanti->last != NULL)
                avanti->previous->next = avanti->last->next;
            else
                avanti->previous->next = NULL;
        }
        else{
            if(avanti->last != NULL){
                testo =(lista)malloc (sizeof(nodo));
                testo = avanti->last->next;
            }
            else
            {
                testo = (lista)malloc(sizeof(nodo));
                testo = NULL;
            }
            
        }
    }
    else
    {   
       return;
    }
    temp = trailer;             //reinstate the tail of the main list at what it was when i executed the command that i am redoing
    trailer = avanti->trl;
    avanti->trl = temp;

    int t= contatore;           //reinstate the counter of the nodes of the main list at what it was when i executed the command that i am redoing
    contatore = avanti->cont;
    avanti->cont = t;   
}

void decideUndo(int start){         //support function to decide which undo command is needed to be done and that the number of given 'u' is right
    for(int i=0;indietro != NULL && i <start  ; i++){
        if(indietro->comando == 'd')
            undoDelete();
        if(indietro->comando == 'c')
            undoChange();
        suppU temp = indietro;     //the undo node i just reinstated is being shifted to the redo list, to keep track of which undo has been done
        indietro = indietro->next;
        temp->next = avanti;
        avanti = temp;
    }
}
void decideRedo(int start){         //support function to decide which redo command is needed to be done and that the number of given 'r' is right
    for(int i=0;i < start && avanti != NULL; i++){
        if(avanti->comando == 'd')
            redoDelete();
        if(avanti->comando == 'c')
            redoChange();
        suppU temp = avanti;        //the redo node i just executed is being shifted to the undo list, to keep track of which redo has been done
        avanti = avanti->next;
        temp->next = indietro;
        indietro = temp;
    }
}

void getInput(int* start, int* finish, char* command){ //getInput function
    
    char comando[N];
    gets(comando);
    *command= comando[strlen(comando) - 1];
    comando[strlen(comando)-1]= '\0';

    if(*command == 'q'){
        return;
    }
    *start=0;
    *finish=0;
    int i=0;
    while(comando[i]>47 && comando[i]<58){
        *start= *start*10 + ((int)comando[i]) - 48;
        i++;
    }
    if(*command == 'r' || *command== 'u')
        return;
    i++;

    while(comando[i]>47 && comando[i]<58){
        *finish= *finish*10 + ((int)comando[i]) - 48;
        i++;
    }
    return;   
}

int main(){
    testo = NULL;                                   //initialization of main lists
    trailer = testo;                                //tail of the list
    indietro=NULL;                                  //initialization of support lists
    avanti = NULL;

    int start, finish;
    char command;
    getInput(&start, &finish, &command);
    
    while(command != 'q'){                                  //termination character 
                                                            //switch case made with if to determine which command has to be done
        if(command == 'c'){                                 //case of a Change command
            if(totComandi != 0){
                if(totComandi > 0){
                    decideUndo(totComandi);
                    totComandi = 0;
                }
                else{
                    decideRedo(abs(totComandi));
                    totComandi = 0;
                }
            }
            suppU new = (suppU) malloc(sizeof(nodo2));       //creation of the nodes for the undo list; every time i do a new command, i create an undo node to keep track of that command
            new->previous = NULL;
            new->next = NULL;
            new->comando = 'c';
            new->numero = finish - start +1;
            new->saving= NULL;
            new->trl = trailer;
            new->cont = contatore;
            new->last = NULL;
            if(indietro != NULL){
                new->next = indietro;
            }
            indietro = new;
            totUndo++;
        
            if(pulizia == 1){                               //after a series of undo/redo, if i give a proper change or delete command i clear the redo list
                while(avanti != NULL){
                    suppU temp = avanti;
                    avanti = avanti->next;
                    free(temp);
                }
                totRedo = 0;
                pulizia = 0;
            }
            Decisore(start, finish, indietro);
        }

        if(command == 'p'){                                 //case of a Print command
            if(totComandi != 0){
                if(totComandi > 0){
                    decideUndo(totComandi);
                    totComandi = 0;
                }
                else{
                    decideRedo(abs(totComandi));
                    totComandi = 0;
                }
            }
            Stampa(start, finish);
        }

        if(command == 'd'){                                 //case of a Delete command
            if(totComandi != 0){
                if(totComandi > 0){
                    decideUndo(totComandi);
                    totComandi = 0;
                }
                else{
                    decideRedo(abs(totComandi));
                    totComandi = 0;
                }
            }
            suppU new = (suppU) malloc(sizeof(nodo2));        //creation of the nodes for the undo list; every time i do a new command, i create an undo node to keep track of that command
            new->previous = NULL;
            new->next = NULL;
            new->comando = 'd';
            new->numero = finish - start +1;
            new->saving= NULL;
            new->trl = trailer;
            new->cont = contatore;
            new->last = NULL;
            if(indietro != NULL){
                new->next = indietro;
            }
            indietro = new;
            totUndo++;
            
            if(pulizia == 1){                                  //after a series of undo/redo, if i give a proper change or delete command i clear the redo list
                while(avanti != NULL){
                    suppU temp = avanti;
                    avanti = avanti->next;
                    free(temp);
            }
            totRedo = 0;
            pulizia = 0;
            }
            delete(start, finish);
        }
        if(command == 'u'){                                    //case of an Undo command
            if(totUndo != 0){
                if(start >= totUndo){
                    totComandi += totUndo;
                    totRedo += totUndo;
                    totUndo = 0;
                }
                else{
                    totComandi += start;
                    totRedo += start;
                    totUndo -= start;
                }
            }
            pulizia = 1;
        }
        if(command == 'r'){                                    //case of a Redo command
            if(totRedo != 0){
                if(start >= totRedo){
                    totComandi -= totRedo;
                    totUndo += totRedo;
                    totRedo = 0;
                }
                else{
                    totComandi -= start;
                    totUndo += start;
                    totRedo -= start;
                }
            }
            pulizia = 1;
        }
        getInput(&start, &finish, &command);
    }

   while(testo != NULL) {                                   //free functions to clear the memory occupied by the lists at the end of the execution
        lista tmp = testo;
        testo = testo->next;
        free(tmp->riga);
        free(tmp);
    }

    while(indietro != NULL){
        suppU temp = indietro;
        indietro = indietro->next;
        free(temp);
    }
    while(avanti != NULL){
        suppU temp = avanti;
        avanti = avanti->next;
        free(temp);
    }
     return 0;
}