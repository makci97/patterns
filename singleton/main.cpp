#include <iostream>

class Factory;

class Singleton
{
    friend class Factory;

public:
    ~Singleton();

private:
    Singleton()
    {
        std::cout << "IT'S ALIVE!" << std::endl;
    }
};

class Factory
{
    friend class Singleton;

private:
    static int counter;
    static bool isFirst;

public:
    Factory()
    {

        if (isFirst)
        {
            counter = 0;
            isFirst = false;
        }
    }

    Singleton* createSingleton()
    {
        if (counter == 0)
        {
            Singleton* single = new Singleton;
            ++counter;

            return single;
        }

        return NULL;
    }

private:
    static void decreaseCounter()
    {
        --counter;
    }
};
int Factory::counter;
bool Factory::isFirst = true;


Singleton::~Singleton()
{
    Factory::decreaseCounter();
}


int main(int argc, char *argv[])
{
    Factory fact;

    Singleton* first = fact.createSingleton();
    Singleton* second = fact.createSingleton();

    delete first;

    second = fact.createSingleton();

    delete second;

    return 0;
}
