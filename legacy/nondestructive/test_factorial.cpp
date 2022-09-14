#include <chrono>

#define DEVELOPMENT0

#include "core.h"

using namespace std;
using namespace std::chrono; 

node* library[100];

int main() {
    for (int i =0; i<100; ++i) {
        library[i] = nullptr;
    }
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
    fun->data.id = 0;
    fun->data.args.push_back(pattern);
    pattern->type = PATTERN;
    pattern->data.args.push_back(var);
    var->type = VAR;
    var->data.id = 0;

    dt->type = COND;

    node* eq = new node;
    eq->type = EQUALS;

        node* zero = new node;
        zero->type = INTEGER;
        zero->data.i = 0;

        node* zerocmp = new node;
        zerocmp->type = VAR;
        zerocmp->data.id = 0;

        eq->data.args.push_back(zero);
        eq->data.args.push_back(zerocmp);



    node* argvar = new node;
    argvar->type = INTEGER;
    argvar->data.i = 1;

    node* timesh = new node;
    timesh->type = TIMES;
        node* timevar = new node;
        timevar->type = VAR;
        timevar->data.id = 0;

        node* timefunc = new node;
        timefunc->type = FUN;
        timefunc->data.id = 0;

            node* plush = new node;
            plush->type = PLUS;
                node* plus1 = new node(-1);
                node* plus2 = new node;
                plus2->type = VAR;
                plus2->data.id = 0;
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
    node* eval = new node;
    eval->type = FUN;
    eval->data.id = 0;
    node* variable = new node(10);
    watch = variable;
    
    eval->data.args.push_back(variable);
    node* res = nullptr;



    for (long int i=0; i<65535; ++i) {

        //you need better isolation for local vars
        //cout << eval;
        //cout << '\n' << "::out::" << '\n' << '\n';

        res = evaluate(eval, library);
        
    }
    auto clk = high_resolution_clock::now() - start;
    
    cout << "time eslaped: " << duration_cast<milliseconds>(clk).count() << '\n';
    cout << res;
    cout << eval;
    return 0;
    //cout << "library" << '\n';
    //for (int i=0; i<library.size(); ++i) {
    //    cout << library[i] << '\n';
    //}
}