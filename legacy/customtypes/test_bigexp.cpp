#define DEVELOPMENT0

#include "core.h"
#include "json.h"

using namespace std;



int main() {

    cout << "Hello!" << '\n';
    init();

    char s[] = "[\"Hold\",[\"ReplaceAll\",[\"Plus\",\"x\",\"y\",\"z\"],[\"List\",[\"Rule\",\"z\",1]]]]";
    node* in = parse(s);
    cout << "IN: " << in << '\n';
    cout << "OUT: " << evaluate(in);
   
    return 0;

}