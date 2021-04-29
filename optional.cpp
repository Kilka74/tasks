#define USE_STD_OPTIONAL    // Раскомментируйте эту строку, чтобы запустить тесты со стандартным std::optional

#ifdef USE_STD_OPTIONAL
    #include <optional>
    template <typename T>
    using Optional = std::optional<T>;
    using BadOptionalAccess = std::bad_optional_access;
#else
#include <utility>
struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;

    Optional(const T& elem) {
        new (data) T(elem);
        defined = true;
    }

    Optional(T&& elem) {
        new (data) T(std::move(elem));
        defined = true;
    }

    Optional(const Optional& other) {
        if (other.defined) {
            value().~T();
            new (data) T(other.value());
            defined = true;
               
        }
    }

    Optional& operator=(const Optional& other) {
        if (other.defined) {
            if (defined) {
                value() = other.value();
            } else {

                new (data) T(other.value());
                defined = true;
            }
        } else {
            reset();
        }
        return *this;
    }

    Optional& operator=(const T& elem) {
        if (defined) {
            value() = elem;
        } else {
            new (data) T(elem);
            defined = true;
        }
    return *this;
    }

    Optional& operator=(T&& elem) {
        if (defined) {
            value() = std::move(elem);
        } else {
            new (data) T(std::move(elem));
            defined = true;
        }
        return *this;
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        return value();
    }

    const T& operator*() const {
        return value();
    }

    T* operator->() {
        return &value();
    }
    const T* operator->() const {
        return &value();
    }

    T& value() {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return *reinterpret_cast<T*>(data);
    }

    const T& value() const {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return *reinterpret_cast<const T*>(data);
    }

    void reset() {
        if (defined) {
            value().~T();
            defined = false;
        }
    }

    ~Optional() {
        reset();
    }
};
#endif

#include <iostream>
#include <string>

using namespace std;

class C {
private:
    int x;
    double y;
    string s;
public:
    C(): x(42), y(3.14), s("Hello, world!") {
        cout << "C()\n";
    }
    C(int x, double y = 2.71828, const string& s = "Hi"): x(x), y(y), s(s) {
        cout << "C(int, double, s)\n";
    }
    C(const C& other): x(other.x), y(other.y), s(other.s) {
        cout << "C(const C&)\n";
    }
    C(C&& other): x(other.x), y(other.y), s(move(other.s)) {
        cout << "C(C&&)\n";
    }
    C& operator=(const C& other) {
        x = other.x;
        y = other.y;
        s = other.s;
        cout << "operator=(const C&)\n";
        return *this;
    }
    C& operator=(C&& other) {
        x = other.x;
        y = other.y;
        s = move(other.s);
        cout << "operator=(C&&)\n";
        return *this;
    }
    ~C() {
        cout << "~C()\n";
    }
    void Print() const {
        cout << x << " " << y << " " << s << "\n";
    }
};

template <typename T>
void Print(const T& arg) {
    cout << arg << "\n";
}

void Print(const C& arg) {
    arg.Print();
}

template <typename T>
void TestInit() {
    cout << "\nTestInit\n";
    Optional<T> o1;
    Optional<T> o2 = T(100500);
    cout << "\nDestroy\n";
};

template <typename T>
void TestAssign() {
    cout << "\nTestAssign\n";
    Optional<T> o1, o2, o3;

    cout << "\nAssign empty to empty\n";
    o1 = o2;

    cout << "\nAssign a value to empty\n";
    o1 = T(100500);

    cout << "\nAssign non-empty to empty\n";
    o2 = o1;

    cout << "\nAssign a value to non-empty\n";
    o2 = T(2128506);

    cout << "\nAssign non-empty to non-empty\n";
    o2 = o1;

    cout << "\nAssign empty to non-empty\n";
    o1 = o3;

    cout << "\nDestroy\n";
}

template <typename T>
void TestReset() {
    cout << "\nTestReset\n";
    Optional<T> o1, o2 = T(100500);

    cout << "\nReset empty\n";
    o1.reset();

    cout << "\nReset non-empty\n";
    o2.reset();

    cout << "\nDestroy\n";
}

template <typename T>
void TestHasValue() {
    cout << "\nTestHasValue\n";
    Optional<T> o1;
    cout << "empty: " << o1.has_value() << "\n";
    const Optional<T> o2(T(123));
    cout << "initialized: " << o2.has_value() << "\n";
    o1 = T(-1);
    cout << "assigned 1: " << o1.has_value() << "\n";
    o1 = o2;
    cout << "assigned 2: " << o1.has_value() << "\n";
    o1.reset();
    cout << "reseted: " << o1.has_value() << "\n";
}

template <typename T>
void TestValue() {
    cout << "\nTestValue\n";
    const T val(31415);
    Optional<T> o1(val);
    Print(*o1);
    Print(o1.value());
    const auto& o2 = o1;
    Print(*o2);
    Print(o2.value());
    Optional<T> o3;
    try {
        o3.value();
        cout << "Got value of empty optional successfully\n";
    } catch (const BadOptionalAccess&) {
        cout << "BadOptionalAccess\n";
    } catch (...) {
        cout << "Other exception\n";
    }
}

void TestArrow() {
    cout << "\nTestArrow\n";
    const C val(451);
    Optional<C> o1(val);
    o1->Print();
    const auto& o2 = o1;
    o2->Print();
}

void TestSizeof() {
    cout << "\nTestSizeof\n";
    cout << ((sizeof(C*) + 4) < sizeof(Optional<C>) && sizeof(Optional<C>) <= 2 * sizeof(C)) << "\n";;
}

template <typename T>
void Test(const string& test) {
    if (test == "init")
        TestInit<T>();
    else if (test == "assign")
        TestAssign<T>();
    else if (test == "reset")
        TestReset<T>();
    else if (test == "has_value")
        TestHasValue<T>();
    else if (test == "value")
        TestValue<T>();
    else
        throw "Unknown test";
}

void TestC(const string& test) {
    if (test == "arrow")
        TestArrow();
    else if (test == "sizeof")
        TestSizeof();
    else
        Test<C>(test);
}

int main() {
    string type, test;
    cin >> type >> test;

    if (type == "int") {
        cout << "Testing Optional<int>...\n";
        Test<int>(test);
    } else if (type == "C") {
        cout << "Testing Optional<C>...\n";
        TestC(test);
    } else {
        throw "Unknown type";
    }
}
