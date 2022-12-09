#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 10240
#define NUM_THREADS 10

int count[NUM_THREADS] = { 0 };
int n1,n2;
char *s1,*s2;
FILE *fp;

int readf(FILE *fp) {
    if((fp=fopen("strings.txt", "r"))==NULL){
        printf("ERROR: can't open string.txt!\n");
        return 0;
    }

    s1=(char *)malloc(sizeof(char)*MAX);
    if(s1==NULL){
        printf("ERROR: Out of memory!\n");
        return -1;
    }

    s2=(char *)malloc(sizeof(char)*MAX);
    if(s2==NULL){
        printf("ERROR: Out of memory\n");
        return -1;
    }

    /*read s1 s2 from the file*/
    s1=fgets(s1, MAX, fp);
    s2=fgets(s2, MAX, fp);
    n1=strlen(s1);  /*length of s1*/
    n2=strlen(s2)-1; /*length of s2*/

    if(s1==NULL || s2==NULL || n1<n2) {  /*when error exit*/
        return -1;
    }

    return 0;
}

void* substring_count(void* args) {
    int value = *(int*)args;
    int i, j, k, flag;

    for (i = 0 + value; i < n1; i = i + NUM_THREADS) {
        flag = 0;
        k = i;

        for (j = 0; j < n2; j++) {
            // flag=0;
            if (s2[j] == s1[k])
                k++;
            else {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
            count[value] += 1;
    }
}

int main(int argc, char *argv[])
{
    readf(fp);
    int sum = 0;
    int x[NUM_THREADS];
    for (int a = 0; a < NUM_THREADS; a++)
        x[a] = a;
    int i, l1;

    pthread_t tid[NUM_THREADS];

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tid[i], NULL,
                       substring_count,
                       (void*)&x[i]);
    }
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(tid[i], NULL);
    for (i = 0; i < NUM_THREADS; i++) {
        printf("The number of substrings that thread %i found is: %i\n", i, count[i]);
        sum = sum + count[i];
    }
    printf("Frequency of substring: %i\n", sum);
    return 0;
}


