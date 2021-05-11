#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <time.h>
#include <cpuid.h>
#include <sys/sysinfo.h>
#include <sys/sysctl.h>
#include "resources.h"
#include <unistd.h>

using namespace std;

string GetStdoutFromCommand(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");

    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

unsigned long long getTotalSystemMemory(){
    long pages = sysconf(_SC_PHYS_PAGES);
    // long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * 4000;
}