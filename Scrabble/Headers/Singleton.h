#pragma once
template<typename T>
//! szablon klasy typu singleton
class Singleton {
public:
    //! funkcja zwracaj¹ca instancjê klasy (instancja jest tworzona za pierwszym wywo³aniem funkcji)
    static T& instance() {
        static T instance{ token{} };
        return instance;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    struct token {};
    Singleton() {}
};

