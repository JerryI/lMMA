#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 2020

using namespace std;

#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

enum TYPE {INTEGER, FLOAT, STRING, BOOL, HOLD, LIST, DOT, TIMES, PLUS, SET, VAR, FUN, SETDELAYED, PAT};


typedef struct node{
    TYPE type;
    struct {
        int i;

        unsigned id;
        vector<unsigned> argv;

        double f;
        string s;
        vector<node*> args;
    } data;  
    
    node() {
        
    }
} node;

//extended space, beyong standart functions
vector<node*> library;

string EnumTypes[] =
{
   "INTEGER", "FLOAT", "STRING","BOOL", "Hold", "List", "Dot", "Times", "Plus", "Set", "Var", "Fun", "SetDelayed", "Pattern"
};

void show(node &data, int level = 0) {
    
    string offset = "";
    for (int i=0; i<level; ++i) offset = offset + "  ";
    
    if (data.type == INTEGER) {
        cout << offset << data.data.i << " : I" << '\n';
        return;
    }
    
    if (data.type == FLOAT) {
        cout << offset << data.data.f << " : F" << '\n';
        return;
    }
    
    if (data.type == STRING) {
        cout << offset << data.data.s << " : S" << '\n';
        return;
    }

    if (data.type == VAR) {
        cout << offset << data.data.id << " : V" << '\n';
        return;
    }

    if (data.type == FUN) {
        cout << offset << data.data.id << " : P" << '\n';

        for (int i=0; i<data.data.args.size(); ++i) {
           show(*data.data.args[i], level+1);
        }        
        return;
    }    
             
    
    cout << offset << "Type: " << EnumTypes[data.type] << '\n';
    
    level = level + 1;
    for (int i=0; i<data.data.args.size(); ++i) {
        node::show(data->data.args[i], level);
    }
    
    return;
}

unsigned constexpr const_hash(char const *input) {
    return *input ?
        static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) :
        5381;
}
unsigned const_hash(string c) {
    return const_hash(c.c_str());
}

node* btree(json &data) {
    node* ptr = new node;
    
    if (data.is_number_integer()) {
        ptr->type = INTEGER;
        ptr->data.i = data.get<int>();
        return ptr;
    }
    
    if (data.is_number_float()) {
        ptr->type = FLOAT;
        ptr->data.f = data.get<double>();
        return ptr;
    }   
    
    if (data.is_string()) {
        ptr->type = VAR;
        ptr->data.id = const_hash(data.get<string>());
        return ptr;
    }    
    
    string name = data[0].get<string>();
    
    switch(const_hash(name)) {
        case const_hash("Hold"):
            ptr->type = HOLD;
            break;
        case const_hash("Dot"):
            ptr->type = DOT;
            break;
        case const_hash("List"):
            ptr->type = LIST;
            break;
        case const_hash("Plus"):
            ptr->type = PLUS;
            break;
        case const_hash("Times"):
            ptr->type = TIMES;
            break;
        case const_hash("Set"):
            ptr->type = SET;
            break;
        case const_hash("Pattern"):
            ptr->type = PAT;
            break;            
        case const_hash("SetDelayed"):
            ptr->type = SETDELAYED;
            //local library, form indexes differently

            break;              
        default:
            //custom function?
            cout << '\n' << name << " - is it a custom function with " << data.size()-1 << "arguements?";
            ptr->type = FUN; 
            ptr->data.id = const_hash(name);           
    }
    
    if (data.size() < 2) return ptr;
    
    for (int i = 1; i<data.size(); ++i) {
        json obj = data[i];
        
        ptr->data.args.push_back(btree(obj));
        
    }
    
    return ptr;
};

json* ftree(node &data) {
    json* ptr = new json;
    
    if (data.type == INTEGER) {
        *ptr = data.data.i;
        return ptr;
    }
    
    if (data.type == FLOAT) {
        *ptr = data.data.f;
        return ptr;
    }   
    
    if (data.type == STRING) {
        *ptr = data.data.s;
        return ptr;
    }    
    
    ptr->push_back(EnumTypes[data.type]);
    
    for (int i = 0; i<data.data.args.size(); ++i) {
        
       ptr->push_back(*ftree(*data.data.args[i]));
        
    }
    
    return ptr;
};

