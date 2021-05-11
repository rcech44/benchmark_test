#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <chrono>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#include "benchmark.h"
#include "Colours.h"

#define clrscr() system("clear")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

using namespace std;

int timeval_diff_to_ms( timeval *t_before, timeval *t_after )
{
    timeval l_res;
    timersub( t_after, t_before, &l_res );
    return (int)(1000 * l_res.tv_sec + l_res.tv_usec / 1000);
}

template<typename T>
void splitAndMerge(T list[], int arrayLength, int numThreads, int complete[])
{
    int counts[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        counts[i] = 0;

    }
    int arrays[numThreads][arrayLength / numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        for (int j = 0; j < arrayLength / numThreads; j++)
        {
            *(*(arrays+i)+j) = list[(i*(arrayLength / numThreads)) + j];
        }
    }
    for (int i = 0; i < arrayLength; i++)
    {
        int index = 0;
        int min = 10;
        for (int k = 0; k < numThreads; k++)
        {
            if (arrays[k][counts[k]] <= min && counts[k] < arrayLength / numThreads)
            {
                min = arrays[k][counts[k]];
                index = k;
            }
        }
        counts[index]++;
        complete[i] = min;
    }
}

template<typename T>
void selectionSort(T list[], int size, int from, int to)
{
  int temp, min;
  for (int i = 0; i < (size - 1); i++)
  {
    min = size - 1;
    // hledani minima
    for (int j = i; j < (size - 1); j++)
    if (list[min] > list[j])
        min = j;
    // prohozeni prvku
    temp = list[min];
    list[min] = list[i];
    list[i] = temp;
  }
}

template<typename T>
void bubbleSort(T array[], int size, int from, int to)
{
    for(int i = from; i < to; i++)
    {
        for(int j = from; j < (to - 1); j++)
        {
            if(array[j] > array[j + 1])
            {
                    int temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
            }
        }
    }
}

void printArray(int arr[], int n, string string, int part)
{
    cout << string << endl;
    
    for (int i = 0; i < n; i++)
    {
        if (i % part == 0 && i != 0)
        {
            std::cout << "| ";
        }
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

template<typename T>
void fillArray(T array[], int numThreads, int arrayLength, int part)
{
    for (int j = 0; j < (arrayLength / numThreads); j++)
    {
        // array[(i*(arrayLength / numThreads)) + j] = i;     // setrizene pole
        if (((part*(arrayLength / numThreads)) + j) <= arrayLength)
        {
            array[(part*(arrayLength / numThreads)) + j] = rand() % 10;
        }
        // array[9] = 10;
    }
}

int launchB1(Benchmark* benchmark)
{  
    clrscr();
    gotoxy(114, 30);
    cout << BRIGHT_BLACK << "ver 0.1" << CRESET;
    gotoxy(1,1);
    srand(42);
    timeval l_time_before;
	timeval l_time_after;
    int arrayLength = 0;
    int numThreads = 0;
    cout << "Please insert number of elements " << BRIGHT_BLACK << "(max allowed is 650000)" << CRESET << ": ";
    cin >> arrayLength;
    cout << "Please insert number of threads: " << BRIGHT_BLACK << "(max allowed is 12)" << CRESET << ": ";
    cin >> numThreads;
    if (arrayLength % numThreads != 0)
    {
        printf("-----------------------------------\nDetected invalid parameters, trying to repair...\n");
        printf("Invalid parameters: %d and %d\n", arrayLength, numThreads);
        int mod = arrayLength % numThreads;
        arrayLength -= mod;
        printf("Repaired parameters: %d and %d\n", arrayLength, numThreads);
    }
    int array[arrayLength];
    int complete[arrayLength];
    thread th[numThreads];
    gettimeofday( &l_time_before, nullptr );
    for (int i = 0; i < numThreads; i++)
    {
        th[i] = thread(fillArray<int>,(int*)array, numThreads, arrayLength, i);
    }
    for (int i = 0; i < numThreads; i++)
    {
        th[i].join();
    }
    //printArray(array, arrayLength, "Unsorted.", arrayLength / numThreads);
    for (int i = 0; i < numThreads; i++)
    {
        th[i] = thread(bubbleSort<int>, (int*)array, arrayLength, i*(arrayLength / numThreads), (i*(arrayLength / numThreads))+(arrayLength / numThreads));
    }
    for (int i = 0; i < numThreads; i++)
    {
        th[i].join();
    }
    cout << CRESET << endl << endl;
    splitAndMerge<int>((int*)array, arrayLength, numThreads, complete);
    gettimeofday( &l_time_after, nullptr );
    //printArray(array, arrayLength, "Sorted.", arrayLength / numThreads);
    //printArray(complete, arrayLength, "Complete.", arrayLength / numThreads);
    printf( "The search time: %d [ms]\n", timeval_diff_to_ms( &l_time_before, &l_time_after ) );
    benchmark->previousBenchmarkTime = timeval_diff_to_ms( &l_time_before, &l_time_after );
    benchmark->previousBenchmarkType = 1;
    benchmark->previousBenchmarkOther1 = arrayLength;
    benchmark->previousBenchmarkOther2 = numThreads;
    sleep(2);
    clrscr();
}