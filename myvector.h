// myvector.h
#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <queue>
#include <functional>
#include <limits>
#include <vector>
#include <algorithm>

template <typename T>
class MyVector {
public:
    MyVector();
    MyVector(size_t size);
    MyVector(size_t size, const T& value);
    MyVector(std::initializer_list<T> init);
    MyVector(const MyVector& other);
    MyVector(MyVector&& other) noexcept;
    ~MyVector();

    MyVector& operator=(const MyVector& other);
    MyVector& operator=(MyVector&& other) noexcept;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();

    size_t size() const;
    size_t capacity() const;
    bool empty() const;

    void resize(size_t new_size);
    void reserve(size_t new_cap);

    T* begin();
    const T* begin() const;
    T* end();
    const T* end() const;

private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void reallocate(size_t new_cap);
};

template <typename T>
MyVector<T>::MyVector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
MyVector<T>::MyVector(size_t size) : size_(size), capacity_(size) {
    data_ = new T[size];
}

template <typename T>
MyVector<T>::MyVector(size_t size, const T& value) : size_(size), capacity_(size) {
    data_ = new T[size];
    for (size_t i = 0; i < size; ++i) {
        data_[i] = value;
    }
}

template <typename T>
MyVector<T>::MyVector(std::initializer_list<T> init) : size_(init.size()), capacity_(init.size()) {
    data_ = new T[capacity_];
    size_t i = 0;
    for (const auto& item : init) {
        data_[i++] = item;
    }
}

template <typename T>
MyVector<T>::MyVector(const MyVector& other) : size_(other.size_), capacity_(other.capacity_) {
    data_ = new T[capacity_];
    for (size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

template <typename T>
MyVector<T>::MyVector(MyVector&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template <typename T>
MyVector<T>::~MyVector() {
    delete[] data_;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    if (this != &other) {
        delete[] data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template <typename T>
T& MyVector<T>::operator[](size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

template <typename T>
const T& MyVector<T>::operator[](size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

template <typename T>
void MyVector<T>::push_back(const T& value) {
    if (size_ >= capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_++] = value;
}

template <typename T>
void MyVector<T>::push_back(T&& value) {
    if (size_ >= capacity_) {
        reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_++] = std::move(value);
}

template <typename T>
void MyVector<T>::pop_back() {
    if (size_ > 0) {
        --size_;
    }
}

template <typename T>
size_t MyVector<T>::size() const {
    return size_;
}

template <typename T>
size_t MyVector<T>::capacity() const {
    return capacity_;
}

template <typename T>
bool MyVector<T>::empty() const {
    return size_ == 0;
}

template <typename T>
void MyVector<T>::resize(size_t new_size) {
    if (new_size > capacity_) {
        reallocate(new_size);
    }
    size_ = new_size;
}

template <typename T>
void MyVector<T>::reserve(size_t new_cap) {
    if (new_cap > capacity_) {
        reallocate(new_cap);
    }
}

template <typename T>
T* MyVector<T>::begin() {
    return data_;
}

template <typename T>
const T* MyVector<T>::begin() const {
    return data_;
}

template <typename T>
T* MyVector<T>::end() {
    return data_ + size_;
}

template <typename T>
const T* MyVector<T>::end() const {
    return data_ + size_;
}

template <typename T>
void MyVector<T>::reallocate(size_t new_cap) {
    T* new_data = new T[new_cap];
    for (size_t i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = new_data;
    capacity_ = new_cap;
}

#endif // MYVECTOR_H