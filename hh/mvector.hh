#ifndef MVECTOR_HH
#define MVECTOR_HH

#include <iostream>

const int V_INCREMENT = 5;

template <typename T>
class Mvector{
public:
    Mvector();
    Mvector(size_t inCapacity);
    Mvector(const Mvector<T>& inMvector);
    ~Mvector();
    Mvector& operator=(const Mvector<T>& inMvector);
    T& operator[](size_t idx) const;
    void push_back(const T& value);
    void resize(size_t newCapacity);
    void pop_back();
    size_t size() const;
    bool empty() const;
    void clear();
    void erase(size_t idx);
    T* begin();
    T* end();
    T& front();
    T& back();
    void reserve(size_t inCapacity);

    T* data;
    size_t capacity;
    size_t length;
};

template <typename T>
Mvector<T>::Mvector(){
    data = nullptr;
    capacity = 0;
    length = 0;
}

template <typename T>
Mvector<T>::Mvector(size_t inCapacity){
    data = new T[inCapacity];
    capacity = inCapacity;
    length = 0;
}

template <typename T>
Mvector<T>::Mvector(const Mvector<T>& inMvector){
    data = new T[inMvector.capacity];
    capacity = inMvector.capacity;
    length = inMvector.length;
    for(size_t i = 0; i < length; i++)
        data[i] = inMvector.data[i];
}

template <typename T>
Mvector<T>::~Mvector(){
    delete[] data;
}

template <typename T>
Mvector<T>& Mvector<T>::operator=(const Mvector<T>& inMvector){
    if(this != &inMvector){
        delete[] data;
        capacity = inMvector.capacity;
        length = inMvector.length;
        data = new T[capacity];
        for(size_t i = 0; i < length; i++)
            data[i] = inMvector.data[i];
    }
    return *this;
}

template <typename T>
T& Mvector<T>::operator[](size_t idx) const{
    return data[idx];
}

template <typename T>
void Mvector<T>::push_back(const T& value){
    if(length >= capacity){
        size_t newCapacity;
        if(!capacity)
            newCapacity = V_INCREMENT;
        else
            newCapacity = capacity + V_INCREMENT;
        resize(newCapacity);
    }
    data[length] = value;
    length++;
}

template <typename T>
void Mvector<T>::resize(size_t newCapacity){
    T* newData = new T[newCapacity];
    for(size_t i = 0; i < length; i++)
        newData[i] = data[i];
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
void Mvector<T>::pop_back(){
    if(length > 0)
        length--;
}

template <typename T>
size_t Mvector<T>::size() const{
    return length;
}

template <typename T>
bool Mvector<T>::empty() const{
    return !length;
}

template <typename T>
void Mvector<T>::clear(){
    length = 0;
}

template <typename T>
void Mvector<T>::erase(size_t idx){
    if(idx >= length)
        return;
    for(size_t i = idx; i < (length - 1); i++)
        data[i] = data[i + 1];
    length--;
}

template <typename T>
T* Mvector<T>::begin(){
    return data;
}

template <typename T>
T* Mvector<T>::end(){
    return data + length;
}

template <typename T>
T& Mvector<T>::front(){
    return data[0];
}

template <typename T>
T& Mvector<T>::back(){
    return data[length - 1];
}

template <typename T>
void Mvector<T>::reserve(size_t inCapacity){
    if(inCapacity > capacity)
        resize(inCapacity);
}

#endif
