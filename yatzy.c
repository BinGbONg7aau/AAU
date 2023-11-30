#include <stdio.h>
#include <time.h> // bruges til srand(time)
#include <stdlib.h> //bruges til srand() og rand()

#define MAX_VALUE  6
#define MAX_POINTS_LARGE 20
#define MAX_POINTS_SMALL 15

const char* types_of_throws[] = {"1'ere", "2'ere", "3'ere", "4'ere", "5'ere", 
"6'ere", "et par", "to par", "tre ens", "fire ens", "lille straight", 
"stor straigt", "fuldt hus", "chance", "Yatzy"};

void all_functions(int arr[], int n);
void roll_multiple_dies(int s[], int n);
void one_to_six(int arr[], int n, int pointarr[]);
int of_a_kind(int arr[], int n, int pair, int a);
int two_pairs(int arr[], int n);
int straight(int arr[], int n, int i, int max);
int full_house(int arr[], int n);
int yatzy(int arr[], int n);
int chance(int arr[], int n);
void scoreboard(int pointarray[]);

int main(void){
    srand(time(NULL));
    int n, i;
    char proev_igen;
    
    do {
    printf("Du skal slå med mindst 5 terninger\n");
    printf("Hvor mange terninger vil du slå med?\n");
    scanf(" %d", &n);

    if (n<5){
        break;
    }
    
    int dice[n];

    all_functions(dice, n);

    printf("q for at slutte programmet, alt andet for at prøve igen\n");
    scanf(" %c", &proev_igen);
    } while (proev_igen != 'q');

    return 0;
}

//i denne funktion sker alle funktionkaldene til funktionerne der regner point ud
//point bliver gemt i et array. 
void all_functions(int dice[], int n){
    int pointarray[15];

    one_to_six(dice, n, pointarray);

    printf("%s :", types_of_throws[6]);
    roll_multiple_dies(dice, n);
    pointarray[6] = of_a_kind(dice, n, 2, 0);
    printf(" -- %d\n", pointarray[6]);

    printf("%s :", types_of_throws[7]);
    roll_multiple_dies(dice, n);
    pointarray[7] = two_pairs(dice, n);
    printf(" -- %d\n", pointarray[7]);

    printf("%s :", types_of_throws[8]);
    roll_multiple_dies(dice, n);
    pointarray[8] = of_a_kind(dice, n, 3, 0);
    printf(" -- %d\n", pointarray[8]);

    printf("%s :", types_of_throws[9]);
    roll_multiple_dies(dice, n);
    pointarray[9] = of_a_kind(dice, n, 4, 0);
    printf(" -- %d\n", pointarray[9]);

    printf("%s :", types_of_throws[10]);
    roll_multiple_dies(dice, n);
    pointarray[10] = straight(dice, n, 1, MAX_POINTS_SMALL);
    printf(" -- %d\n", pointarray[10]);

    printf("%s :", types_of_throws[11]);
    roll_multiple_dies(dice, n);
    pointarray[11] = straight(dice, n, 2, MAX_POINTS_LARGE);
    printf(" -- %d\n", pointarray[11]);

    printf("%s :", types_of_throws[12]);
    roll_multiple_dies(dice, n);
    pointarray[12] = full_house(dice, n);
    printf(" -- %d\n", pointarray[12]);

    printf("%s :", types_of_throws[13]);
    roll_multiple_dies(dice, n);
    pointarray[13] = chance(dice, n);
    printf(" -- %d\n", pointarray[13]);

    printf("%s :", types_of_throws[14]);
    roll_multiple_dies(dice, n);
    pointarray[14] = of_a_kind(dice, n, 5, 0);
    printf(" -- %d\n", pointarray[14]);

    scoreboard(pointarray);
}


//laver et terningekast, og printer
void roll_multiple_dies(int dice[], int n){
    int i;
    for (i = 0; i < n; i++){
        dice[i] = rand() % MAX_VALUE+1;
    }

    for (i = 0; i < n; i++){
        printf(" %d |", dice[i]);
    }
}

//funktion der checker 1'ere til 6'ere og printer point
//pointene gemmes i pointarrayet
void one_to_six(int dice[], int n, int pointarray[]){
    int i, j, point;
    
    for (i = 1; i <= MAX_VALUE; i++){   
        point = 0;
        printf("%s :", types_of_throws[i-1]);
        roll_multiple_dies(dice, n);
        
        for (j = 0; j < n && point < 5*i; j++){
            if (dice[j] == i){
                point = point + i;
            }
        }

        pointarray[i-1] = point;
        printf(" -- %d\n", point);
    }
}

//checker for ens terninger
//i bruges som nummeret på terningen og j bruges som et indeks til løkken
//excluded bruges når der skal tjekkes for fuldt hus
//excluded er 0 indtil terningeværdien for de tre par er fundet
//x er til om der skal findes 2, 3, 4 eller 5 ens
//pointene sendes tilbage og gemmes i arrayet
int of_a_kind(int dice[], int n, int x, int excluded){
    int point = 0, i, j;
    
    for (i = MAX_VALUE; i >= 1 && point < (x*(i+1)); i--){
        if (i == excluded){
            i--;
        }

        for (j = 0; j < n && point < (x*(i)); j++){
            if (dice[j]== i){
                point = point + i;
            }
        }

        if (point != x*i){
            point = 0; 
        } else if (point == 5*i){
            point = 50;
        }
    }

    return point;
}

//finder to par
//point2 gemmer pointene for det første par der findes. 
//j bruges som et indeks til at kigge arrayet igennem
//Counteren bruges til at tjekke hvor mange par der er fundet. 
//Hvis counteren bliver 2 er der fundet to par, og funktionen skal returnere pointene
int two_pairs(int dice[], int n){
    int point = 0, point2 = 0, counter = 0, i, j;
    
    for (i = MAX_VALUE; i >= 1 && counter < 2 ; i--){
        if (i == 1 && point2 == 0){
            break;
        } 
        //hvis der ikke er fundet point inden 1'ere, kan der ikke være to par

        for (j = 0; j < n && point < i*2; j++){
            if (dice[j]== i){
                point = point + i;
            }
        }

        if (point == 2*i){
            counter = counter + 1;
            point2 = point2 + point;
        } 
        point = 0;
    }

    if (counter == 1){
        point2 = 0;
    }
    
    return point2;
}

//i er det første tal i straigthen altså 1 for lille og 2 for stor
//max er det maximale point for straigthen
//counteren bruges til at tjekke om det aktuelle tal er fundet
//hvis et tal ikke findes er counteren 0 og løkken brydes
int straight(int dice[], int n, int i, int max){
    int point = 0, counter, j;

    for (i = i; i <= MAX_VALUE && point < max; i++){
        counter = 0;
        for (j = 0; j < n; j++){
            if (dice[j]== i){   
                counter = i;
                point = point + i;
                break;
            }
        }

        if (counter==0){
            point = 0;
            break;
        }   
    }

    return point;
}

//regner fuldt hus
//pointene regnes for 3 ens
//excluded er den talværdi som der er tre af 
//denne bruges anden gang så der ikke ledes efter talværdien i 2 ens
//hvis der ikke er enten 2 ens eller 3 ens, skal pointene være 0
int full_house(int dice[], int n){
    int three_o_a_k, two_o_a_k, point = 0, excluded = 0;

    three_o_a_k = of_a_kind(dice, n, 3, 0);
    excluded = three_o_a_k/3;
    two_o_a_k = of_a_kind(dice, n, 2, excluded);

    if (three_o_a_k == 0 || two_o_a_k == 0){
        point = 0;
    } else{
        point = three_o_a_k + two_o_a_k;
    }
    
    return point;
}

// regner for chancen
//counteren tæller op når der findes den ønskede talværdi
//når der er fundet de fem højeste brydes løkken
int chance(int dice[], int n){
    int point = 0, i, j, counter = 0;
    
    for (i = MAX_VALUE; i >= 1 && counter < 5; i--){
        for (j = 0; j < n && counter < 5; j++){
            if (dice[j]== i){
                point = point + i;
                counter = counter + 1;
            }
        }
    }

    return point;
}

//printer scoreboardet, og regner bonus ud
//bruger const char* arrayet lavet i starten til at printe navne for forskellige slag
void scoreboard(int pointarray[]){
    int i, bonus = 0, total_points = 0;

    printf("Scoreboard:\n");

//ved at checke de første 6 pladser i arrayet
//og lægge dem samme kan der findes ud af om der skal gives point for bonus
    for (i = 0; i < 6; i++){   
        bonus = bonus + pointarray[i];
        total_points = total_points + pointarray[i];
        printf("%s : %d\n",types_of_throws[i] , pointarray[i]);
    }

    if (bonus >= 63){   
        total_points = total_points + 50;
        printf("Bonus: 50\n");
    }
    
    printf("\n");

    for (i = 0; i < 9; i++){
        printf("%s : %d\n", types_of_throws[i+6], pointarray[i+6]);
        total_points = total_points + pointarray[i+6];
    }

    printf("TOTAL POINTS: %d\n", total_points);
}



