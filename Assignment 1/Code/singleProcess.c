#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sub(int s)
{
  int r = s, c = s;
  int *arr = (int *)malloc(r * c * sizeof(int));
  int *arr2 = (int *)malloc(r * c * sizeof(int));
  int *arr3 = (int *)malloc(r * c * sizeof(int));

  int i, j;

  for (i = 0; i < r; i++)
  {
    for (j = 0; j < c; j++)
    {
      *(arr + i * c + j) = j;
    }
  }

  for (i = 0; i < r; i++)
  {
    for (j = 0; j < c; j++)
    {
      *(arr2 + i * c + j) = j;
    }
  }

  for (i = 0; i < r; i++)
  {
    for (j = 0; j < c; j++)
    {
      *(arr3 + i * c + j) = *(arr + i * c + j) - *(arr2 + i * c + j);
    }
  }
}

void add(int s)
{
  int r = s, c = s;
  int *arr = (int *)malloc(r * c * sizeof(int));
  int *arr2 = (int *)malloc(r * c * sizeof(int));
  int *arr3 = (int *)malloc(r * c * sizeof(int));

  int i, j;

  for (i = 0; i < r; i++)
  {
    for (j = 0; j < c; j++)
    {
      *(arr + i * c + j) = j;
    }
  }

  for (i = 0; i < r; i++)
  {
    for (j = 0; j < c; j++)
    {
      *(arr2 + i * c + j) = j;
    }
  }

  for (i = 0; i < r; i++)
  {
    for (j = 0; j < c; j++)
    {
      *(arr3 + i * c + j) = *(arr + i * c + j) + *(arr2 + i * c + j);
    }
  }
}
void matrixMul(int size)
{
  int row = size, col = row;
  int *first = (int *)malloc(row * col * sizeof(int));
  int *second = (int *)malloc(row * col * sizeof(int));
  int *multiply = (int *)malloc(row * col * sizeof(int));
  int m = row, c, d, k, sum = 0;

  for (c = 0; c < m; c++)
  {
    for (d = 0; d < m; d++)
    {
      *(first + c * col + d) = d;
      //first[c][d] = d;
    }
  }

  for (c = 0; c < m; c++)
  {
    for (d = 0; d < m; d++)
    {
      *(second + c * col + d) = d;
      //second[c][d] = d;
    }
  }
  for (c = 0; c < m; c++)
  {
    for (d = 0; d < m; d++)
    {
      for (k = 0; k < m; k++)
      {
        sum = sum + (*(first + c * col + k)) * (*(second + k * col + d));
        //sum = sum + first[c][k] * second[k][d];
      }

      *(multiply + c * col + d) = sum;
      //multiply[c][d] = sum;
      sum = 0;
    }
  }
}

int main()
{
  int size;
  scanf("%d", &size); //Inputs
  int sizeTable = 0;
  //double total = 0;
  for (int i = 1; i <= 5; i++)
  {
    sizeTable = size * i;
    clock_t t;
    clock_t a;
    clock_t s;
    t = clock();
    matrixMul(sizeTable);
    t = clock() - t;
    double executionTime = ((double)t) / CLOCKS_PER_SEC;
    a = clock();
    add(sizeTable);
    a = clock() - a;
    double executionTimeadd = ((double)a) / CLOCKS_PER_SEC;
    s = clock();
    sub(sizeTable);
    s = clock() - s;
    double executionTimesub = ((double)s) / CLOCKS_PER_SEC;
    printf("Mat Size: %dx%d Execution Time(Multiply): %f\n", sizeTable, sizeTable, executionTime);
    printf("Mat Size: %dx%d Execution Time(Addition): %f\n", sizeTable, sizeTable, executionTimeadd);
    printf("Mat Size: %dx%d Execution Time(Subraction): %f\n", sizeTable, sizeTable, executionTimesub);
    /*total += executionTime;
        if (i == 10)
        {
            printf("Average Execution Time: %f\n", total / ((double)i));
        }*/
  }
  return 0;
}