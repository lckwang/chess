
#include <iostream>
//#define app_operator_cout
//#define app_object_slicing
#define app_megative_modula
#ifdef app_operator_cout
class Base
{
public:
    Base() {}

    // Here's our overloaded operator<<
    friend std::ostream& operator<<(std::ostream& out, const Base& b)
    {
        // Delegate printing responsibility for printing to member function print()
        return b.print(out);
    }

    // We'll rely on member function print() to do the actual printing
    // Because print is a normal member function, it can be virtualized
    virtual std::ostream& print(std::ostream& out) const
    {
        out << "Base";
        return out;
    }
};

class Derived : public Base
{
public:
    Derived() {}

    // Here's our override print function to handle the Derived case
    virtual std::ostream& print(std::ostream& out) const override
    {
        out << "Derived";
        return out;
    }
};

int main()
{
    Base b;
    std::cout << b << '\n';

    Derived d;
    std::cout << d << '\n'; // note that this works even with no operator<< that explicitly handles Derived objects

    Base& bref = d;
    std::cout << bref << '\n';

    return 0;
}
#endif
#ifdef app_object_slicing
using namespace std;
struct A
{
    A(int a) : a_var(a) {}
    int a_var;
    virtual void print() { cout << "A:a=" << a_var << endl; };
};

struct B : public A
{
    B(int a, int b) : A(a), b_var(b) {}
    int b_var;
    virtual void print() { cout << "B:a=" << a_var << " B:b=" << b_var << endl; };
};

B& getB()
{
    static B b(1, 2);
    return b;
}

int main()
{
    // Normal assignment by value to a
    A a(3);
    //a.a_var ==3
    a = getB();
    // a.a_var == 1, b.b_var not copied to a
    a.print();

    B b2(3, 4);
    //b2.a_var==3 ,b2.b_var==4
    A& a2 = b2;
    a2.print();

    // Partial assignment by value through reference to b2
    a2 = getB();
    // b2.a_var == 1, b2.b_var == 4!
    a2.print();

    return 0;
}
#endif
#ifdef app_megative_modula
int main() {
    int x = -1234;
    std::cout << x % 10 << std::endl;
    return 0;
}

#endif