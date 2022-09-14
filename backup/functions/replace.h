/**replace operation**/
//acts like a local function
node* replace(node* itself, vector<node*> &args, definition* library, char params) {
    //how many rules?
    node* rules = evaluate(args[1], library, params);
    int count = 0;

    if (rules->type == RULE) count = 1;
    if (rules->type == LIST) count = rules->data.args.size();

    if (count == 0) return itself;

    //cout << "rules: " << rules << '\n';
    //cout << "count: " << count << '\n';

    //load all rules into library like in a function
    definition stack[count];

    if (rules->type == RULE) {
        //stack[0] = library[rules->data.args[0]->type];
        //stack[0].restore_id = rules->data.args[0]->type;   

        //The general case. We don't know.
        //library[rules->data.args[0]->type].function = undefined;
        //library[rules->data.args[0]->type].data = evaluate(rules->data.args[1], library, params);

        //node* res = evaluate(args[0], library, params); 
        //library[stack[0].restore_id] = stack[0];

        //return res;
        return itself;
    }
    //cout << "list of rules" << '\n';
    //add all definitions
    for (int i=0; i<count; ++i) {
        node* rl = rules->data.args[i];
        //cout << "rule: " << i << "  is :" << rl << '\n';
        stack[i] = library[rl->data.args[0]->type];
        stack[i].restore_id = rl->data.args[0]->type;

        //library[stack[i].restore_id].function = prototype_var;
        //library[stack[i].restore_id].data = evaluate(rl->data.args[1], library, params);

        //use the native method
        //do not prevent evaluation, because rule function does nothing
        set(nullptr, rl->data.args, library, params);
    }
 
    //run the fake function
    node* res = evaluate(args[0], library, params); 

    //restore
    
    for (int i=0; i<count; ++i) {
        library[stack[i].restore_id] = stack[i];
    }    
    return res;    

}

/**container for rules**/
node* rule(node* itself, vector<node*> &args, definition* library, char params) {
    //nop
    return itself;
}