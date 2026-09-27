#include <stdio.h>
int main(){
    float a,b;
    char c;
    printf("Enter two numbers in a_b format:");
    scanf("%f %f",&a,&b);
    printf("Enter operator (+ - * /):"); 
    scanf(" %c",&c);
    if(c=='+'){printf("%.2f\n",a+b);}
    else if (c=='-'){printf("%.2f\n",a-b);}
    else if (c=='*'){printf("%.2f\n",a*b);}
    else if (c=='/'){
        if(b!=0 && a!=0){
            printf("%.2f\n",a/b);
        }
        else{printf("Divison by zero error\n");}    
    }
    else{printf("Please enetr the valid operator.");}
    return 0;
}
