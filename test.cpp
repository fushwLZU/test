#include <iostream>
#include <vector>
#include <memory>
#include "helper1.h"
#include "helper2.h"
using namespace std;

class A{
public:
    virtual void func();
};
class B : public A{
    void func(){
        cout << "hel" <<endl;
    }
};
int main(){
    A a = A();
    a.func();
}