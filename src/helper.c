#include "helper.h"
#include "structs.h"

unsigned short rbgToColour(int r, int g, int b){
    return (r << 11) | (g << 5) | b;
}



void swap(int *a, int *b){
    int c = *b;
    *b = *a;
    *a = c;

}

