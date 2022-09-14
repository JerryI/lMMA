#define DEVELOPMENT

#include "reuse.h"

using namespace std;

vector<node*> library;

int main() {
    cout << "Hello!" << '\n';

    node* op = new node;

    node* var1 = new node();
    var1->type = VAR;
    var1->data.id = 8644;
    node* var2 = new node({{1,0},{0,1}});



    op->type = TIMES;
    op->data.args.push_back(var1);
    op->data.args.push_back(var2);

    

    cout << "testing the times" << '\n' << op << '\n';
    op = evaluate(op, library);
    cout << "out" << '\n' << op;

}