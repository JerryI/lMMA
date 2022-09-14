#define DEVELOPMENT

#include "core.h"


using namespace std;


int main() {
    cout << "Hello!" << '\n';

    node* tree = new node;

    tree->type = DEF;

    node* op = new node;
    node* val = new node(12);
    node* var = new node;
    node* pattern = new node;
    node* body = new node;

    op->type = (TYPE)17;


    pattern->type = PATTERN;
    pattern->data.args.push_back(var);
    op->data.args.push_back(pattern);
    var->type = (TYPE)6;

    body->type = (TYPE)6;

    tree->data.args.push_back(op);
    tree->data.args.push_back(body); 

    init();

    cout << "define a float variable" << '\n' << tree << '\n';
    cout << evaluate(tree);


    node* func = new node;
    func->type = (TYPE)17;
    func->data.args.push_back(val);

    cout << "call it "<<'\n' << func;
    cout << evaluate(func);
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