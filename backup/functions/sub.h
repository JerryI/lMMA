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