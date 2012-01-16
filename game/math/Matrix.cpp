#include "Matrix.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <windows.h>
#include <math.h>

Matrix::Matrix()
{
	matrix[0] = 1;
	matrix[1] = 0;
	matrix[2] = 0;
	matrix[3] = 0;
	matrix[4] = 0;
	matrix[5] = 1;
	matrix[6] = 0;
	matrix[7] = 0;
	matrix[8] = 0;
	matrix[9] = 0;
	matrix[10] = 1;
	matrix[11] = 0;
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;
}

Matrix::Matrix(int x, int y)
{
	// static 90-deg fov vertical
	matrix[0] = 1.0f/x;
	matrix[1] = 0;
	matrix[2] = 0;
	matrix[3] = 0;
	matrix[4] = 0;
	matrix[5] = 1.0f/y;
	matrix[6] = 0;
	matrix[7] = 0;
	matrix[8] = 0;
	matrix[9] = 0;
	matrix[10] = 1;
	matrix[11] = 0;
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;
}

Matrix::Matrix(float aspect, float zNear, float zFar)
{
	// static 90-deg fov vertical
	matrix[0] = 1/aspect;
	matrix[1] = 0;
	matrix[2] = 0;
	matrix[3] = 0;
	matrix[4] = 0;
	matrix[5] = 1;
	matrix[6] = 0;
	matrix[7] = 0;
	matrix[8] = 0;
	matrix[9] = 0;
	matrix[10] = (-zFar - zNear)/(zFar - zNear);
	matrix[11] = -1;
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = -(2 * zFar * zNear)/(zFar - zNear);
	matrix[15] = 0;
}

Matrix::Matrix(float aspect, float zNear, float zFar, float scale)
{
	matrix[0] = 1/aspect;
	matrix[1] = 0;
	matrix[2] = 0;
	matrix[3] = 0;
	matrix[4] = 0;
	matrix[5] = 1;
	matrix[6] = 0;
	matrix[7] = 0;
	matrix[8] = 0;
	matrix[9] = 0;
	matrix[10] = 2/(zFar-zNear);
	matrix[11] = 0;
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1/scale;
}

Matrix::Matrix(const Vector3 &t, const Quaternion &r)
{
	float x2 = r.x * r.x;
	float y2 = r.y * r.y;
	float z2 = r.z * r.z;
	float xy = r.x * r.y;
	float xz = r.x * r.z;
	float yz = r.y * r.z;
	float wx = r.w * r.x;
	float wy = r.w * r.y;
	float wz = r.w * r.z;

	matrix[0] = 1.0f - 2.0f * (y2 + z2);
	matrix[1] = 2.0f * (xy + wz);
	matrix[2] = 2.0f * (xz - wy);
	matrix[3] = 0.0f;
	matrix[4] = 2.0f * (xy - wz);
	matrix[5] = 1.0f - 2.0f * (x2 + z2);
	matrix[6] = 2.0f * (yz + wx);
	matrix[7] = 0.0f;
	matrix[8] = 2.0f * (xz + wy);
	matrix[9] = 2.0f * (yz - wx);
	matrix[10] = 1.0f - 2.0f * (x2 + y2);
	matrix[11] = 0.0f;
	matrix[12] = t.x;
	matrix[13] = t.y;
	matrix[14] = t.z;
	matrix[15] = 1.0f;
}

Vector3 Matrix::transform(const Vector3 &vect)
{
	Vector3 rv;
	float t = matrix[3] * vect.x +
			  matrix[7] * vect.y +
			  matrix[11] * vect.z +
			  matrix[15];
	rv.x = (matrix[0] * vect.x + 
		    matrix[4] * vect.y + 
		    matrix[8] * vect.z +
		    matrix[12]) / t;
	rv.y = (matrix[1] * vect.x + 
		    matrix[5] * vect.y + 
		    matrix[9] * vect.z +
		    matrix[13]) / t;
	rv.z = (matrix[2] * vect.x + 
		    matrix[6] * vect.y + 
		    matrix[10] * vect.z +
		    matrix[14]) / t;
	return rv;
}

Matrix Matrix::operator*(const Matrix &other) const
{
	Matrix rv;
	rv.matrix[0] = matrix[0] * other.matrix[0] + matrix[4] * other.matrix[1] + matrix[8] * other.matrix[2] + matrix[12] * other.matrix[3];
	rv.matrix[1] = matrix[1] * other.matrix[0] + matrix[5] * other.matrix[1] + matrix[9] * other.matrix[2] + matrix[13] * other.matrix[3];
	rv.matrix[2] = matrix[2] * other.matrix[0] + matrix[6] * other.matrix[1] + matrix[10] * other.matrix[2] + matrix[14] * other.matrix[3];
	rv.matrix[3] = matrix[3] * other.matrix[0] + matrix[7] * other.matrix[1] + matrix[11] * other.matrix[2] + matrix[15] * other.matrix[3];

	rv.matrix[4] = matrix[0] * other.matrix[4] + matrix[4] * other.matrix[5] + matrix[8] * other.matrix[6] + matrix[12] * other.matrix[7];
	rv.matrix[5] = matrix[1] * other.matrix[4] + matrix[5] * other.matrix[5] + matrix[9] * other.matrix[6] + matrix[13] * other.matrix[7];
	rv.matrix[6] = matrix[2] * other.matrix[4] + matrix[6] * other.matrix[5] + matrix[10] * other.matrix[6] + matrix[14] * other.matrix[7];
	rv.matrix[7] = matrix[3] * other.matrix[4] + matrix[7] * other.matrix[5] + matrix[11] * other.matrix[6] + matrix[15] * other.matrix[7];

	rv.matrix[8] = matrix[0] * other.matrix[8] + matrix[4] * other.matrix[9] + matrix[8] * other.matrix[10] + matrix[12] * other.matrix[11];
	rv.matrix[9] = matrix[1] * other.matrix[8] + matrix[5] * other.matrix[9] + matrix[9] * other.matrix[10] + matrix[13] * other.matrix[11];
	rv.matrix[10] = matrix[2] * other.matrix[8] + matrix[6] * other.matrix[9] + matrix[10] * other.matrix[10] + matrix[14] * other.matrix[11];
	rv.matrix[11] = matrix[3] * other.matrix[8] + matrix[7] * other.matrix[9] + matrix[11] * other.matrix[10] + matrix[15] * other.matrix[11];

	rv.matrix[12] = matrix[0] * other.matrix[12] + matrix[4] * other.matrix[13] + matrix[8] * other.matrix[14] + matrix[12] * other.matrix[15];
	rv.matrix[13] = matrix[1] * other.matrix[12] + matrix[5] * other.matrix[13] + matrix[9] * other.matrix[14] + matrix[13] * other.matrix[15];
	rv.matrix[14] = matrix[2] * other.matrix[12] + matrix[6] * other.matrix[13] + matrix[10] * other.matrix[14] + matrix[14] * other.matrix[15];
	rv.matrix[15] = matrix[3] * other.matrix[12] + matrix[7] * other.matrix[13] + matrix[11] * other.matrix[14] + matrix[15] * other.matrix[15];

	return rv;
}

Matrix Matrix::Invert()
{
	float tmpmat[8][4] = {0};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tmpmat[i][j] = matrix[j*4+i];
			tmpmat[i+4][j] = (i == j) ? 1.0f : 0.0f;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = i+1; j < 4; j++) {
			if (fabsf(tmpmat[i][j]) > fabsf(tmpmat[i][i]))
			{
				for (int k = 0; k < 8; k++)
				{
					float t = tmpmat[k][i];
					tmpmat[k][i] = tmpmat[k][j];
					tmpmat[k][j] = t;
				}
				break;
			}
		}
		if (tmpmat[i][i] == 0)
			DebugBreak();
		float row_det = tmpmat[i][i];
		for (int j = 0; j < 8; j++) {
			tmpmat[j][i] /= row_det;
		}
		for (int j = i+1; j < 4; j++) {
			float row_det = tmpmat[i][j];
			for (int k = 0; k < 8; k++) {
				tmpmat[k][j] -= row_det * tmpmat[k][i];
			}
		}
	}
	for (int i = 3; i >= 0; i--) {
		for (int j = i-1; j >= 0; j--) {
			float row_det = tmpmat[i][j];
			for (int k = i; k < 8; k++) {
				tmpmat[k][j] -= row_det * tmpmat[k][i];
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix[j*4+i] = tmpmat[i+4][j];
		}
	}
	return *this;
}

