//
// Created by janusz on 05.02.2022.
//

#pragma once

#include <iostream>
#include <optional>

template<typename T>
class circular_buffer {
public:
    circular_buffer(int size)
            : size_(size) {
        buffer_ = new T[size]{0};
        head_ = -1;
        tail_ = -1;
    }

    ~circular_buffer() {
        delete[] buffer_;
    }

    int get_size() {
        return size_;
    }

    int get_head() {
        return head_;
    }

    int get_tail() {
        return tail_;
    }

    void add_value(T value) {

        if (is_full()) {
            std::cout << "Buffer is full! \n";
        } else {
            tail_ = (tail_ + 1) % size_;
            buffer_[tail_] = value;

            if (head_ == -1) {
                head_ = tail_;
            }
        }
    }

    std::optional<T> delete_value() {

        std::optional<T> result = std::nullopt;
        if (is_empty()) {
            std::cout << "Buffer is empty \n";
        } else {
            result = buffer_[head_];
            head_ = (head_ + 1) % size_;
        }

        return result;
    }

    void print_buffer(){
        for (int i = 0; i < size_; ++i) {
            std::cout << buffer_[i] << " ";
        }
        std::cout << "\n";
    }

private:

    bool is_full() {
        return (head_ == (tail_ % size_) + 1)
               || (head_ == 0 && tail_ == size_ - 1);
    }


    bool is_empty() {
        return head_ == tail_;
    }

private:
    T *buffer_;
    int head_;
    int tail_;
    int size_;
};