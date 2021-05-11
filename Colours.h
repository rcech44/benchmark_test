#pragma once
//format "\x1B[Fm"	for just foreground color ("F" == "FOREGROUND_COLOR")
//format "\x1B[F;Bm"	for foreground and background color ("F" == "FOREGROUND_COLOR", "B" == "BACKGROUND_COLOR")

//Default colors
#define BLACK "\x1B[30m"	//(0,0,0)
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"	//==LIGHT GREY

//Bright variations
#define BRIGHT_BLACK "\x1B[90m" //==DARK GREY
#define BRIGHT_RED "\x1B[91m"
#define BRIGHT_GREEN "\x1B[92m"
#define BRIGHT_YELLOW "\x1B[93m"
#define BRIGHT_BLUE "\x1B[94m"
#define BRIGHT_MAGENTA "\x1B[95m" //==PINK
#define BRIGHT_CYAN "\x1B[96m"
#define BRIGHT_WHITE "\x1B[97m"	//BRIGHT WHITE (255,255,255)

//Console color reset
#define CRESET "\033[0m"