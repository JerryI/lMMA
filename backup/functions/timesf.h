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