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