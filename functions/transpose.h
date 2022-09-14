node* transpose(node* itself, vector<node*> &args, definition* library, char params) {
    node* m = evaluate(args[0], library, params);
    if (m->type != LIST) goto return_itself;



    //when failed
    return_itself:
    if (itself == nullptr) {
        itself = new node();
        itself->type = TR;
        itself->data.args.push_back(args[0]);
    }
    return itself;
}