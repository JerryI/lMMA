/**define a function or delayed variable**/
node* def(node* itself, vector<node*> &args, definition* library, char params) {
    //add to the global library
    
    library[args[0]->type].function = prototype_fun;
    library[args[0]->type].data = args[1];

    //write argument's ids to the body of the function
    //each id corresponds to the number of the argument passed to the function
    for (int i=0; i<args[0]->data.args.size(); ++i) {
        //skip pattern-type
        library[args[0]->type].argv.push_back(args[0]->data.args[i]->data.args[0]->type);
    }

    //nothing todo
    return itself;
}