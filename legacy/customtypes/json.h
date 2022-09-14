#include "assets/json.hpp"
#include <unordered_set>

using json = nlohmann::json;

size_t fix_2digits(size_t n) { return n % 150LU; }

string json_namespace[200];

node* btree(json &data) {
    node* ptr = new node;

    std::hash<std::string> hash_fn;
    
    //default types
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

    if (data.is_null()) {
        ptr->type = UNDEFINED;
        return ptr;
    }

    const int size = data.size();
    json name = data;

    if (size > 1) name = data[0];
    if (name.is_array()) name = name[0];

    bool undef = true;

    string str = name.get<string>();
    
    //Check defined functions
    for (int i=0; i<sizeof(EnumTypes)/sizeof(EnumTypes[0]); ++i) {
        if (EnumTypes[i].compare(str) == 0) {
            ptr->type = (TYPE) i;
            undef = false;
            break;
        }
    }

    if (undef) {
        ptr->type = (TYPE) (fix_2digits(hash_fn(str)) + 30);
    }

    //add to the namespace
    json_namespace[ptr->type] = str;

    for (int i=1; i<size; ++i) {
        ptr->data.args.push_back(btree(data[i]));
    }    
    
    return ptr;
};

json* ftree(node *data) {
    json* ptr = new json;
    
    if (data->type == INTEGER) {
        *ptr = data->data.i;
        return ptr;
    }
    
    if (data->type == FLOAT) {
        *ptr = data->data.f;
        return ptr;
    }   
    
    if (data->type == BOOL) {
        *ptr = data->data.b;
        return ptr;
    }    
    
    json name;
    if (json_namespace[data->type].length() == 0 ) {
        name = EnumTypes[data->type];
    } else {
        name = json_namespace[data->type];
    }

    if (data->data.args.size() == 0) {
        *ptr = name;
        

        return ptr;
    }
    
    ptr->push_back(name);
    
    for (int i = 0; i<data->data.args.size(); ++i) {
       ptr->push_back(*ftree(data->data.args[i]));
    }
    
    return ptr;
};

node* parse(char* buffer) {
    json message = json::parse(buffer);
    node* tree = btree(message);

    return tree;
}

string stringify(node* buffer) {
    json* message = ftree(buffer);
    return message->dump();
}