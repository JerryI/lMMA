#define DEVELOPMENT0

#include "../core.h"
#include "../json.h"

using namespace std;



int main() {

    cout << "Hello!" << '\n';
    init();

    char s[] = "[\"Hold\",[\"Normalize\",[\"List\",\"x\",\"y\",\"z\"]]]"; 
    node* in = parse(s);
    cout << "IN: " << in << '\n';
    cout << "OUT: " << evaluate(in);
   
    return 0;

}