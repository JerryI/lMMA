node* norm(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate
    node* p = evaluate(args[0], library, params);
    if (p->type != LIST) goto return_itself;
    {
        //construct the sum
        node* _plus = new node();
        _plus->type = PLUS;

        //calcaulte the sum of squares
        for (int i=0; i<p->data.args.size(); ++i) {
            //TODO: somehow i need to optimize it...
            vector<node*>_args = {p->data.args[i], new node(2)};
            //prevent evaluation (already done)
            _plus->data.args.push_back(power(nullptr, _args, library, params | NOEVALUATION));
        }
        //sqrt
        vector<node*> _a {_plus};
        return sqrtf(nullptr, _a, library, params);
    }
    //default behaviour, when failed
    return_itself: 

    if (itself == nullptr) {
        itself = new node();
        itself->type = NORM;
        itself->data.args = args;
    }
    return itself; 
}