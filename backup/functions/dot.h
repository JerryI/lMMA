/**mixed types matrix multiplication**/
node* dot(node* itself, vector<node*> &args, definition* library, char params) {
    //mandatory evaluate them
    node* s1 = evaluate(args[0], library, params);
    node* s2 = evaluate(args[1], library, params);

    //only for lists
    if (s1->type != LIST || s2->type != LIST) return itself;

    //get numbers of columns 
    const int size1y = s1->data.args.size();
    const int size2y = s2->data.args.size();
    int size1x = 0;
    int size2x = 0;
    
    //get numbers of rows
    for (int i=0; i<size1y; ++i) {
        if (s1->data.args[i]->type == LIST) {
            size1x = s1->data.args[i]->data.args.size();
        } else {
            return itself;
        }
    }
            
    for (int i=0; i<size2y; ++i) {
        if (s2->data.args[i]->type == LIST) {
            size2x = s2->data.args[i]->data.args.size();
        } else {
            return itself;
        }
    }  

    //must match
    if (size1x != size2y) {
        #ifdef DEVELOPMENT
            cout << "dims must agree!";
        #endif
        return itself;
    }

    //TODO: Parallelize!

    //construct a new table
    node* table = new node;
    table->type = LIST;

    vector<node*> p(2);
    vector<node*> sum(size1x);

    //traditional shitslow matrix multiplication
    //#pragma omp parallel for private(sum,p)
    for (int j=0; j<size1y; ++j) {
        node* column = new node;
        column->type = LIST;
        table->data.args.push_back(column);

        for (int i=0; i<size2x; ++i) {
            for (int k=0; k<size1x; ++k) {
                p[0] = s1->data.args[j]->data.args[k];
                p[1] = s2->data.args[k]->data.args[i];

                sum[k] = timesf(nullptr, p, library, params | NOEVALUATION);
            }
            column->data.args.push_back(plusf(nullptr, sum, library, params | NOEVALUATION));
        }   
    }  

    return table;
}