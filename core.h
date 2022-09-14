#include <vector>
#include <iostream>
#include <cmath>
#include <functional>

using namespace std;

#include "shared.h"

#define NOEVALUATION 0b00000001
#define NUMERIZE     0b00000010

//TO-DO
//--NEW PROTOTY OF THE FUNCTION, WHICH HAS FLAGS
//node* anyfunction(node* itself, vector<node*> &args, definition* library, bool evaluate = true, bool numerize = false)

//--results caching (when the same operation evals all over again)
//--bgarbage collector
//--implement soultion with Times, Plus and variables like  3x - 6x == -3x ex.
//--DOT operation written on this language just for fun :0
//--N operator, optimizes all numerical data to xtensor arrays with BLAS
//--reqursive destructor of the class node

definition library[1088];

node* evaluate(node *data, definition* library = library, char params = 0);

/**hold function. do nothing by now**/
node* hold(node* itself, vector<node*> &args, definition* library, char params) {
    return(evaluate(args[0], library, params));
}

/**list function. One of the most important**/
node* list(node* itself, vector<node*> &args, definition* library, char params) {
    //seems to be slow
    //recreate to update definitions
    node* res = new node();
    //! WARNING: memory leaking
    res->type = LIST;
    for (int i=0; i<args.size(); ++i) {
        res->data.args.push_back(evaluate(args[i],library, params));
    }
    return res;
}

//14.05.2021
//Rule matrix
/*
type    0   1   2   3   4   5
        1   func1
        2   func3 
        3
        4
        5



*/







//29.03.2021
//the paradigm of functions:
//1. use delete[] and new[] as less as possible. Only, when new things come from the process of evaluation
//2. return itself, when the inner data is unevaluatable

//too many arguments, i believe
//(node, args, lib)
// ^      ^     ^   
// the main object and the function itself (by the type)
//        |     |   
//     its arguements 
//              |
//         the library with data of the all defined varibles and functions
//
//ha, but convinient to use... So, we shall see

//We should not copy function! Clonning is slow as shit
//do like c++ compilers

/**call defined variable**/
node* prototype_var(node* itself, vector<node*> &args, definition* library, char params) {
    #ifdef DEVELOPMENT
        cout << "prototype variable" << '\n';
    #endif

    //evaluate from the library by ID (type)
    return evaluate(library[itself->type].data, library, params); 
}

/**call defined shared variable**/
node* prototype_var_shared(node* itself, vector<node*> &args, definition* library, char params) {
    //send get command and wait... 
}

//paterns science

/**declare variables**/
node* set(node* itself, vector<node*> &args, definition* library, char params) {
    #ifdef DEVELOPMENT
        cout << "setting the varuable to addr " << args[0]->type << '\n';
    if (args.size() != 2) 
        throw runtime_error("SET operator must have two arguments!");
    #endif
    
    //check if shared variable
    //if ()
    //----
    switch (args[0]->type)
    {
    case PART: {
            //**modify the specific element of stored list
            //recreate the list, don't overwrite, because it belongs to the source expression
            //cost: 1 clonning, 1 deleting, 2 evalutions.

            //get the id and clone
            int t = args[0]->data.args[0]->type;
            library[t].data = library[t].data->clone();    
            //! WARNING: memory leaking
            //Replace operation is preferable
            
            //now we can remove the old one
            //! WARNING: slow?
            node* iterator = evaluate(args[0]->data.args[1],library, params);
            if (iterator->type != INTEGER) return itself;

            int pos = iterator->data.i-1;
            //delete library[t].data->data.args[pos]; //Only head
            //! WARNING: need a garbage collector
            //assign new one
            library[t].data->data.args[pos] = evaluate(args[1], library, params);

            return library[t].data->data.args[pos]; }
        break;
    
    case SUB: {
            //**rewrite the specific subscript of the element
            //cost: as hell
            //cout << "sub type";
            //get the primary and secondary symbols (evaluated)
            int primary = args[0]->data.args[0]->type;
            int secondary = evaluate(args[0]->data.args[1], library, params)->type;
            
            //if doesnt exists or ... who cares
            library[primary].function = prototype_var;
            //i care
            //this is shit slow, i need a symbolic container!
            //oh God
            if (library[primary].data == nullptr)
                library[primary].data = new node(CONT);
            
            node* container = library[primary].data;
            //check all variables inside
            for (int i=0; i<container->data.args.size(); ++i) {
                if (container->data.args[i]->type == secondary) {
                    //rewrite it freely
                    //ANOTHER CONTAINER
                    container->data.args[i]->data.args[0] = evaluate(args[1], library, params);
                    //immediately return it
                    return container->data.args[i]->data.args[0];
                }
            }
            //create one

            //virtual variables, something like this
            node* container_ = new node((TYPE)secondary);
            container->data.args.push_back(container_);
            //single element container
            container_->data.args.push_back(evaluate(args[1], library, params));
            return container_->data.args[0]; 
        }
        break;
    
    default:
        //default case
        //cost: 1 evaluation - more preferable
        break;
    }

    //store it to the library
    library[args[0]->type].function = prototype_var;
    library[args[0]->type].data = evaluate(args[1], library, params);
    //do not delete the previous one, because It belongs to the tree somewhere.

    //return defined variable
    return library[args[0]->type].data;    
}

/**call defined function**/
node* prototype_fun(node* itself, vector<node*> &args, definition* library, char params) {
    #ifdef DEVELOPMENT
        if (args.size() != library[itself->type].argv.size())
            throw runtime_error("args for the function doesnt match!"); 
    #endif

    //create something like a stack. local library
    //actually, this is the backup of the global library, but, who cares
    definition stack[args.size()];
    //foreach argument
    for (int i=0; i<args.size(); ++i) {
        //save global lib definitions
        stack[i] = library[library[itself->type].argv[i]];
        stack[i].restore_id = library[itself->type].argv[i];
        //add arguments passed to the function
        library[library[itself->type].argv[i]].function = prototype_var;
        //mandatory evaluate each rgument
        library[library[itself->type].argv[i]].data = evaluate(args[i], library, params);
    }

    //run the function
    node* res = evaluate(library[itself->type].data, library, params); 
    //restore global library
    for (int i=0; i<args.size(); ++i) {
        library[stack[i].restore_id] = stack[i];
    }    

    return res;
}

/**define a function or delayed variable**/
node* def(node* itself, vector<node*> &args, definition* library, char params) {
    //add to the global library
    
    library[args[0]->type].function = prototype_fun;
    library[args[0]->type].data = args[1];

    //write argument's ids to the body of the function
    //each id corresponds to the number of the argument passed to the function
    for (int i=0; i<args[0]->data.args.size(); ++i) {
        //skip pattern-type
        library[args[0]->type].argv.push_back(args[0]->data.args[i]->data.args[0]->type);
    }

    //nothing todo
    return itself;
}

//helps to solve conditions
node* expand(node* itself, vector<node*> &args, definition* library, char params) {

}

//define rule of permuttaions and etc...
node* perm() {
    
}

/**conditions**/
node* cond(node* itself, vector<node*> &args, definition* library, char params) {
    //check the condition and store the result
    node* check = evaluate(args[0], library, params);
    //if it is not BOOL type, skip it
    if (check->type != BOOL) goto return_itself;
    
    if (check->data.b) {
        //purge the result
        //no memory leaking!
        //delete check;
        //UPD: unnecessary, garbage collector exists
        //return evaluated 1th arg
        return evaluate(args[1], library, params);
    } else {
        //purge the result
        //delete check;
        //return evaluated 2th arg        
        return evaluate(args[2], library, params);
    }  

    //default behaviour, when failed
    return_itself:

    if (itself == nullptr) {
        itself = new node(COND);
        itself->data.args = args;
    }
    return itself;      
}

/**less or equal operator**/
node* leq(node* itself, vector<node*> &args, definition* library, char params) {

}

/**equal operator**/
node* equals(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory  evaluate each
    node* a = evaluate(args[0], library, params);
    node* b = evaluate(args[1], library, params);
    

    //check if comparable
    switch (a->type)
    {
    case INTEGER: {
        //create the result
        node* res = new node(BOOL);
        res->data.b = a->data.i == b->data.i;
        //easy!
        return res;
    }
        break;   
    
    default:
        goto return_itself;
        break;
    }

    //default behaviour, when failed
    return_itself:

    if (itself == nullptr) {
        itself = new node(EQUALS);
        itself->data.args = args;
    }
    return itself;
    
}

//temporary structure, just for the sorting
typedef struct shelf {
    vector<node*> items;
    unsigned type;
} shelf;

/**take a product of elements**/
node* timesf(node* itself, vector<node*> &args, definition* library, char params);

/**take a sum of elements**/
node* plusf(node* itself, vector<node*> &args, definition* library, char params) {
    //only for homogenerous types
    //we have to check all pairs!
    //shit!
    
    //plus is the affine operation
    #ifdef DEVELOPMENT
        if (args.size() < 2) throw runtime_error("not enough arguements for Plus");
        cout << "at plus" << '\n';
    #endif
    
    vector<shelf> shell;
    
    //mandatory evaluate, then sort by the groups
    //i could do it better, i believe
    for (int i=0; i<args.size(); ++i) {
        node* r = evaluate(args[i], library, params);
        //no clonning, no leaking
        bool exit = false;
        for (int j=0; j<shell.size(); ++j) {
            //check if the group is already exists
            if (r->type == shell[j].type) {
                //add the pointer to the item
                shell[j].items.push_back(r);
                //must be replace with GOTO
                exit = true;
                break;
            }
        }
        
        if (exit) continue;
        //group doenst exist, so create it
        shelf newshelf;
        newshelf.type = r->type;
        newshelf.items.push_back(r);
        //add the pointer to the item
        shell.push_back(newshelf);
    
    }

    vector<node*> sum;
    //so I need to sum all homogenerous objects

    //but, what should i do with mixed-like FLOAT + INT and etc?..
    //*some crutches
    int cr_int = -1;
    int cr_float = -1;
    //*end of some crutches

    //collapse sum on each element
    for (int i=0; i<shell.size(); ++i) {
        //sort by types
        switch (shell[i].type) {

        case INTEGER:
            cr_int = sum.size();
   
            //clone the first instance of sum for the integers
            //because it is a new result. We shuld not corrupt the original data!
            //smart memory management...
            sum.push_back(shell[i].items[0]->clone());
            for (int j=1; j<shell[i].items.size(); ++j) {
                //add the others to it numerically
                sum[cr_int]->data.i = sum[cr_int]->data.i + shell[i].items[j]->data.i;
            }
            break;

        case FLOAT:
            cr_float = sum.size();
            sum.push_back(shell[i].items[0]->clone());
            for (int j=1; j<shell[i].items.size(); ++j) {
                sum[cr_int]->data.f = sum[cr_int]->data.f + shell[i].items[j]->data.f;
            }
            
            break;

        case LIST: {
            //the depth of sum is 1 like 1D arrays
            int depth = 1;
            //at random, just set the default size by 1th element
            int size = shell[i].items[0]->data.args.size();
            
            //create a new one. remmber - do not affect the original data!
            node* table = new node;
            table->type = LIST;

            for (int k=0; k<size; ++k) {
                vector<node*> elements;
                for (int j=0; j<shell[i].items.size(); ++j)
                    elements.push_back(shell[i].items[j]->data.args[k]);

                //prevent repeated evaluation
                table->data.args.push_back(plusf(nullptr, elements, library, params | NOEVALUATION));
            }

            sum.push_back(table);    
        }

            break; 

        case TIMES:
        case PLUS:
        case SIN:
        case COS:
        case EXP:
        case COND:
        case EXPRESSION:
        case POWER:
        case SQRT:
        case SUB:
        case PART:
        case UNDEFINED:
            //just push them
            for (int j=0; j<shell[i].items.size(); ++j) {
                sum.push_back(shell[i].items[j]);
            }
            break;

        default:
            //collect the same types vars (we can't collect functions, because they can differ in arguments)

            //skip functions
            if (shell[i].items[0]->data.args.size() > 0) {
                for (int j=0; j<shell[i].items.size(); ++j) {
                    sum.push_back(shell[i].items[j]);
                }
                break;                
            }

            //only for vars

            //when one
            if (shell[i].items.size() < 2) {
                //only one
                sum.push_back(shell[i].items[0]);
                break;
            }
            //when more than one
            vector<node*> st;
            st.push_back(shell[i].items[0]);
            st.push_back(new node((int)shell[i].items.size()));
            //prevent evaluation
            sum.push_back(timesf(nullptr, st, library, params | NOEVALUATION));
            break;
        }
    }

    //if the sum is collapsed, return only single el
    if (sum.size() == 1) {
        #ifdef DEVELOPMENT
            cout << "sum collapsed!" << '\n';
            cout << args[0] << '\n';
        #endif    
        
        return sum[0];
    }

    //create the new result
    //! WARNING: possible memory leaking. Who will remove it later?
    //! WARINNG: need a garbage collector
    node* result = new node(PLUS);
    result->data.args = sum;
    
    return result;
}

node* power(node* itself, vector<node*> &args, definition* library, char params);

/**take a product of elements**/
node* timesf(node* itself, vector<node*> &args, definition* library, char params) {
    #ifdef DEVELOPMENT
        if (args.size() < 2) throw runtime_error("not enough arguements for Times");
    #endif
    //This is a linear operation
    //cannot be used on two or more lists directly.
    //it seems to be, that I have to sort them again...
    vector<shelf> shell;
    
    //mandatory evaluate, then sort
    for (int i=0; i<args.size(); ++i) {
        node* r = evaluate(args[i], library, params);
        //no clonning, no leaking
        bool exit = false;
        for (int j=0; j<shell.size(); ++j) {
            if (r->type == shell[j].type) {
                shell[j].items.push_back(r);
                exit = true;
                break;
            }
        }
        
        if (exit) continue;

        shelf newshelf;
        newshelf.type = r->type;
        newshelf.items.push_back(r);

        shell.push_back(newshelf);
    
    }
    
    //create new product
    vector<node*> product;

    //for the list and times type exlusively
    node* list_ptr = nullptr;
    node* times_ptr = nullptr;

    //*some crutches
    int cr_int = -1;
    int cr_float = -1;
    //*end of some crutches

    //collapse times on each element
    for (int i=0; i<shell.size(); ++i) {
        switch(shell[i].type) {
        case INTEGER:
            cr_int = product.size();
            //clone the first instance of product for the integers
            product.push_back(shell[i].items[0]->clone());
            for (int j=1; j<shell[i].items.size(); ++j) {
                product[cr_int]->data.i = product[cr_int]->data.i * shell[i].items[j]->data.i;
            }
            break;

        case FLOAT:
            cr_float = product.size();
            product.push_back(shell[i].items[0]->clone());
            for (int j=1; j<shell[i].items.size(); ++j) {
                product[cr_float]->data.f = product[cr_float]->data.f * shell[i].items[j]->data.f;
            }
            break;

        case IMUNIT:
            //superfast calc
            //cout << "Imagine" << '\n';
            switch(shell[i].items.size() % 4) {
                case 1: 
                    product.push_back(shell[i].items[0]);
                    break;
                case 2:
                    product.push_back(new node(-1));
                    break;
                case 3: {
                    vector<node*> _args = {shell[i].items[0], new node(-1)};
                    
                    //prevent evaluation flag
                    product.push_back(timesf(nullptr, _args, library, params | NOEVALUATION));
                }
                    break; 
                case 0:
                    product.push_back(new node(1));
                    break;                                                         
            }
            break;

        case LIST:
            //it must not contain more than 1 list!
            //TODO: do it better!
            list_ptr = shell[i].items[0];
            if (shell[i].items.size() > 1) runtime_error("Times at pair of lists"); 
      
            break; 

        case TIMES:
            //it must not contain more than 1 list!
            //TODO: do it better!
            times_ptr = shell[i].items[0];
            for (int j=1; j<shell[i].items.size(); ++j) {
                product.push_back(shell[i].items[j]);
            }
      
            break;
            
        //TODO: add power operation
        default: {
            for (int j=0; j<shell[i].items.size(); ++j) {
                product.push_back(shell[i].items[j]);
            }
            break;

            //doent work, i dont know why
            /*if (shell[i].items.size() == 1) {
                product.push_back(shell[i].items[0]);
                break;
            }
            //power!
            vector<node*> _args = {shell[i].items[0], new node((int)shell[i].items.size())};
            product.push_back(power(nullptr, _args, library, params));*/
        }
            break;                       
        }
    }


    //if there is a list in the product
    if (list_ptr != nullptr) {
        //Its a linear operation
        //create a new list
        node* table = new node(LIST);

        const int size = list_ptr->data.args.size();
        for (int k=0; k<size; ++k) {
            //! WARNING: Slow?
            product.push_back(list_ptr->data.args[k]);
            //acts on the each item in the list 
            //do not prevent evaluation (if the list is nested?)
            table->data.args.push_back(timesf(nullptr, product, library, params));
            //push-pop things
            product.pop_back();
        }
        //return the result
        return table;
    }

    
    //the same for times
    if (times_ptr != nullptr) {
        //Its a linear operation
        //create a new list
        node* table = new node(TIMES);

        const int size = times_ptr->data.args.size();
        for (int k=0; k<size; ++k) {
            //! WARNING: Slow?
            product.push_back(times_ptr->data.args[k]);
            //acts on the each item in the list 
            table->data.args.push_back(timesf(nullptr, product, library, params));
            //push-pop things
            product.pop_back();
        }
        //return the result
        return table;
    }  

    //optimization for the case INT = 0
    //do not spend extra time 
    if (cr_int > -1) {
        if (product[cr_int]->data.i == 0)
            return product[cr_int];
    }

    //crunches for the case "float x int"
    if (cr_float > -1 && cr_int > -1) {
        #ifdef DEVELOPMENT
            cout << "int * float detected!" << '\n';
            cout << product[cr_float] << '\n';
            cout << product[cr_int] << '\n';
        #endif        
        product[cr_float]->data.f = product[cr_float]->data.f * product[cr_int]->data.i;
        if (cr_int != product.size() - 1) {
            swap(product[cr_int], product.back());
        }
        product.pop_back();
    } 

    //if the timesf is collapsed, return only single el
    if (product.size() == 1) {
        #ifdef DEVELOPMENT
            cout << "timesf collapsed!" << '\n';
            cout << product[0] << '\n';
        #endif
        //cout <<  " after times coll" << '\n' << watch << '\n';
        return product[0];
    }

    //create itself
    node* result = new node;
    //write the result
    //! WARNING: memory leaking
    result->data.args = product;
    result->type = TIMES;
    
    return result;        
}

/**mixed types matrix multiplication**/
node* dot(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate them
    node* s1 = evaluate(args[0], library, params);
    node* s2 = evaluate(args[1], library, params);

    //only for lists
    if (s1->type != LIST || s2->type != LIST) return itself;

    //get numbers of columns 
    const int size1y = s1->data.args.size();
    const int size2y = s2->data.args.size();
    int size1x = 0;
    int size2x = 0;
    
    //get numbers of rows
    for (int i=0; i<size1y; ++i) {
        if (s1->data.args[i]->type == LIST) {
            size1x = s1->data.args[i]->data.args.size();
        } else {
            return itself;
        }
    }
            
    for (int i=0; i<size2y; ++i) {
        if (s2->data.args[i]->type == LIST) {
            size2x = s2->data.args[i]->data.args.size();
        } else {
            return itself;
        }
    }  

    //must match
    if (size1x != size2y) {
        #ifdef DEVELOPMENT
            cout << "dims must agree!";
        #endif
        return itself;
    }

    //TODO: Parallelize!

    //construct a new table
    node* table = new node;
    table->type = LIST;

    vector<node*> p(2);
    vector<node*> sum(size1x);

    //traditional shitslow matrix multiplication
    //#pragma omp parallel for private(sum,p)
    for (int j=0; j<size1y; ++j) {
        node* column = new node;
        column->type = LIST;
        table->data.args.push_back(column);

        for (int i=0; i<size2x; ++i) {
            for (int k=0; k<size1x; ++k) {
                p[0] = s1->data.args[j]->data.args[k];
                p[1] = s2->data.args[k]->data.args[i];

                sum[k] = timesf(nullptr, p, library, params | NOEVALUATION);
            }
            column->data.args.push_back(plusf(nullptr, sum, library, params | NOEVALUATION));
        }   
    }  

    return table;
}



node* sqrtf(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate
    node* a = evaluate(args[0], library, params);
    
    //calculatable cases
    if (a->type == FLOAT) {
        return(new node(sqrt(a->data.f)));
    }

    //if not, automatically create one
    if (itself == nullptr) {
        itself = new node();
        itself->type = SQRT;
        itself->data.args = args;
    }
    return itself;    
}

node* norm(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate
    node* p = evaluate(args[0], library, params);
    if (p->type != LIST) goto return_itself;
    {
        //construct the sum
        node* _plus = new node();
        _plus->type = PLUS;

        //calcaulte the sum of squares
        for (int i=0; i<p->data.args.size(); ++i) {
            //TODO: somehow i need to optimize it...
            vector<node*>_args = {p->data.args[i], new node(2)};
            //prevent evaluation (already done)
            _plus->data.args.push_back(power(nullptr, _args, library, params | NOEVALUATION));
        }
        //sqrt
        vector<node*> _a {_plus};
        return sqrtf(nullptr, _a, library, params);
    }
    //default behaviour, when failed
    return_itself: 

    if (itself == nullptr) {
        itself = new node();
        itself->type = NORM;
        itself->data.args = args;
    }
    return itself; 
}

node* transpose(node* itself, vector<node*> &args, definition* library, char params) {
    node* m = evaluate(args[0], library, params);
    if (m->type != LIST) goto return_itself;



    //when failed
    return_itself:
    if (itself == nullptr) {
        itself = new node();
        itself->type = TR;
        itself->data.args.push_back(args[0]);
    }
    return itself;
}

node* normalize_n(node* itself, vector<node*> &args, definition* library, char params) {
    //numerical fast version
    if (type != NVECTOR) convert()
    if (type != NMATRIX) convert()
}

node* normalize(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate
    //
    //UPD: unnecessary, will be done by norm operation or times
    //! WARNING: too many evaluate calls

    //check if list is numerical

    //if not
    //calculate norm
    //node* p = evaluate(args[0], library, params);

    if (params & NUMERIZE) return normalize_n();

    node* n = norm(nullptr, args, library, params);

    //inverse it
    vector<node*> _args {n, new node(-1)};

    //times
    vector<node*> __args {power(nullptr, _args, library, params | NOEVALUATION), args[0]};
    return timesf(nullptr, __args, library, params);
}

/**power function**/
node* power(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate
    node* p = evaluate(args[1], library, params);
    node* o = evaluate(args[0], library, params);

    //calculatable cases
    if (p->type == INTEGER && o->type == INTEGER && p->data.i >= 0) {
        return(new node(simple_pow(p->data.i, o->data.i)));
    }

    if (p->type == INTEGER && o->type == FLOAT) {
        return(new node(pow(p->data.i, o->data.f)));
    }   

    if (p->type == FLOAT && o->type == INTEGER) {
        return(new node(pow(p->data.f, o->data.i)));
    }     

    if (p->type == FLOAT && o->type == FLOAT) {
        return(new node(pow(p->data.f, o->data.f)));
    } 

    if (p->type == INTEGER && o->type == IMUNIT) {
            //superfast calc
            //cout << "Imagine" << '\n';
            switch(p->data.i % 4) {
                case 1: 
                    return o;
                    break;
                case 2:
                    return new node(-1);
                    break;
                case 3: {
                    vector<node*> _args = {o, new node(-1)};
                    return timesf(nullptr, _args, library, params | NOEVALUATION);
                }
                    break; 
                case 0:
                    return new node(1);
                    break;                                                         
            }    
    } 

    //default behaviour, when failed
    if (itself == nullptr) {
        itself = new node();
        itself->type = POWER;
        itself->data.args = args;
    }
    return itself;    
}

/*MMA container for procedural coding*/
node* expression(node* itself, vector<node*> &args, definition* library, char params) {
    //evaluate all and the last one will be the output
    for(int i=0; i<args.size()-1; ++i) {
        node* ctl = evaluate(args[i], library, params);
        if (ctl->type == BREAK || ctl->type == CONTINUE) return ctl;
    }

    return evaluate(args.back(), library, params);
}

node* subscript(node* itself, vector<node*> &args, definition* library, char params) {
    //may be try to call a variable from library by the following id
    //like hash(args[1], args[2]) == pseudo_type - uninique combination
    //cout << "subscript detected";
    //UPD
    //seek for the primary variable
    node* el = library[args[0]->type].data; 
    if (el == nullptr) return itself;

    //check the secondary
    int sz = el->data.args.size();

    //el must be CONT type
    
    for (int i=0; i<sz; ++i) {
        if (el->data.args[i]->type == args[1]->type) 
            //return the content of the inner container
            //by default 1 arg
            return evaluate(el->data.args[i]->data.args[0], library, params); 
    }
      

    return itself;
}

/**for every undefined variable or function**/
node* undefined(node* itself, vector<node*> &args, definition* library, char params) {
    //nop
    #ifdef DEVELOPMENT
        cout << "undefined type?" << '\t' << itself->type << '\n';
    #endif
    //check args
    if (args.size() == 0) return itself;

    //create a new evaluated copy
    itself = itself->clone();
    for (int i=0; i<args.size(); ++i) {
        itself->data.args[i] = evaluate(args[i], library, params);
    }
    return itself;
}


node* sinf(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate
    node* a = evaluate(args[0], library, params);

    //calculatable cases
    if (a->type == FLOAT) {
        return(new node(sin(a->data.f)));
    }

    //return new copy
    itself = itself->clone();
    itself->data.args[0] = a;

    return itself;  
}

node* cosf(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate
    node* a = evaluate(args[0], library, params);

    //calculatable cases
    if (a->type == FLOAT) {
        return(new node(cos(a->data.f)));
    }
    
    //return new copy
    itself = itself->clone();
    itself->data.args[0] = a;

    return itself;    
}

node* expf(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate
    node* a = evaluate(args[0], library, params);

    //calculatable cases
    if (a->type == FLOAT) {
        return(new node(exp(a->data.f)));
    }

    //return new copy
    itself = itself->clone();
    itself->data.args[0] = a;

    return itself;    
}

/**replace operation**/
//acts like a local function
node* replace(node* itself, vector<node*> &args, definition* library, char params) {
    //how many rules?
    node* rules = evaluate(args[1], library, params);
    int count = 0;

    if (rules->type == RULE) count = 1;
    if (rules->type == LIST) count = rules->data.args.size();

    if (count == 0) return itself;

    //cout << "rules: " << rules << '\n';
    //cout << "count: " << count << '\n';

    //load all rules into library like in a function
    definition stack[count];

    if (rules->type == RULE) {
        //stack[0] = library[rules->data.args[0]->type];
        //stack[0].restore_id = rules->data.args[0]->type;   

        //The general case. We don't know.
        //library[rules->data.args[0]->type].function = undefined;
        //library[rules->data.args[0]->type].data = evaluate(rules->data.args[1], library, params);

        //node* res = evaluate(args[0], library, params); 
        //library[stack[0].restore_id] = stack[0];

        //return res;
        return itself;
    }
    //cout << "list of rules" << '\n';
    //add all definitions
    for (int i=0; i<count; ++i) {
        node* rl = rules->data.args[i];
        //cout << "rule: " << i << "  is :" << rl << '\n';
        stack[i] = library[rl->data.args[0]->type];
        stack[i].restore_id = rl->data.args[0]->type;

        //library[stack[i].restore_id].function = prototype_var;
        //library[stack[i].restore_id].data = evaluate(rl->data.args[1], library, params);

        //use the native method
        //do not prevent evaluation, because rule function does nothing
        set(nullptr, rl->data.args, library, params);
    }
 
    //run the fake function
    node* res = evaluate(args[0], library, params); 

    //restore
    
    for (int i=0; i<count; ++i) {
        library[stack[i].restore_id] = stack[i];
    }    
    return res;    

}

/**container for rules**/
node* rule(node* itself, vector<node*> &args, definition* library, char params) {
    //nop
    return itself;
}

/**take a part of anything **/
//return pointer?
node* part(node* itself, vector<node*> &args, definition* library, char params) {
    
    //mandatory evaluate the object
    node* obj = evaluate(args[0],library);
    //go deep
    for (int i=1; i<args.size(); ++i) {
        //mandatory evaluate part specification
        node* n = evaluate(args[i], library, params);
        if (n->type != INTEGER) return itself;
        if (obj->data.args.size() < n->data.i) return itself;
        //go deeper
        obj = obj->data.args[n->data.i-1];
    }
    return obj;
}

node* length(node* itself, vector<node*> &args, definition* library, char params) {
    return(new node((int) args.size()));
}

/**complex do cycle**/
node* dof(node* itself, vector<node*> &args, definition* library, char params) {
    //get local variables
    
    node* itr = args[args.size()-1];
    if (itr->type != LIST) return itself;

    //nested cycles
    node* exp = args[0];
    if (args.size() > 2) {
        //cout << "Nested cycles" << '\n';
        //create a fake cycle
        exp = new node();
        exp->type = DO;
        exp->data.args.push_back(args[0]);
        exp->data.args.push_back(args[args.size()-2]);
    }

    //create local stack and save
    definition var;
    var = library[itr->data.args[0]->type];
    var.restore_id = itr->data.args[0]->type;
    //create local iteratie with unknown data
    library[itr->data.args[0]->type].function = prototype_var;
    
    //  I
    //evaluate throuth the list

    if (itr->data.args.size() == 2) {
        //mandatory evaluate
        node* lst = evaluate(itr->data.args[1],library);
        if (lst->type != LIST) return itself;
        //evaluate EXP and repeats
        for (int i=0; i<lst->data.args.size(); ++i) {
            library[itr->data.args[0]->type].data = lst->data.args[i];

            node* res = evaluate(exp, library, params);
            if (res->type == BREAK) break;
        }

        return &null_node;
    }

    //typical befviour

    //mandatory evaluate
    node* begin = evaluate(itr->data.args[1], library, params);
    node* end   = evaluate(itr->data.args[2], library, params);
    node* step;

    if (itr->data.args.size() > 3) {
        step = evaluate(itr->data.args[3], library, params);
    } else {
        step = new node(1);
        //dont forget to purge it!
    }


    //check  types

    //  II
    //the simplest cases

    if (begin->type == INTEGER && end->type == INTEGER && step->type == INTEGER) {
        //evaluate EXP and repeats
        for (int i=begin->data.i; i<=end->data.i; i = i + step->data.i) {
            //create a value for each point
            //! WARNING: memory leaking
            library[itr->data.args[0]->type].data = new node(i);
            
            node* res = evaluate(exp, library, params);
            if (res->type == BREAK) break;
        }

        //purge
        delete step;

        //purge nested parts
        if (args.size() > 2) delete exp;

        return &null_node;    
    }

    if (begin->type == FLOAT && end->type == FLOAT && step->type == FLOAT) {
        //evaluate EXP and repeats
        for (double i=begin->data.f; i<=end->data.f; i = i + step->data.f) {
            //create a value for each point
            //! WARNING: hard memory leaking
            library[itr->data.args[0]->type].data = new node(i);

            node* res = evaluate(exp, library, params);
            if (res->type == BREAK) break;
        }

        //purge, because it was calculatable
        delete step;

        //purge nested parts
        if (args.size() > 2) delete exp;

        return &null_node;    
    }    

    throw runtime_error("not implemented");
    //  III
    //the hardest cases
    /*
    vector<node*> vargs = {begin, step};
    vector<node*> sargs = {begin, end};
    node* i = begin;

    for (;;) {
            //naturally calculate iteratie
            sargs[0] = i;
            node* eq = leq(nullptr, sargs, library, params);
            if (!eq->data.b) break;
            

            library[itr->data.args[0]->type].data = i;
            node* res = evaluate(exp, library, params);
            if (res->type == BREAK) break;

            //purge eq
            delete eq;

            //increment
            vargs[0] = i;
            i = plusf(nullptr, vargs, library, params);
    }
    //I'am afraid to purge i...

    //do not purge step, who knows
    //delete step;
    //purge nested parts
    if (args.size() > 2) delete exp;*/

    return &null_node;

}

node* sumf(node* itself, vector<node*> &args, definition* library, char params) {
    //get local variables
    
    node* itr = args[args.size()-1];
    if (itr->type != LIST) return itself;

    //nested cycles
    node* exp = args[0];
    if (args.size() > 2) {
        //create a fake cycle
        exp = new node();
        exp->type = SUM;
        exp->data.args.push_back(args[0]);
        exp->data.args.push_back(args[args.size()-2]);
    }

    //create local stack and save
    definition var;
    var = library[itr->data.args[0]->type];
    var.restore_id = itr->data.args[0]->type;
    //create local iteratie with unknown data
    library[itr->data.args[0]->type].function = prototype_var;

    //create a sum
    vector<node*> sum;
    
    //  I
    //evaluate throuth the list

    if (itr->data.args.size() == 2) {
        //mandatory evaluate
        node* lst = evaluate(itr->data.args[1],library);
        if (lst->type != LIST) return itself;
        //evaluate EXP and repeats
        for (int i=0; i<lst->data.args.size(); ++i) {
            library[itr->data.args[0]->type].data = lst->data.args[i];

            sum.push_back(evaluate(exp, library, params));
            //if (res->type == BREAK) break;
        }
        //! WARNING:
        //TODO: add no evaluate flag
        return plusf(nullptr, sum, library, params);
    }

    //typical befviour

    //mandatory evaluate
    node* begin = evaluate(itr->data.args[1], library, params);
    node* end   = evaluate(itr->data.args[2], library, params);
    node* step;

    if (itr->data.args.size() > 3) {
        step = evaluate(itr->data.args[3], library, params);
    } else {
        step = new node(1);
        //dont forget to purge it!
    }


    //check  types

    //  II
    //the simplest cases

    if (begin->type == INTEGER && end->type == INTEGER && step->type == INTEGER) {
        //evaluate EXP and repeats
        for (int i=begin->data.i; i<=end->data.i; i = i + step->data.i) {
            //create a value for each point
            //! WARNING: memory leaking
            
            library[itr->data.args[0]->type].data = new node(i);
            cout << "here is " << itr->data.args[0]->type << " :::  " << library[itr->data.args[0]->type].data << '\n';
            
            sum.push_back(evaluate(exp, library, params));
            //if (res->type == BREAK) break;
        }

        //purge, because it was calculatable
        delete step;

        //purge nested parts
        if (args.size() > 2) delete exp;

        return plusf(nullptr, sum, library, params);    
    }

    if (begin->type == FLOAT && end->type == FLOAT && step->type == FLOAT) {
        //evaluate EXP and repeats
        for (double i=begin->data.f; i<=end->data.f; i = i + step->data.f) {
            //create a value for each point
            //! WARNING: hard memory leaking
            library[itr->data.args[0]->type].data = new node(i);

            sum.push_back(evaluate(exp, library, params));
        }

        //purge, because it was calculatable
        delete step;

        //purge nested parts
        if (args.size() > 2) delete exp;

        return plusf(nullptr, sum, library, params);    
    }    

    throw runtime_error("not implemented");

    return &null_node;

}

/**the array of the all built-in functions**/
node* (*library_defaults[])(node* itself, vector<node*> &args, definition* library, char params) = {
    undefined, undefined, undefined, undefined, hold, list, set, undefined, plusf, timesf, undefined, undefined, def, undefined,
    cond, equals, dot, expression, power, subscript, sinf, cosf, expf, rule, replace, part, dof, undefined, undefined, undefined, 
    undefined, leq, sqrtf, length, normalize, norm, undefined, sumf, transpose
};



//init defaults
void init() {

    null_node.type = UNDEFINED;
    break_node.type = BREAK;
    return_node.type = RETURN; 

    for (int i=0; i<sizeof(library)/sizeof(library[0]); ++i) {
        library[i].function = undefined;
    }
    for (int i=0; i<sizeof(library_defaults)/sizeof(library_defaults[0]); ++i) {
        library[i].function = library_defaults[i];
        cout << "initialised: " << EnumTypes[i] << '\n';
    }
}



node* apply() {

}

/**holy grail evaluate function**/
node* evaluate(node *data, definition* library, char params) {
    //check type
    switch(data->type) {
        //primitive types
        case INTEGER:
        case FLOAT:
        case STRING:
        case BOOL:
            return data;

        //functions
        default:

            break;
    }

    //check params
    if (params & NOEVALUATION) return data;

    //functions handling  
    #ifdef DEVELOPMENT
    cout << "calling by addr " << data->type << '\n';
    #endif
    //call it from the library
    return library[data->type].function(data, data->data.args, library, params);
}
