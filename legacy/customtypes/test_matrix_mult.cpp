#define DEVELOPMENT0

#include "core.h"

using namespace std;



int main() {

    cout << "Hello!" << '\n';
    init();

    node* op = new node;
    op->type = TIMES;

        node* m1 = new node({{1,0},{-1,0}});

        node* var = new node(0);
        //var->type = (TYPE)77;
    
    op->data.args.push_back(m1);
    op->data.args.push_back(var);

    cout << "In:" << '\t' << '\n' << op;
    cout << "Out:" << '\t' << '\n' << evaluate(op);

}