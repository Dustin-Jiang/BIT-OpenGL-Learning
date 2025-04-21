#pragma once
#include "stdafx.h"

#include <cmath>
#include <array>
#include <iostream>
#include <iomanip>

#include "Vector.h"

template<typename _T, std::size_t W, std::size_t H>
class Matrix
{
	std::array<_T, W* H> data;
public:
	Matrix() : data() { data.fill(0); };

	template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == W * H>>
	Matrix(Args... args) : data{ { static_cast<_T>(args)... } } {};

	Matrix(const Matrix<_T, W, H>&) = default;
	Matrix<_T, W, H>& operator=(const Matrix<_T, W, H>&) = default;

	template<typename _T, std::size_t N>
	static Matrix<_T, 1, N> VMatrix(const Vector<_T, N>& v) {
		Matrix<_T, 1, N> result;
		for (std::size_t i = 0; i < N; i++) {
			result(0, i) = v[i];
		}
		return result;
	}

private:
	_T getValue(std::size_t i, std::size_t j) const {
		return data[i * W + j];
	}

public:
	_T& operator()(std::size_t i, std::size_t j) {
		return data[i * W + j];
	}
	const _T& operator()(std::size_t i, std::size_t j) const {
		return data[i * W + j];
	}

	std::array<_T, W* H > getGlMatrix() const {
		std::array<_T, W* H> gl_data;
		for (size_t col = 0; col < W; ++col) {
			for (size_t row = 0; row < H; ++row) {
				gl_data[col * W + row] = (*this)(row, col);
			}
		}
		return gl_data;
	}

	Matrix<_T, W, H> operator+(const Matrix<_T, W, H>& other) const {
		Matrix<_T, W, H> result;
		for (std::size_t i = 0; i < W * H; i++) {
			result.data[i] = data[i] + other.data[i];
		}
		return result;
	}

	Matrix<_T, W, H> operator-(const Matrix<_T, W, H>& other) const {
		Matrix<_T, W, H> result;
		for (std::size_t i = 0; i < W * H; i++) {
			result.data[i] = data[i] - other.data[i];
		}
		return result;
	}

	Matrix<_T, W, H> operator*(_T scalar) const {
		Matrix<_T, W, H> result;
		for (std::size_t i = 0; i < W * H; i++) {
			result.data[i] = data[i] * scalar;
		}
		return result;
	}

	Matrix<_T, W, H> operator/(_T scalar) const {
		Matrix<_T, W, H> result;
		for (std::size_t i = 0; i < W * H; i++) {
			result.data[i] = data[i] / scalar;
		}
		return result;
	}

	template<std::size_t W2>
	Matrix<_T, W2, H> operator*(const Matrix<_T, W2, W>& other) const {
		Matrix<_T, W2, H> result;
		for (std::size_t i = 0; i < H; i++) {
			for (std::size_t j = 0; j < W2; j++) {
				for (std::size_t k = 0; k < W; k++) {
					result(i, j) += data[i * W + k] * other(k, j);
				}
			}
		}
		return result;
	}

	Matrix<_T, H, W> Transpose() const {
		Matrix<_T, H, W> result;
		for (std::size_t i = 0; i < W; i++) {
			for (std::size_t j = 0; j < H; j++) {
				result(i, j) = getValue(j, i);
			}
		}
		return result;
	}

	Matrix<_T, H, W> T() const { return Transpose(); }

	Matrix<_T, W, H> Inverse() const {
		static_assert(W == H, "Matrix must be square");
		Matrix<_T, W, H> result;
		_T det = Determinant();
		if (int(det) == 0)
			throw std::runtime_error("Matrix is singular");
		for (std::size_t i = 0; i < W; i++) {
			for (std::size_t j = 0; j < H; j++) {
				result(i, j) = Cofactor(j, i) / det;
			}
		}
		return result;
	}

	_T Determinant() const {
		static_assert(W == H, "Matrix must be square to compute determinant");
		if constexpr (W == 1) {
			return data[0];
		}
		else if constexpr (W == 2) {
			return data[0] * data[3] - data[1] * data[2];
		}
		else {
			_T det = 0;
			for (std::size_t i = 0; i < W; i++) {
				det += (i % 2 == 0 ? 1 : -1) * data[i] * Minor(0, i).Determinant();
			}
			return det;
		}
	}

	template<typename T = _T, typename = std::enable_if_t<W == H>>
	Matrix<T, W, H> Orthogonalize() const {
		Matrix<T, W, H> result{};
		std::vector<Vector<T, W>> vs;

		// 处理第一个向量
		Vector<T, W> v1 {};
		for (std::size_t i = 0; i < W; ++i) {
			v1.data[i] = (*this)(i, 0);
		}
		T norm = v1.Length();
		if (norm < 1e-10) {
			throw std::runtime_error("Near-zero vector encountered in orthogonalization");
		}
		v1 = v1 / norm;
		vs.push_back(v1);
		for (std::size_t i = 0; i < W; ++i) {
			result(i, 0) = v1.data[i];
		}

		for (std::size_t j = 1; j < W; ++j) {
			Vector<T, W> vj{};
			for (std::size_t i = 0; i < W; ++i) {
				vj.data[i] = (*this)(i, j);
			}

			// 对所有之前的正交向量进行正交化
			for (const auto& vi : vs) {
				vj = vj - vi * vj.Dot(vi);
			}

			// 规范化并检查是否为零向量
			norm = vj.Length();
			if (norm < 1e-10) {
				throw std::runtime_error("Linearly dependent vectors encountered in orthogonalization");
			}
			vj = vj / norm;
			vs.push_back(vj);

			for (std::size_t i = 0; i < W; ++i) {
				result(i, j) = vj.data[i];
			}
		}

		// 验证正交性
		T det = result.Determinant();
		if (std::abs(std::abs(det) - 1.0) > 1e-6) {
			std::cout << "Warning: Orthogonalization may be numerically unstable. Determinant: " << det << std::endl;
		}

		return result;
	}

	Matrix<_T, W - 1, H - 1> Minor(std::size_t row, std::size_t col) const {
		Matrix<_T, W - 1, H - 1> result;
		for (std::size_t i = 0, r = 0; i < H; i++) {
			if (i == row) continue;
			for (std::size_t j = 0, c = 0; j < W; j++) {
				if (j == col) continue;
				result(r, c) = data[i * W + j];
				c++;
			}
			r++;
		}
		return result;
	}

	_T Cofactor(std::size_t row, std::size_t col) const {
		return ((row + col) % 2 == 0 ? 1 : -1) * Minor(row, col).Determinant();
	}

	friend std::ostream& operator<<(std::ostream& os, const Matrix<_T, W, H>& m) {
		for (size_t i = 0; i < W * H - 1; i++)
		{
			os << m.data[i] << ",";
		}
		os << m.data[W * H - 1];
		return os;
	}

	template<typename U = _T>
	std::enable_if_t<W == 1 || H == 1, Vector<U, H* W>> ToVector()
	{
		Vector<U, H* W> result;
		for (std::size_t i = 0; i < H; i++) {
			result[i] = data[i];
		}
		return result;
	}

	static Matrix<_T, W, H> Identity() {
		static_assert(W == H, "Identity Matrix must be square");

		Matrix<_T, W, H> identity;
		for (std::size_t i = 0; i < W; ++i) {
			identity(i, i) = static_cast<_T>(1);
		}
		return identity;
	}
};

using Matrix3f = Matrix<float, 3, 3>;
using Matrix4f = Matrix<float, 4, 4>;
using HVector3f = Matrix<float, 3, 1>;
using HVector4f = Matrix<float, 4, 1>;
using VVector3f = Matrix<float, 1, 3>;
using VVector4f = Matrix<float, 1, 4>;