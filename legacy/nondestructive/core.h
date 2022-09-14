#include <vector>
#include <iostream>

using namespace std;

#include "shared.h"

//isolation of the arguments
#define FUNC_ADDR 777
//hope it will not corrupt

//TO-DO
//--benchmark tests with garbage collector instead of delete
//--benchmark with factorial
//--implement soultion with Times, Plus with variebles  3x - 6x == -3x ex.
//--library based DOT operation for just fun :0
//--build isolate() function to shift randomly all ids of local variables
//--init member by template (node::initInteger(4), node::initFloat(4))
//--N operator, optimizes all numerical data to xtensor arrays with BLAS
//--reqursive destructor of class node
//--fast library search without loops. Use id as a pointer to location. Catalog

node* clone(node* src);
node* evaluate(node *data, node** library, node* stack = nullptr, unsigned stack_size = 0);

/**hold function. do nothing by now**/
node* hold(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) {
    return(evaluate(args[0], library, stack, stack_size));
}

/**list function. One of the most important**/
node* list(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) {
    return itself;
}

//29.03.2021
//paradigm of functions:
//1. return new data, when it was processed
//--or allocate a new one, when processed (intensive memeory load. NOT GOOD)
//--and you can build the tree on-fly when parsing! I think, prebuild tree is much faster
//2. then, if data is shit ("unevaluatable")
//2. return itself

//too many arguments, i believe
//(node, args, id, lib)
// ^      ^     ^   ^
// the main object and the function itself (by the name)
//        |     |   |
//     its arguements 
//              |   |
//             id, just for a variable type... sucks
//               the library with data of all defined varibles and functions
//
//ha, but convinient to use... So, we shall see

//29.03.2021 --hours 1 later
//new paradigm of functions!:
//1. create an instance of function if itself == nulptr
//--good for creating new branches
//2. process data and return it
//3. if it is shitty, return iself
//*my be the 1th should be before the last one.

//dare to build loops and conditions features, you will be fucked.

//I must process all this tree once!

//UPD: 03.04.2021
//I need more parameters!
//We should not copy function! Clonning is slow as shit
//More like c++ compilers use

/**set function. declare variables**/
node* set(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) {
    #ifdef DEVELOPMENT
    if (args.size() != 2) 
        throw runtime_error("SET operator must have two arguments!");
    #endif
    if (library[args[0]->data.id] != nullptr) {
        //purge old one
        //! WARNING: cross ref case? or garbage collector?
        delete library[args[0]->data.id]; //only head?
    }
    
    library[args[0]->data.id] = evaluate(args[1], library, stack, stack_size);
    return itself;      
}

/**call varieble from the library by id**/
node* var(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) {
    if (stack != nullptr && stack_size > id)
        return evaluate(&stack[id], library, stack, stack_size); 
    //search in the library
    if (library[id] != nullptr) {
        //no clonning, just return the pointer to the evaluated value
        return evaluate(library[id], library, stack, stack_size); 
    }

    //if the instance doesn't exists, create one
    //may be unnecessary...
    if (itself == nullptr) {
        itself = new node;
        itself->type = VAR;
        itself->data.id = id;
    }
    //return undefined variable...
    
    return itself;
}

/**calling a function defined in library**/
node* fun(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) {
    #ifdef DEVELOPMENT
        cout << "calling fun" << '\n';
    #endif
    //for (int i=0; i<library.size(); ++i) 
        //we found function!
        if (library[id] != nullptr) {
            #ifdef DEVELOPMENT
                cout << "orig library is" <<'\n';

                for (int ii=0; ii<library.size(); ++ii)
                    cout << library[ii];       

                cout << "found" << '\n';
                //check length of args
            
                if (library[i]->data.argv.size() != args.size()) 
                    throw runtime_error("args for the function doesnt match!");
            #endif
            //copy additional data to the local library
            //vector<node*> local;
            //local.push_back(library[i]);
            #ifdef DEVELOPMENT
                cout << "with args" << '\n';
            #endif
            
            const unsigned local_size = args.size();
            node local[local_size];
            //local.reserve(12);

            for (int j=0; j<local_size; ++j) {
                #ifdef DEVELOPMENT
                    cout << "arg: " << args[j] << '\n';
                #endif
                //we need a container for a variable to pass it without changing it
                //lest create a buffer variable with a pointer to actual variable
                //node* buffer = new node();
                const unsigned idt = library[id]->data.argv[j];
                //local[idt] = new node;
                local[idt].type = CONT;
                //link to function's args space
                local[idt].data.id = idt;
                //add an evaluated argument
                local[idt].data.args.push_back(evaluate(args[j], library, stack, stack_size));
                ////add it to the local lib
                //library[buffer->data.id] = buffer;
            }

            //evaluate function in the local env
            
            //cout << cnt<<  " before fun evaluated was" << '\n' << library[i] << '\n';
            //cout <<  " befo fun" << '\n' << watch << '\n';
            node* func_run = evaluate(library[id], library, local, local_size);
            //cout <<  " after fun" << '\n' << watch << '\n';
            //cnt++;
            

            return func_run;
        }
    
    return itself;
}

/**define a function**/
node* def(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) {
    //add to the library
    //save the original node from first arg
    #ifdef DEVELOPMENT
        if (args[0]->type != FUN)
            throw runtime_error("setdelayed cannot be computed");
    #endif
    
    
    //new container 
    node* container = new node;
    container->type = CONT;
    container->data.args.push_back(args[1]);
    container->data.id = args[0]->data.id;

    //write all arguments to the future function
    for (int i=0; i<args[0]->data.args.size(); ++i) {
        //skip pattern-type
        container->data.argv.push_back(args[0]->data.args[i]->data.args[0]->data.id);
    }

    //add to the library
    library[container->data.id] = container;

    return itself;

}

/**condition function**/
node* cond(node* itself, vector<node*> &args, unsigned id,  node** library, node* stack, unsigned stack_size) {
    //check the condition and take it into account
    //cout <<  " before cond" << '\n' << watch << '\n';
    node* check = evaluate(args[0], library, stack, stack_size);

    
    if (check->data.b) {
        //purge the result
        delete check;
        //return evaluated 1th arg
        return evaluate(args[1], library, stack, stack_size);
    } else {
        //purge the result
        delete check;
        //return evaluated 2th arg        
        return evaluate(args[2], library, stack, stack_size);
    }    
    //do not destruct itself
}

/** == **/
node* equals(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) {
    //forcely  evaluate
    node* a = evaluate(args[0], library, stack, stack_size);
    node* b = evaluate(args[1], library, stack, stack_size);
    
    //create the result
    node* res = new node;
    res->type = BOOL;
    //only for integers
    switch (a->type)
    {
    case INTEGER:
        res->data.b = a->data.i == b->data.i;
        return res;
        break;   
    
    default:
        return itself;
        break;
    }
    
}

/**take a sum of elements**/
node* plusf(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) {
    //only for homogenerous type
    //we have to check all pairs!
    //and should pass only one time!
    
    //perhabs, it should evaluate forcely each arguement?
    //plus is the affine operation
    #ifdef DEVELOPMENT
        if (args.size() < 2) throw runtime_error("not enough arguements for Plus");
        cout << "at plus" << '\n' << watch << '\n';
    #endif
    
    //UPD: sort them'all fast!
    const int count = sizeof(EnumTypes)/sizeof(EnumTypes[0]);

    vector<node*> types[count];
    //cout << "sizeof EnumTypes: " << count << '\n';
    //forcely evaluate, then sort
    for (int i=0; i<args.size(); ++i) {
        node* r = evaluate(args[i], library, stack, stack_size);
        //no clonning, no leaking
        types[r->type].push_back(r);
    }

    vector<node*> sum;
    //so I need to sum all homogenerous objects

    //but, what should i do with mixed-like FLOAT + INT and etc?..
    //*some crutches
    int cr_int = -1;
    int cr_float = -1;
    //*end of some crutches

    //collapse sum on each element
    for (int i=0; i<count; ++i) {
        if (types[i].size() == 0) continue;
        //sort by types
        switch (i) {
        case INTEGER:
            cr_int = sum.size();
            //clone the first instance of sum for the integers
            sum.push_back(types[i][0]->clone());
            for (int j=1; j<types[i].size(); ++j) {
                sum[cr_int]->data.i = sum[cr_int]->data.i + types[i][j]->data.i;
            }
            break;

        case FLOAT:
            cr_float = sum.size();
            sum.push_back(types[i][0]->clone());
            for (int j=1; j<types[i].size(); ++j) {
                sum[cr_float]->data.f = sum[cr_float]->data.f + types[i][j]->data.f;
            }
            break; 

        case VAR: {           
            //take it simple
            //no clonning, pass by pointer
            for (int j=0; j<types[i].size(); ++j) {
                sum.push_back(types[i][j]);
            }            
        }
            break;

        default:
            throw runtime_error("unknown type for Plus");
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

    //create the result
    //! WARNING: possible memory leaking. Who will remove it later?
    //! WARINNG: need a garbage collector
    node* result = new node;
    result->type = PLUS;
    //TODO: get rid of copying
    result->data.args = sum;
    
    return result;

}

/**take a product of elements**/
node* timesf(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) {
    #ifdef DEVELOPMENT
        if (args.size() < 2) throw runtime_error("not enough arguements for Times");
    #endif
    //This is a linear operation
    //cannot be used on list directly.
    //cout <<  " before times" << '\n' << watch << '\n';
    //it seems to be, that I have to sort again... Sad shit for the perfomance
    const int count = sizeof(EnumTypes)/sizeof(EnumTypes[0]);

    vector<node*> types[count];

    //forcely evaluate, then sort
    for (int i=0; i<args.size(); ++i) {
        node* r = evaluate(args[i], library, stack, stack_size);
        //no clonning, no leaking
        types[r->type].push_back(r);
    }
    
    //create a products
    vector<node*> product;

    //*some crutches
    int cr_int = -1;
    int cr_float = -1;
    //*end of some crutches
    //cout <<  " times before switch" << '\n' << watch << '\n';
    //collapse times on each element
    for (int i=0; i<count; ++i) {
        if (types[i].size() == 0) continue;
        switch(i) {
        case INTEGER:
            cr_int = product.size();
            //clone the first instance of product for the integers
            product.push_back(types[i][0]->clone());
            for (int j=1; j<types[i].size(); ++j) {
                product[cr_int]->data.i = product[cr_int]->data.i * types[i][j]->data.i;
            }
            break;

        case FLOAT:
            cr_float = product.size();
            product.push_back(types[i][0]->clone());
            for (int j=1; j<types[i].size(); ++j) {
                product[cr_float]->data.f = product[cr_float]->data.f * types[i][j]->data.f;
            }
            break;

        case VAR:
            //nothing to do with vars, just pass them
            //TODO: merge two arrays
            for (int j=0; j<types[i].size(); ++j) {
                product.push_back(types[i][j]);
            }

            break;

        case LIST:
            //it should not contain more than 1 list!
            //TODO: do it better!
            #ifdef DEVELOPMENT
                if (types[i].size()>1) throw runtime_error("Times at pair of lists");
            #endif
      
            break; 
        
        case PLUS:
            throw runtime_error("plus for times is not implemented");
            break;          
        
        default:
            throw runtime_error("unknown type for Times");
            break;                       
        }
    }

    //if the timesf is collapsed, return only single 
    if (product.size() == 1) {
        #ifdef DEVELOPMENT
            cout << "timesf collapsed!" << '\n';
            cout << product[0] << '\n';
        #endif
        //cout <<  " after times coll" << '\n' << watch << '\n';
        return product[0];
    }

    //create itself if it doesnt exists
    node* result = new node;
    //write the result
    //! WARNING: memory leaking
    result->data.args = product;
    result->type = TIMES;
    
    return result;        
}

/**the array of all defined functions**/
//variebles are also functinos, which perform calls to the library
//first four are basic types 
node* (*functptr[])(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) = {
    nullptr, nullptr, nullptr, nullptr, hold, list, set, var, plusf, timesf, fun, nullptr, def, nullptr,
    cond, equals
};

/**evaluation function is a replacement to switch-case**/
node* evaluate(node *data, node** library, node* stack, unsigned stack_size) {
    //for standart primitive types
    switch(data->type) {
        //primitive types
        case INTEGER:
        case FLOAT:
        case STRING:
        case BOOL:
            return data;
        //container for anything
        case CONT: {
            return evaluate(data->data.args[0], library, stack, stack_size); 
        }   
        //functions
        default:

            break;
    }


    //functions handling  
    return (*functptr[data->type])(data, data->data.args, data->data.id, library, stack, stack_size);
}

/**clonning utility**/
//to copy some parts of tree from library

