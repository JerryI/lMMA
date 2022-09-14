#include "libraries.h"

node* evaluate(node *data, vector<node*> &library) {
    switch(data->type) {       
        case HOLD:
            for(int i=0; i<data->data.args.size(); ++i) {
                data->data.args[i] = evaluate(data->data.args[i]);
            }
            break;
            
        case LIST:
            for(int i=0; i<data->data.args.size(); ++i) {
                data->data.args[i] = evaluate(data->data.args[i]);
            }
            break; 

        case SET: {
            #ifndef DEVELOPMENT
            if (data->data.args.size() != 2) 
                throw runtime_error('SET operator must have two arguments!');
            #endif

            //Search for a variable in the library
            int exists = -1;
            for (int i=0; i<library.size(); ++i) {
                if (library[i]->data.id == data->data.args[0]->data.id) {
                    exists = i;
                    break;
                }
            }
            
            if (exists > -1) {
                //if yes, so, replace it
                //rewrite the id
                data->data.args[1]->data.id = data->data.args[0]->data.id;
                //move the data to to the library by pointer (no clonning)
                library[exists] = data->data.args[1];
                //delete var object from SET operator
                delete data->data.args[0];

                //to-do
                //return by the value by clonning it to SET object
                //may be reutulization... call function                
            } else {
                //if no, add it to the library
                data->data.args[1]->data.id = data->data.args[0]->data.id;
                library.push_back(data->data.args[1]);

                //delete var object from SET operator
                delete data->data.args[0];

                //to-do
                //return by the value by clonning it to SET object
            }            
        }
            break;

        default:
            throw runtime_error('Some shit happed');

    return data;
};

//I cannot implement it...
void clone(node* ) {
    *dest = *source;
}