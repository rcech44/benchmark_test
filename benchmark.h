#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>

using namespace std;

class Benchmark
{
    public:
        int consoleWidth;
        int consoleHeight;
        std::string CPUModel;
        std::string CPUCores;
        std::string CPUFrequency;
        unsigned long long memory;
        int previousBenchmarkTime;
        int previousBenchmarkType;
        int previousBenchmarkOther1;
        int previousBenchmarkOther2;
        float floatMemory;
        Benchmark(int w, int h, string s, unsigned long long mem, string cores, string freq);
        void mainMenu();
        void printOptions(int options, int param);
        void previousResults(int time, int type, int other1, int other2);
};

