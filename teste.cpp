#include <iostream>
using namespace std;
void test2(void (*test)(string, int))
{
    test("Raja", 2);
}
int main()
{
    auto test1 = [](string name, int a) { cout << "Hello " << name << a << endl; };
    test1("atif", 1);
    test2(test1);
    return 0;
}