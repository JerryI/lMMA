#include <vector>
#include <iostream>

#include "logger.h"

using namespace std;
//everything we want types of object
enum TYPE {INTEGER, FLOAT, STRING, BOOL, HOLD, LIST, SET, VAR, PLUS, TIMES, FUN, CONT, DEF, PATTERN, COND, EQUALS};
//yess...
//a function and an ordinal number are the same thing...

/**Names for pretty output and reverse compatibillity with Mathematica**/
string EnumTypes[] =
{
   "INTEGER", "FLOAT", "STRING","BOOL", "Hold", "List", "Set", "Var", "Plus", "Times", "Function", "Container", "SetDelayed", "Pattern",
   "If", "Equals"
};

//The smallest structural element or building block
//multipurpose usage due to the struct type of the inner data
typedef struct node{
    TYPE type;
    struct {
        //for integers (an infinite percision) 
        int i;
        //boolean
        bool b;
        //only for variables and user-defined functions
        unsigned id;
        vector<unsigned> argv;
        //numerical calulations
        double f;
        string s;
        //only for build-in functions and procedures
        vector<node*> args;
    } data;  
    //default constructor
    node() {
        
    }
    //construct integers, floats
    node(const double f) {
        this->type = FLOAT;
        this->data.f = f;
    }
    node(const int i) {
        this->type = INTEGER;
        this->data.i = i;
    }
    //construct a vector
    template<typename T>
    node(initializer_list<T> arr) {
        this->type = LIST;
        for (int i=0; i<arr.size(); ++i) {
            this->data.args.push_back(new node(arr.begin()[i]));
        }
    }
    //construct a matrix
    template<typename T>
    node(initializer_list<initializer_list<T>> arr) {
        this->type = LIST;
        for (int i=0; i<arr.size(); ++i) {
            this->data.args.push_back(new node(arr.begin()[i]));
        }
    }    
          
    //pretty output of the tree
    friend ostream& operator<<(ostream& os, const node* data);

    //clone all tree
    node* clone() {
        //copy an object itself
        node* ptr = new node;

        *ptr = *this;

        //copy all arguments recursively
        for (int i=0; i<ptr->data.args.size(); ++i)
            ptr->data.args[i] = ptr->data.args[i]->clone();

        return ptr;
    }
    //remove all tree reqursively
    /*void operator delete(void *p) {
        node *pp = (node*) p;
        for (int i=0; i<pp->data.args.size(); ++i)
            delete pp->data.args[i];   

        free(p);     
    }*/

} node;

void formatted_out(const node* data, Logger &log) {
        switch(data->type) {
            case INTEGER:
                log << "I:" << '\t' << data->data.i << '\n';
                return;
            case FLOAT:
                log << "F:" << '\t' << data->data.f << '\n';
                return;
            case BOOL:
                log << "B:" << '\t' << data->data.b << '\n';
                return;                
            case STRING:
                log << "S:" << '\t' << data->data.s << '\n';
                return;
            case VAR:
                log << "V:" << '\t' << data->data.id << '\n';
                return;
            case FUN:
                log << "F:" << '\t' << data->data.id << '\n';
                break;                               
    
            default:
                log << EnumTypes[data->type] << '\n';
                break;
        }
             
        
    
        //increase the level of hierarchy
        log.increaseLevel();
        for (int i=0; i<data->data.args.size(); ++i) {
            formatted_out(data->data.args[i], log);
        }
        log.decreaseLevel();
}

ostream& operator<<(ostream& os, const node* data) {
        //custom logger class from https://stackoverflow.com/a/25217417/12913322
        Logger log(os);

        formatted_out(data, log);

        return os;
}

/**extended space, beyong standart functions**/
//yes, we can store defined functions and varibles using standart type node
//good data type, ha?
//vector<node*> library;



node* watch;
int cnt = 0;