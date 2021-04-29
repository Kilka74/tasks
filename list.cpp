#include <iostream>
#include <algorithm>

struct Node {
    Node* previous;
    Node* next;

    virtual ~Node() = default;
};

template<class T>
struct ListNode: public Node {
    ListNode() = default;

    ListNode(const T& v): v(std::move(v)) {}

    T v;
};

template<class T>
class List {
private:
    Node* base;
    size_t length;

    void Disconnnect(Node* node) {
        auto left = node->previous;
        auto right = node->next;
        left->next = right;
        right->previous = left;
        delete node;
    }

    void Сonnect(Node* node, Node* place) {
        auto right = place->next;
        right->previous = node;
        node->next = right;
        node->previous = place;
        place->next = node;
    }


public:
    List(): base(new Node()), length(0) {
        base->previous = base;
        base->next = base;
    }

    List(const List& other): length(other.length) {
        base = new Node();
        base->previous = other.base->previous;
        base->next = other.base->next;
        auto right_ptr = other.base;
        auto left_ptr = base;
        while (right_ptr->next != other.base) {
            auto val = (static_cast<ListNode<T>*>(right_ptr->next))->v;
            left_ptr->next = new ListNode(std::move(val));
            right_ptr = right_ptr->next;
            left_ptr = left_ptr->next;
        }
        base->previous = left_ptr;
        left_ptr->next = base;
    }

    List operator=(const List& other) {
        if (this == &other) {
            return *this;
        }
        List l(other);
        Swap(l);
        return *this;
    }

    List operator=(List&& other) {
        List l(std::move(other));
        Swap(l);
        return *this;
    }

    void push_back(const T& v) {
        auto node = new ListNode(v);
        Сonnect(node, base->previous);
        ++length;
    }

    void push_back(T&& v) {
        auto node = new ListNode(std::move(v));
        Сonnect(node, base->previous);
        ++length;
    }

    void push_front(const T& v) {
        auto node = new ListNode(v);
        Сonnect(node, base);
        ++length;
    }

    void pop_front() {
        Disconnnect(base->next);
        --length;
    }


    void push_front(T&& v) {
        auto node = new ListNode(std::move(v));
        Сonnect(node, base);
        ++length;
    }

    void pop_back() {
        Disconnnect(base->previous);
        --length;
    }

    size_t size() const {
        return length;
    }

    void Swap(List& other) {
        std::swap(base, other.base);
        std::swap(length, other.length);
    }

    class ListIterator {
    private:
        Node* ptr;

    public:
        ListIterator(Node* p): ptr(p) {}

        ListIterator(const ListIterator& other): ptr(other.ptr) {}

        ListIterator(ListIterator&& other): ptr(std::move(other.ptr)) {}

        ListIterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        ListIterator& operator++(int) {
            auto temp = new ListIterator(*this);
            ++(*this);
            return *temp;
        }

        ListIterator& operator--(int) {
            auto temp = new ListIterator(*this);
            ++(*this);
            return *temp;
        }

        ListIterator& operator--() {
            ptr = ptr->previous;
            return *this;
        }

        T& operator*() const {
            return static_cast<ListNode<T>*>(ptr)->v;
        }

        T* operator->() const {
            return &(static_cast<ListNode<T>*>(ptr)->v);
        }

        bool operator==(const ListIterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const ListIterator& other) const {
            return !(ptr == other.ptr);
        }
    };
    ListIterator end() {
        return ListIterator(base);
    }

    ListIterator begin() {
        return ListIterator(base->next);
    }

    ~List() {
        auto curr = base->next;
        while (curr != base) {
            auto right = curr->next;
            delete curr;
            curr = right;
        }
        length = 0;
        delete base;
    }
};
