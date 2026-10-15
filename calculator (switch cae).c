#include <stdio.h>
int main(){
    float a,b;
    char c;
    printf("Enter two numbers in a_b format:");
    scanf("%f %f",&a,&b);
    printf("Enter operator (+ - * /):"); 
    scanf(" %c",&c);
    switch (c){
    case '+':
        printf("%.2f\n",a+b);
        break;
    case '-':
        printf("%.2f\n",a-b);
        break;
    case '*':
        printf("%.2f\n",a*b);
        break;
    case '/':
        if(b!=0){
            printf("%.2f\n",a/b);
        }
        else{printf("Zero division error.\n");}    
        break;        
    default:
        printf("Enter a valid operator.\n");
        break;
    }
    return 0;
}
