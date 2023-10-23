#include <stdio.h>
#include <math.h>

void scan_data(char *, double *);
void do_next_op(char, double, double *);
void run_calculator(char *, double *, double *);

int main(void){
    double akkumulator=0, operand;
    char operator;

    printf("Binary operators:\n + for addition\n - for substraction\n * for multiplication\n / for division\n ^ for the power\n\n");
    printf("Unary operators:\n # for square root\n %% to multiply by -1\n ! to divide 1 by the accumulated sum\n\n");

    run_calculator(&operator, &operand, &akkumulator);

    return 0;
}

void scan_data(char* o, double* n){
// Der bruges o for operator og n for operand, for at nemmere differentiere mellem pointere og variablen i main
// mellemrum foran %c, ellers læser den ikke input
    scanf(" %c", o);
    
    if (*o=='!'||*o=='%'||*o=='#')
    {
        *n=0.0;

    } else if (*o=='+'||*o=='-'||*o=='*'||*o=='/'||*o=='^')
    {
        printf("Write a number: ");
        scanf("%lf", n);
    }
}

void do_next_op(char operator, double operand, double *a){
//Her bruges a i stedet for akkumulator
    switch (operator){
        case '+': *a = *a + operand; break;
        case '-': *a = *a - operand; break;
        case '*': *a = *a * operand; break;
        case '/': *a = *a / operand; break;
        case '^': *a = pow(*a, operand); break;
        case '%': *a *= -1; break;
        case '#': if (*a >= 0)
        {
            *a = sqrt(*a); break;
        }
        case '!': if (*a != 0)
        {
            *a = 1 / *a; break;
        }
        default: break;
    }
}

void run_calculator(char *o, double *n, double *a){ 
//loopet kører uendeligt indtil operatoren er q og loopet stoppes midt i kroppen
    while (1)
    {
        printf("Enter an operator: ");

        scan_data(o, n);

        if (*o == 'q')
        {
            break;
        }
        
        do_next_op(*o, *n, a);

        printf("The result is: %lf \n\n", *a);
    }
    
    printf("The final result is: %lf \n", *a);

}

