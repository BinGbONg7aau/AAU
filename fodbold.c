#include <stdio.h>
#include <stdlib.h> //used for qsort and exit(EXIT_FAILURE)
#include <string.h> //used for strcmp and strcpy
#define NUMBER_OF_MATCHES 90
#define NUMBER_OF_TEAMS 12

//typedef is used so struct all_matches or team doesn't need to be typed every time
typedef struct {
    char weekday[4], team[4], opp[4];
    int day, month, goal1, goal2, attendance;
    double time;
} all_matches;

typedef struct {
    char name[4];
    int point, goals_against, goals_for, goal_delta;
} team;
//i've added a goal_delta member for this struct,
//this will be used in the sorting of standings

void read_from_file(FILE* f, all_matches match[]);
all_matches read_match_from_file(FILE *f);
int are_strings_equal(char* str1, char* str2);
int h(char name[]);
void calculate_standing(all_matches match[], team team_arr[]);
void print_team_array(team team_arr[]);
int sort(const void *p1, const void *p2);
void team_names(team team_arr[], all_matches match[]);
void test_file_reader(all_matches match[]);

int main(void){
    team team_arr[NUMBER_OF_TEAMS];
    all_matches match[NUMBER_OF_MATCHES];

    FILE *f = fopen("kampe-2023-2024.txt", "r");
    if (f == NULL){
        printf("The file wasn't opened");
        exit(EXIT_FAILURE);
    }

    read_from_file(f, match);
    fclose(f);

    team_names(team_arr, match);
    calculate_standing(match, team_arr);
    print_team_array(team_arr);
}

//puts every line of match information into a place in the array
void read_from_file(FILE* f, all_matches match[]){
    int i;
    for (i = 0; i < NUMBER_OF_MATCHES; i++){
        match[i] = read_match_from_file(f);
    }
}

//Reads a single line of text and returns it to the read_from_file function
//returns a match struct
all_matches read_match_from_file(FILE *f){
    all_matches match;
    fscanf(f, "%s %d/%d %lf %s - %s %d - %d %d",
           match.weekday, &match.day, &match.month, &match.time,
           match.team, match.opp, &match.goal1, &match.goal2, &match.attendance);
    return match;
}

//This function assigns 12 team names to structs and initialises each int member the value 0
//The team order is made from the match schedule
//team 1 and 2 from the first match will be assigned to index 0 and 1 in the team array and so forth until the 6th game has been played
//All int members is assigned the value 0, otherwise they can attain random values, which will mess up the scoreboard
//strcpy is used to copy the name of a team in a match, to the name of a team in the team array
void team_names(team team_arr[], all_matches match[]){
    int i, j = 0;
    //two teams are assigned in each loop. i is used as an index for team_arr, and j as an index for match
    //only the first 6 matches are looked through, as all teams play in the first six games.
    for (i = 0; i < NUMBER_OF_TEAMS; i+=2, j++) {
        strcpy(team_arr[i].name, match[j].team);
        strcpy(team_arr[i+1].name, match[j].opp);

        team_arr[i].point = team_arr[i].goals_for = team_arr[i].goals_against = team_arr[i].goal_delta = 0;
        team_arr[i+1].point = team_arr[i+1].goals_for = team_arr[i+1].goals_against = team_arr[i+1].goal_delta = 0;
    }
}

//this function goes through all 90 matches
//first it check which team wom the match with an if statement. The team with the most goals win, and is assigned three points
//if the score is equal both teams get 1 point.
//then it adds the goals for and conceded for both teams, and calculates the goal delta for each team
//uses associative arrays, as the h() function is called it returns an index
//sorts the team_arr at the end
void calculate_standing(all_matches match[], team team_arr[]){
    int i;
    for (i = 0; i < NUMBER_OF_MATCHES; ++i) {
        if (match[i].goal1 > match[i].goal2){
            team_arr[h(match[i].team)].point += 3;
        }
        else if (match[i].goal1 == match[i].goal2){
            team_arr[h(match[i].team)].point++;
            team_arr[h(match[i].opp)].point++;
        } 
        else {
            team_arr[h(match[i].opp)].point += 3;
        }

    team_arr[h(match[i].team)].goals_for += match[i].goal1;
    team_arr[h(match[i].team)].goals_against += match[i].goal2;
    team_arr[h(match[i].opp)].goals_against += match[i].goal1;
    team_arr[h(match[i].opp)].goals_for += match[i].goal2;
    team_arr[h(match[i].team)].goal_delta += match[i].goal1-match[i].goal2;
    team_arr[h(match[i].opp)].goal_delta += match[i].goal2-match[i].goal1;
    }

    qsort(team_arr, NUMBER_OF_TEAMS, sizeof(team), sort);
}

//This function is used to make the associative array
//takes a string as input, and uses the are_strings_equal function to check the two strings
//every team returns a different index for the team_arr, where function is called
//the order of teams is determined by the order of which a team plays a game
//this order is also used by the team_names function
//inspired by Kurt Nørmark
int h(char name[]){
    if (are_strings_equal(name, "FCM")){
        return 0;
    } else if (are_strings_equal(name, "HIF")){
        return 1;
    } else if (are_strings_equal(name, "LBK")){
        return 2;
    } else if (are_strings_equal(name, "FCK")){
        return 3;
    } else if (are_strings_equal(name, "AGF")){
        return 4;
    } else if (are_strings_equal(name, "VB")){
        return 5;
    } else if (are_strings_equal(name, "OB")){
        return 6;
    } else if (are_strings_equal(name, "RFC")){
        return 7;
    } else if (are_strings_equal(name, "SIF")){
        return 8;
    } else if (are_strings_equal(name, "BIF")){
        return 9;
    } else if (are_strings_equal(name, "FCN")){
        return 10;
    } else if (are_strings_equal(name, "VFF")){
        return 11;
    } else {
        return -1;
    }
}

//this function uses a library function strcmp to compare two strings
//if the two strings are equal, the function will return 0 if the strings are equal
//Works as an if-statement, and returns 1 if the two string are equal. 
int are_strings_equal(char *str1, char *str2){
    return strcmp(str1, str2) == 0;
}

//this function sorts the teams
//Primarily the teams with a higher point tally will be placed above
//Secondarily if two teams has equal points, the team with a higher goal delta will be placed above.
//points, and goal delta are assigned to an type int variable, which will then be compared.
//((team*)p1) takes a void datatype and converts it into a team*
//-> is an indirect component selection, that assumes the value of the member
int sort(const void *p1, const void *p2){
    int ip1 = ((team*)p1)->point;
    int ip2 = ((team*)p2)->point;
    int pp1 = ((team*)p1)->goal_delta;
    int pp2 = ((team*)p2)->goal_delta;

    if (ip1 > ip2){
        return -1;
    } else if (ip1 < ip2){
        return 1;
    } else if (ip1 == ip2 && pp1 > pp2){
        return -1;
    } else if (ip1 == ip2 && pp1 < pp2){
        return 1;
    } else {
        return 0;
    }
}

//This function prints the 12 sorted team structs as a scoreboard
void print_team_array(team team_arr[]){
    int i, j;

    printf("THESE ARE THE STANDINGS:\n");
    printf("P: Points, GS: Goals Scored, GC: Goals Conceded\n\n");
    printf("Team | P  | GS | GC \n");

    for (j = 0; j < 20; j++)
    {
        if(j%5 == 0 && j!= 0){
            printf("|");
        } else {
            printf("-");
        }
    }
    
    printf("\n");
    for (i = 0; i < NUMBER_OF_TEAMS; ++i){
        printf("%4s | %2d | %2d | %2d\n", team_arr[i].name, team_arr[i].point,
               team_arr[i].goals_for, team_arr[i].goals_against);
    }
}

//simply a test to check whether all information is read correctly from the file.
//this prints all match information to the terminal
void test_file_reader(all_matches match[]){
    int i;

    for (i = 0; i < NUMBER_OF_MATCHES; i++){
        printf("%s %d %d %lf %s %s %d %d %d\n",
               match[i].weekday, match[i].day, match[i].month,
               match[i].time, match[i].team, match[i].opp,
               match[i].goal1, match[i].goal2, match[i].attendance);
    }
}
