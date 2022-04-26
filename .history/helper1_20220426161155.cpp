
// #include <iostream>
// #include <vector>
// #include <unordered_map>
#include "helper1.h"
// // #include "helper2.h"
// using namespace std;

// extern int* exta;
// void func(){
//     exta[0] = 111;
// }
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <stdexcept>
#include <atomic>

void func(){
    A aaa = A(3);
    std::cout << aaa.a << std::endl;
}
A::A(int aa){
    a = aa;
}

//hhh