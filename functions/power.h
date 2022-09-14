/**power function**/
node* power(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate
    node* p = evaluate(args[1], library, params);
    node* o = evaluate(args[0], library, params);

    //calculatable cases
    if (p->type == INTEGER && o->type == INTEGER && p->data.i >= 0) {
        return(new node(simple_pow(p->data.i, o->data.i)));
    }

    if (p->type == INTEGER && o->type == FLOAT) {
        return(new node(pow(p->data.i, o->data.f)));
    }   

    if (p->type == FLOAT && o->type == INTEGER) {
        return(new node(pow(p->data.f, o->data.i)));
    }     

    if (p->type == FLOAT && o->type == FLOAT) {
        return(new node(pow(p->data.f, o->data.f)));
    } 

    if (p->type == INTEGER && o->type == IMUNIT) {
            //superfast calc
            //cout << "Imagine" << '\n';
            switch(p->data.i % 4) {
                case 1: 
                    return o;
                    break;
                case 2:
                    return new node(-1);
                    break;
                case 3: {
                    vector<node*> _args = {o, new node(-1)};
                    return timesf(nullptr, _args, library, params | NOEVALUATION);
                }
                    break; 
                case 0:
                    return new node(1);
                    break;                                                         
            }    
    } 

    //default behaviour, when failed
    if (itself == nullptr) {
        itself = new node();
        itself->type = POWER;
        itself->data.args = args;
    }
    return itself;    
}