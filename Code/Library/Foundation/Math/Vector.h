// Vector.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_MATH_VECTOR_H
#define FOUNDATION_MATH_VECTOR_H

// Includes
//------------------------------------------------------------------------------
#include <array>
#include "Foundation/Platform/Types.h"

// Math
//------------------------------------------------------------------------------
namespace Math
{
	template <int N, typename Real>
	class TVector
	{
	public:
		TVector();
		TVector(std::array<Real, N> const& values);
		TVector(std::initializer_list<Real> values);

		// For 0 <= d < N, element d is 1 and all others are 0.  If d is invalid,
		// the zero vector is created.  This is a convenience for creating the
		// standard Euclidean basis vectors; see also MakeUnit(int) and Unit(int).
		TVector(int d);

		// The copy constructor, destructor, and assignment operator are generated
		// by the compiler.

		// Member access.  The first operator[] returns a const reference rather
		// than a Real value.  This supports writing via standard file operations
		// that require a const pointer to data.
		inline int GetSize() const;  // returns N
		inline Real const& operator[](int i) const;
		inline Real& operator[](int i);

		// Comparisons for sorted containers and geometric ordering.
		inline bool operator==(TVector const& vec) const;
		inline bool operator!=(TVector const& vec) const;
		inline bool operator< (TVector const& vec) const;
		inline bool operator<=(TVector const& vec) const;
		inline bool operator> (TVector const& vec) const;
		inline bool operator>=(TVector const& vec) const;

		// Special vectors.
		void MakeZero();  // All components are 0.
		void MakeUnit(int d);  // Component d is 1, all others are zero.
		static TVector Zero();
		static TVector Unit(int d);

	public:
		// This data structure takes advantage of the built-in operator[],
		// range checking, and visualizers in MSVS.
		//StackArray<Real, N> mTuple;
		union {			
			struct { Real x, y, z, w; };
			std::array<Real, N> mTuple;
		};
	};

	// Unary operations.
	template <int N, typename Real>
	TVector<N, Real> operator+(TVector<N, Real> const& v);

	template <int N, typename Real>
	TVector<N, Real> operator-(TVector<N, Real> const& v);

	// Linear-algebraic operations.
	template <int N, typename Real>
	TVector<N, Real>
		operator+(TVector<N, Real> const& v0, TVector<N, Real> const& v1);

	template <int N, typename Real>
	TVector<N, Real>
		operator-(TVector<N, Real> const& v0, TVector<N, Real> const& v1);

	template <int N, typename Real>
	TVector<N, Real> operator*(TVector<N, Real> const& v, Real scalar);

	template <int N, typename Real>
	TVector<N, Real> operator*(Real scalar, TVector<N, Real> const& v);

	template <int N, typename Real>
	TVector<N, Real> operator/(TVector<N, Real> const& v, Real scalar);

	template <int N, typename Real>
	TVector<N, Real>& operator+=(TVector<N, Real>& v0, TVector<N, Real> const& v1);

	template <int N, typename Real>
	TVector<N, Real>& operator-=(TVector<N, Real>& v0, TVector<N, Real> const& v1);

	template <int N, typename Real>
	TVector<N, Real>& operator*=(TVector<N, Real>& v, Real scalar);

	template <int N, typename Real>
	TVector<N, Real>& operator/=(TVector<N, Real>& v, Real scalar);

	// Componentwise algebraic operations.
	template <int N, typename Real>
	TVector<N, Real> operator*(TVector<N, Real> const& v0, TVector<N, Real> const& v1);

	template <int N, typename Real>
	TVector<N, Real> operator/(TVector<N, Real> const& v0, TVector<N, Real> const& v1);

	template <int N, typename Real>
	TVector<N, Real>& operator*=(TVector<N, Real>& v0, TVector<N, Real> const& v1);

	template <int N, typename Real>
	TVector<N, Real>& operator/=(TVector<N, Real>& v0, TVector<N, Real> const& v1);

	// Geometric operations.  The functions with 'robust' set to 'false' use the
	// standard algorithm for normalizing a vector by computing the length as a
	// square root of the squared length and dividing by it.  The results can be
	// infinite (or NaN) if the length is zero.  When 'robust' is set to 'true',
	// the algorithm is designed to avoid floating-point overflow and sets the
	// normalized vector to zero when the length is zero.
	template <int N, typename Real>
	Real Dot(TVector<N, Real> const& v0, TVector<N, Real> const& v1);

	template <int N, typename Real>
	Real Length(TVector<N, Real> const& v, bool robust = false);

	template <int N, typename Real>
	Real Normalize(TVector<N, Real>& v, bool robust = false);

	// Gram-Schmidt orthonormalization to generate orthonormal vectors from the
	// linearly independent inputs.  The function returns the smallest length of
	// the unnormalized vectors computed during the process.  If this value is
	// nearly zero, it is possible that the inputs are linearly dependent (within
	// numerical round-off errors).  On input, 1 <= numElements <= N and v[0]
	// through v[numElements-1] must be initialized.  On output, the vectors
	// v[0] through v[numElements-1] form an orthonormal set.
	template <int N, typename Real>
	Real Orthonormalize(int numElements, TVector<N, Real>* v, bool robust = false);

	// Construct a single vector orthogonal to the nonzero input vector.  If the
	// maximum absolute component occurs at index i, then the orthogonal vector
	// U has u[i] = v[i+1], u[i+1] = -v[i], and all other components zero.  The
	// index addition i+1 is computed modulo N.
	template <int N, typename Real>
	TVector<N, Real> GetOrthogonal(TVector<N, Real> const& v, bool unitLength);

	// Compute the axis-aligned bounding box of the vectors.  The return value is
	// 'true' iff the inputs are valid, in which case vmin and vmax have valid
	// values.
	template <int N, typename Real>
	bool ComputeExtremes(int numVectors, TVector<N, Real> const* v, TVector<N, Real>& vmin, TVector<N, Real>& vmax);

	// Lift n-tuple v to homogeneous (n+1)-tuple (v,last).
	template <int N, typename Real>
	TVector<N + 1, Real> HLift(TVector<N, Real> const& v, Real last);

	// Project homogeneous n-tuple v = (u,v[n-1]) to (n-1)-tuple u.
	template <int N, typename Real>
	TVector<N - 1, Real> HProject(TVector<N, Real> const& v);

	// Lift n-tuple v = (w0,w1) to (n+1)-tuple u = (w0,u[inject],w1).  By
	// inference, w0 is a (inject)-tuple [nonexistent when inject=0] and w1 is a
	// (n-inject)-tuple [nonexistent when inject=n].
	template <int N, typename Real>
	TVector<N + 1, Real> Lift(TVector<N, Real> const& v, int inject, Real value);

	// Project n-tuple v = (w0,v[reject],w1) to (n-1)-tuple u = (w0,w1).  By
	// inference, w0 is a (reject)-tuple [nonexistent when reject=0] and w1 is a
	// (n-1-reject)-tuple [nonexistent when reject=n-1].
	template <int N, typename Real>
	TVector<N - 1, Real> Project(TVector<N, Real> const& v, int reject);


	template <int N, typename Real>
	TVector<N, Real>::TVector()
	{
		// Uninitialized.
	}

	template <int N, typename Real>
	TVector<N, Real>::TVector(std::array<Real, N> const& values)
		: mTuple(values)
	{
	}

	template <int N, typename Real>
	TVector<N, Real>::TVector(std::initializer_list<Real> values)
	{
		int i = 0;
		for (auto value : values)
		{
			if (i < N)
			{
				mTuple[i++] = value;
			}
			else
			{
				break;
			}
		}
		for (/**/; i < N; ++i)
		{
			mTuple[i] = (Real)0;
		}
	}

	template <int N, typename Real>
	TVector<N, Real>::TVector(int d)
	{
		MakeUnit(d);
	}

	template <int N, typename Real> inline
		int TVector<N, Real>::GetSize() const
	{
		return N;
	}

	template <int N, typename Real> inline
		Real const& TVector<N, Real>::operator[](int i) const
	{
		return mTuple[i];
	}

	template <int N, typename Real> inline
		Real& TVector<N, Real>::operator[](int i)
	{
		return mTuple[i];
	}

	template <int N, typename Real> inline
		bool TVector<N, Real>::operator==(TVector const& vec) const
	{
		return mTuple == vec.mTuple;
	}

	template <int N, typename Real> inline
		bool TVector<N, Real>::operator!=(TVector const& vec) const
	{
		return mTuple != vec.mTuple;
	}

	template <int N, typename Real> inline
		bool TVector<N, Real>::operator<(TVector const& vec) const
	{
		return mTuple < vec.mTuple;
	}

	template <int N, typename Real> inline
		bool TVector<N, Real>::operator<=(TVector const& vec) const
	{
		return mTuple <= vec.mTuple;
	}

	template <int N, typename Real> inline
		bool TVector<N, Real>::operator>(TVector const& vec) const
	{
		return mTuple > vec.mTuple;
	}

	template <int N, typename Real> inline
		bool TVector<N, Real>::operator>=(TVector const& vec) const
	{
		return mTuple >= vec.mTuple;
	}

	template <int N, typename Real>
	void TVector<N, Real>::MakeZero()
	{
		std::fill(mTuple.begin(), mTuple.end(), (Real)0);
	}

	template <int N, typename Real>
	void TVector<N, Real>::MakeUnit(int d)
	{
		std::fill(mTuple.begin(), mTuple.end(), (Real)0);
		if (0 <= d && d < N)
		{
			mTuple[d] = (Real)1;
		}
	}

	template <int N, typename Real>
	TVector<N, Real> TVector<N, Real>::Zero()
	{
		TVector<N, Real> v;
		v.MakeZero();
		return v;
	}

	template <int N, typename Real>
	TVector<N, Real> TVector<N, Real>::Unit(int d)
	{
		TVector<N, Real> v;
		v.MakeUnit(d);
		return v;
	}



	template <int N, typename Real>
	TVector<N, Real> operator+(TVector<N, Real> const& v)
	{
		return v;
	}

	template <int N, typename Real>
	TVector<N, Real> operator-(TVector<N, Real> const& v)
	{
		TVector<N, Real> result;
		for (int i = 0; i < N; ++i)
		{
			result[i] = -v[i];
		}
		return result;
	}

	template <int N, typename Real>
	TVector<N, Real> operator+(TVector<N, Real> const& v0,
		TVector<N, Real> const& v1)
	{
		TVector<N, Real> result = v0;
		return result += v1;
	}

	template <int N, typename Real>
	TVector<N, Real> operator-(TVector<N, Real> const& v0,
		TVector<N, Real> const& v1)
	{
		TVector<N, Real> result = v0;
		return result -= v1;
	}

	template <int N, typename Real>
	TVector<N, Real> operator*(TVector<N, Real> const& v, Real scalar)
	{
		TVector<N, Real> result = v;
		return result *= scalar;
	}

	template <int N, typename Real>
	TVector<N, Real> operator*(Real scalar, TVector<N, Real> const& v)
	{
		TVector<N, Real> result = v;
		return result *= scalar;
	}

	template <int N, typename Real>
	TVector<N, Real> operator/(TVector<N, Real> const& v, Real scalar)
	{
		TVector<N, Real> result = v;
		return result /= scalar;
	}

	template <int N, typename Real>
	TVector<N, Real>& operator+=(TVector<N, Real>& v0, TVector<N, Real> const& v1)
	{
		for (int i = 0; i < N; ++i)
		{
			v0[i] += v1[i];
		}
		return v0;
	}

	template <int N, typename Real>
	TVector<N, Real>& operator-=(TVector<N, Real>& v0, TVector<N, Real> const& v1)
	{
		for (int i = 0; i < N; ++i)
		{
			v0[i] -= v1[i];
		}
		return v0;
	}

	template <int N, typename Real>
	TVector<N, Real>& operator*=(TVector<N, Real>& v, Real scalar)
	{
		for (int i = 0; i < N; ++i)
		{
			v[i] *= scalar;
		}
		return v;
	}

	template <int N, typename Real>
	TVector<N, Real>& operator/=(TVector<N, Real>& v, Real scalar)
	{
		if (scalar != (Real)0)
		{
			Real invScalar = ((Real)1) / scalar;
			for (int i = 0; i < N; ++i)
			{
				v[i] *= invScalar;
			}
		}
		else
		{
			for (int i = 0; i < N; ++i)
			{
				v[i] = (Real)0;
			}
		}
		return v;
	}

	template <int N, typename Real>
	TVector<N, Real> operator*(TVector<N, Real> const& v0,
		TVector<N, Real> const& v1)
	{
		TVector<N, Real> result = v0;
		return result *= v1;
	}

	template <int N, typename Real>
	TVector<N, Real> operator/(TVector<N, Real> const& v0,
		TVector<N, Real> const& v1)
	{
		TVector<N, Real> result = v0;
		return result /= v1;
	}

	template <int N, typename Real>
	TVector<N, Real>& operator*=(TVector<N, Real>& v0, TVector<N, Real> const& v1)
	{
		for (int i = 0; i < N; ++i)
		{
			v0[i] *= v1[i];
		}
		return v0;
	}

	template <int N, typename Real>
	TVector<N, Real>& operator/=(TVector<N, Real>& v0, TVector<N, Real> const& v1)
	{
		for (int i = 0; i < N; ++i)
		{
			v0[i] /= v1[i];
		}
		return v0;
	}

	template <int N, typename Real>
	Real Dot(TVector<N, Real> const& v0, TVector<N, Real> const& v1)
	{
		Real dot = v0[0] * v1[0];
		for (int i = 1; i < N; ++i)
		{
			dot += v0[i] * v1[i];
		}
		return dot;
	}

	template <int N, typename Real>
	Real Length(TVector<N, Real> const& v, bool robust)
	{
		if (robust)
		{
			Real maxAbsComp = fabs(v[0]);
			for (int i = 1; i < N; ++i)
			{
				Real absComp = fabs(v[i]);
				if (absComp > maxAbsComp)
				{
					maxAbsComp = absComp;
				}
			}

			Real length;
			if (maxAbsComp > (Real)0)
			{
				TVector<N, Real> scaled = v / maxAbsComp;
				length = maxAbsComp * sqrt(Dot(scaled, scaled));
			}
			else
			{
				length = (Real)0;
			}
			return length;
		}
		else
		{
			return sqrt(Dot(v, v));
		}
	}

	template <int N, typename Real>
	Real Normalize(TVector<N, Real>& v, bool robust)
	{
		if (robust)
		{
			Real maxAbsComp = fabs(v[0]);
			for (int i = 1; i < N; ++i)
			{
				Real absComp = fabs(v[i]);
				if (absComp > maxAbsComp)
				{
					maxAbsComp = absComp;
				}
			}

			Real length;
			if (maxAbsComp > (Real)0)
			{
				v /= maxAbsComp;
				length = sqrt(Dot(v, v));
				v /= length;
				length *= maxAbsComp;
			}
			else
			{
				length = (Real)0;
				for (int i = 0; i < N; ++i)
				{
					v[i] = (Real)0;
				}
			}
			return length;
		}
		else
		{
			Real length = sqrt(Dot(v, v));
			if (length > (Real)0)
			{
				v /= length;
			}
			else
			{
				for (int i = 0; i < N; ++i)
				{
					v[i] = (Real)0;
				}
			}
			return length;
		}
	}

	template <int N, typename Real>
	Real Orthonormalize(int numInputs, TVector<N, Real>* v, bool robust)
	{
		if (v && 1 <= numInputs && numInputs <= N)
		{
			Real minLength = Normalize(v[0], robust);
			for (int i = 1; i < numInputs; ++i)
			{
				for (int j = 0; j < i; ++j)
				{
					Real dot = Dot(v[i], v[j]);
					v[i] -= v[j] * dot;
				}
				Real length = Normalize(v[i], robust);
				if (length < minLength)
				{
					minLength = length;
				}
			}
			return minLength;
		}

		return (Real)0;
	}

	template <int N, typename Real>
	TVector<N, Real> GetOrthogonal(TVector<N, Real> const& v, bool unitLength)
	{
		Real cmax = std::abs(v[0]);
		int imax = 0;
		for (int i = 1; i < N; ++i)
		{
			Real c = std::abs(v[i]);
			if (c > cmax)
			{
				cmax = c;
				imax = i;
			}
		}

		TVector<N, Real> result;
		result.MakeZero();
		int inext = imax + 1;
		if (inext == N)
		{
			inext = 0;
		}
		result[imax] = v[inext];
		result[inext] = -v[imax];
		if (unitLength)
		{
			Real sqrDistance =
				result[imax] * result[imax] + result[inext] * result[inext];
			Real invLength = ((Real)1) / sqrt(sqrDistance);
			result[imax] *= invLength;
			result[inext] *= invLength;
		}
		return result;
	}

	template <int N, typename Real>
	bool ComputeExtremes(int numVectors, TVector<N, Real> const* v,
		TVector<N, Real>& vmin, TVector<N, Real>& vmax)
	{
		if (v && numVectors > 0)
		{
			vmin = v[0];
			vmax = vmin;
			for (int j = 1; j < numVectors; ++j)
			{
				TVector<N, Real> const& vec = v[j];
				for (int i = 0; i < N; ++i)
				{
					if (vec[i] < vmin[i])
					{
						vmin[i] = vec[i];
					}
					else if (vec[i] > vmax[i])
					{
						vmax[i] = vec[i];
					}
				}
			}
			return true;
		}

		return false;
	}

	template <int N, typename Real>
	TVector<N + 1, Real> HLift(TVector<N, Real> const& v, Real last)
	{
		TVector<N + 1, Real> result;
		for (int i = 0; i < N; ++i)
		{
			result[i] = v[i];
		}
		result[N] = last;
		return result;
	}

	template <int N, typename Real>
	TVector<N - 1, Real> HProject(TVector<N, Real> const& v)
	{
		static_assert(N >= 2, "Invalid dimension.");
		TVector<N - 1, Real> result;
		for (int i = 0; i < N - 1; ++i)
		{
			result[i] = v[i];
		}
		return result;
	}

	template <int N, typename Real>
	TVector<N + 1, Real> Lift(TVector<N, Real> const& v, int inject, Real value)
	{
		TVector<N + 1, Real> result;
		int i;
		for (i = 0; i < inject; ++i)
		{
			result[i] = v[i];
		}
		result[i] = value;
		int j = i;
		for (++j; i < N; ++i, ++j)
		{
			result[j] = v[i];
		}
		return result;
	}

	template <int N, typename Real>
	TVector<N - 1, Real> Project(TVector<N, Real> const& v, int reject)
	{
		static_assert(N >= 2, "Invalid dimension.");
		TVector<N - 1, Real> result;
		for (int i = 0, j = 0; i < N - 1; ++i, ++j)
		{
			if (j == reject)
			{
				++j;
			}
			result[i] = v[j];
		}
		return result;
	}
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_MATH_VECTOR_H
