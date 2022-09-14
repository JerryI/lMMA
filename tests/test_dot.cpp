#define DEVELOPMENT0

#include "core.h"

using namespace std;



int main() {

    cout << "Hello!" << '\n';
    init();

    node* op = new node;
    op->type = DOT;

        node* m1 = new node({{1,2},{3,4}});
        node* m2 = new node({{0,1},{1,0}});

        node* var = new node;
        var->type = (TYPE)77;

        //m2->data.args[0]->data.args[0] = var;
    
    op->data.args.push_back(m1);
    op->data.args.push_back(m2);

    cout << "In:" << '\t' << '\n' << op;
    cout << "Out:" << '\t' << '\n' << evaluate(op);

}