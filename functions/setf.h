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