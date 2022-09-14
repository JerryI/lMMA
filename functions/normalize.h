node* normalize_n(node* itself, vector<node*> &args, definition* library, char params) {
    //numerical fast version
    if (type != NVECTOR) convert()
    if (type != NMATRIX) convert()
}

node* normalize(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate
    //
    //UPD: unnecessary, will be done by norm operation or times
    //! WARNING: too many evaluate calls

    //check if list is numerical

    //if not
    //calculate norm
    //node* p = evaluate(args[0], library, params);

    if (params & NUMERIZE) return normalize_n();

    node* n = norm(nullptr, args, library, params);

    //inverse it
    vector<node*> _args {n, new node(-1)};

    //times
    vector<node*> __args {power(nullptr, _args, library, params | NOEVALUATION), args[0]};
    return timesf(nullptr, __args, library, params);
}