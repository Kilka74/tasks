#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>

using namespace std;

template <typename T>
class Matrix {
    private:
        vector<T> matrix;
        size_t rows;
        size_t columns;

    public:
        typename vector<T>::iterator it;
        Matrix(const vector<vector<T>>& v) {
            rows = v.size();
            columns = v[0].size();
            for (size_t i = 0; i < v.size(); i++) {
                for (size_t j = 0; j < v[0].size(); j++) {
                    matrix.push_back(v[i][j]);
                }
            }
        }

        Matrix(const vector<T>& v, size_t r, size_t col) : matrix(v), rows(r), columns(col) {
        }

        std::pair<size_t, size_t> size() const {
            return make_pair(rows, columns);
        }

        vector<T> operator[](size_t i) const {
            vector<T> result;
            for (size_t j = 0; j < size().second; j++) {
                result.push_back(matrix[i * size().second + j]);
            }
            return result;
        }

        Matrix& operator +=(const Matrix& b) {
            for (size_t i = 0; i < matrix.size(); i++) {
                matrix[i] += b.matrix[i];
            }
            return *this;
        }


        Matrix operator +(const Matrix& c) const {
            Matrix result = *this;
            result += c;
            return result;
        }
        template <typename N>
        Matrix operator *(const N& number) const {
            Matrix result = *this;
            for (size_t i = 0; i < result.matrix.size(); i++) {
                result.matrix[i] *= number;
            }
            return result;
        }
        template <typename N>
        Matrix& operator *=(const N& number) {
            for (size_t i = 0; i < matrix.size(); i++) {
                matrix[i] *= number;
            }
            return *this;
        }
        Matrix transposed() const {
            vector<T> result(matrix.size());
            for (size_t i = 0; i < matrix.size(); i++) {
                size_t curr_row = i / size().first;
                size_t curr_col = i % size().first;
                result[i] = matrix[curr_col * size().second + curr_row];
            }
            return Matrix(result, size().first, size().second);
        }

        Matrix& transpose() {
            Matrix tmp = (*this).transposed();
            *this = tmp;
            return *this;
        }

        Matrix operator *(const Matrix& other) const {
            assert(size().second == other.size().first);
            vector<T> result(size().first * other.size().second);
            for (size_t i = 0; i < result.size(); i++) {
                    size_t row_of_the_first = i / other.size().second;
                    size_t row_of_the_second = i % other.size().second;
                for (size_t j = 0; j < size().second; j++) {
                    result[i] += (*this)[row_of_the_first][j] * other[j][row_of_the_second];
                }
            }
            return Matrix<T>(result, other.size().second, size().first);
        }

        Matrix& operator *=(const Matrix other) {
            assert(size().second == other.size().first);
            Matrix result = *this * other;
            *this = result;
            return *this;
        }

        typename vector<T>::const_iterator begin() const {
            return matrix.begin();
        }

        typename vector<T>::iterator begin() {
            return matrix.begin();
        }

        typename vector<T>::const_iterator end() const {
            return matrix.end();
        }

        typename vector<T>::iterator end() {
            return matrix.end();
        }

        T operator *(typename vector<T>::iterator it) const {
            return *it;
        }

        typename vector<T>::iterator& operator ++() {
            it = next(it);
            return it;
        }

        typename vector<T>::iterator operator ++(int) {
            it = next(it);
            return prev(it);
        }
};



template <typename T>
bool operator == (typename vector<T>::iterator one, typename vector<T>::iterator other) {
    return one == other;
}

template <typename T>
bool operator != (typename vector<T>::iterator one, typename vector<T>::iterator other) {
    return one != other;
}


template <typename T>
std::ostream& operator <<(std::ostream& out, const Matrix<T>& m) {
    for (size_t i = 0; i < m.size().first * m.size().second; i++) {
        out << m[i / m.size().second][i % m.size().first];
        if ((i+1) % (m.size().second) != 0) {
            out << '\t';
        } else {
            if (i != m.size().first * m.size().second - 1) {
            out << '\n';
            }
        }
    }
    return out;
}
