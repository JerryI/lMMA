#define DEVELOPMENT0

#include "core.h"
#include "json.h"

using namespace std;



int main() {

    cout << "Hello!" << '\n';
    init();

    char s[] = "[\"Hold\",[\"CompoundExpression\",[\"Set\",\"hamiltonian\",[\"Plus\",[\"Dot\",\"Sx\",\"Sx\"],[\"Times\",-1,[\"Subscript\",\"Î»\\
   ",\"Fe\"]]]],null]]"; 
    node* in = parse(s);
    cout << "IN: " << in << '\n';
    evaluate(in);
    cout << "reading hamiltonian..."<< '\n';
    char l[] = "[ \"hamiltonian\" ]";
    node* in2 = parse(l);
    cout << "OUT: " << evaluate(in2);
   
    return 0;

}