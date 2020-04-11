#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 2
#define AZ 26
#define SMERY 8

void vypis(int mlines, int ncolumns, char **array, char **vylustena){
    int malePismena = 'a' - 'A';

    for(int i = 0; i< mlines; i++){
        for(int j=0; j < ncolumns; j++){
            if(vylustena[i][j] ==0) printf("%c", array[i][j]);
            else printf("%c", array[i][j] + malePismena);
        }
        printf("\n");
    }
    printf("\n");
}

void hladanie_slova(char slovo[], FILE *fr, int **abeceda, int pocty[], int mlines, int ncolumns, char **array, char **vylustena){

    int hl_pis=0, x=0, y=0, najdene=0, dlzka, zaciatokRiadky, zaciatokStlpce, riadok, stlpec, temp;

    //dvojice tvoria svetove strany posuvania riadkov a stlpcov
    int smer_riadky[SMERY] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    int smer_stlpce[SMERY] = {0, 1, 1, 1, 0, -1, -1, -1 };

    while (fscanf(fr, "%s", slovo) > 0){
        dlzka=strlen(slovo);
        hl_pis= slovo[0] - 'A';
        najdene=0;

        //najdeny zaciatok slova
        for(int zaciatok = 0; zaciatok < pocty[hl_pis]; zaciatok++){ //pre pripad, ze by sa tam niektore pismenko nachadzalo viac krat

            //suradnice zaciatku slova
            zaciatokRiadky = abeceda[hl_pis][zaciatok*2]; //posuvam sa o dva indexy
            zaciatokStlpce = abeceda[hl_pis][zaciatok*2 +1]; //index y
//            printf("%d %d\n", zaciatokRiadky, zaciatokStlpce);

            //kontrolujem smery osemsmerovky
            for(int smer =0; smer<SMERY; smer++){
                riadok = zaciatokRiadky;
                stlpec= zaciatokStlpce;
                temp=0;

                //osetrenie aby som nevysla zo slova && z pola
                while((temp < dlzka) && (riadok < mlines) && (stlpec < ncolumns) && (riadok>=0) && (stlpec>=0)){

                    //ak sa pismenka nezhoduju, rovno skonci
                    if(slovo[temp] != array[riadok][stlpec]) {
                        break;
                    }
                    //ak uz sa temp doratalo do dlzky tak mame slovo najdene, bomba, musime zmensovat pismenka
                    if(temp >= dlzka-1) {
                        najdene = 1;
                        for(int k=0, riadok = zaciatokRiadky, stlpec = zaciatokStlpce; k<dlzka; k++, riadok += smer_riadky[smer], stlpec += smer_stlpce[smer])
                            vylustena[riadok][stlpec] = 1;
                        break;
                    }
                    temp++;
                    riadok += smer_riadky[smer];
                    stlpec += smer_stlpce[smer];
                    if((temp >= dlzka) && (riadok >= mlines) && (stlpec >= ncolumns) && (riadok<0) && (stlpec<0) ) break;
                }
            }
            if(najdene == 1)
                vypis(mlines, ncolumns, array, vylustena);
            else printf("%s nebolo najdene\n", slovo);
        }
    }
}

void index(int mlines, int ncolumns, char **array, int **abeceda, int pocty[]){

    int dlzky[AZ];

    //vynulovanie pola pocty a pole dlzky nastavenie na N
    for(int i=0; i<AZ; i++){
        pocty[i]=0;
        dlzky[i]=N;
    }

    int temp;
    for(int riadky=0; riadky<mlines; riadky++){
        for(int stlpce=0; stlpce<ncolumns; stlpce++){

            temp = array[riadky][stlpce] - 'A';
            pocty[temp]++;
            if(pocty[temp]*2 > dlzky[temp]){
                abeceda[temp] = (int*)realloc(abeceda[temp], (dlzky[temp] + N) * sizeof(int));
                dlzky[temp] += N;
            }

            abeceda[temp][pocty[temp] * 2 - 2]=riadky;
            abeceda[temp][pocty[temp]*2-1]=stlpce;
        }
    }

}

void freearray(char **array, int mlines) {

    for (int i=0; i<mlines; i++) {
        if (array[i] != NULL)
            free(array[i]);
    }
}

int main() {

    FILE *fr;
    if ((fr = fopen("osemsmerovka.txt", "r")) == NULL) {
        printf("Subor sa nepodarilo otvorit.\n");
        return 0;
    }

    int mlines, ncolumns;
    fscanf(fr, "%d %d", &mlines, &ncolumns);
    getc(fr);
//    printf("%d %d\n", mlines, ncolumns);

    //alokacia pola
    char **array;
    array= (char** )calloc(mlines, sizeof(char*));
    for(int i=0; i<mlines; i++){
        array[i] =  (char* )calloc(ncolumns+1, sizeof(char)); //+1 pre nenulovy znak
    }

    //zapisovanie do matice zo suboru
    for(int i=0; i < mlines; i++){
        for(int j=0; j<= ncolumns; j++){
            fscanf(fr, "%c", &array[i][j]);
        }
    }

    //alokacia pola kam neskor zapisem vyskyt pismenok
    int **abeceda, pocty[AZ];
    abeceda = (int **) malloc (AZ * sizeof(int));
    for(int i=0; i < AZ; i++)
        abeceda[i] = (int *) malloc (N*sizeof(int));
    index(mlines, ncolumns, array, abeceda, pocty);


    //uz idem hladat slovaaa
    char **vylustena;
    vylustena=(char**)malloc(mlines*sizeof(char));
    for(int i=0; i<mlines; i++)
        vylustena[i]= (char *) malloc (ncolumns*sizeof(char));
    char hladanie_slov[100];
    hladanie_slova(hladanie_slov,fr, abeceda, pocty, mlines, ncolumns, array, vylustena);

    freearray(array, mlines);
    if(fclose(fr) == EOF){
        printf("Subor sa nepodarilo zatvorit");
        return 0;
    }
}