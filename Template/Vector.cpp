#include "stdafx.h"
#include "Vector.h"
#include <math.h>

template<class T>
VectorBase<T>::operator T* ()
{
	return &x;
}

template<class T>
void VectorBase<T>::add(VectorBase<T>* v)
{
	static_assert(dimension == v->dimension, "Dimension mismatch");
	for (int i = 0; i < dimension; i++)
	{
		((float*)this)[i] += ((float*)v)[i];
	}
}

template<class T>
void VectorBase<T>::sub(VectorBase<T>* v)
{
	static_assert(dimension == v->dimension, "Dimension mismatch");
	for (int i = 0; i < dimension; i++)
	{
		((float*)this)[i] -= ((float*)v)[i];
	}
}

template<class T>
void VectorBase<T>::mul(T f)
{
	for (int i = 0; i < dimension; i++)
	{
		((T*)this)[i] *= f;
	}
}

template<class T>
void VectorBase<T>::div(T f)
{
	for (int i = 0; i < dimension; i++)
	{
		((T*)this)[i] /= f;
	}
}

template<class T>
T VectorBase<T>::dot(VectorBase<T>* v)
{
	static_assert(dimension == v->dimension, "Dimension mismatch");
	T result = 0;
	for (int i = 0; i < dimension; i++)
	{
		result += ((T*)this)[i] * ((T*)v)[i];
	}
	return result;
}

template<class T>
void VectorBase<T>::normalize()
{
	T len = length();
	for (int i = 0; i < dimension; i++)
	{
		((T*)this)[i] /= len;
	}
}

template<class T>
T VectorBase<T>::length()
{
	T result = 0;
	for (int i = 0; i < dimension; i++)
	{
		result += ((T*)this)[i] * ((T*)this)[i];
	}
	return sqrt(result);
}
