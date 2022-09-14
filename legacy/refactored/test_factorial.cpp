#include <chrono>

#define DEVELOPMENT0

#include "reuse.h"

using namespace std;
using namespace std::chrono; 

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
    fun->data.id = 1;
    fun->data.args.push_back(pattern);
    pattern->type = PATTERN;
    pattern->data.args.push_back(var);
    var->type = VAR;
    var->data.id = 11;

    dt->type = COND;

    node* eq = new node;
    eq->type = EQUALS;

        node* zero = new node;
        zero->type = INTEGER;
        zero->data.i = 0;

        node* zerocmp = new node;
        zerocmp->type = VAR;
        zerocmp->data.id = 11;

        eq->data.args.push_back(zero);
        eq->data.args.push_back(zerocmp);



    node* argvar = new node;
    argvar->type = INTEGER;
    argvar->data.i = 1;

    node* timesh = new node;
    timesh->type = TIMES;
        node* timevar = new node;
        timevar->type = VAR;
        timevar->data.id = 11;

        node* timefunc = new node;
        timefunc->type = FUN;
        timefunc->data.id = 1;

            node* plush = new node;
            plush->type = PLUS;
                node* plus1 = new node(-1);
                node* plus2 = new node;
                plus2->type = VAR;
                plus2->data.id = 11;
            plush->data.args.push_back(plus1);
            plush->data.args.push_back(plus2);


        timefunc->data.args.push_back(plush);

        timesh->data.args.push_back(timevar);
        timesh->data.args.push_back(timefunc);

    dt->data.args.push_back(eq);
    dt->data.args.push_back(argvar);
    dt->data.args.push_back(timesh);

    cout << "::define function::" << '\n' << '\n';

    cout << set;
    cout << '\n' << "::out::"<< '\n' << '\n';
    
    cout << evaluate(set,library);

    //cout << '\n' << "::call factorial::" << '\n' << '\n';
    auto start = high_resolution_clock::now();
    for (long int i=0; i<65536; ++i) {
        node* eval = new node;
        eval->type = FUN;
        eval->data.id = 1;
        eval->data.args.push_back(new node(10));
        //you need better isolation for local vars
        //cout << eval;
        //cout << '\n' << "::out::" << '\n' << '\n';
     
        eval = evaluate(eval, library);
        //cout << eval;
    }
    auto clk = high_resolution_clock::now() - start;
    
    cout << "time eslaped: " << duration_cast<seconds>(clk).count();
}