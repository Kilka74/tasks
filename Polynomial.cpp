#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

template <typename T>
class Polynomial {
    private:
        vector<T> coeffs;

    public:
        Polynomial(vector<T> v) {
            if (v.size() == 0) {
                coeffs = {};
            } else {
                size_t i = v.size() - 1;
                while (v[i] == T(0)) {
                    v.pop_back();
                    if (i == 0) {
                        break;
                    }
                    i -= 1;
                }
                coeffs = v;
            }
        }


        Polynomial(T value = T()) {
            coeffs = {value};
        }

        template <typename Iter>
        Polynomial(Iter first, Iter last) {
            vector<T> v;
            for (auto it = first; it != last; it++) {
                v.push_back(*it);
            }
            if (v.size() == 0) {
                coeffs = {};
            } else {
                size_t i = v.size() - 1;
                while (v[i] == T(0)) {
                    v.pop_back();
                    if (i == 0) {
                        break;
                    }
                    i -= 1;
                }
                coeffs = v;
            }
        }

        bool operator ==(const Polynomial& q) const {
            return coeffs == q.coeffs;
        }

        bool operator !=(const Polynomial& q) const {
            return coeffs != q.coeffs;
        }

        int Degree() const {
            if (coeffs.size() > 1) {
                return static_cast<int>(coeffs.size()) - 1;
            }
            if (coeffs.empty()) {
                return -1;
            }
            return 0;
        }

        T operator[](int i) const {
            if ((*this).Degree() == -1) {
                return T(0);
            }
            if (i > (*this).Degree()) {
                return T(0);
            }
            return coeffs[i];
        }

        Polynomial operator +(const Polynomial& q) const {
            vector<T> result;
            if (coeffs.size() > q.coeffs.size()) {
                for (size_t i = 0; i < q.coeffs.size(); i++) {
                    result.push_back(coeffs[i] + q[i]);
                }
                for (size_t i = q.coeffs.size(); i != coeffs.size(); i++) {
                    result.push_back(coeffs[i]);
                }
            } else {
                for (size_t i = 0; i < coeffs.size(); i++) {
                    result.push_back(coeffs[i] + q[i]);
                }
                for (size_t i = coeffs.size(); i != q.coeffs.size(); i++) {
                    result.push_back(q[i]);
                }
            }
            return Polynomial(result);
        }

        Polynomial operator +(const T& x) const {
            return *this + Polynomial(x);
        }

        Polynomial& operator +=(const Polynomial& q) {
            Polynomial tmp = *this + q;
            coeffs = tmp.coeffs;
            return *this;
        }

        Polynomial& operator +=(const T& x) {
            *this += Polynomial(x);
            return *this;
        }

        Polynomial operator -(const Polynomial& q) const {
            vector<T> result;
            if (coeffs.size() > q.coeffs.size()) {
                for (size_t i = 0; i < q.coeffs.size(); i++) {
                    result.push_back(coeffs[i] - q[i]);
                }
                for (size_t i = q.coeffs.size(); i != coeffs.size(); i++) {
                    result.push_back(coeffs[i]);
                }
            } else {
                for (size_t i = 0; i < coeffs.size(); i++) {
                    result.push_back(coeffs[i] - q[i]);
                }
                for (size_t i = coeffs.size(); i != q.coeffs.size(); i++) {
                    result.push_back(-q[i]);
                }
            }
            return Polynomial(result);
        }

        Polynomial operator -(const T& x) const {
            *this - Polynomial(x);
        }

        Polynomial& operator -=(const Polynomial& q) {
            Polynomial tmp = *this - q;
            coeffs = tmp.coeffs;
            return *this;
        }

        Polynomial& operator -=(const T& x) {
            *this -= Polynomial(x);
            return *this;
        }

        Polynomial operator *(const Polynomial& q) const {
            if (q.coeffs.size() + coeffs.size() == 0) {
                return Polynomial(T(0));
            }
            vector<T> result(q.coeffs.size() + coeffs.size() - 1);
            for (size_t i = 0; i != q.coeffs.size() + coeffs.size() - 1; i++) {
                for (size_t j = 0; j <= i; j++) {
                    result[i] += (*this)[j] * q[i - j];
                }
            }
            return Polynomial(result);
        }

        Polynomial& operator *= (const Polynomial& q) {
            auto tmp = *this * q;
            *this = tmp;
            return *this;
        }

        typename vector<T>::const_iterator begin() const {
            return coeffs.begin();
        }

        typename vector<T>::const_iterator end() const {
            return coeffs.end();
        }

        T operator()(const T& x) const {
                T result = T(0);
            for (size_t i = 0; i < coeffs.size(); i++) {
                result = result * x + coeffs[coeffs.size() - i - 1];
            }
            return result;
        }
};

template <typename T>
bool operator ==(const T& x, const Polynomial<T>& p) {
    if (x == T(0) && p.Degree() == -1) {
        return true;
    }
    return Polynomial(x) == p;
}

template <typename T>
bool operator !=(const T& x, const Polynomial<T>& p) {
    if (x == T(0) && p.Degree() == -1) {
        return false;
    }
    return Polynomial(x) != p;
}

template <typename T>
bool operator ==(const Polynomial<T>& p, const T& x) {
    if (x == T(0) && p.Degree() == -1) {
        return true;
    }
    return Polynomial(x) == p;
}

template <typename T>
bool operator !=(const Polynomial<T>& p, const T& x) {
    if (x == T(0) && p.Degree() == -1) {
        return false;
    }
    return Polynomial(x) != p;
}


template <typename T>
Polynomial<T>& operator +=(Polynomial<T>& p, const T& x) {
    auto tmp = p + Polynomial(x);
    p = tmp;
    return p;
}

template <typename T>
Polynomial<T>& operator -=(Polynomial<T>& p, const T& x) {
    auto tmp = p - Polynomial(x);
    p = tmp;
    return p;
}

template <typename T>
Polynomial<T> operator *(const T& c, const Polynomial<T>& p) {
    return p * Polynomial(c);
}

template <typename T>
Polynomial<T>& operator *=(Polynomial<T>& p, const T& x) {
    auto tmp = p * Polynomial(x);
    p = tmp;
    return p;
}

template <typename T>
Polynomial<T> operator &(const Polynomial<T>& p, const Polynomial<T>& q) {
    if (p.Degree() <= 0) {
        return p;
    }
    auto tmp = q;
    Polynomial<T> result = Polynomial<T>(p(T(0)));
    for (int i = 1; i <= p.Degree(); i++) {
        result += p[i] * tmp;
        tmp *= q;
    }
    return result;
}

template <typename T>
Polynomial<T> operator &(const T& x, [[maybe_unused]] const Polynomial<T>& p) {
    return Polynomial(x);
}

template <typename T>
Polynomial<T> operator &(const Polynomial<T>& p, const T& x) {
    return Polynomial(p(x));
}

template <typename T>
bool operator > (const Polynomial<T>& p, const Polynomial<T>& q) {
    if (p.Degree() >= q.Degree()) {
        return true;
    }
    return false;
}

template <typename T>
Polynomial<T> operator /(const Polynomial<T>& p, const Polynomial<T>& q) {
    assert(q.Degree() != -1);
    auto tmp = p;
    if (p.Degree() < q.Degree()) {
        return Polynomial(T(0));
    }
    vector<T> curr_dif_powers(p.Degree() + 2);
    Polynomial<T> fraction = Polynomial(T(0));
    while (tmp > q) {
        int curr_diff = tmp.Degree() - q.Degree();
        curr_dif_powers[curr_diff] = (tmp[tmp.Degree()] / q[q.Degree()]);
        fraction += Polynomial(curr_dif_powers);
        tmp -= Polynomial(curr_dif_powers) * q;
        curr_dif_powers[curr_diff] = T(0);
    }
    return fraction;
}

template <typename T>
Polynomial<T> operator /(const T& x, const Polynomial<T>& p) {
    if (p.Degree() > 0) {
        return Polynomial(T(0));
    }
    return Polynomial(x / p[0]);
}

template <typename T>
Polynomial<T> operator /(const Polynomial<T>& p, const T& x) {
    return p / Polynomial<T>(x);
}

template<typename T>
Polynomial<T> operator %(const Polynomial<T>& p, const Polynomial<T>& q) {
    if (p == T(0)) {
        return Polynomial(T(0));
    }
    return p - ((p / q) * q);
}

template <typename T>
Polynomial<T> operator %(const T& x,  const Polynomial<T>& p) {
    return (Polynomial(x) % p);
}

template <typename T>
Polynomial<T> operator %(const Polynomial<T>& p, const T& x) {
    return (p % Polynomial(x));
}

template <typename T>
Polynomial<T> operator ,(const Polynomial<T>& p, const Polynomial<T>& q) {
    if (p == T(0)) {
        if (q == T(0)) {
            return Polynomial(T(0));
        }
        return q / q[q.Degree()];
    }
    if (q == T(0)) {
        return p / p[p.Degree()];
    }
    if (p.Degree() == 0 || q.Degree() == 0) {
        return Polynomial(T(1));
    }
    if (p > q) {
        return (q, (p % q));
    }
    return (p, (q % p));
}

template <typename T>
Polynomial<T> operator ,(const T& x, const Polynomial<T>& p) {
    return (p, Polynomial(x));
}

template <typename T>
Polynomial<T> operator ,(const Polynomial<T>& p, const T& x) {
    return (p, Polynomial(x));
}

template <typename T>
Polynomial<T> operator ,(const T& p, const T& x) {
    return (Polynomial(p), Polynomial(x));
}


template <typename T>
ostream& operator <<(ostream& out, const Polynomial<T>& p) {
    if (p.Degree() < 1) {
        if (p.Degree() == -1) {
            out << T(0);
            return out;
        }
        out << p[0];
        return out;
    }
    for (int i = 0; i != p.Degree(); i++) {
        if (i == 0) {
            if (p[p.Degree()] == T(1)) {
                out << "x";
            } else if (p[p.Degree()] == T(-1)) {
                out << "-x";
            } else {
                out << p[p.Degree()] << '*' << "x";
            }
            if (p.Degree()!= 1) {
                out << '^' << p.Degree();
            }
        } else {
            if (p[p.Degree() - i] > T(0)) {
                out << '+';
                if (p[p.Degree() - i] != T(1)) {
                    out << p[p.Degree() - i] << '*' << "x";
                    if (p.Degree() - i != 1) {
                        out << '^' << p.Degree() - i;
                    }
                } else {
                    out << "x";
                    if (p.Degree() - i != 1) {
                        out << '^' << p.Degree() - i;
                    }
                }
            } else if (p[p.Degree() - i] < T(0)) {
                if (p[p.Degree() - i] == T(-1)) {
                    out << '-' << "x";
                    if (p.Degree() - i != 1) {
                        out << '^' << p.Degree() - i;
                    }
                } else {
                    out << p[p.Degree() - i] << "*x";
                    if (p.Degree() - i != 1) {
                        out << '^' << p.Degree() - i;
                    }
                }
            }
        }
    }
    if (p[0] > T(0)) {
        out << '+' << p[0];
    } else if (p[0] < T(0)) {
        out << p[0];
    }
    return out;
}