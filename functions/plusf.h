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