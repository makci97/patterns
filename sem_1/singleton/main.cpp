#include <iostream>

class Singleton
{
public:
    static Singleton* createSingleton()
    {
        if (isFirst)
        {
            Singleton* single = new Singleton;
            isFirst = false;

            return single;
        }

        return NULL;
    }

    ~Singleton()
    {
        isFirst = true;
    }

    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;

private:
    Singleton()
    {
        std::cout << "IT'S ALIVE!" << std::endl;
    }

private:
    static bool isFirst;
};
bool Singleton::isFirst = true;


int main(int argc, char *argv[])
{
    Singleton* first = Singleton::createSingleton();
    Singleton* second = Singleton::createSingleton();

    delete first;

    second = Singleton::createSingleton();

    delete second;

    return 0;
}
