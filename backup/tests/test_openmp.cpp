#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

using namespace std;


int main() {
    
    omp_set_num_threads(4);
    
    cout << "threads: " << omp_get_num_threads();
   
    return 0;

}