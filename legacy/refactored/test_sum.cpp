#define DEVELOPMENT5

#include "reuse.h"

using namespace std;

vector<node*> library;

int main() {
    cout << "Hello!" << '\n';

    node* op = new node;
    node* val1 = new node;
    node* val2 = new node;
    node* val3 = new node;
    node* val4 = new node;

    node* matrix1 = new node;
    node* matrix2 = new node;
    node* m11 = new node;
    node* m12 = new node;
    node* m21 = new node;
    node* m22 = new node;

    op->type = PLUS;

    val1->type = INTEGER;
    val1->data.i = 3;

    val2->type = INTEGER;
    val2->data.i = -3; 

    val3->type = FLOAT;
    val3->data.f = 0.77f; 

    val4->type = FLOAT;
    val4->data.f = 0.9f;

    matrix1->type = LIST;
    matrix1->data.args.push_back(m11);
    matrix1->data.args.push_back(m12);

    matrix2->type = LIST;
    matrix2->data.args.push_back(m22);
    matrix2->data.args.push_back(m21);    

    m11->type = INTEGER;
    m11->data.i = -1;
    m12->type = INTEGER;
    m12->data.i = 1; 
    m22->type = INTEGER;
    m22->data.i = -1;
    m21->type = INTEGER;
    m21->data.i = 1;        

    op->data.args.push_back(val1);
    op->data.args.push_back(val2);
    op->data.args.push_back(val3);
    op->data.args.push_back(val4);
    op->data.args.push_back(matrix1);
    op->data.args.push_back(matrix2);
    

    cout << "testing the sum" << '\n' << op << '\n';
    op = evaluate(op, library);
    cout << "out" << '\n' << op;

}