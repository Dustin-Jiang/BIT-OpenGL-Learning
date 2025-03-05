#pragma once
#include "stdafx.h"
#include <array>
#include <cmath>
#include <stdexcept>
#include <cstddef>
#include <iostream>

//#include "Matrix.h"

template<typename T = float, std::size_t N = 2>
class Vector {
public:
    std::array<T, N> data;

    Vector() {
        data.fill(T(0));
    }

    template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == N>>
    Vector(Args... args) : data{ static_cast<T>(args)... } {}

    Vector(const Vector<T, N>&) = default;
    Vector<T, N>& operator=(const Vector<T, N>&) = default;

    Vector<T, N> operator+(const Vector<T, N>& other) const {
        Vector<T, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    Vector<T, N> operator-(const Vector<T, N>& other) const {
        Vector<T, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }

	Vector<T, N> operator-(void) const {
		Vector<T, N> result;
		for (std::size_t i = 0; i < N; i++) {
			result.data[i] = -data[i];
		}
		return result;
	}

    Vector<T, N> operator*(T scalar) const {
        Vector<T, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] * scalar;
        }
        return result;
    }

    Vector<T, N> operator/(T scalar) const {
        if (scalar == T(0))
            throw std::runtime_error("Division by zero");
        Vector<T, N> result;
        for (std::size_t i = 0; i < N; i++) {
            result.data[i] = data[i] / scalar;
        }
        return result;
    }

    Vector<T, N>& operator+=(const Vector<T, N>& other) {
        for (std::size_t i = 0; i < N; i++) {
            data[i] += other.data[i];
        }
        return *this;
    }

    Vector<T, N>& operator-=(const Vector<T, N>& other) {
        for (std::size_t i = 0; i < N; i++) {
            data[i] -= other.data[i];
        }
        return *this;
    }

    Vector<T, N>& operator*=(T scalar) {
        for (std::size_t i = 0; i < N; i++) {
            data[i] *= scalar;
        }
        return *this;
    }

    Vector<T, N>& operator/=(T scalar) {
        if (scalar == T(0))
            throw std::runtime_error("Division by zero");
        for (std::size_t i = 0; i < N; i++) {
            data[i] /= scalar;
        }
        return *this;
    }

    T Dot(const Vector<T, N>& other) const {
        T result = T(0);
        for (std::size_t i = 0; i < N; i++) {
            result += data[i] * other.data[i];
        }
        return result;
    }

    T Length() const {
        return std::sqrt(Dot(*this));
    }

    Vector<T, N> Normalized() const {
        T len = Length();
        if (len == T(0))
            throw std::runtime_error("Zero-length vector cannot be normalized");
        return (*this) / len;
    }

    // 提供转换为原始数组指针的操作
    operator T* () {
        return data.data();
    }
    operator const T* () const {
        return data.data();
    }

    // 输出支持
	friend std::ostream& operator<<(std::ostream& os, const Vector<T, N>& v) {
		os << "(";
		for (std::size_t i = 0; i < N; i++) {
			os << v.data[i];
			if (i < N - 1)
				os << ", ";
		}
		os << ")";
		return os;
	}
};

template<typename T = float>
class Vector2 : public Vector<T, 2> {
public:
    using Vector<T, 2>::data;

    Vector2() : Vector<T, 2>() {}

    Vector2(T x, T y) : Vector<T, 2>(x, y) {}

    T& x() { return data[0]; };
    T& y() { return data[1]; };
    const T& x() const { return data[0]; }
    const T& y() const { return data[1]; }
};

template<typename T = float>
class Vector3 : public Vector<T, 3> {
public:
    using Vector<T, 3>::data;

    Vector3() : Vector<T, 3>() {}

    Vector3(T x, T y, T z) : Vector<T, 3>(x, y, z) {}
    Vector3(const Vector<T, 3>& v) : Vector<T, 3>(v) {}

    Vector3<T> operator=(const Vector<T, 3> &v)
    {
        this->data[0] = v.data[0];
        this->data[1] = v.data[1];
        this->data[2] = v.data[2];
        return *this;
    };

    // 访问器
    T& x() { return data[0]; }
    T& y() { return data[1]; }
    T& z() { return data[2]; }
    const T& x() const { return data[0]; }
    const T& y() const { return data[1]; }
    const T& z() const { return data[2]; }

    Vector3<T> Cross(const Vector3<T>& other) const {
        return Vector3<T>(
            this->data[1] * other.data[2] - this->data[2] * other.data[1],
            this->data[2] * other.data[0] - this->data[0] * other.data[2],
            this->data[0] * other.data[1] - this->data[1] * other.data[0]
        );
    }
};

using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
