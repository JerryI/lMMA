#define DEVELOPMENT5

#include "reuse.h"

using namespace std;

vector<node*> library;

int main() {
    cout << "Hello!" << '\n';

    node* op = new node;

    node* matrix1 = new node({{1.0,3.0},{2.0,3.0}});
    node* matrix2 = new node({{2,7},{4,5}});


    op->type = PLUS;
     

    op->data.args.push_back(matrix1);
    op->data.args.push_back(matrix2);

    

    cout << "testing the sum" << '\n' << op << '\n';
    op = evaluate(op, library);
    cout << "out" << '\n' << op;

}