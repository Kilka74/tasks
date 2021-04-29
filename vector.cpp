#include <algorithm>
#include <cstddef>

class Vector {
private:
    int* Data_;
    size_t Size_;
    size_t Capacity_;

    void Copy(const int * from, int * to, size_t sz) noexcept {
        for (size_t i = 0; i < sz; ++i) {
            to[i] = from[i];
        }
    }

public:
    Vector() {
        Data_ = new int[1];
        Size_ = 0;
        Capacity_ = 1;
    }

    Vector(const Vector& other): Size_(other.Size_), Capacity_(other.Capacity_) {
        Data_ = new int[Capacity_];
        Copy(other.Data_, Data_, Size_);
    }

    Vector(Vector&& other) : Size_(other.Size_), Capacity_(other.Capacity_) {
        std::swap(Data_, other.Data_);
    }

    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }
        delete[] Data_;
        Capacity_ = other.Capacity_;
        Size_ = other.Size_;
        Data_ = new int[Capacity_];
        Copy(other.Data_, Data_, Size_);
        return *this;
    }

    void push_back(int n) {
        if (Size_ == Capacity_) {
            Capacity_ *= 2;
            auto tmpData = new int[Capacity_];
            Copy(Data_, tmpData, Size_);
            delete[] Data_;
            Data_ = tmpData;
        }

        Data_[Size_++] = n;
    }

    void pop_back() {
        --Size_;
    }

    size_t size() const {
        return Size_;
    }

    int& operator[](size_t i) {
        return Data_[i];
    }

    const int& operator[](size_t i) const {
        return Data_[i];
    }

    ~Vector() {
        delete[] Data_;
    }
};
