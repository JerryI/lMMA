#include <iostream>
#include <windows.h>
#include <vector>

#define BOOL BOOLD
#define FLOAT FLOATD
#define PATTERN PATTERND

#include "core.h"


#undef BOOL
#undef FLOAT
#undef PATTERN


using namespace std;

node vv(10);
node nn(17);


unsigned long long multiply(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size) 
{
    return (unsigned long long)&vv;
}

int main()
{  

    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    auto const page_size = system_info.dwPageSize;
    cout << page_size;

    // prepare the memory in which the machine code will be put (it's not executable yet):
    auto const buffer = VirtualAlloc(nullptr, page_size, MEM_COMMIT, PAGE_READWRITE);

    // copy the machine code into that memory:
    vector<unsigned char> code( (char*)&multiply, (char*)&main );

    memcpy(buffer, code.data(), code.size());

    // mark the memory as executable:
    DWORD dummy;
    VirtualProtect(buffer, code.size(), PAGE_EXECUTE_READ, &dummy);

    // interpret the beginning of the (now) executable memory as the entry
    // point of a function taking no arguments and returning a 4-byte int:
    auto const function_ptr = reinterpret_cast<node*(*)(node* itself, vector<node*> &args, unsigned id, node** library, node* stack, unsigned stack_size)>(buffer);

    vector<node*> lib;
    // call the function and store the result in a local std::int32_t object:
    auto result = function_ptr(nullptr, lib, 0, nullptr, nullptr, 0);

    // free the executable memory:
    //VirtualFree(buffer, 0, MEM_RELEASE);

    // use your std::int32_t:
    std::cout << '\n' << result << "\n";
    std::cout << '\n' << (unsigned long long )&vv << "\n";
}