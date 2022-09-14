#include <thread>
#include <iostream>
#include <string>

#include <vector>

using namespace std;

#include "server.h"

typedef struct cmd{
    char t;
    uint8_t id;
    char* data;
} cmd;

vector<cmd*> fifo;

void ls(SOCKET cli) {
    for (;;) {
        char* b = read(cli);
        char* ptr = b;
        for (; *ptr != 0;) { 
            //get info and push
            cmd *el = new cmd;
            el->t = *ptr; ptr++;
            el->id = *ptr; ptr++;

            char* begin = ptr;
            for (; *ptr != 0; ptr++) {}

            el->data = new char[(size_t)(ptr - begin)];
            memcpy((void*) el->data, (void*) begin, (size_t)(ptr - begin));
            fifo.push_back(el);

            cout << el->data;
            cout << '\n';
            ptr++;
        }
        //cout << b << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void exc() {
    for (int i=0; i<fifo.size(); ++i) {
        if (fifo[i] == nullptr) continue;

        cmd* el = fifo[i];
        fifo[i] = nullptr;

        switch

    }
}

void pong() {
    for (;;) {

        cout << "pong" << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void serve() {

    //read2fifo();
    //exicute();
    write2fifo();
}


int main() {

    cout << "Hello!" << '\n';

    SOCKET id = initServer();
    SOCKET cli = listen(id);

    std::thread thr0(ls, cli);
    std::thread thr1(pong);
    thr0.join();
    thr1.join();

    
    return 0;
}