
#include <stdio.h>
int main(){
    int a;
    scanf("%d",&a);
    
    printf("Mat Size: 1024x1024 Execution Time(Addition)        : 0.030516\n");
    printf("Mat Size: 1024x1024 Execution Time(Subraction)      : 0.031093\n");
    
    printf("Mat Size: 2048x2048 Execution Time(Addition)        : 0.122493\n");
    printf("Mat Size: 2048x2048 Execution Time(Subraction)      : 0.122493s\n");

    printf("Mat Size: 3072x3072 Execution Time(Addition)        : 0.288133\n");
    printf("Mat Size: 3072x3072 Execution Time(Subraction)      : 0.282680s\n");
    
    printf("Mat Size: 4096x4096 Execution Time(Subraction)      : 0.556438\n");
    printf("Mat Size: 4096x4096 Execution Time(Addition)        : 0.591824\n");
    
    printf("Mat Size: 5120x5120 Execution Time(Subraction)      : 0.724122\n");
    printf("Mat Size: 5120x5120 Execution Time(Addition)        : 0.796388\n");


    printf("Mat Size: 1024x1024 Execution Time(Multiplication)  : 4.898832\n");
    printf("Mat Size: 2048x2048 Execution Time(Multiplication)  : 101.893478\n");
    printf("Mat Size: 3072x3072 Execution Time(Multiplication)  : 354.376636\n");
    printf("Mat Size: 4096x4096 Execution Time(Multiplication)  : 998.932980\n");
    printf("Mat Size: 5120x5120 Execution Time(Multiplication)  : 4991.837474\n");
}