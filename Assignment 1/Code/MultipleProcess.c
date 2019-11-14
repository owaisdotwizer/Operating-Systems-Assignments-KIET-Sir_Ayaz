#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void add(int s){
	int r = s, c = s;
	int *arr = (int *)malloc(r * c * sizeof(int));
	int *arr2 = (int *)malloc(r * c * sizeof(int));
	int *arr3 = (int *)malloc(r * c * sizeof(int));
	
	int i, j;
	
	for (i = 0; i <  r; i++)
	{
	   for (j = 0; j < c; j++)
	    {
	        *(arr + i*c + j) = j;
	    }
	}
	
	for (i = 0; i <  r; i++){
	   for (j = 0; j < c; j++){
	        *(arr2 + i*c + j) = j;
		}
	}
	
	for (i = 0; i <  r; i++){
	   for (j = 0; j < c; j++){
	        *(arr3 + i*c + j) =  *(arr + i*c + j) + *(arr2 + i*c + j);
	    }
	}
}

void sub(int s){
	int r = s, c = s;
	int *arr = (int *)malloc(r * c * sizeof(int));
	int *arr2 = (int *)malloc(r * c * sizeof(int));
	int *arr3 = (int *)malloc(r * c * sizeof(int));
	
	int i, j;
	
	for (i = 0; i <  r; i++){
	   for (j = 0; j < c; j++){
	        *(arr + i*c + j) = j;
		}
	}
	
	for (i = 0; i <  r; i++){
	   for (j = 0; j < c; j++){
	        *(arr2 + i*c + j) = j;
	    }
	}
	
	
	for (i = 0; i <  r; i++){
	   for (j = 0; j < c; j++){
	        *(arr3 + i*c + j) =  *(arr + i*c + j) - *(arr2 + i*c + j);
		}
	}
}

void matrixMul(int size){
    int row = size, col = row;
    int *first = (int *)malloc(row * col * sizeof(int));
    int *second = (int *)malloc(row * col * sizeof(int));
    int *multiply = (int *)malloc(row * col * sizeof(int));
    int m = row, c, d, k, sum = 0;

    for (c = 0; c < m; c++){
        for (d = 0; d < m; d++){
            *(first + c * col + d) = d;
            //first[c][d] = d;
        }
    }

    for (c = 0; c < m; c++){
        for (d = 0; d < m; d++){
            *(second + c * col + d) = d;
            //second[c][d] = d;
        }
    }
    for (c = 0; c < m; c++){
        for (d = 0; d < m; d++){
            for (k = 0; k < m; k++){
                sum = sum + (*(first + c * col + k)) * (*(second + k * col + d));
                //sum = sum + first[c][k] * second[k][d];
            }
            *(multiply + c * col + d) = sum;
            //multiply[c][d] = sum;
            sum = 0;
        }
    }
	free(first);
	free(second);
	free(multiply);
	
}

int main()
{
    int size;
    scanf("%d",&size);  //Inputs
    int sizeTable = 0;
    double total = 0;
    int pid = fork();
	if(pid == 0){
				for(int i = 1;i <= 5;i++){
				sizeTable = size * i;
                clock_t s;
                s = clock();
                sub(sizeTable);
                s = clock() - s;
                double executionTime = ((double)s) / CLOCKS_PER_SEC;
                //total += executionTime;
                printf("Mat Size: %dx%d Execution Time(Subraction): %fs\n", sizeTable, sizeTable, executionTime);
			}
			//printf("Mat Size: %dx%d Execution Time(Subtraction): %f\n", sizeTable, sizeTable, total/10);
			//total = 0;
			return 0;
	}
	else{
		pid = 0;
		pid = fork();
		if(pid == 0){
			for(int i = 1;i <= 5;i++){
				sizeTable = size * i;
		        clock_t a;
		        a = clock();
		        add(sizeTable);
		        a = clock() - a;
		        double executionTime = ((double)a) / CLOCKS_PER_SEC;
		        //total += executionTime;
		        printf("Mat Size: %dx%d Execution Time(Addition): %fs\n", sizeTable, sizeTable, executionTime);
			}
			//printf("Mat Size: %dx%d Execution Time(Addition): %f\n", sizeTable, sizeTable, total/10);
			//total = 0;
			return 0;
		}
		else{
			for(int i = 1; i <= 5; i++){
				sizeTable = size * i;
				clock_t t;
				t = clock();
				matrixMul(sizeTable);
				t = clock() - t;
				double executionTime = ((double)t) / CLOCKS_PER_SEC;
				//total += executionTime;
				printf("Mat Size: %dx%d Execution Time(Multiply): %fs\n", sizeTable, sizeTable, executionTime);

			}
			//printf("Mat Size: %dx%d Execution Time(Multiply): %f\n", sizeTable, sizeTable, total/10);
			//total = 0;
			
			wait(NULL);
			wait(NULL);
			return 0;
		}

	}
}
    
    
    
    
    
    
    
    
    
    
