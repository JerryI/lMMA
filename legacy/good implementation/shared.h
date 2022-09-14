#include <vector>
#include <iostream>

#include "logger.h"

using namespace std;
//everything we want - defined types of objects
enum TYPE {INTEGER, FLOAT, STRING, BOOL, HOLD, LIST, SET, VAR, PLUS, TIMES, FUN, CONT, DEF, PATTERN, 
COND, EQUALS, DOT, EXPRESSION, POWER, SUB, SIN, COS, EXP, RULE, REPLACE, PART, DO, UNDEFINED, BREAK, RETURN, CONTINUE, LESSEQUAL, SQRT, LENDTH, NORMALIZE, NORM,
IMUNIT, SUM, TR};
//yess...
//a function and an ordinal number are the same shi.. thing...

/**Names for pretty output and reverse compatibillity with MMA**/
string EnumTypes[] =
{
   "INTEGER", "FLOAT", "STRING","BOOL", "Hold", "List", "Set", "Var", "Plus", "Times", "Function", "Container", "SetDelayed", "Pattern",
   "If", "Equal", "Dot", "CompoundExpression", "Power", "Subscript", "Sin", "Cos", "Exp", "Rule", "ReplaceAll", "Part", "Do", "Null", "Break",
   "Return", "Continue", "LessEqual", "Sqrt", "Length", "Normalize", "Norm", "I", "Sum", "Transpose"
};


//The smallest structural element or building block
//for multipurpose usage
typedef struct node{
    TYPE type;
    struct {
        //for integers (an infinite percision) 
        long int i;
        //boolean
        bool b;
        //numerical calulations
        double f;
        string s;
        //only for the functions and procedures
        vector<node*> args;
    } data;  

    friend inline void allocate(node* t);

    //default constructor
    node() {
        //for the garbage collector
        
        allocate(this);
    }
    //construct integers, floats
    node(const double f) {
        this->type = FLOAT;
        this->data.f = f;

        allocate(this);
    }
    node(const int i) {
        this->type = INTEGER;
        this->data.i = i;

        allocate(this);
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
          
    //pretty output of the nodes
    friend ostream& operator<<(ostream& os, const node* data);

    //clone the whole tree
    node* clone() {
        //copy an object itself
        node* ptr = new node;

        *ptr = *this;

        //copy all arguments recursively
        for (int i=0; i<ptr->data.args.size(); ++i)
            ptr->data.args[i] = ptr->data.args[i]->clone();

        return ptr;
    }

    //compare
    bool diff(node* other) {
        if (other->type != this->type) return true;
        //is it a simple type?
        switch (this->type)
        {
        case INTEGER:
            if (this->data.i == other->data.i) return false;
            break;

        case FLOAT:
            if (this->data.f == other->data.f) return false;
            break;

        case BOOL:
            if (this->data.b == other->data.b) return false;
            break;            

        default:
            break;
        }
        //seems to be a variable or a function
        if (other->data.args.size() != this->data.args.size()) return true;

        for (int i=0; i<other->data.args.size(); ++i)
            if (this->data.args[i]->diff(other->data.args[i])) return true;
        
        //nop
        return false;
    }
    //remove all tree reqursively
    /*void operator delete(void *p) {
        node *pp = (node*) p;
        for (int i=0; i<pp->data.args.size(); ++i)
            delete pp->data.args[i];   

        free(p);     
    }*/

} node;

//structure for the library
//each element contains it's data, function pointer and id (for restoring)
typedef struct definition{
    vector<unsigned> argv;
    node* data;
    node* (*function)(node* itself, vector<node*> &args, definition* library, char params);
    unsigned restore_id;
    //all operations are sent to shared library ("SET", "GET", "Append") via TCP
    bool shared;
    
} definition;

//some helpers for cycles and etc (to avoid allocating extra memory)
//are used as a some sort of flags for the program.
node null_node;
node break_node;
node return_node;

//garbage collector
vector<node*> memory;

inline void allocate(node* t) {
    memory.push_back(t);
}

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
    
            default:
                if (data->type < sizeof(EnumTypes)/sizeof(EnumTypes[0]))
                    log << EnumTypes[data->type] << '\n';
                else
                    log << data->type << '\n';
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

/**utilities**/

//for integers
int simple_pow(long int &p, long int &o) {
    long int y = p;
    for (long int i = 0; i < p; ++i)
        y *= o;

    return y;
}
