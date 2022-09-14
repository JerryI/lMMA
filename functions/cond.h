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