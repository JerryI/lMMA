#define DEVELOPMENT6

#include "reuse.h"

using namespace std;

vector<node*> library;

int main() {
    cout << "Hello!" << '\n';

    node* set = new node();
    node* var = new node();
    node* fun = new node();
    node* pattern = new node();
    node* dt = new node();

    set->type = DEF;
    set->data.args.push_back(fun);
    set->data.args.push_back(dt);

    fun->type = FUN;
    fun->data.id = 19;
    fun->data.args.push_back(pattern);
    pattern->type = PATTERN;
    pattern->data.args.push_back(var);
    var->type = VAR;
    var->data.id = 23;

    node* var2 = new node;
    node* var3 = new node(10000);

    dt->type = TIMES;
    dt->data.args.push_back(var2);
    dt->data.args.push_back(var3);
    var2->type = VAR;
    var2->data.id = 23;


    

    cout << "testing the setfunc" << '\n' << set << '\n';
    node* ev = evaluate(set, library);
    cout << "out" << '\n' << ev;

    /*cout << "show library" << '\n';
    for (int i=0; i<library.size(); ++i) {
        cout << "------------------------------" << '\n';
        cout << library[i];
    }*/

   

    for (int h=0; h<3; ++h) {
    cout << '\n' << "call a function" << '\n';
    node* val = new node(h);

    node* fc = new node;
    fc->type = FUN;
    fc->data.args.push_back(val);
    fc->data.id = 19;
    cout << fc;

    fc = evaluate(fc,library);
    cout << "out" << '\n';
    cout << fc;
    }
   
}