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
            int exists = -1;
            for (int i=0; i<library.size(); ++i) {
                if (library[i]->data.id == data->data.args[0]->data.id) {
                    exists = i;
                    break;
                }
            }
            
            if (exists > -1) {
                data->data.args[1]->data.id = data->data.args[0]->data.id;
                library[exists] = data->data.args[1];
                
                data->type = STRING;
                data->data.s = "Null";                
            } else {
                data->data.args[1]->data.id = data->data.args[0]->data.id;
                library.push_back(data->data.args[1]);
                
                data->type = STRING;
                data->data.s = "Null";
            }
            
            
        }
            break;
        
        case SETDELAYED: {
            int exists = -1;
            for (int i=0; i<library.size(); ++i) {
                if (library[i]->data.id == data->data.args[0]->data.id) {
                    exists = i;
                    break;
                }
            }
            
            //Only for functions
            if (data->data.args[0]->type == VAR) break;
            
            if (exists > -1) {
                data->data.args[1]->data.id = data->data.args[0]->data.id;
                library[exists] = data->data.args[1];
                
                //only 1 arg
                node* pat = data->data.args[0]->data.args[0];
                if (pat->type == PAT) {
                    library[exists]->data.argv.clear();
                    library[exists]->data.argv.push_back(pat->data.args[0]->data.id);
                }
                
                data->type = STRING;
                data->data.s = "Null";                
            } else {
                data->data.args[1]->data.id = data->data.args[0]->data.id;
                
                //only 1 arg
                node* pat = data->data.args[0]->data.args[0];
                if (pat->type == PAT) {
                    data->data.args[1]->data.argv.push_back(pat->data.args[0]->data.id);
                }                  
                
                library.push_back(data->data.args[1]);

                data->type = STRING;
                data->data.s = "Null";
            }            
        }
            break;
        
        case FUN: {
            for (int i=0; i<library.size(); ++i) {
                if (library[i]->data.id == data->data.id) {
                    node* record = new node;
                    
                    //localize env
                    vector<node*> local;
                    //copy(library.begin(), library.end(), local.begin());
                    
                    //copy arguments to local env
                    for (int k=0; k<library[i]->data.argv.size(); ++k) {
                        node* localVar = new node;
                        
                        //copy argument
                        *localVar = *(data->data.args[0]);
                        localVar->data.id = library[i]->data.argv[k];
                        local.push_back(localVar);
                    }
                    
                    //copy function ha-ha. I have to copy the whole tree!!!!!!
                    node* function = new node;
                    *function = *library[i];
                    
                    //swap global to local
                    library.swap(local);
                    
                    //evaluate
                    function = evaluate(function);
                    
                    //swap back
                    library.swap(local);
                    
                    
                    data = function;
                    return data;
                }
            }            
        }   
            break;
            
        case VAR: {
            for (int i=0; i<library.size(); ++i) {
                if (library[i]->data.id == data->data.id) {
                    node* record = new node;
                    *record = *library[i]; 
                    data = record;
                }
            }
        }
            break;
            
        case PLUS: {
            cout << "do plus" << '\n';
            vector<node*> sum;
            vector<int> indexes;
            
            for (int k=0; k<data->data.args.size(); ++k)
                for (int l=k+1; l<data->data.args.size(); ++l) {
                
                    node* s1 = data->data.args[k];
                    node* s2 = data->data.args[l]; 
                    
                    if (s1->type == INTEGER and s2->type == INTEGER) {
                        
                        node* r = new node;
                        r->type = INTEGER;
                        r->data.i = s1->data.i + s2->data.i;
                        sum.push_back(r);
                        indexes.push_back(k);
                        indexes.push_back(l);
                        continue;
                    } 
                    
                    if (s1->type == FLOAT and s2->type == FLOAT) {
               
                        node* r = new node;
                        r->type = FLOAT;
                        r->data.f = s1->data.f + s2->data.f;
                        sum.push_back(r);
                        indexes.push_back(k);
                        indexes.push_back(l);
                        continue;
                    }   
                    
                    if (s1->type == INTEGER and s2->type == FLOAT) {
                
                        node* r = new node;
                        r->type = FLOAT;
                        r->data.f = s1->data.i + s2->data.f;
                        sum.push_back(r);
                        indexes.push_back(k);
                        indexes.push_back(l);
                        continue;
                    }  
                    
                    if (s1->type == FLOAT and s2->type == INTEGER) {
        
                        node* r = new node;
                        r->type = FLOAT;
                        r->data.f = s1->data.f + s2->data.i;
                        sum.push_back(r);
                        indexes.push_back(k);
                        indexes.push_back(l);
                        continue;
                    }
                    
                    if (s1->type == LIST and s2->type == LIST) {
                        //matrix summation
                        int size1y = s1->data.args.size();
                        int size2y = s2->data.args.size();
                        int size1x = 0;
                        int size2x = 0;
                        
                        for (int i=0; i<size1y; ++i) {
                            if (s1->data.args[i]->type == LIST) {
                                size1x = s1->data.args[i]->data.args.size();
                            } else {
                                cout << "nop \n";
                                data->data.args[k] = evaluate(s1);
                                
                                continue;
                            }
                        }
        
                        for (int i=0; i<size2y; ++i) {
                            if (s2->data.args[i]->type == LIST) {
                                size2x = s2->data.args[i]->data.args.size();
                            } else {
                                cout << "nop \n";
                                data->data.args[l] = evaluate(s2);
                                
                                continue;
                            }
                        }
                        
                        if (size1x == size2x and size1y == size2y) {
                            
                            
                            for (int i=0; i<size1x; ++i) {
                                for (int j=0; j<size1y; ++j) {
                                    node* r = new node;
                                    
                                    r->type = PLUS;
                                    r->data.args.push_back(s1->data.args[j]->data.args[i]);
                                    r->data.args.push_back(s2->data.args[j]->data.args[i]);
                                    
                                    s1->data.args[j]->data.args[i] = r;
                                }
                            }
                            
                            sum.push_back(s1);
                            
                            
                            indexes.push_back(k);
                            indexes.push_back(l);
                        } 
                        
                    } else {
                        data->data.args[k] = evaluate(data->data.args[k]);
                        data->data.args[l] = evaluate(data->data.args[l]);
                    }
                }
            
            //remove indexes
            for (int k=0; k<data->data.args.size(); ++k) {
                bool check = true;
                for (int i=0; i<indexes.size(); ++i)
                    if (indexes[i] == k) check = false;
                
                if (check) sum.push_back(data->data.args[k]);
            }
            
            if (sum.size() == 1)
                data = sum[0];
            else
                data->data.args = sum;
            
        }
            break;
            
        case TIMES: {
            cout << "do times with" << '\n';
            
            node* s1 = data->data.args[0];
            node* s2 = data->data.args[1];
            
            show(*s1);
            show(*s2);
            
            if (s1->type == INTEGER and s2->type == INTEGER) {
                cout << "ints";
                //return data;
                data->type = INTEGER;
                data->data.i = s1->data.i * s2->data.i;
                //delete s1; delete s2;
                return data;
            } 
            
            if (s1->type == FLOAT and s2->type == FLOAT) {
                cout << "floats";
                //return data;
                data->type = FLOAT;
                data->data.f = s1->data.f * s2->data.f;
                //delete s1; delete s2;
                return data;
            }   
            
            if (s1->type == INTEGER and s2->type == FLOAT) {
                cout << "mixed mult";
                //return data;
                data->type = FLOAT;
                data->data.f = s1->data.i * s2->data.f;
                //delete s1; delete s2;
                return data;
            }  
            
            if (s1->type == FLOAT and s2->type == INTEGER) {
                cout << "mixed mult";
                //return data;
                data->type = FLOAT;
                data->data.f = s1->data.f * s2->data.i;
                //delete s1; delete s2;
                return data;
            }
            
            
            
           
            node* cast;
            node* var;
            bool times = false;
                        
            if (s1->type == LIST or s1->type == PLUS) {
                cast = s1;
                var = s2;
            } else if (s2->type == LIST or s2->type == PLUS) {
                cast = s2;
                var = s1;
            } else if (s1->type == TIMES) {
                cast = s1;
                var = s2;    
                times = true;
            } else if (s2->type == TIMES) {
                cast = s2;
                var = s1;    
                times = true;
            } else {
                data->data.args[0] = evaluate(data->data.args[0]);
                data->data.args[1] = evaluate(data->data.args[1]);
                return data;
            }
            
            if (times) {
               
            } else {
            
                cout << "go futher" << '\n';
                
                for (int i=0; i<cast->data.args.size(); ++i) {
                    node *times = new node;
                    times->type = TIMES;
                    
                    times->data.args.push_back(var);
                    times->data.args.push_back(cast->data.args[i]);
                    cast->data.args[i] = times;
                }
                
                //delete data;
                
                data = cast;
            
            }
            
            
        }
            break;
        
        case DOT: {
            node* s1 = data->data.args[0];
            node* s2 = data->data.args[1];
            
            if (s1->type != LIST) {
                cout << "not lists..." << '\n';
                data->data.args[0] = evaluate(s1);
                return data;
            }
            
            if (s2->type != LIST) {
                cout << "not lists..." << '\n';
                data->data.args[1] = evaluate(s2);
                return data;
            }            
            
            int size1y = s1->data.args.size();
            int size2y = s2->data.args.size();
            int size1x = 0;
            int size2x = 0;
            
            for (int i=0; i<size1y; ++i) {
                if (s1->data.args[i]->type == LIST) {
                    size1x = s1->data.args[i]->data.args.size();
                } else {
                    cout << "nop \n";
                    data->data.args[0] = evaluate(s1);
                    return data;
                }
            }
            
            for (int i=0; i<size2y; ++i) {
                if (s2->data.args[i]->type == LIST) {
                    size2x = s2->data.args[i]->data.args.size();
                } else {
                    cout << "nop \n";
                    data->data.args[1] = evaluate(s2);
                    return data;
                }
            } 
            
            cout << "dims1 " << size1x << " x " << size1y << '\n';
            cout << "dims2 " << size2x << " x " << size2y << '\n';
            
            if (size1x != size2y) {cout << "err. Dims must agree"; break;}
            
            node *row = new node;
            row->type = LIST;
            
            for (int j=0; j<size1y; ++j) {
                node* column = new node;
                column->type = LIST;
                
                for (int i=0; i<size2x; ++i) {
                    node* sum = new node;
                    sum->type = PLUS;
                    
                    for (int k=0; k<size1x; ++k) {
                        node* product = new node;
                        product->type = TIMES;
                        
                        product->data.args.push_back(s1->data.args[j]->data.args[k]);
                        product->data.args.push_back(s2->data.args[k]->data.args[i]);
                        
                        sum->data.args.push_back(product);
                    }
                    
                    column->data.args.push_back(sum);
                }
                row->data.args.push_back(column);
            }
            
            //Purge
            for (int j=0; j<size1y; ++j) {
                //delete s1->data.args[j];
            }
            for (int j=0; j<size2y; ++j) {
                //delete s2->data.args[j];
            } 
            //delete s1;
            //delete s2;
            
            //delete data;
            
            
            data = row;            
            
        }
            break;
        
        default:
        break;
    }
    
    return data;
};