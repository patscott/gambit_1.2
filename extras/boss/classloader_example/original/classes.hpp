#ifndef __CLASSES_HPP__
#define __CLASSES_HPP__

#include <iostream>

class T
{
    public:
        // Member variables
        int i;
        double d;

        // Constructor
        T() : i(1), d(3.14) {}

        // Class methods
        void printMe();
};



class X
{
    public:
        // Member variables
        T t;

        // Constructor
        X() {}

        // Class methods
        T getT();

        void setT(T t_in);

        void refTest(T& t_in, int& i_in)
        {
            T new_t;
            new_t.i = 123;
            new_t.d = 1.23;
            t_in = new_t;

            int new_i = 987;
            i_in = new_i;
        }
};


template <typename Type>
class Container
{
    public:
        // Member variables
        Type var;

        // Constructor
        Container() {}

        Container(Type in) : var(in) {}

        void printMsg()
        {
            std::cout << std::endl;
            std::cout << "A message from class 'Container'." << std::endl;
            std::cout << std::endl;
        }

};

#endif  /* __CLASSES_HPP__ */
