#define DEVELOPMENT

#include "reuse.h"

using namespace std;

vector<node*> library;

int main() {
    cout << "Hello!" << '\n';

    node* tree = new node;

    tree->type = SET;

    node* op = new node;
    node* val = new node;

    op->type = VAR;
    op->data.id = 777;

    val->type = FLOAT;
    val->data.f = 0.89;

    tree->data.args.push_back(op);
    tree->data.args.push_back(val); 

    cout << "define a float variable" << '\n' << tree << '\n';
    tree = evaluate(tree, library);
    cout << "out" << '\n' << tree;

    val = new node;
    val->type = VAR;
    val->data.id = 777;

    cout << "get variable" << '\n' << val;
    val = evaluate(val, library);
    cout << "out" << '\n' << val;
}