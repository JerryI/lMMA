#define DEVELOPMENT0

#include "core.h"
#include "json.h"

using namespace std;



int main() {

    cout << "Hello!" << '\n';
    init();

    char s[] = "[\"Hold\",[\"ReplaceAll\",[\"Plus\",\"x\",[\"Subscript\",\"Î»\",\"u\"]],[\"List\",[\"Rule\",[\"Subscript\",\"Î»\",\"u\"],\"\
   U\"]]]]";
    node* in = parse(s);
    cout << "IN: " << in << '\n';
    cout << "OUT: " << evaluate(in);
   
    return 0;

}