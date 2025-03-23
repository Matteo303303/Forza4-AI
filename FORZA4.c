#include <stdio.h>
#include <stdlib.h>

#define SKILL 10
#define ROW 6
#define COLUMN 7
 
int board[ROW*COLUMN]; //grandezza tavola
long depth,skill;	//depth=profondit nell' albero delle mosse--skill=abilit CPU
long nodes; //posizioni viste nell' albero delle possibilit

 
void display_board()
{
    int i,j;
    printf("\n"); //senza, campo squilibrato
    for(j=0;j<ROW;j++)
    {
        for(i=0;i<COLUMN;i++)
        {
            if(board[i+j*COLUMN]==1) printf("X "); //se gioca utente stampa X
            if(board[i+j*COLUMN]==-1) printf("O ");	//se gioca CPU stampa O
            if(board[i+j*COLUMN]==0) printf(". ");	//se non gioca nessuno aka normalmente il campo
        }
        printf("\n");
    }
//parte inferiore tavola
    for(i=0;i<(COLUMN*2);i++) printf("-");
    printf("\n");
    for(i=0;i<COLUMN;i++) printf("%d ",i+1);
    printf("\n");
}
 //controllo vittoria
int checkwin(int player,int column,int lenght)	
{
	//controllo orizzontale
    int i,j,r,l,height;
    i = column;
    j = ROW-1;
    lenght--;		
    while(board[i+j*COLUMN]!=0) j--;
    j++;
    height = j;
 //c. verticale
    r = 0;
    l = 0;
    while(((++i)<COLUMN)&&(board[i+j*COLUMN]==player)) r++;
   	i = column;
    while(((--i)>=0)&&(board[i+j*COLUMN]==player)) l++;
    if ((r+l)>=lenght) return 1;
   	i = column;
 
    r = 0;
    while(((++j)<ROW)&&(board[i+j*COLUMN]==player)) r++;
    if (r>=lenght) return 1;
    j = height;
 
    r = 0;		//controlli obliqui
    l = 0;
    while(((++i)<COLUMN)&&((++j)<ROW)&&(board[i+j*COLUMN]==player)) r++;
    i = column;
    j = height;
    while(((--i)>=0)&&((--j)>=0)&&(board[i+j*COLUMN]==player)) l++;
    if ((r+l)>=lenght) return 1;
    i = column;
    j = height;
 
    r = 0;
    l = 0;
    while(((++i)<COLUMN)&&((--j)>=0)&&(board[i+j*COLUMN]==player)) r++;
    i = column;
    j = height;
    while(((--i)>=0)&&((++j)<ROW)&&(board[i+j*COLUMN]==player)) l++;
    if ((r+l)>=lenght) return 1;
 
    return 0;
}
 //funzione che sceglie la mossa del CPU
int goodness(int player,int depth,int column,int trigger)
{
    long max,i,value,j;
    max = -200;
    if (checkwin(-player,column,4)) return -128;
    if (depth==0) return 0;
    for(i=0;i<COLUMN;i++)
    {
        if(board[i]==0)
        {
            j = ROW-1;
            while(board[i+j*COLUMN]!=0) j--;
            board[i+j*COLUMN] = player;
            nodes++;
            value = -goodness(-player,depth-1,i,-max)/2;
            board[i+j*COLUMN] = 0;
            if (value>max) max = value;
            if (value>trigger) return max;
        }
    }
    return max;
}
 	//funzione che calcola la mossa migliore per il CPU
int best_move(int player)
{
    long i,j,max,value,best,same,trigger,old,att;
    long res[COLUMN];
    max = -100;
    best = -1;
    for(i=0;i<COLUMN;i++)
    {
        if(board[i]==0)
        {
            nodes = 0;
            j = ROW-1;
            while((board[i+j*COLUMN]!=0)&&(j>=0)) j--;
            board[i+j*COLUMN] = player;
            value = -goodness(-player,skill,i,200);
            printf("\nmossa %d debolezza: %d   posizione nell' albero per questa mossa: %d posti",i+1,value,nodes);
            res[i] = value;
            board[i+j*COLUMN] = 0;	//prende la mossa che ha analizzato pi� nodi dell' albero
            if (value>max)
            {
                max = value;
                best = i;
            }
        }
    }
    if(best==-1)
    {
        for(i=0;i<COLUMN;i++) if(board[i]==0) return i;
    }
 
    return best;
}
int main()
{
	
    int move,j,i,coins;
    coins = ROW*COLUMN;
    skill = SKILL-3;
    
    printf("***BENVENUTO NEL FORZAQUATTRO***");
    printf("\n\n\nREGOLE DEL GIOCO:\n\n\n1)Scegli una colonna tra 1-7, per inserire una moneta.\n\n2)Le monete possono essere inserite una sopra l' altra.\n\n3)Il tuo obiettivo e' quello di allineare 4 monete prima del tuo avversario (in orizzontale, in verticale o in obliquo) cercando di evitare che il tuo avversario riesca a fare lo stesso, prima di te! ");
    printf("\n\n\nBUONA FORTUNA!!!\n\n\n ");
    
    display_board();
 
    while(coins!=0) //finch� ci sono posti nella tavola da gioco
    {
        if (coins==42) skill=SKILL-2; //CPU inserisce monete pi� velocemente
        if (coins==36) skill=SKILL-1; //incrementa la skill...
        if (coins==34) skill=SKILL;   //...al massimo
		
        do
        {
			printf("\n\nSCEGLI UNA COLONNA TRA: [1-%d]...",COLUMN);	//interfaccia utente
			scanf("%d",&move);
			fflush(stdin); //pulisce il buffer, in caso di spazi fra numeri o lettere legger� soltanto il primo valore inserito
			printf("\nHai scelto la colonna: %d",move);	
          	 if (board[move-1]!=0) printf("\n\nLA COLONNA E' PIENA SCEGLI UN ALTRA COLONNA!!\n\n");	//messaggi di errore
          	 if (move<1||move>7) printf("\n\nHO DETTO TRA 1 E 7 !!!\nRIPROVA");    
        }
        
		while(board[move-1]!=0||move>7||move<1);	//permette la riesecuzione nei casi di errore
		
		move--;
        j=ROW-1;
        while((board[move+j*COLUMN]!=0)&&(j>=0)) j--;
        board[move+j*COLUMN] = 1;
        coins--;
        display_board();
        
        if(checkwin(1,move,4))	//stampa vincita player
        {
            printf("\nCOMPLIMENTI!!HAI VINTO!!!\n");
            return 1;
        }
        
		move = best_move(-1); //CPU fa miglior mossa
        j = ROW-1;
        while((board[move+j*COLUMN]!=0)&&(j>=0)) j--;
        board[move+j*COLUMN] = -1;
        display_board();
        printf("\nCPU muove in %d",move+1);
        coins--;
        if(checkwin(-1,move,4)) //stampa vincita CPU
        {
            display_board();
            printf("\nCPU vince :( \n");
            return 1;
        }
        printf("\n");
 
    }
    printf("\n");
    return 0;
}
