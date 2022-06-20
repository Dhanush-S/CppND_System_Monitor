#include "ncurses_display.h"
#include "system.h"

#include "format.h"
#include "iostream"


int main() {
    System system;
    NCursesDisplay::Display(system);
}