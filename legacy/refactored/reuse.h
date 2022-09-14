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
node* evaluate(node *data, vector<node*> &library);

/**hold function. do nothing by now**/
node* hold(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
    for(int i=0; i<args.size(); ++i) {
        args[i] = evaluate(args[i], library);
    }
    //have no idea how to return
    //29.03.2021
    //if i recreate it, so, it will cause memory leaks. Sucks

    //may be this one
    return itself;
}

/**list function. One of the most important**/
node* list(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
    for(int i=0; i<args.size(); ++i) {
        args[i] = evaluate(args[i], library);
    }
    //yep, nothing it can do with the data
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
node* set(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
    #ifdef DEVELOPMENT
    if (args.size() != 2) 
        throw runtime_error("SET operator must have two arguments!");
    #endif
    //Search for a variable in the library
    int exists = -1;
    for (int i=0; i<library.size(); ++i) {
        if (library[i]->data.id == args[0]->data.id) {
            exists = i;
            break;
        }
    }
       
    if (exists > -1) {
        //if yes, so, replace it
        //rewrite the id
        args[1]->data.id = args[0]->data.id;
        //move the data to to the library by pointer (no clonning)
        //but before - evaluate it!
        node* evaluated = evaluate(args[1], library);
        //remove data from container
        delete library[exists]->data.args[0]; //can recursively
        library[exists]->data.args[0] = evaluated;
        //delete var object from SET operator
        delete args[0];
        //return by the value by clonning it to SET object    
        //return var(nullptr, nullptr, library[exists]->data.id, library);
        //Oh shit, you have to locate it AGAIN!
        //may be I will rewrite this part to just clone operation
        //30.03.2021
        //yep
        //*itself = *clone(evaluated);
        //! WARNING: possible memory leaking!

        //UPD: 31.03.2021
        delete itself;
        itself = evaluated->clone();

        return itself;

    } else { 
        //if no, evaluate and add it to the library
        
        node* evaluated = evaluate(args[1], library);
        //create container
        node* container = new node;
        container->type = CONT;
        container->data.args.push_back(evaluated);
        container->data.id = args[0]->data.id;

        library.push_back(container);
        //delete var object from SET operator
        delete args[0];
    
        //return by the value by clonning it to SET object
        //*itself = *clone(evaluated);

        //UPD: 31.03.2021
        delete itself;
        itself = evaluated->clone();     

        return itself;
    }            
}

/**call varieble from the library by id**/
node* var(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
    //search in the library
    for (int i=0; i<library.size(); ++i) {
        if (library[i]->data.id == id) {
            //we found! we have to clone the whole tree... sad :(
            //clone to itself

            //possible memory leaking till the itself is an object
            //clone(itself, library[i]);
            //however
            //*itself = *clone(library[i]);
            //looks rediculous, but fuck it.
            //no memory leaking! we are c++ programmers!
            //UPD: 30.03.2021
            //! WARNING: possible memory leaking!

            //UPD: 31.03.20201
            delete itself;
            itself =  library[i]->clone();

            //seems to be good paradigm...

            //? question: is it necessary to evaluate the cloned version? Hm.
            //by now, i would not prefer to do that
            return evaluate(itself, library);
        }
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

/**container for the variebles*
//UPD: moved to evaluate
node* cont(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
    node* object = args[0];
    if (itself != nullptr) delete itself; //only head
    return object;
}
*/

/**calling a function defined in library**/
node* fun(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
    //it is easy, just find
    //! WARNING: this is slow!
    //TODO: How to optimize search in the library?
    #ifdef DEVELOPMENT
        cout << "calling fun" << '\n';
    #endif
    for (int i=0; i<library.size(); ++i) 
        //we found function!
        if (library[i]->data.id == id) {
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
            vector<node*> local;
            local.push_back(library[i]);
            #ifdef DEVELOPMENT
                cout << "with args" << '\n';
            #endif
            
            for (int j=0; j<args.size(); ++j) {
                //forcely evaluate
                args[j] = evaluate(args[j], library);
                #ifdef DEVELOPMENT
                    cout << "arg: " << args[j] << '\n';
                #endif
                //we need a container for a variable to pass it without changing it
                //lest create a buffer variable with a pointer to actual variable
                node* buffer = new node();
                buffer->type = CONT;
                //link to function's args space
                buffer->data.id = library[i]->data.argv[j];
                //add an argument
                buffer->data.args.push_back(args[j]);
                //temporary add to the global library
                local.push_back(buffer);
            }

            //clone and evaluate function
            node* func_run = evaluate(library[i]->clone(), local);

            //revert original library
            //library.resize(library.size() - args.size());
            //return the result and delete itself
            if (itself != nullptr) delete itself;
            return func_run;
        }
    
    return itself;
}

/**define a function**/
node* def(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
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
        //purge varibale container
        delete args[0]->data.args[i]; //can recustively
    }

    //add to the library
    library.push_back(container);
    //purge arguments
    delete args[0]; //can recursively

    //return garbage
    itself->type = STRING;
    itself->data.s = "Null";
    return itself;

}

node* cond(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
    node* check = evaluate(args[0], library);
    node* res;

    if (check->data.b) {
        res = args[1];
        delete args[2];
        delete check;
    } else {
        res = args[2];
        delete args[1];
        delete check;
    }    
    delete itself;
    return evaluate(res, library);
}

node* equals(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
    //forcely  evaluate
    node* a = evaluate(args[0], library);
    node* b = evaluate(args[1], library);
    
    a->type = BOOL;
    a->data.b = a->data.i == b->data.i;
    delete b; //can recursively

    if (itself != nullptr) delete itself; //only head
    return a;
}

/**take a sum of elements**/
node* plusf(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
    //only for homogenerous type
    //we have to check all pairs!
    //and should pass only one time!

    //perhabs, it should evaluate forcely each arguement?
    //plus is the affine operation
    #ifdef DEVELOPMENT
        if (args.size() < 2) throw runtime_error("not enough arguements for Plus");
    #endif

    /**if there are a lot  of items**/
    //typically, "function + variable + number..."
    /*//first time - just evaluate them and sort by the type
    //for (int i=0; i<args.size(); ++i) {
        //forcely evaluate each
        args[0] = evaluate(args[0], library);
        for (int j=1; j<args.size(); ++j) {
            args[j] = evaluate(args[j], library);
            if (args[0]->type != args[j]->type) continue;
            
            switch(args[0]->type) {

            }
        }
    //}*/

    //UPD: sort them'all fast!
    const int count = sizeof(EnumTypes)/sizeof(EnumTypes[0]);

    vector<node*> types[count];
    //cout << "sizeof EnumTypes: " << count << '\n';
    //forcely evaluate, then sort
    for (int i=0; i<args.size(); ++i) {
        node* r = evaluate(args[i], library);
        //no clonning, no leaking
        types[r->type].push_back(r);
    }
    //flush args
    args.clear(); //only array of pointers
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
            cr_int = args.size();
            //save memory calls. use old stuff to collect garbage
            args.push_back(types[i][0]);
            for (int j=1; j<types[i].size(); ++j) {
                types[i][0]->data.i = types[i][0]->data.i + types[i][j]->data.i;
                delete types[i][j]; //can reqursively
            }
            break;

        case FLOAT:
            cr_float = args.size();
            args.push_back(types[i][0]);
            for (int j=1; j<types[i].size(); ++j) {
                types[i][0]->data.f = types[i][0]->data.f + types[i][j]->data.f;
                delete types[i][j]; //can reqursively
            }
            break; 

        case TIMES:
            //TODO: optimize
            for (int j=0; j<types[i].size(); ++j) {
                args.push_back(types[i][j]);
            }
            break;

        case LIST: {
            //the depth of sum is 1 like 1D arrays
            int depth = 1;
            //at random, just set the default size by 1th element
            int size = types[i][0]->data.args.size();
            //push to the arguments first
            args.push_back(types[i][0]);

            //collect all valid elements
            vector<node*> valid;
            for (int j=1; j<types[i].size(); ++j) {
                if (types[i][j]->data.args.size() != size) {
                    args.push_back(types[i][j]);
                    continue;
                }
                valid.push_back(types[i][j]);
            }
            //if there is no valid exept the first one
            if (valid.size() == 0) break;

            //sum only the same sized tensors recursively
            node* first = types[i][0];
            //for all 1 level elements
            for (int j=0; j<size; ++j) {
                vector<node*> total;
                //push the first one
                total.push_back(first->data.args[j]);
                //and all others
                for (int k=0; k<valid.size(); ++k) {
                    //push the jth element of kth list
                    total.push_back(valid[k]->data.args[j]);
                    //purge the list
                    delete valid[k]; //only head!
                }
                //rewrite new data to each position of the first array
                //recursively
                first->data.args[j] = plusf(nullptr, total, 0, library);
                #ifdef DEVELOPMENT
                    cout << "column " << j << '\n';
                    cout << first->data.args[j];
                #endif
            }
        }

            break; 

        case VAR: {
            //i need to implement Times to collect the same vars...
            //UPD: 03.04.2021 Got it!
            //we did have to sort them, sad :c

            //03.04.2021
            //! IDEA: VAR->data.id and FUN->data.id are some custom type!
            //it would be really cool if they were a custom type of data.
            //imagine... direct access to the memory.

            //No, I'am a dump.
            
            //take it simple
            for (int j=0; j<types[i].size(); ++j) {
                args.push_back(types[i][j]);
            }            
            
            /*
            //scoring multiplers
            int score[types[i].size()] = {};
            //Just pair them and hope for the best
            //TODO: optimize or get rid of.

            //Algorithm O(3), this is SHIIIIIIT
            for (int reps=0; reps<types[i].size()-2; ++reps) {
                for (int k=1; k<types[i].size(); ++k) {
                    for (int l=k+1; l<types[i].size(); ++l) {
                        //check removed el
                        if (score[k] == -1 || score[l] == -1) continue;
                        //when they have the same id
                        if (types[i][k]->data.id == types[i][l]->data.id) {
                            //do it manually for the faster code
                            delete types[i][l]; //can recursively
                            score[k]++;
                            score[l] = -1;
                        }
                    }
                    //args.push_back(types[i][j]);
                }
            }
            //taking everything into account
            for (int k=0; k<types[i].size(); ++k) {
                if (score[k] > 0) 
            }
            */
        }
            break;

        default:
            throw runtime_error("unknown type for Plus");
            break;
        }
    }

    //some cruthes for INT + FLOAT. To get a fully numerical result
    if (cr_float > -1 && cr_int > -1) {
        #ifdef DEVELOPMENT
            cout << "int + float detected!" << '\n';
            cout << args[cr_float] << '\n';
            cout << args[cr_int] << '\n';
        #endif        
        args[cr_float]->data.f = args[cr_float]->data.f + args[cr_int]->data.i;

        //here is the O(1) solution to remove cr_int's element
        if (cr_int != args.size() - 1) {
            // Beware of move assignment to self
            // see http://stackoverflow.com/questions/13127455/
            //C++17
            //args[cr_int] = move(args.back());
            //Old-shit
            swap(args[cr_int], args.back());
        }
        delete args[args.size()-1]; //can recursively
        args.pop_back();
    }

    //if the sum is collapsed, return only single arg[0]
    if (args.size() == 1) {
        #ifdef DEVELOPMENT
            cout << "sum collapsed!" << '\n';
            cout << args[0] << '\n';
        #endif
        
        //if (itself != nullptr) delete itself; //remove only head
        //! WARNING: conflict when args belongs to itself
        //! WARNING: Memory leaking at args

        //UPD: 01.04.2021
        //creating a refence 
        node* result = args[0];
        if (itself != nullptr) delete itself; //remove only head
        //no memory leaking

        itself = result;
        
        return result;
    }

    //create itself if it doesnt exists
    if (itself == nullptr) itself = new node;
    //write the result
    itself->data.args = args;
    itself->type = PLUS;
    return itself;

}

/**take a product of elements**/
node* timesf(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) {
    #ifdef DEVELOPMENT
        if (args.size() < 2) throw runtime_error("not enough arguements for Times");
    #endif
    //This is a linear operation
    //cannot be used on list directly.
    
    //it seems to be, that I have to sort again... Sad shit for the perfomance
    const int count = sizeof(EnumTypes)/sizeof(EnumTypes[0]);

    vector<node*> types[count];

    //forcely evaluate, then sort
    for (int i=0; i<args.size(); ++i) {
        node* r = evaluate(args[i], library);
        //no clonning, no leaking
        types[r->type].push_back(r);
    }
    //flush args
    args.clear(); //only array of pointers 

    //*some crutches
    int cr_int = -1;
    int cr_float = -1;
    //*end of some crutches

    //For lists exlusively
    node* list_ptr = nullptr;

    //collapse times on each element
    for (int i=0; i<count; ++i) {
        if (types[i].size() == 0) continue;
        switch(i) {
        case INTEGER:
            cr_int = args.size();
            //save memory calls. use old stuff to collect garbage
            args.push_back(types[i][0]);
            for (int j=1; j<types[i].size(); ++j) {
                types[i][0]->data.i = types[i][0]->data.i * types[i][j]->data.i;
                delete types[i][j]; //can reqursively
            }
            break;

        case FLOAT:
            cr_float = args.size();
            args.push_back(types[i][0]);
            for (int j=1; j<types[i].size(); ++j) {
                types[i][0]->data.f = types[i][0]->data.f * types[i][j]->data.f;
                delete types[i][j]; //can reqursively
            }
            break;

        case VAR:
            //nothing to do with vars, just pass them
            //TODO: merge two arrays
            for (int j=0; j<types[i].size(); ++j) {
                args.push_back(types[i][j]);
            }
            break;

        case LIST:
            //it should not contain more than 1 list!
            //TODO: do it better!
            #ifdef DEVELOPMENT
                if (types[i].size()>1) throw runtime_error("Times at pair of lists");
            #endif
            list_ptr = types[i][0];
            break; 
        
        case PLUS:
            throw runtime_error("plus for times is not implemented");
            break;          
        
        default:
            throw runtime_error("unknown type for Times");
            break;                       
        }
    }

    //optimization for the case INT = 0
    //do not spend extra time 
    if (cr_int > -1) {
        if (args[cr_int]->data.i == 0) {
            #ifdef DEVELOPMENT
                cout << "int = 0 detected!" << '\n';
                cout << args[cr_int] << '\n';
            #endif
            //purge all other elements
            for (int j=args.size()-1; j>0; --j) {
                delete args[j]; //can recursively
                args.pop_back();
            }
            #ifdef DEVELOPMENT
                cout << "size of args after purge op. " << args.size() << '\n';
            #endif            
            //to prevent float x int op.
            cr_float = -1;
        }
    }

    //crunches for float x int case
    if (cr_float > -1 && cr_int > -1) {
        #ifdef DEVELOPMENT
            cout << "int * float detected!" << '\n';
            cout << args[cr_float] << '\n';
            cout << args[cr_int] << '\n';
        #endif        
        args[cr_float]->data.f = args[cr_float]->data.f * args[cr_int]->data.i;
        if (cr_int != args.size() - 1) {
            swap(args[cr_int], args.back());
        }
        delete args[args.size()-1]; //can recursively
        args.pop_back();
    }    

    //there are two ways: when Times contains list and not
    //when not
    if (list_ptr == nullptr) {
        //if the timesf is collapsed, return only single arg[0]
        if (args.size() == 1) {
            #ifdef DEVELOPMENT
                cout << "timesf collapsed!" << '\n';
                cout << args[0] << '\n';
            #endif
            node* result = args[0];
            if (itself != nullptr) delete itself; //remove only head
            //no memory leaking
            itself = result;
            return result;
        }

        //create itself if it doesnt exists
        if (itself == nullptr) itself = new node;
        //write the result
        itself->data.args = args;
        itself->type = TIMES;
        return itself;        
    }

    //when yes
    #ifdef DEVELOPMENT
        cout << "list structure detected!" << '\n';
    #endif    
    //go deeper into the list
    //foreach element in the list
    for (int i=0; i<list_ptr->data.args.size(); ++i) {
        //this is a linear operation
        //make a deep copy of the each arguement
            //create a new vector
            vector<node*> args_cp;
            //clone each node
            for (int j=0; j<args.size(); ++j) {
                args_cp.push_back(args[j]->clone());
            }
        //add to it the element of the list
        args_cp.push_back(list_ptr->data.args[i]);
        //compute times expression on each
        list_ptr->data.args[i] = timesf(nullptr, args_cp, 0, library);
    }
    //time to return the list

    //destruct itself if it doesnt exists
    if (itself != nullptr) delete itself; //only head
    //write the result
    return list_ptr;
}

/**the array of all defined functions**/
//variebles are also functinos, which perform calls to the library
//first four are basic types 
node* (*functptr[])(node* itself, vector<node*> &args, unsigned id, vector<node*> &library) = {
    nullptr, nullptr, nullptr, nullptr, hold, list, set, var, plusf, timesf, fun, nullptr, def, nullptr,
    cond, equals
};

/**evaluation function is a replacement to switch-case**/
node* evaluate(node *data, vector<node*> &library) {
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
            node* buffer = data->data.args[0];
            delete data;
            return evaluate(buffer, library); 
        }   
        //functions
        default:

            break;
    }
    #ifdef DEVELOPMENT
        if (data->type > 12)
            throw runtime_error("function beyong the available");
    #endif

    //functions handling  
    return (*functptr[data->type])(data, data->data.args, data->data.id, library);
}

/**clonning utility**/
//to copy some parts of tree from library

