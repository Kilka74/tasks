#include <cstddef>
#include <algorithm>
#include <memory>
#include <iostream>
#include <tuple>

template<class T, class Deleter = std::default_delete<T>>
class UniquePtr {
    private:
        std::tuple<T*, Deleter> pair;

    public:
        UniquePtr() {
            std::get<0>(pair) = nullptr;
        }

        UniquePtr(T* ptr) {
            std::get<0>(pair) = ptr;
        }

        UniquePtr(UniquePtr&& other) noexcept {
            std::get<0>(pair) = std::get<0>(other.pair);
            std::get<0>(other.pair) = nullptr;
            std::swap(std::get<1>(pair), std::get<1>(other.pair));
        }

        UniquePtr(T* ptr, Deleter del) {
            std::get<0>(pair) = ptr;
            std::get<1>(pair) = del;
        }

        UniquePtr(UniquePtr &other) = delete;

        UniquePtr& operator =(std::nullptr_t) noexcept {
            std::get<1>(pair)(std::get<0>(pair));
            std::get<0>(pair) = nullptr;
            return *this;
        }

        UniquePtr& operator =(UniquePtr&& other) noexcept {
            swap(other);
            return *this;
        }

        UniquePtr &operator=(UniquePtr &other) = delete;

        ~UniquePtr() {
            std::get<1>(pair)(std::get<0>(pair));
        }
        const T& operator *() const noexcept {
            return *(std::get<0>(pair));
        }
        const T* operator->() const noexcept {
            return std::get<0>(pair);
        }
        const Deleter& get_deleter() const noexcept {
            return std::get<1>(pair);
        }
        Deleter& get_deleter() noexcept {
            return std::get<1>(pair);
        }
        T* release() noexcept {
            auto res = std::get<0>(pair);
            std::get<0>(pair) = nullptr;
            return res;
        }
        void reset(T* ptr) noexcept {
            if (std::get<0>(pair) != ptr) {
                std::get<1>(pair)(std::get<0>(pair));
                std::get<0>(pair) = ptr;
            }
        }
        void swap(UniquePtr& other) noexcept {
            std::swap(pair, other.pair);
        }
        T * get() const noexcept {
           return std::get<0>(pair);
        }
        explicit operator bool() const noexcept {
            return std::get<0>(pair) != nullptr;
        }
};
