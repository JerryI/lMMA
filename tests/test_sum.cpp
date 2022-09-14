#include <chrono>
#include <omp.h>
#define DEVELOPMENT0

#define SPEED_EXP_OPT1

#include "../core.h"
#include "../json.h"

using namespace std;
using namespace std::chrono; 



int main() {

    cout << "Hello!" << '\n';
    init();
    auto start = high_resolution_clock::now();
    char s[] = "[\"Hold\",[\"Sum\",[\"Exp\",[\"Times\",-1,[\"f\",\"i\"]]],[\"List\",\"i\",1,5]]]";
    node* in = parse(s);
    cout << "IN: " << in << '\n';
    node* res = evaluate(in);
    
    cout << "OUT: " << stringify(res) << '\n';
    auto clk = high_resolution_clock::now() - start;
    
    cout << "time eslaped: " << duration_cast<milliseconds>(clk).count() << '\n';
    cout << "threads: " << omp_get_num_threads();
   
    return 0;

}