/*************************************************************************
* DNA Engine - http://gitweb.louis-schnellbach.com
* C++ Modular Data Oriented Game Enginee
*------------------------------------------------------------------------
* Copyright (c) 2014-2015, Louis Schnellbach
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
*************************************************************************/

#include <Muon/System/Assert.hpp>
#include <Muon/System/Log.hpp>
#include "Ilargia/Type/Quaternion.hpp"
#include "Ilargia/Type/Matrix.hpp"

namespace ilg
{
	const Matrix Matrix::identity =
	{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};

	const Matrix Matrix::zero =
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	};


	MatrixRow::MatrixRow(muon::f32 _x, muon::f32 _y, muon::f32 _z, muon::f32 _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{
	}

	Matrix::Matrix(MatrixRow _x, MatrixRow _y, MatrixRow _z, MatrixRow _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{
	}

	Matrix Matrix::transpose() const
	{
		Matrix n = {};
		for(muon::i32 i = 0; i < 4; ++i)
			for(muon::i32 j = 0; j < 4; ++j)
				n[i][j] = (*this)[j][i];
		return n;
	}

	muon::f32 Matrix::determinant() const
	{
		const Matrix& m = *this;
		//Laplace Expansion
		muon::f32 subFactor[6] = {m[2][2] * m[3][3] - m[3][2] * m[2][3],
							  m[2][1] * m[3][3] - m[3][1] * m[2][3],
							  m[2][1] * m[3][2] - m[3][1] * m[2][2],
							  m[2][0] * m[3][3] - m[3][0] * m[2][3],
							  m[2][0] * m[3][2] - m[3][0] * m[2][2],
							  m[2][0] * m[3][1] - m[3][0] * m[2][1]
							 };

		MatrixRow detCof =
		{
			+ (m[1][1] * subFactor[0] - m[1][2] * subFactor[1] + m[1][3] * subFactor[2]),
			- (m[1][0] * subFactor[0] - m[1][2] * subFactor[3] + m[1][3] * subFactor[4]),
			+ (m[1][0] * subFactor[1] - m[1][1] * subFactor[3] + m[1][3] * subFactor[5]),
			- (m[1][0] * subFactor[2] - m[1][1] * subFactor[4] + m[1][2] * subFactor[5])
		};

		return m[0][0] * detCof[0] + m[0][1] * detCof[1] +
			   m[0][2] * detCof[2] + m[0][3] * detCof[3];
	}

	Matrix Matrix::inverse() const
	{
		//Gaussian Elimination

		Matrix inv = Matrix::zero;
		muon::f32 tmp[4][8] = {{0}};

		//Copy matrix
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				tmp[i][j] = (*this)[i][j];

		//Set identity to right side of it
		tmp[0][4] = tmp[1][5] = tmp[2][6] = tmp[3][7] = 1;

		//Resolution
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 4; ++j)
			{
				if(i != j)
				{
					muon::f32 ratio = tmp[j][i]/tmp[i][i];
					for(int k = 0; k < 8; ++k)
					{
						tmp[j][k] -= ratio * tmp[i][k];
					}
				}
			}
		}

		for(int i = 0; i < 4; ++i)
		{
			muon::f32 diag = tmp[i][i];
			for(int k = 0; k < 8; ++k)
				tmp[i][k] /= diag;
		}

		for(int i = 0; i < 4; ++i)
			for(int j = 4; j < 8; ++j)
				inv[i][j-4] = tmp[i][j];

		return inv;
	}

	void Matrix::translate(const Vector& v)
	{
		w.x = x.x * v.x + y.x * v.y + z.x * v.z + w.x;
		w.y = x.y * v.x + y.y * v.y + z.y * v.z + w.y;
		w.z = x.z * v.x + y.z * v.y + z.z * v.z + w.z;
		w.w = x.w * v.x + y.w * v.y + z.w * v.z + w.w;
	}

	void Matrix::rotate(const Quaternion& q)
	{
		*this = operator*(q.toMatrix());
	}

	void Matrix::scale(const Vector& s)
	{
		x.x *= s.x;
		x.y *= s.x;
		x.z *= s.x;
		x.w *= s.x;

		y.x *= s.y;
		y.y *= s.y;
		y.z *= s.y;
		y.w *= s.y;

		z.x *= s.z;
		z.y *= s.z;
		z.z *= s.z;
		z.w *= s.z;
	}

	bool MatrixRow::operator==(const MatrixRow& m) const
	{
		return (x == m.x
				&& y == m.y
				&& z == m.z
				&& w == m.w);
	}

	bool MatrixRow::operator!=(const MatrixRow& m) const
	{
		return !operator==(m);
	}

	bool Matrix::operator==(const Matrix& m) const
	{
		return (x == m.x
				&& y == m.y
				&& z == m.z
				&& w == m.w);
	}

	bool Matrix::operator!=(const Matrix& m) const
	{
		return !operator==(m);
	}

	muon::f32 MatrixRow::operator[](muon::i32 i) const
	{
		return *(&x + i);
	}

	muon::f32& MatrixRow::operator[](muon::i32 i)
	{
		return *(&x + i);
	}

	MatrixRow Matrix::operator[](muon::i32 i) const
	{
		return *(&x + i);
	}
	MatrixRow& Matrix::operator[](muon::i32 i)
	{
		return *(&x + i);
	}

	Vector Matrix::operator*(const Vector& v) const
	{
		Vector mv;
		mv.x = x.x * v.x + x.y * v.y + x.z * v.z;
		mv.y = y.x * v.x + y.y * v.y + y.z * v.z;
		mv.z = z.x * v.x + z.y * v.y + z.z * v.z;
		return mv;
	}

	Matrix Matrix::operator*(const Matrix& n) const
	{
		Matrix mn = {};

		for(muon::i32 i = 0; i < 4; ++i)
		{
			for(muon::i32 j = 0; j < 4; ++j)
			{
				for(muon::i32 k = 0; k < 4; ++k)
					mn[i][j] += n[i][k] * (*this)[k][j];
			}
		}
		return mn;
	}

	Matrix Matrix::operator*(muon::f32 k) const
	{
		Matrix n = *this;
		for(muon::i32 i = 0; i < 4; ++i)
			for(muon::i32 j = 0; j < 4; ++j)
				n[i][j] *= k;
		return n;
	}

	Matrix Matrix::operator/(muon::f32 k) const
	{
		MUON_ASSERT_BREAK(k != 0.0f, "Matrix is being divided by 0 !");
		if(k != 0.0f)
		{
			return (*this * (1.f / k));
		}
		return *this;
	}
}

/*
	memory::IStream& MatrixRow::operator<<(memory::IStream& stream)
	{
		return stream << x << y << z << w;
	}

	memory::IStream& MatrixRow::operator>>(memory::IStream& stream)
	{
		return stream >> x >> y >> z >> w;
	}

	memory::IStream& Matrix::operator<<(memory::IStream& stream)
	{
		return stream << x << y << z << w;
	}

	memory::IStream& Matrix::operator>>(memory::IStream& stream)
	{
		return stream >> x >> y >> z >> w;
	}
	//*/

muon::system::Log& operator<<(muon::system::Log& stream, const ilg::MatrixRow& r)
{
	return stream << "[" << r.x << ", " << r.y << ", " << r.z << ", " << r.w << "]";
}


muon::system::Log& operator<<(muon::system::Log& stream, const ilg::Matrix& m)
{
	return stream << "[" << m.x << ", " << m.y << ", " << m.z << ", " << m.w << "]";
}
