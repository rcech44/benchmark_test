#include "benchmark.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <time.h>
#include <cpuid.h>
#include <string>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "Colours.h"
#include "benchmark1.h"
//#include "benchmark2.h"

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define clrscr() system("clear")
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;

void changemode(int);
int  kbhit(void);

Benchmark::Benchmark(int w, int h, std::string model, unsigned long long mem, std::string cores, std::string freq){
    this->consoleWidth = w;
    this->consoleHeight = h;
    this->CPUModel = model;
    this->CPUCores = cores;
    this->CPUFrequency = freq;
    this->memory = mem;
    this->floatMemory = mem / 1000000000;
    this->CPUFrequency.pop_back();
    this->CPUCores.pop_back();
    this->previousBenchmarkTime = 0;
    this->previousBenchmarkType = 0;
    this->previousBenchmarkOther1 = 0;
    this->previousBenchmarkOther2 = 0;
}

void Benchmark::previousResults(int time, int type, int other1, int other2)
{
    if (type != 0)
    {
        gotoxy(1, 16);
        if (type == 1)
        {
            char c = 201;
            gotoxy(1, 16);
            cout << BRIGHT_BLACK;
            for (int i = 1; i < 50; i++)
            {
                gotoxy(i, 16);
                printf("*");
                gotoxy(i, 22);
                printf("*");
            }
            for (int i = 1; i < 8; i++)
            {
                gotoxy(1, i+15);
                printf("*");
                gotoxy(49, i+15);
                printf("*");
            }
            gotoxy(3, 17);
            cout << BRIGHT_BLACK << "Previous benchmark: " << endl;
            gotoxy(3, 18);
            cout << "  -> type: " << "multithreaded bubblesort (1)" << endl;
            gotoxy(3, 19);
            cout << "  -> time: " << time << " ms" << endl;
            gotoxy(3, 20);
            cout << "  -> number of elements: " << other1 << endl;
            gotoxy(3, 21);
            cout << "  -> number of threads: " << other2 << CRESET << endl;
        }
    }
}

void Benchmark::printOptions(int options, int param){
    if (options == 1)
    {
        gotoxy(1,10);
        if (param == 0)
        {
            cout << CRESET << "1. Start benchmark 1    " << endl;
            cout << "2. Start benchmark 2   " << endl;
            cout << "3. Third option    " << endl;
            cout << "4. Exit    " << endl;
        }
        else if (param == 1)
        {
            cout << CYAN << " -> 1. Start benchmark 1 - launches multithreaded benchmark" << CRESET << endl;
            cout << "2. Start benchmark 2                                                   " << endl;
            cout << "3. Third option                                                   " << endl;
            cout << "4. Exit                                                   " << endl;
        }
        else if (param == 2)
        {
            cout << CRESET << "1. Start benchmark 1                                                   " << endl;
            cout << CYAN << " -> 2. Start benchmark 2 - here will be option info" << CRESET << endl;
            cout << "3. Third option                                              " << endl;
            cout << "4. Exit                                              " << endl;
        }
        else if (param == 3)
        {
            cout << CRESET << "1. Start benchmark 1                                                   " << endl;
            cout << "2. Start benchmark 2                                                   " << endl;
            cout << CYAN << " -> 3. Third option - here will be option info" << CRESET << endl;
            cout << "4. Exit                                        " << CRESET << endl;
        }
        else if (param == 4)
        {
            cout << CRESET << "1. Start benchmark 1                                                   " << endl;
            cout << "2. Start benchmark 2                                                   " << endl;
            cout << "3. Third option                                " << CRESET << endl;
            cout << CYAN << " -> 4. Exit                                        " << CRESET << endl;
        }
    }
}

void Benchmark::mainMenu()
{
    clrscr();
    gotoxy(1,1);
    cout << CRESET << "Welcome to ultimate benchmarking tool." << endl; 
    cout << "System info: " << endl << "\t-> CPU: " << YELLOW << this->CPUModel << CRESET << "\t-> Number of threads: " << YELLOW << this->CPUCores << CRESET << endl << "\t-> CPU base frequency: " << YELLOW << this->CPUFrequency << " MHz" << CRESET << endl;
    cout << "\t-> Total memory: " << YELLOW;
    printf("%.1f GB\n\n", this->floatMemory);
    cout << CRESET << "Please choose option by pressing number and confirm with 'Enter':" << endl << endl;
    this->printOptions(1, 0);
    this->previousResults(this->previousBenchmarkTime, this->previousBenchmarkType, this->previousBenchmarkOther1, this->previousBenchmarkOther2);
    gotoxy(114, 30);
    cout << BRIGHT_BLACK << "ver 0.1";
    gotoxy(1,10);
    int ch;
    bool inputActive = true;
    bool exit = false;
    int option = 0;
    while (inputActive)
    {
        inputActive = true;
        changemode(1);
        ch = getchar();
        switch (ch)
        {
            case 49:
                this->printOptions(1, 1);
                option = 1;
                break;
            case 50:
                this->printOptions(1, 2);
                option = 2;
                break;
            case 51:
                this->printOptions(1, 3);
                option = 3;
                break;
            case 52:
                this->printOptions(1, 4);
                option = 4;
                break;
            case 10:
                inputActive = false;
                break;
        }
        while ( !kbhit() && inputActive == true)
        {
            usleep(100);
        }

        changemode(0);
    }
    switch(option)
    {
        case 1:
            launchB1(this);
            printf("\nCompleted benchmark.\n");
            break;
        case 2:
            //launchB2(this);
            printf("\nCompleted benchmark.\n");
            break;
        case 4:
            clrscr();
            printf("See you soon!\n");
            exit = true;
            break;
    }
    if(exit == false)
    {
        this->mainMenu();
    }
}

void changemode(int dir){
  static struct termios oldt, newt;

  if ( dir == 1 )
  {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

int kbhit (void){
  struct timeval tv;
  fd_set rdfs;

  tv.tv_sec = 0;
  tv.tv_usec = 0;

  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);

  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);

}