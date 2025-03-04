#pragma once
template<class T = float>
class VectorBase
{
public:
	VectorBase(T x, T y) : x(x), y(y) {};
    VectorBase(): x(0), y(0) {};
	static const int dimension = 2;
	T x, y;
	void add(VectorBase* v);
	void sub(VectorBase* v);
	void mul(T f);
	void div(T f);
	T dot(VectorBase* v);
	T length();
	void normalize();
	operator T* ();
};

template<class T = float>
class Vector2 : VectorBase<T>
{
public:
	static const int dimension = 2;
	T x, y;
	Vector2(T x, T y): x(x), y(y) {};
	Vector2(): x(0), y(0) {};
};

using Vector2f = Vector2<float>;

//template<class T = float>
//class Vector3 : VectorBase<T>
//{
//public:
//	static const int dimension = 3;
//	Vector3();
//	Vector3(T x, T y, T z);
//	~Vector3();
//	T x, y, z;
//};
//
//using Vector3f = Vector3<float>;