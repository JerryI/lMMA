#define DEVELOPMENT0

#include "../core.h"
#include "../json.h"

using namespace std;



int main() {

    cout << "Hello!" << '\n';
    init();

    char s[] = "[\"Hold\",[\"Dot\",[\"List\",[\"List\",0,\"I\"],[\"List\",[\"Times\",-1,\"I\"],0]],[\"List\",[\"List\",0,\"I\"],[\"List\",[\"Times\",-1,\"I\"],0]]]]";

    node* op = parse(s);

    cout << "In:" << '\t' << '\n' << op;
    cout << "Out:" << '\t' << '\n' << stringify(evaluate(op)) << '\n';
}