#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
typedef struct Unilex
{
    int num;
    char* ul ;
    char att[30];
}Unilex;
typedef struct symboles
{
    int num;
    char att[30];
    char* type ;
}symboles;
char* symbole;
FILE *f;
FILE *fichier;
char mot_cle[15][20]={"program","var","integer","char","begin","if","then","else","while","do","read","readln","write","writeln","end"};
symboles tabSem[255];
Unilex tablesSymboles[255];
int indiceALex=0;//taille de la tablesSymboles
char caractere;
int termine=0;
int taille=0;//taille de la tab sem
int limiteAjout=0;

void P();
void exp();
void list_id();
void list_inst();
void INST();
void inst_composee();
void type();
void DCL();
void DCL1();
void error();
void list_id1();
void list_inst1();
char* exp1();
char* exp_simple1();
char* exp_simple();
char* terme1();
char* facteur();
char* terme();
char* analLex();
char* chercher_type(char* nomVar);

char *strconcatChaine (const char *chaine1, const char *chaine2) {
    size_t len1 = strlen(chaine1);
    size_t len2 = strlen(chaine2);
    size_t lenTotal = len1 + len2 + 1;
    char *result = (char *)malloc(lenTotal);
    if (result == NULL) {
        perror("Erreur lors de l'allocation de m moire");
        exit(EXIT_FAILURE);
    }
    strcpy(result, chaine1);
    strcat(result, chaine2);

    return result;
}


void accepter(const char *T)
{
    if (strcmp(symbole,T) == 0)
     {
         symbole=analLex();
    }
    else
    {   printf("Erreur de syntaxe:\" %s\" on trouve: \" %s \" \n", T,symbole);
        exit(0);
    }
}
void P()
{
	symbole=analLex();
        accepter("program");
        accepter("id");
        accepter(";");
        DCL();
        inst_composee();
        accepter(".");
        printf("\n compilation Correcte!");
}

void inst_composee()
{
    accepter("begin");
    INST();
    accepter("end");
}

void DCL()
{
    DCL1();
}

void DCL1()
{
    if (strcmp(symbole, "var") == 0)
    {
        accepter("var");
        list_id();
        accepter(":");
        ajouter_type(symbole);
        type();
        accepter(";");
        DCL1();
    }
}

void list_id()
{

    accepter("id");
    list_id1();

}


void list_id1()
{
    if(strcmp(symbole,",")==0)
    {
        accepter(",");
        accepter("id");
        list_id1();
    }
}

void type()
{

    if(strcmp(symbole,"integer")==0)
        accepter("integer");
    else
        if (strcmp(symbole,"char")==0)
            accepter("char");
    else
    {
        printf("%s error in type\n",symbole );
    }
}



void INST()
{
    list_inst();
}


void list_inst()
{
    I();
    list_inst1();
}


void list_inst1()
{
        if(strcmp(symbole,";")==0)
        {
            accepter(";");
            I();
            list_inst1();
        }
}


void I()
{
    if(strcmp(symbole,"id")==0)
    {

        accepter("id");
        char* var1 = tablesSymboles[indiceALex-2].att;
        if(! isdigit(var1)){
           char* input1=strconcatChaine("valeurg @",var1);
           emettre(input1);
        }
       else{
            emettre(var1);
       }
        char* declaredType = chercher_type(var1);
        accepter(":=");

        char* var2 = tablesSymboles[indiceALex - 1].att;


        char* assignedType = exp_simple();

		if(! isdigit(var2)){
            char* input2=strconcatChaine("valeurd @",var2);
            emettre(input2);
        }
        else{
                char* input2=strconcatChaine("empiler ",var2);
                emettre(input2);
        }
        emettre(":=");
        if (compatible(declaredType, assignedType) != 0)
        {
            printf("\n Type declar : %s diff rent de type assign :  %s \n", declaredType, assignedType);
            exit(0);
        }
    }

    else if(strcmp(symbole,"if")==0)
    {

        accepter("if");
        exp();
        accepter("then");
        I();
        accepter("else");
        I();

    }
    else if(strcmp(symbole,"while")==0){
        accepter("while");
        exp();
        accepter("do");
        I();
    }else if(strcmp(symbole,"read")==0){
            accepter("read");
            accepter("(");
            accepter("id");
            char* cara =chercher_type(tablesSymboles[indiceALex-2].att);
            accepter(")");

        }else if (strcmp(symbole,"readln")==0){
            accepter("readln");
            accepter("(");
            accepter("id");
            char* cara =chercher_type(tablesSymboles[indiceALex-2].att);
            accepter(")");

        }else if(strcmp(symbole,"write")==0)
        {
            accepter("write");
             accepter("(");
            accepter("id");

            char* cara =chercher_type(tablesSymboles[indiceALex-2].att);
            accepter(")");

        }else if(strcmp(symbole,"writeln")==0)
        {
            accepter("writeln");
             accepter("(");
            accepter("id");
            char* cara =chercher_type(tablesSymboles[indiceALex-2].att);
            accepter(")");

        }
    }


void exp( )
{
    exp_simple();
    exp1();
}

char* exp1()
{
    char* type;
    if(strcmp(symbole,"oprel")==0)
    {
        accepter("oprel");
         if (!isdigit(tablesSymboles[indiceALex-1].att))
        {
            char* chaine=strconcatChaine("valeurd @",tablesSymboles[indiceALex-1].att);
            emettre(chaine);
        }

        else {
                char* chaine=strconcatChaine("empiler",tablesSymboles[indiceALex-1].att);
                emettre(chaine);
        }

       if (!isdigit(tablesSymboles[indiceALex-3].att)){
            char* chaine=strconcatChaine("valeurd @",tablesSymboles[indiceALex-3].att);

            emettre(chaine);
       }
        else {
                char* chaine=strconcatChaine("empiler",tablesSymboles[indiceALex-3].att);
                emettre(chaine);
        }

        emettre(tablesSymboles[indiceALex-2].att);
        if(compatible(chercher_type(tablesSymboles[indiceALex-1].att),chercher_type( tablesSymboles[indiceALex-3].att))!= 0)
        {
             printf("\n Type incompatible: %s %s %s \n",tablesSymboles[indiceALex-1].att,tablesSymboles[indiceALex-2].att,tablesSymboles[indiceALex-3].att);
             exit(0);
        }
        exp_simple();
    }
    return type;
}

char* exp_simple()
{
    char* type = terme();
    exp_simple1(type);
    return type;
}

char* exp_simple1(char* inType)
{
    if (strcmp(symbole, "opadd") == 0)
    {
        accepter("opadd");
        char* op = tablesSymboles[indiceALex - 2].att;
        char* type = terme();

        if (!isdigit(tablesSymboles[indiceALex-1].att))
        {
            char* chaine=strconcatChaine("valeurd @",tablesSymboles[indiceALex-1].att);
            emettre(chaine);
        }

        else {
                char* chaine=strconcatChaine("empiler",tablesSymboles[indiceALex-1].att);
                emettre(chaine);
        }

       if (!isdigit(tablesSymboles[indiceALex-3].att)){
            char* chaine=strconcatChaine("valeurd @",tablesSymboles[indiceALex-3].att);

            emettre(chaine);
       }
        else {
                char* chaine=strconcatChaine("empiler",tablesSymboles[indiceALex-3].att);
                emettre(chaine);
        }

        emettre(tablesSymboles[indiceALex-2].att);

        if (compatible(inType, type) != 0)
        {
            printf("Type incompatible in addition: %s %s %s\n", inType, op, type);
            exit(0);
        }

        exp_simple1(inType);
    }
}

char* terme()
{
    char* type = facteur();
    terme1(type);
    return type;
}

char* terme1(char* inType)
{
    if (strcmp(symbole, "opmul") == 0)
    {
        accepter("opmul");
        char* op = tablesSymboles[indiceALex - 2].att;
        char* type = facteur();

        if (!isdigit(tablesSymboles[indiceALex-1].att))
        {
            char* chainee=strconcatChaine("valeurd @",tablesSymboles[indiceALex-1].att);
            emettre(chainee);
        }
        else {
                char* chainee=strconcatChaine("empiler",tablesSymboles[indiceALex-1].att);

                emettre(chainee);
        }

       if (!isdigit(tablesSymboles[indiceALex-3].att))
        {
            char* chainee=strconcatChaine("valeurd @",tablesSymboles[indiceALex-3].att);
            emettre(chainee);
       }
        else {
                char* chainee=strconcatChaine("empiler",tablesSymboles[indiceALex-3].att);
                emettre(chainee);
        }

        emettre(tablesSymboles[indiceALex-2].att);
        if (compatible(inType, type) != 0)
        {
            printf("Type incompatible lors de la  multiplication: %s %s %s\n", inType, op, type);
            exit(0);
        }

        terme1(inType);
    }
}


char* afficherType(void *variable) {

    if (*(int*)variable) {
        return "integer";
    } else if (*(float*)variable) {
            return "real";
    } else if (*(char*)variable) {
        return "char";
    } else {
        return "erreur_de_type";
    }
}

char* facteur()
{
    if(strcmp(symbole,"id")==0 )
      {
        accepter("id");
        char* type=chercher_type(tablesSymboles[indiceALex-2].att);
        return type;

      } else
      if(strcmp(symbole,"nb")==0){
       {
            accepter("nb");
            char* type=afficherType(tablesSymboles[indiceALex-2].att);
            return type;
       }
    }
    else
    if (strcmp(symbole,"(")==0)
    {
        accepter("(");
        exp_simple();
        accepter(")");
    }
     return "";



}







void ajouterTableSymboles(char* ul)
{
    tablesSymboles[indiceALex].ul= ul ;
    tablesSymboles[indiceALex].num=indiceALex;
    indiceALex++;
}

void erreur(char* erreurMsg)
{
    termine=1;
    printf("Erreur !! %s\n",erreurMsg);
}
void reculer(int a)
{
    fseek(f, -a, SEEK_CUR);

}
void supprimerEspaces(char *chaine) {
    int i, j = 0;

    for (i = 0; chaine[i] != '\0'; i++) {
        if (chaine[i] != ' '&& chaine[i] != '\t' ) {
            chaine[j++] = chaine[i];
        }
    }
    chaine[j] = '\0';
}
int RangerId(char* lexeme)
{int i;
    supprimerEspaces(lexeme);
    for ( i=0;i<15;i++)
    {

        if(strcmp(mot_cle[i],lexeme)==0)
        {
            return 0;
        }

    }
    return 1;
}

char* uniLexId(char* lexeme)
{

    supprimerEspaces(lexeme);
    if (RangerId(lexeme) != 0)//si n'est pas un mot cl 
        return "id";
    else
        return lexeme;


}




void carsuivant()
{
    caractere=fgetc(f);
    int len = strlen(tablesSymboles[indiceALex].att);
    if(caractere != '\n' )
        tablesSymboles[indiceALex].att[len]=caractere;
    }


char* analLex()
{
    int etat=0;
    while(1)
    {
        switch(etat)
        {
            case 0 :
                carsuivant();
                if(isalpha(caractere)){etat=1;}
                else if (isdigit(caractere)){etat=3;}
                 else if(caractere==':'){etat=24;}
                 else if(caractere==','){etat=29;}
                 else if(caractere=='\t' ||caractere==' ' || caractere=='\n' ){etat=0;}
                 else if(caractere==';') {etat=25;}
                 else if(caractere=='.'){etat=26;}
                 else if (caractere=='+'){etat=5;}
                 else if (caractere=='-'){etat=6;}
                 else if (caractere=='|'){etat=7;}
                 else if (caractere=='='){etat=8;}
                 else if (caractere=='<'){etat=9;}
                 else if (caractere=='>'){etat=13;}
                 else if (caractere=='*'){etat=16;}
                 else if (caractere=='/'){etat=17;}
                 else if (caractere=='%') {etat=18;}
                 else if (caractere=='&'){etat=19;}
                else if (caractere=='('){etat=30;}
                else if (caractere==')'){etat=31 ;}
                 else {
                   etat=21;}

                break;
            case 1 :
                carsuivant();
                if(isalpha(caractere)||isdigit(caractere))
                {
                    etat=1;
                }
                else
                    etat=2;
                break;
            case 2 :
                reculer(1);
                int len =strlen(tablesSymboles[indiceALex].att);
                if(caractere != '\n')
                   tablesSymboles[indiceALex].att[len-1]='\0';
                char* id= uniLexId( tablesSymboles[indiceALex].att);
                tablesSymboles[indiceALex].ul=id;
                tablesSymboles[indiceALex].num=indiceALex;
                indiceALex++;
                return id ;

            case 3 :
                carsuivant();

                if(isdigit(caractere))
                {
                    etat=3;
                }
                else
                    etat=4;
                break;

            case 4 :
                reculer(1);
                 len =strlen(tablesSymboles[indiceALex].att);
                tablesSymboles[indiceALex].att[len-1]='\0';
               ajouterTableSymboles("nb");

                return "nb";
            case 5 :
                ajouterTableSymboles("opadd");
                return "opadd";
            case 6 :
               ajouterTableSymboles("opadd");
                return "opadd";
            case 7 :
                carsuivant();
                if(caractere=='|')
                    etat=23;
                else
                 {
                     erreur("On doit avoir deux '|'");
                     return;
                 }
                break;
            case 8 :
                ajouterTableSymboles("oprel");

                return "oprel";
            case 9 :
                carsuivant();
                if(caractere=='>')
                    etat=10;
                else if(caractere == '=')
                    etat=11;
                else
                    etat=12;
                break;
            case 10:
                ajouterTableSymboles("oprel");
                return "oprel";
            case 11:
                 ajouterTableSymboles("oprel");
                 return "oprel";
            case 12:
                 ajouterTableSymboles("oprel");
                return "oprel";
            case 13 :
                carsuivant();

                if(caractere=='=')
                    etat=15;
                else
                {
                    etat=14;
                }
                break;
            case 14:
                  ajouterTableSymboles("oprel");
                 return "oprel";
            case 15 :
                 ajouterTableSymboles("oprel");
                 return "oprel";
            case 16 :
                 ajouterTableSymboles("opmul");

                 return "opmul";
            case 17 :
                ajouterTableSymboles("opmul");
                 return "opmul";
            case 18 :
                ajouterTableSymboles("opmul");
                 return "opmul";
            case 19 :
                carsuivant();
                if(caractere=='&')
                    etat=20;
                else
                {
                    erreur("On doit avoir deux '&'");
                    return;
                }

                break;
            case 20 :
                ajouterTableSymboles("opmul");
                 return "opmul";
            case 21 :
                termine=1;
                return "EOF";

            case 23 :
                ajouterTableSymboles("opadd");

                return "opadd";
            case 24 :
                carsuivant();
                if(caractere=='=')
                    etat=27;
                else
                    etat=28;
                break;
            case 25 :
                 ajouterTableSymboles("PVR");
                 return ";";
            case 26 :
                ajouterTableSymboles("pt");
                return ".";
            case 27 :
                ajouterTableSymboles("AFF");
                 return ":=";
            case 28 :
                ajouterTableSymboles("DIP");
                return ":";
            case 29 :
                ajouterTableSymboles("VR");
                return ",";
            case 30 :
                ajouterTableSymboles("(");
                return "(";
             case 31 :
                ajouterTableSymboles(")");
                return ")";
            default :
                etat=22;


        }

    }
}
int compatible(char* a, char* b)
{
    if (strcmp(a, b) == 0)
        return 0;
    return 1;
}


void ajouter_type(char* typeS )
{

        int i=indiceALex-1;
        while(i>limiteAjout)
        {
            if(tablesSymboles[i].ul!=NULL)
            {
               if(strcmp(tablesSymboles[i].ul ,"id")==0 && strcmp(tablesSymboles[i].att ,"id")!=0)
                   {
                        tabSem[taille].num=taille;
                        tabSem[taille].type = typeS;
                        strcpy(tabSem[taille].att,tablesSymboles[i].att);

                        taille++;
                    }
            }
            i--;
        }
      limiteAjout=indiceALex;

}


char* chercher_type(char* nomVar)
{
	int i;
    for (i=0 ; i<taille;i++)
   {
       if(strcmp(tabSem[i].att,nomVar)== 0)
        {
            return tabSem[i].type;
        }
   }


    return "erreur_de_type";
}
void emettre(const char *contenu) {


    fprintf(fichier, "%s\n", contenu);

}

int main()
{
    f=fopen("test.txt","r");
    fichier = fopen("codepile.txt", "a");


    
    P();

    fclose(fichier);
    fclose(f);



    return 0;
}
