#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#include "benchmark.h"
#include "Colours.h"

#define clrscr() system("clear")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

using namespace std;

int processedElems = 0;

int timeval_diff_to_ms( timeval *t_before, timeval *t_after )
{
    timeval l_res;
    timersub( t_after, t_before, &l_res );
    return (int)(1000 * l_res.tv_sec + l_res.tv_usec / 1000);
}

template<typename T>
void splitAndMerge(T list[], int arrayLength, int numThreads, int complete[])
{
    const int len = arrayLength / numThreads;
    printf("test1\n");
    int counts[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        counts[i] = 0;

    }
    printf("test2\n");
    int** arrays = new int*[numThreads];
    printf("test3\n");
    for(int i = 0; i < numThreads; ++i) arrays[i] = new int[len];
    //int arrays[numThreads][arrayLength / numThreads];
    printf("test4\n");
    for (int i = 0; i < numThreads; i++)
    {
        for (int j = 0; j < arrayLength / numThreads; j++)
        {
            arrays[i][j] = list[(i*(arrayLength / numThreads)) + j];
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
        processedElems++;
    }
}

// template<typename T>
// void splitAndMerge(T list[], int arrayLength, int numThreads, int complete[])
// {
//     ofstream myfile;
//     myfile.open ("debug.txt");
    
    
//     int count = 0;
//     for (int i = 0; i < arrayLength; i++)
//     {
//         int index = 0;
//         int min = 10;
//         for (int k = i; k < arrayLength; k++)
//         {
//             if (list[k] <= min)
//             {
//                 min = list[k];
//                 index = k;
//             }
//         }
//         count++;
//         //complete[i] = min;
//         myfile << min << "\n";
//     }
//     myfile.close();
// }

template<typename T>
void selectionSort(T list[], int size, int from, int to)
{
  processedElems = 0;
  int temp, min;
  for (int i = 0; i < size; i++)
  {
    min = size - 1;
    // hledani minima
    for (int j = i; j < size; j++)
    if (list[min] > list[j])
        min = j;
    // prohozeni prvku
    temp = list[min];
    list[min] = list[i];
    list[i] = temp;
    processedElems++;
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

void monitor(int len)
{
    while (processedElems < len)
    {
        printf("\ntest: %d\n", processedElems);
        
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
    cout << "Please insert number of elements " << BRIGHT_BLACK << "(max allowed is 690000 - for memes)" << CRESET << ": ";
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
    th[0] = thread(splitAndMerge<int>,(int*)array, arrayLength, numThreads, (int*)complete);
    monitor(arrayLength);
    th[0].join();
    
    // ofstream myfile;
    // myfile.open ("debug.txt");
    // for (int i = 0; i < arrayLength; i++)
    // {
    //     myfile << array[i] << "\n";
    // }
    
    // myfile.close();
    gettimeofday( &l_time_after, nullptr );
    //printArray(array, arrayLength, "Sorted.", arrayLength / numThreads);
    //printArray(complete, arrayLength, "Complete.", arrayLength / numThreads);
    gotoxy(1,15);
    printf( "The search time: %d [ms]\n", timeval_diff_to_ms( &l_time_before, &l_time_after ) );
    benchmark->previousBenchmarkTime = timeval_diff_to_ms( &l_time_before, &l_time_after );
    benchmark->previousBenchmarkType = 1;
    benchmark->previousBenchmarkOther1 = arrayLength;
    benchmark->previousBenchmarkOther2 = numThreads;
    sleep(2);
    clrscr();
}