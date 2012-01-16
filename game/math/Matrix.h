#ifndef MATRIX_H
#define MATRIX_H

class Vector3;
class Quaternion;

class Matrix
{
public:
	Matrix();
	Matrix(const Vector3 &vector, const Quaternion &rotation);
	Matrix(int x, int y); // pixel-perfect
	Matrix(float aspect, float zNear, float zFar); // perspective
	Matrix(float aspect, float zNear, float zFar, float scale); // ortho
	Matrix operator*(const Matrix &other) const;
	Vector3 transform(const Vector3 &vect);
	Matrix Invert();
	float matrix[16];
};

#endif

