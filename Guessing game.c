#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(){
    int guessed;
    int no_of_guessess=0;
    int got;
    srand(time(NULL));
    int secret = rand() % 101;
    printf("Guess a number(0-100):");
    do{
        got=scanf("%d",&guessed);
        if(got==EOF) break;
        if(got!=1){
            printf("Please enter a number.\n");
            while(getchar()!='\n');
            continue;
        }
        no_of_guessess++;
        if(guessed>secret){
            printf("Too high! guess a lower number:\n");}
        else if (guessed<secret){
            printf("Too low! guess a higher number:\n");}
        else{
            printf("Correct! You got the number right in %d guesses.\n",no_of_guessess);}
    }while (guessed!=secret);
    return 0;
}
