#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>
#include <memory> // For std::allocator
#include <iterator> // For std::reverse_iterator

template <typename T>
class Vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void resize(size_t new_capacity);

public:
    Vector() : data_(nullptr), size_(0), capacity_(0) {}
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector(std::initializer_list<T> init_list);
    ~Vector() { delete[] data_; }

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    Vector& operator=(std::initializer_list<T> init_list);
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    void swap(Vector& other) noexcept;
    void push_back(const T& value);
    void pop_back();
    size_t getSize() const;
    bool empty() const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    void assign(size_t count, const T& value);
    template <typename InputIt>
    void assign(InputIt first, InputIt last);

    T& at(size_t index);
    const T& at(size_t index) const;
    T* data() noexcept;
    const T* data() const noexcept;
    std::allocator<T> get_allocator() const;

    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cbegin() const noexcept { return data_; }
    const_iterator cend() const noexcept { return data_ + size_; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }
};

template <typename T>
Vector<T>::Vector(const Vector& other) : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ > 0) {
        data_ = new T[capacity_];
        std::copy(other.data_, other.data_ + size_, data_);
    }
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> init_list) : data_(new T[init_list.size()]), size_(init_list.size()), capacity_(init_list.size()) {
    std::copy(init_list.begin(), init_list.end(), data_);
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        Vector temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
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
Vector<T>& Vector<T>::operator=(std::initializer_list<T> init_list) {
    delete[] data_;
    size_ = init_list.size();
    capacity_ = init_list.size();
    data_ = new T[capacity_];
    std::copy(init_list.begin(), init_list.end(), data_);
    return *this;
}

template <typename T>
void Vector<T>::swap(Vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        resize(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_++] = value;
}

template <typename T>
void Vector<T>::resize(size_t new_capacity) {
    T* new_data = new T[new_capacity];
    std::copy(data_, data_ + size_, new_data);
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}

template <typename T>
void Vector<T>::pop_back() {
    if (size_ > 0) {
        --size_;
    } else {
        throw std::out_of_range("Vector is empty");
    }
}

template <typename T>
T& Vector<T>::operator[](size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

template <typename T>
const T& Vector<T>::operator[](size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

template <typename T>
size_t Vector<T>::getSize() const {
    return size_;
}

template <typename T>
bool Vector<T>::empty() const {
    return size_ == 0;
}

template <typename T>
T& Vector<T>::front() {
    if (size_ == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return data_[0];
}

template <typename T>
const T& Vector<T>::front() const {
    if (size_ == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return data_[0];
}

template <typename T>
T& Vector<T>::back() {
    if (size_ == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return data_[size_ - 1];
}

template <typename T>
const T& Vector<T>::back() const {
    if (size_ == 0) {
        throw std::out_of_range("Vector is empty");
    }
    return data_[size_ - 1];
}

template <typename T>
void Vector<T>::assign(size_t count, const T& value) {
    delete[] data_;
    size_ = count;
    capacity_ = count;
    data_ = new T[capacity_];
    std::fill(data_, data_ + size_, value);
}

template <typename T>
template <typename InputIt>
void Vector<T>::assign(InputIt first, InputIt last) {
    delete[] data_;
    size_ = std::distance(first, last);
    capacity_ = size_;
    data_ = new T[capacity_];
    std::copy(first, last, data_);
}

template <typename T>
T& Vector<T>::at(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

template <typename T>
const T& Vector<T>::at(size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

template <typename T>
T* Vector<T>::data() noexcept {
    return data_;
}

template <typename T>
const T* Vector<T>::data() const noexcept {
    return data_;
}

template <typename T>
std::allocator<T> Vector<T>::get_allocator() const {
    return std::allocator<T>();
}

#endif // VECTOR_H
