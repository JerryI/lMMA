#define DEVELOPMENT

#include "core.h"


using namespace std;


int main() {
    cout << "Hello!" << '\n';

    node* tree = new node;

    tree->type = SET;

    node* op = new node;
    node* val = new node;

    op->type = (TYPE)17;
    op->data.id = 777;

    val->type = FLOAT;
    val->data.f = 0.89;

    tree->data.args.push_back(op);
    tree->data.args.push_back(val); 

    init();

    cout << "define a float variable" << '\n' << tree << '\n';
    cout << evaluate(tree);
    
    cout << "getting the variables from " << '\n';
    cout << evaluate(op);

    //node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size
    /*tree = evaluate(tree, library);
    cout << "out" << '\n' << tree;

    val = new node;
    val->type = VAR;
    val->data.id = 777;

    cout << "get variable" << '\n' << val;
    val = evaluate(val, library);
    cout << "out" << '\n' << val;*/
}