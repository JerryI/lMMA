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