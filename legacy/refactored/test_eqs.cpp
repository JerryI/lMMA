#define DEVELOPMENT6

#include "reuse.h"

using namespace std;

vector<node*> library;

int main() {
    cout << "Hello!" << '\n';

    node* condf = new node();
    condf->type = COND;

    node* eq1 = new node(10);
    node* eq2 = new node(-10);

    node* eq = new node();

    node* s1 = new node(1);
    node* s2 = new node(-1);

    eq->type = EQUALS;
    eq->data.args.push_back(s1);
    eq->data.args.push_back(s2);

    condf->data.args.push_back(eq);
    condf->data.args.push_back(eq1);
    condf->data.args.push_back(eq2);

    cout << evaluate(condf, library);


   
}