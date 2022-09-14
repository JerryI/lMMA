/**complex do cycle**/
node* dof(node* itself, vector<node*> &args, definition* library, char params) {
    //get local variables
    
    node* itr = args[args.size()-1];
    if (itr->type != LIST) return itself;

    //nested cycles
    node* exp = args[0];
    if (args.size() > 2) {
        //cout << "Nested cycles" << '\n';
        //create a fake cycle
        exp = new node();
        exp->type = DO;
        exp->data.args.push_back(args[0]);
        exp->data.args.push_back(args[args.size()-2]);
    }

    //create local stack and save
    definition var;
    var = library[itr->data.args[0]->type];
    var.restore_id = itr->data.args[0]->type;
    //create local iteratie with unknown data
    library[itr->data.args[0]->type].function = prototype_var;
    
    //  I
    //evaluate throuth the list

    if (itr->data.args.size() == 2) {
        //mandatory evaluate
        node* lst = evaluate(itr->data.args[1],library);
        if (lst->type != LIST) return itself;
        //evaluate EXP and repeats
        for (int i=0; i<lst->data.args.size(); ++i) {
            library[itr->data.args[0]->type].data = lst->data.args[i];

            node* res = evaluate(exp, library, params);
            if (res->type == BREAK) break;
        }

        return &null_node;
    }

    //typical befviour

    //mandatory evaluate
    node* begin = evaluate(itr->data.args[1], library, params);
    node* end   = evaluate(itr->data.args[2], library, params);
    node* step;

    if (itr->data.args.size() > 3) {
        step = evaluate(itr->data.args[3], library, params);
    } else {
        step = new node(1);
        //dont forget to purge it!
    }


    //check  types

    //  II
    //the simplest cases

    if (begin->type == INTEGER && end->type == INTEGER && step->type == INTEGER) {
        //evaluate EXP and repeats
        for (int i=begin->data.i; i<=end->data.i; i = i + step->data.i) {
            //create a value for each point
            //! WARNING: memory leaking
            library[itr->data.args[0]->type].data = new node(i);
            
            node* res = evaluate(exp, library, params);
            if (res->type == BREAK) break;
        }

        //purge
        delete step;

        //purge nested parts
        if (args.size() > 2) delete exp;

        return &null_node;    
    }

    if (begin->type == FLOAT && end->type == FLOAT && step->type == FLOAT) {
        //evaluate EXP and repeats
        for (double i=begin->data.f; i<=end->data.f; i = i + step->data.f) {
            //create a value for each point
            //! WARNING: hard memory leaking
            library[itr->data.args[0]->type].data = new node(i);

            node* res = evaluate(exp, library, params);
            if (res->type == BREAK) break;
        }

        //purge, because it was calculatable
        delete step;

        //purge nested parts
        if (args.size() > 2) delete exp;

        return &null_node;    
    }    

    throw runtime_error("not implemented");
    //  III
    //the hardest cases
    /*
    vector<node*> vargs = {begin, step};
    vector<node*> sargs = {begin, end};
    node* i = begin;

    for (;;) {
            //naturally calculate iteratie
            sargs[0] = i;
            node* eq = leq(nullptr, sargs, library, params);
            if (!eq->data.b) break;
            

            library[itr->data.args[0]->type].data = i;
            node* res = evaluate(exp, library, params);
            if (res->type == BREAK) break;

            //purge eq
            delete eq;

            //increment
            vargs[0] = i;
            i = plusf(nullptr, vargs, library, params);
    }
    //I'am afraid to purge i...

    //do not purge step, who knows
    //delete step;
    //purge nested parts
    if (args.size() > 2) delete exp;*/

    return &null_node;

}