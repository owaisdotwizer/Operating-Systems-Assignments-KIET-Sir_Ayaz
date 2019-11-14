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
  
}

int main()
{
  int size;
  scanf("%d", &size); //Inputs
  matrixMul(size);
  while(1){printf("1\n");}
  return 0;
}



















