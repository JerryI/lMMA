/**take a part of anything **/
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