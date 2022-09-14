#include <chrono>

#define DEVELOPMENT0

#define FLOAT FLOATF
#define PATTERN PATTERNF
#define BOOL BOOLF

#include "../core.h"
#include "../json.h"

#include "../server.h"

using namespace std;



int main() {

    cout << "Hello!" << '\n';
    init();

    SOCKET id = initServer();
    SOCKET cli = listen(id);

    for (;;) {
        char* b = read(cli);
        cout << b;
        if (strstr(b, "Exit")  != nullptr) break;
        node* in = parse(b);
        cout << "In[x]:" << '\n';
        cout << in << '\n';
        node* out = evaluate(in);
        cout << "Out[x]:" << '\n';
        cout << out;
        reply(stringify(out), cli);
    }

    closesocket(id);

}