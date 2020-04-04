#include<stdio.h>
#include <stdlib.h>
#define N 2
#define AZ 26

void hladane_slova(int mlines, int ncolumns, FILE *fr, char slova[], char **array){
    int i=0, j=0;
    while((fscanf(fr, "%c", &slova[i]))!= EOF){
            printf("%c", slova[i]);
            i++;
    }
}


void pocet_znak(char **array,int mlines,int ncolumns,char *index[]) {
    int novePole[26],pocet[26],x=65;
    //nastavenie pismen od A po Z
    for (int i = 0; i < 26; ++i) {
        novePole[i] = N;
        pocet[i] = -1;
        index[i][0]=x++;
    }
    for ( int i = 0; i < mlines; i++) {
        for ( int j = 0; j < ncolumns; j++) {
            for ( int k = 0; k < AZ; k++) {
                if (array[i][j]==index[k][0]&&novePole[k]>pocet[k]){
                    if(pocet[k]==-1)
                        pocet[k]=0;
                    pocet[k]+=2;
                    index[k][pocet[k]-1]=i;
                    index[k][pocet[k]]=j;
                    break;
                }
                else if(array[i][j]==index[k][0]&&novePole[k]==pocet[k]){
                    index[k]=(char*)realloc(index[k], (pocet[k] + N) * sizeof(char));//strasny sef keby to funguje
                    novePole[k]+=N;
                    pocet[k]+=2;
                    index[k][pocet[k]-1]=i;
                    index[k][pocet[k]]=j;
                    break;
                }
            }
        }
    }
    //vypisovanie vsetkych vyskytov
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < pocet[i]+1; j++) {
            printf("%d ",index[i][j]);
        }

    }
    printf("\n");
}
void freearray(char **array, int mlines) {

    for (int i=0; i<mlines; i++) {
        if (array[i] != NULL)
            free(array[i]);
    }
}

int main(){

    FILE *fr;
    if((fr=fopen("osemsmerovka.txt", "r")) == NULL) {
        printf("Subor sa nepodarilo otvorit.\n");
        return 0;
    }

    int mlines, ncolumns;
    fscanf(fr, "%d %d", &mlines, &ncolumns);
    getc(fr);
    printf("%d %d\n", mlines, ncolumns);


    //alokacia pola
    char **array;
    array= (char** )calloc(mlines, sizeof(char*));
    for(int i=0; i<mlines; i++){
        array[i] =  (char* )calloc(ncolumns, sizeof(char*));
    }


    //zapisovanie do pola
    for(int i=0; i < mlines; i++){
        for(int j=0; j<= ncolumns; j++){
            fscanf(fr, "%c", &array[i][j]);
            printf("%c", array[i][j]);
        }
    }

    //vytvorenie pola na pamatanie vyskytu a zapisanie pismen donho
    char *abeceda[AZ];
    char temp = 'A';

    for(int i=0; i < 26; i++){
        abeceda[i] = (char*) calloc (N, sizeof(char)); //malloc mi spravi 2d pole
        abeceda[i][0] = temp++; //na i index zapis hodnoty od A po Z, index j ostane prazdny, tam si neskor zapisem suradnice kde sa to pismenko nachadza
        abeceda[0][i] = -1;
    }

    //zapisovanie si pocet vyskytu slova
    int pocet[AZ]; //hodnoty si nastavim na -1, pretoze 2ak sa slovo nevyskytuje tak ma hodnotu -1
    pocet_znak(array,mlines,ncolumns,abeceda);

    //nacitanie zvysnych slov zo suboru
    char slova_na_skrtanie[100];
    hladane_slova(mlines, ncolumns, fr, slova_na_skrtanie, array);

    freearray(array, mlines);
    freearray(abeceda, mlines);

    if(fclose(fr) == EOF){
        printf("Subor sa nepodarilo zatvorit");
        return 0;
    }

    return 0;
}