#include <stdlib.h>
int main(){
    system("rm -rf build/ && cmake -S ./ -B build/ && cmake --build build/");
}