#define DEVELOPMENT0

#include "core.h"
#include "json.h"

using namespace std;



int main() {

    cout << "Hello!" << '\n';
    init();

    char s[] = "[\"Hold\",[\"CompoundExpression\",[\"Set\",\"i\",0],[\"Do\",[\"CompoundExpression\",[\"Set\",\"i\",[\"Plus\",\"i\",1]],null\
   ],[\"List\",\"j\",1,5],[\"List\",\"k\",1,2]],\"i\"]]"; 
    node* in = parse(s);
    cout << "IN: " << in << '\n';
    cout << "OUT: " << evaluate(in);
   
    return 0;

}