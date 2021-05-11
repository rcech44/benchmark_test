#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <time.h>
#include <cpuid.h>
#include "benchmark.h"
#include "resources.h"
#include <sys/types.h>

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define clrscr() system("clear");
#define CONSOLE_WIDTH 120
#define CONSOLE_HEIGHT 30

using namespace std;

int main()
{
    string CPUModel = GetStdoutFromCommand("lscpu | grep 'Model name' | cut -f 2 -d \":\" | awk '{$1=$1}1'");
    string CPUCores = GetStdoutFromCommand("lscpu | grep 'CPU(s):' | cut -f 2 -d \":\" | awk '{$1=$1}1'");
    string CPUFrequency = GetStdoutFromCommand("lscpu | grep 'CPU MHz' | cut -f 2 -d \":\" | awk '{$1=$1}1'");
    Benchmark* benchmark = new Benchmark(CONSOLE_WIDTH, CONSOLE_HEIGHT, CPUModel, getTotalSystemMemory(), CPUCores, CPUFrequency);

    benchmark->mainMenu();
}

