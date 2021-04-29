#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

unsigned long int Gcd(unsigned long long int a, unsigned long long int b) {
    if (a == 0 || b == 0) {
        return max(a, b);
    }
    if (a == 1 || b == 1) {
        return 1;
    }
    if (a > b) {
        return Gcd(b, a % b);
    }
    return Gcd(a, b % a);
}


class Rational {
    private:
    long long int a;
    long long int b;

    public:

        Rational(long long int c = 0, long long int d = 1) : a(c), b(d) {
        }

        long long int Numerator() const {
            if (a * b < 0) {
                return -abs(a) / Gcd(abs(a), abs(b));
            }
            return abs(a) / Gcd(abs(a), abs(b));
        }

        long long int Denominator() const {
            return abs(b) / Gcd(abs(a), abs(b));
        }

        Rational operator -() {
            return Rational(-Numerator(), Denominator());
        }

        Rational operator +() {
            return Rational(Numerator(), Denominator());
        }
        

        bool operator ==(const Rational& other) const {
            return (Numerator() * other.Denominator() == other.Numerator() * Denominator());
        }

        bool operator !=(const Rational& other) const {
            return (Numerator() * other.Denominator() != Denominator() * other.Numerator());
        }
        Rational& operator ++() {
            *this = *this + 1;
            return *this;
        }

        Rational operator ++(int) {
            Rational temporary = *this;
            ++(*this);
            return temporary;
        }

        Rational& operator --() {
            *this = *this - 1;
            return *this;
        }

        Rational operator --(int) {
            Rational temporary = *this;
            --(*this);
            return temporary;
        }


        friend std::ostream& operator << (std::ostream& out, const Rational& rational) {
        if (rational.Denominator() == 1) {
            out << rational.Numerator();
            return out;
        }
        if (rational.Numerator() == 0) {
            out << 0;
            return out;
        }
        out << rational.Numerator() << " / " << rational.Denominator();
        return out;
    }
};

Rational operator /=(const Rational& q2, const Rational& other) {
    q2 = q2 / other;
    return q2;
}

Rational operator *=(const Rational& q2, const Rational& other) {
    q2 = q2 * other;
    return q2;
}

Rational operator +=(const Rational& q2, const Rational& other) {
    q2 = q2 + other;
    return q2;
}

Rational operator -=(const Rational& q2, const Rational& other) {
    q2 = q2 - other;
    return q2;
}


Rational operator +(const Rational& q2, const Rational& other) {
            long long int numer = q2.Numerator() * other.Denominator() +
                    q2.Denominator() * other.Numerator();
            long long int denom = q2.Denominator() * other.Denominator();
            return Rational(numer, denom);
        }

        

Rational operator -(const Rational& q2, const Rational& other) {
    long long int numer = q2.Numerator() * other.Denominator() -
            q2.Denominator() * other.Numerator();
    long long int denom = q2.Denominator() * other.Denominator();
    return Rational(numer, denom);
}

Rational operator *(const Rational& q2, const Rational& other) {
    int sign = 1;
    if (!(q2.Numerator() < 0 && q2.Denominator() < 0) &&
            !(q2.Numerator() > 0 && q2.Denominator() > 0)) {
        sign *= -1;
    }
    if (!(other.Numerator() < 0 && other.Denominator() < 0) &&
            !(other.Numerator() > 0 && other.Denominator() > 0)) {
        sign *= -1;
    }
    return Rational(sign * abs(q2.Numerator() * other.Numerator()),
            abs(q2.Denominator() * other.Denominator()));
}
Rational operator /(const Rational& q2, const Rational& other) {
    int sign = 1;
    if (!(q2.Numerator() < 0 && q2.Denominator() < 0) &&
            !(q2.Numerator() > 0 && q2.Denominator() > 0)) {
        sign *= -1;
    }
    if (!(other.Numerator() < 0 && other.Denominator() < 0) &&
            !(other.Numerator() > 0 && other.Denominator() > 0)) {
        sign *= -1;
    }
    return Rational(sign * abs(q2.Numerator() * other.Denominator()),
            abs(q2.Denominator() * other.Numerator()));
}


int main() {
    Rational a = Rational(3, 5);
    Rational b = Rational(4, 9);
    a += 5;
    cout << a;
}
