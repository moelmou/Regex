
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define FINAL_STATES 1
#define ALPHABET_CHARCTERS 2



struct node
{
        int st;//etat suivante
        struct node *link; 
};

void     DefineDFA();    //Fill transition table
void findclosure(int,int);
void insert_trantbl(int ,char, int);
int findalpha(char);
void unionclosure(int);
void print_e_closure(int,FILE*);
static int set[20],nostate,s,notransition,start,finalstate,c,r,buffer[20];
char alphabet[4]={'a','b','e'};
static int e_closure[20][20]={0};
struct node * transition[20][20]={NULL};

char stack[100];
int top = -1;

void push(char x)
{
    stack[++top] = x;
}

char pop()
{
    if(top == -1)
        return -1;
    else
        return stack[top--];
}

int priority(char x)
{
    if(x == '(')
        return 0;
    if(x == '|')
        return 1;
    if(x == '&')
        return 2;
    if(x == '*')
        return 3;
    return 0;
}
FILE *RegExToPostfix(char *e){
FILE *filePointer ;
filePointer = fopen("input.txt", "w");
char x;
while(*e != '\0')
    {
        if(isalnum(*e))
        fprintf(filePointer, "%c", *e);
        else if(*e == '(')
            push(*e);
        else if(*e == ')')
        {
            while((x = pop()) != '(')
                fprintf(filePointer, "%c", x);
        }
        else
        {
            while(priority(stack[top]) >= priority(*e))
               fprintf(filePointer, "%c", pop());
            push(*e);
        }
        e++;
    }
    
    while(top != -1)
    {
fprintf(filePointer, "%c", pop());
    }
    fprintf(filePointer, "\n");

    
    fclose(filePointer);
return filePointer;

}
void EnterRegEx(void)
{
    char exp[100];
    char *e;
    printf("Entrez l'expression reguliere: ");
    scanf("%s",exp);
    e = exp;
   RegExToPostfix(e);
}

int main()
{


    FILE *fp;
    char frm_file[25][50]; //Donnees de input.txt
    char stack[15][150];   //Stack 
    char temp[150];		   //Tableau temporaire pour les transitions
    int state;			   //Iterateur curent d etat 
    int stackPtr;		   //Stack pointer
    char next; 			   //l Expresion suivante(Operateur)
    int x;				   //parser de character 
    int t, i, j, k, v, l1, l2; //Indice 

    /* Read the file from the terminal and check for validity of the file
     * Parse the file data into 2D array
     * If the file data is not valid return 0
     */
        EnterRegEx();
        i = j = 0;
        fp = fopen("input.txt", "r");

        if(fp == NULL)
        {
            printf("fichier Corrupt ou n existe pas\n");
            return(0);
        }
        else
        {
            while(EOF != (x = getc(fp)))
            {
                if(x == '\n')
                {
                    frm_file[i][j] = '\0';
                    i = i + 1;
                    j = 0;
                }
                else
                {
                    if(((x >= 'a') && (x <= 'b')) || (x == '*') || (x == '&') || (x == '|') || (x == 'e'))
                    {
                        frm_file[i][j++] = (char)x;
                    }
                    else
                    {
                        printf("ficheir Corrupt\n");
                        return(0);
                    }
                }
            }
        }
    

    t = v = k = 0;
    state = 1;
    stackPtr = 0;

    while(k < i)
    {
        printf("\n%d: ", k + 1);
        for(v = 0; '\0' != frm_file[k][v]; v++) 	//boucle jusquau fin de la line d input.txt
        {
            next = frm_file[k][v];
            printf("%c", next);

            /* Switch case basé sur |,*,& et pas d'operateur
             * parse l'expression et mise a jour de stack
             */
            switch(next)
            {
            //Concatenation
            case('&'):
                if((stackPtr - 2) < 0)
                {
                    printf("Input Data Corrupted\n");
                    return(0);
                }

                t = 0;

                //Fragment 1
                for(j = 0; stack[stackPtr - 2][j] != '\0'; j++)
                {
                    temp[t++] = stack[stackPtr - 2][j];
                }

                //Fragment 2
                temp[t++] = stack[stackPtr - 2][j - 1];
                temp[t++] = 'e';
                temp[t++] = stack[stackPtr - 1][0];


                //Fragment 3
                for(j = 0; stack[stackPtr - 1][j] != '\0'; j++)
                {
                    temp[t++] = stack[stackPtr - 1][j];
                }

                temp[t] = '\0';

                // Creer une copie de tous les fragments puis les combineer dans le stack (1 NFA) 

                for(j = 0; temp[j] != '\0'; j++)
                {
                    stack[stackPtr - 2][j] = temp[j];
                }

                //\0 terminateur
                stack[stackPtr - 2][j] = '\0';
                stackPtr = stackPtr - 1;

                break;

            //Union
            case('|'):
                if((stackPtr - 2) < 0)
                {
                    printf("Donnees Corrupt\n");
                    return(0);
                }

                t = 0;

                //Fragment 1
                temp[t++] = state;
                temp[t++] = 'e';
                temp[t++] = stack[stackPtr - 1][0];

                //Fragment 2
                temp[t++] = state++;
                temp[t++] = 'e';
                temp[t++] = stack[stackPtr - 2][0];

                //Fragment 3
                for(j = 0; stack[stackPtr - 1][j] != '\0'; j++)
                {
                    temp[t++] = stack[stackPtr - 1][j];
                }

                //Longeur de tableau
                l1 = j - 1;

                //Fragment 4
                for(j = 0; stack[stackPtr - 2][j] != '\0'; j++)
                {
                    temp[t++] = stack[stackPtr - 2][j];
                }

                //Longeur de tableau
                l2 = j - 1;

                //Fragment 5
                temp[t++] = stack[stackPtr - 1][l1];
                temp[t++] = 'e';
                temp[t++] = state;

                //Fragment 6
                temp[t++] = stack[stackPtr - 2][l2];
                temp[t++] = 'e';
                temp[t++] = state++;
                temp[t] = '\0';

                // Creer une copie de tous les fragments puis les combineer dans le stack (1 NFA) 

                for(j = 0; temp[j] != '\0'; j++)
                {
                    stack[stackPtr - 2][j] = temp[j];
                }

                //\0 terminateur et decrementer le stackpointer
                stack[stackPtr - 2][j] = '\0';
                stackPtr = stackPtr - 1;

                break;

            //KLEENE Star
            case('*'):
                if((stackPtr - 1) < 0)
                {
                    printf("Donnees corrupt\n");
                    return(0);
                }

                t = 0;

                //Fragment 1
                temp[t++] = state;
                int state1=state;
                temp[t++] = 'e';
                temp[t++] = stack[stackPtr - 1][0];

                //Fragment 2
                for(j = 0; stack[stackPtr - 1][j] != '\0'; j++) 	//segment 2
                {
                    temp[t++] = stack[stackPtr - 1][j];
                }

                //Fragment 3
                temp[t++] = stack[stackPtr - 1][j - 1];
                temp[t++] = 'e';
                temp[t++] = stack[stackPtr - 1][0];
               
                 //Fragment 4
                temp[t++] = stack[stackPtr - 1][j - 1];
                temp[t++] = 'e';
                temp[t++] = ++state;
                //Fragment 5
                temp[t++] = state1;
                temp[t++] = 'e';
                temp[t++] = state;
                temp[t] = '\0';

                // Creer une copie de tous les fragments puis les combineer dans le stack (1 NFA) 
                for(j = 0; temp[j] != '\0'; j++)
                {
                    stack[stackPtr - 1][j] = temp[j];
                }

                //\0 terminateur
                stack[stackPtr - 1][j] = '\0';
                break;

            default:
                stack[stackPtr][0] = state++;	//Etat de debut
                stack[stackPtr][1] = next;		//Transition
                stack[stackPtr][2] = state++;	//Etat finale

                //\0 terminateur et incrementation de  stackpointer
                stack[stackPtr][3] = '\0';
                stackPtr++;

                break;

            }
        }

        /* Printer le stack(NFA)
         * aussi printer l etat finale  */
        if((stackPtr - 1) == 0) 	
        {
            int notransition=0;
            FILE *fp=fopen("nfa.txt","a+");
            FILE *Fdfa = fopen("nonMinDFA.txt", "a+");


           
            int s,m1;
            char ch;

                for(j = 0; stack[stackPtr - 1][j] != '\0'; j += 3)
                {
                    fprintf(fp,"\n(q%d, %c) --> q%d", stack[stackPtr - 1][j], stack[stackPtr - 1][j + 1], stack[stackPtr - 1][j + 2]);
                    s=stack[stackPtr - 1][j];
                    ch=stack[stackPtr - 1][j + 1];
                    m1=stack[stackPtr - 1][j + 2];
                                insert_trantbl(s,ch,m1);

                }
                int nostate=stack[stackPtr - 1][j - 1];
                int finalstate=stack[stackPtr - 1][j - 1];
                int start=stack[stackPtr - 1][0];

                fprintf(fp,"\nStart State:\tq%d\nAccept State:\tq%d\n",stack[stackPtr - 1][0], stack[stackPtr - 1][j - 1]);

        
        
                int i1,j1,k,m,t,n;

           struct node *temp;
             
            fprintf(Fdfa,"\n");

            for(i1=1;i1<=nostate;i1++)
                {
                        c=0;
                        for(j1=0;j1<20;j1++)

                        {
                                buffer[j1]=0;
                                e_closure[i][j1]=0;
                        }

                        findclosure(i1,i1);
                }
        fprintf(Fdfa,"Dfa equivalent d epsilon-NFA \n");
        fprintf(Fdfa,"-----------------------------------\n");
        FILE *filePointer0;
            filePointer0 = fopen("Start_state.txt", "w");
        fprintf(Fdfa,"Etat depart: ");
            print_e_closure(start, Fdfa);
            print_e_closure(start, filePointer0);
        FILE *filePointerr;
            filePointerr = fopen("all_states_enum.txt", "w");
        fprintf(Fdfa,"\n Etats : \n" );
        for(i1=2;i1<nostate;i1++){
            print_e_closure(i1, Fdfa);
              fprintf(Fdfa,"\n");
            print_e_closure(i1,filePointerr);
              fprintf(filePointerr,"\n");
        }
        fprintf(Fdfa,"\nLes tnransitions sont...:\n");
            FILE *filePointer1 ;
            filePointer1 = fopen("transitions.txt", "w");
        for(i1=2;i1<nostate;i1++)
        {

          for(j1=0;j1<2;j1++)
            {
                for(m=1;m<=nostate;m++)
                    set[m]=0;
                for(k=0;e_closure[i1][k]!=0;k++)
                {

                            t=e_closure[i1][k];
                            temp=transition[t][j1];
                            while(temp!=NULL)
                                {
                                    unionclosure(temp->st);
                                    temp=temp->link;
                                }
                }
                            fprintf(Fdfa,"\n");
                            fprintf(filePointer1,"\n");
                            int Khaoula=0;
                             for(n=1;n<=nostate;n++)
                            {
                                 if(set[n]!=0)
                                        Khaoula++;
                            }
                            if(Khaoula!=0){
                            print_e_closure(i1,filePointer1);
                            print_e_closure(i1,Fdfa);
                            fprintf(filePointer1,"  %c  ",alphabet[j1]  );
                            fprintf(Fdfa,"  %c  ",alphabet[j1]  );

                            for(n=1;n<=nostate;n++)
                            {
            
                                        if(set[n]!=0){
                                                fprintf(Fdfa,"%d",n);
                                                fprintf(filePointer1,"%d",n);
                                        }
                                               

                            }
                           }
            }
    }
        fprintf(Fdfa,"\n Etats Finaux:");
          FILE *filePointer2 ;
            filePointer2 = fopen("Final_states.txt", "w");
          int j2,k2;
                      for(j2=1;j2<nostate;j2++)
                      {
                              for(k2=0;e_closure[j2][k2]!=0;k2++)
                                {
                                         if(e_closure[j2][k2]==finalstate)
                                        {
                                                 print_e_closure(j2,Fdfa);
                                                 print_e_closure(j2,filePointer2);
                                        }
                               }
                                    fprintf(Fdfa," ");
                                    fprintf(filePointer2," ");

                      }
        }
        else
        {
            printf("Donnees corrupt\n");
            return(0);
        }

        printf("\n");

        //Reset the indexs for next regex
        k = k + 1;
        j = t = v = 0;	
        stackPtr = 0;
        state = 1;
      
    }

    printf("\n");
    return(0);
}

void findclosure(int x,int sta)//Trouver des e_clotures
{
            struct node *temp;
            int i;
           if(buffer[x])
                     return;
             e_closure[sta][c++]=x;
            buffer[x]=1;
             if(alphabet[2]=='e' && transition[x][2]!=NULL)
                {
                             temp=transition[x][2];
                             while(temp!=NULL)
                            {
                                         findclosure(temp->st,sta);
                                         temp=temp->link;
                             }
                }
  }

void insert_trantbl(int r,char c,int s)//insertion dans la table des transitions
{
           int j;
           struct node *temp;
            j=findalpha(c);
          if(j==999)
          {
                     printf("%c  \n",c);
                     printf("error\n");
                    exit(0);
          }
         temp=(struct node *) malloc(sizeof(struct node));
         temp->st=s;
         temp->link=transition[r][j];
         transition[r][j]=temp;
}

int findalpha(char c)//check si les symboles sont a,b,e(epsilon)
{
            int i;
            for(i=0;i<3;i++)
                   if(alphabet[i]==c)
                          return i;
            return(999);
}

void unionclosure(int i) //les e cloture sources
{
              int j=0,k;
             while(e_closure[i][j]!=0)
             {
                k=e_closure[i][j];
                      set[k]=1;
                j++;
             }
}
void print_e_closure(int i, FILE* Fdfa) //Ecrire les e-cloture
{
        int j;
        int k;
        int TRI[50];
        for(j=0;e_closure[i][j]!=0;j++){
            TRI[j]=e_closure[i][j];
        }
    int i1,j1,c;
        for(i1=0;i1<j-1;i1++){
            for(j1=i1+1;j1<j;j1++){
               //swap
                if ( TRI[i1] > TRI[j1] ) {
                c = TRI[i1];
                TRI[i1] = TRI[j1];
                TRI[j1] = c;
        }

        }}
        for(k=0;k<j;k++){
            if(TRI[k]!=0)
            fprintf(Fdfa,"%d",TRI[k]);
        }
        
}