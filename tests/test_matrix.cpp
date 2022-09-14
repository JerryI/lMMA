#define DEVELOPMENT0

#include "../core.h"

using namespace std;



int main() {

    cout << "Hello!" << '\n';
    init();

    node* op = new node;
    op->type = PLUS;

        node* m1 = new node({{1,0},{-1,0}});
        node* m2 = new node({{10,10},{0,0}});

        node* var = new node;
        var->type = (TYPE)77;

        m1->data.args[0]->data.args[0] = var;
    
    op->data.args.push_back(m1);
    op->data.args.push_back(m2);

    cout << "In:" << '\t' << '\n' << op;
    cout << "Out:" << '\t' << '\n' << evaluate(op);
    cout << "In:" << '\t' << '\n' << op;
}