#include "BERender.h"
#include <math.h>

Render defaultRender;

typedef struct Vector
{
	real32 x;
	real32 y;
	real32 z;

	Vector(real32 x = 0, real32 y = 0, real32 z = 0)
		: x(x), y(y), z(z)
	{
	}

	Vector& operator=(const Vector& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	Vector operator+(const Vector& a) const
	{
		return Vector(a.x + x, a.y + y, z + a.z);
	}

	Vector operator-(const Vector& a) const
	{
		return Vector(x - a.x, y - a.y, z - a.z);
	}

	Vector operator*(const Vector& a) const
	{
		return Vector(x * a.x, y * a.y, z * a.z);
	}

	Vector operator*(real32 a) const
	{
		return Vector(x * a, y * a, z * a);
	}

	Vector operator/(const Vector& a) const
	{
		return Vector(x / a.x, y / a.y, z / a.z);
	}

	Vector operator/(const real32 a) const
	{
		return Vector(x / a, y / a, z / a);
	}

	bool operator==(const Vector& a) const
	{
		return (x == a.x && y == a.y && z == a.z);
	}

	/*
		Cos Angle between two vectors if both vectors are normalized.
	*/
	real32 DotProduct(Vector vector)
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}

	/*
		Returns a directional Vector to the two directional vectors
	*/
	Vector CrossProduct(Vector vector)
	{
		return Vector(
			y * vector.z - z * vector.y,
			z * vector.x - x * vector.z,
			x * vector.y - y * vector.x
			);
	}
 
	real32 Length()
	{
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	real32 LengthSquared()
	{
		return (x * x) + (y * y) + (z * z);
	}

	Vector Normalized()
	{
		return *this / (Length() + 0.0001f);
	}

	Vector Convert2D()
	{
		return Vector(x / z, y / z);
	}

	Vector Rotate(Vector angle, real32 degree)
	{
		real32 radian = degree * pi32 / 180.0f;
		Vector newVector = *this - angle;
		real32 newX = newVector.x * cosf(radian) - newVector.y * sinf(radian);
		real32 newY = newVector.y * cosf(radian) + newVector.x * sinf(radian);
		return Vector(newX, newY, 0);
	}

	real32 Angle(Vector vector)
	{
		return acos(x * vector.x + y * vector.y + z * vector.z /
			(((x * x) + (y * y) + (z * z)) *
			((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z))));
	}

	Vector Interpolate(Vector vector, real32 t)
	{
		return *this + (vector - *this) * t;
	}

	Vector InterpolateNormalized(Vector vector, real32 t)
	{
		return Vector(*this + (vector - *this) * t).Normalized();
	}
} Vector;

typedef struct Matrix
{
	union
	{
		struct
		{
			real32 m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44;
		};
		float m[4][4];
	};

	Matrix()
	{}

	Matrix(real32 c11, real32 c12, real32 c13, real32 c14,
		real32 c21, real32 c22, real32 c23, real32 c24,
		real32 c31, real32 c32, real32 c33, real32 c34,
		real32 c41, real32 c42, real32 c43, real32 c44) : m11(c11), m12(c12), m13(c13), m14(c14),
		m21(c21), m22(c22), m23(c23), m24(c24),
		m31(c31), m32(c32), m33(c33), m34(c34),
		m41(c41), m42(c42), m43(c43), m44(c44)
	{		
	}

	Matrix operator+(const Matrix& matrix)
	{
		return Matrix(m11 + matrix.m11, m12 + matrix.m12, m13 + matrix.m13, m14 + matrix.m14,
			m21 + matrix.m21, m22 + matrix.m22, m23 + matrix.m23, m24 + matrix.m24,
			m31 + matrix.m31, m32 + matrix.m32, m33 + matrix.m33, m34 + matrix.m34,
			m41 + matrix.m41, m42 + matrix.m42, m43 + matrix.m43, m44 + matrix.m44);
	}

	Matrix operator*(const Matrix& matrix)
	{
		return Matrix(
			matrix.m11 * m11 + matrix.m21 * m12 + matrix.m31 * m13 + matrix.m41 * m14,
			matrix.m12 * m11 + matrix.m22 * m12 + matrix.m32 * m13 + matrix.m42 * m14,
			matrix.m13 * m11 + matrix.m23 * m12 + matrix.m33 * m13 + matrix.m43 * m14,
			matrix.m14 * m11 + matrix.m24 * m12 + matrix.m34 * m13 + matrix.m44 * m14,
			matrix.m11 * m21 + matrix.m21 * m22 + matrix.m31 * m23 + matrix.m41 * m24,
			matrix.m12 * m21 + matrix.m22 * m22 + matrix.m32 * m23 + matrix.m42 * m24,
			matrix.m13 * m21 + matrix.m23 * m22 + matrix.m33 * m23 + matrix.m43 * m24,
			matrix.m14 * m21 + matrix.m24 * m22 + matrix.m34 * m23 + matrix.m44 * m24,
			matrix.m11 * m31 + matrix.m21 * m32 + matrix.m31 * m33 + matrix.m41 * m34,
			matrix.m12 * m31 + matrix.m22 * m32 + matrix.m32 * m33 + matrix.m42 * m34,
			matrix.m13 * m31 + matrix.m23 * m32 + matrix.m33 * m33 + matrix.m43 * m34,
			matrix.m14 * m31 + matrix.m24 * m32 + matrix.m34 * m33 + matrix.m44 * m34,
			matrix.m11 * m41 + matrix.m21 * m42 + matrix.m31 * m43 + matrix.m41 * m44,
			matrix.m12 * m41 + matrix.m22 * m42 + matrix.m32 * m43 + matrix.m42 * m44,
			matrix.m13 * m41 + matrix.m23 * m42 + matrix.m33 * m43 + matrix.m43 * m44,
			matrix.m14 * m41 + matrix.m24 * m42 + matrix.m34 * m43 + matrix.m44 * m44
			);
	}

	float& operator()(int row, int column)
	{
		return m[row - 1][column - 1];
	}

	float operator()(int row, int column) const
	{
		return m[row - 1][column - 1];
	}

	Matrix IdentityMatrix()
	{
		return Matrix(1.0f, 0.0f, 0.0f, 0.0f, 
			0.0f, 1.0f, 0.0f, 0.0f, 
			0.0f, 0.0f, 1.0f, 0.0f, 
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix TranslationMatrix(const Vector& vector)
	{
		return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			vector.x, vector.y, vector.z, 1.0f);
	}

	Matrix RotationMatrixX(real32 rotation)
	{
		Matrix result;
		result.m11 = 1.0f; result.m12 = 0.0f; result.m13 = 0.0f; result.m14 = 0.0f;
		result.m21 = 0.0f; result.m24 = 0.0f;
		result.m31 = 0.0f; result.m34 = 0.0f;
		result.m41 = 0.0f; result.m42 = 0.0f; result.m43 = 0.0f; result.m44 = 1.0f;
		result.m22 = result.m33 = cosf(rotation);
		result.m23 = sinf(rotation);
		result.m32 = -result.m24;
		return result;
	}

	Matrix RotationMatrixY(real32 rotation)
	{
		Matrix result;
		result.m12 = 0.0f; result.m14 = 0.0f;
		result.m21 = 0.0f; result.m22 = 1.0f; result.m23 = 0.0f; result.m24 = 0.0f;
		result.m32 = 0.0f; result.m34 = 0.0f;
		result.m41 = 0.0f; result.m42 = 0.0f; result.m43 = 0.0f; result.m44 = 1.0f;
		result.m11 = result.m33 = cosf(rotation);
		result.m31 = sinf(rotation);
		result.m13 = -result.m31;
		return result;
	}

	Matrix RotationMatrixZ(real32 rotation)
	{
		Matrix result;
		result.m13 = 0.0f; result.m14 = 0.0f;
		result.m23 = 0.0f; result.m24 = 0.0f;
		result.m31 = 0.0f; result.m32 = 0.0f; result.m33 = 1.0f; result.m34 = 0.0f;
		result.m41 = 0.0f; result.m42 = 0.0f; result.m43 = 0.0f; result.m44 = 1.0f;
		result.m11 = result.m22 = cosf(rotation);
		result.m12 = sinf(rotation);
		result.m21 = -result.m12;
		return result;
	}

	Matrix RoationMatrixAxis(Vector& vector, const real32 rotation)
	{
		const real32 fSin = sinf(-rotation), fCos = cosf(-rotation);
		const real32 fOneMinusCos = 1.0f - fCos;

		const Vector vAxis = vector.Normalized();

		return Matrix((vAxis.x * vAxis.x) * fOneMinusCos + fCos,
			(vAxis.x * vAxis.y) * fOneMinusCos - (vAxis.z * fSin),
			(vAxis.x * vAxis.z) * fOneMinusCos + (vAxis.y * fSin), 0.0f,
			(vAxis.y * vAxis.x) * fOneMinusCos + (vAxis.z * fSin),
			(vAxis.y * vAxis.y) * fOneMinusCos + fCos,
			(vAxis.y * vAxis.z) * fOneMinusCos - (vAxis.x * fSin), 0.0f,
			(vAxis.z * vAxis.x) * fOneMinusCos - (vAxis.y * fSin),
			(vAxis.z * vAxis.y) * fOneMinusCos + (vAxis.x * fSin),
			(vAxis.z * vAxis.z) * fOneMinusCos + fCos, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
	}

	Matrix ScalingMatrix(const Vector& vector)
	{
		return Matrix(vector.x, 0.0f, 0.0f, 0.0f,
			0.0f, vector.y, 0.0f, 0.0f,
			0.0f, 0.0f, vector.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
	}

	real32 Determinant()
	{
		return m11 * (m22 * m33 - m23 * m32) -
			m12 * (m21 * m33 - m23 * m31) +
			m13 * (m21 * m32 - m22 * m31);
	}

	Matrix Inverted()
	{
		float fInvDet = Determinant();
		if (fInvDet == 0.0f) return IdentityMatrix();
		fInvDet = 1.0f / fInvDet;
		Matrix result;
		result.m11 = fInvDet * (m22 * m33 - m23 * m32);
		result.m12 = -fInvDet * (m12 * m33 - m13 * m32);
		result.m13 = fInvDet * (m12 * m23 - m13 * m22);
		result.m14 = 0.0f;
		result.m11 = -fInvDet * (m21 * m33 - m23 * m31);
		result.m12 = fInvDet * (m11 * m33 - m13 * m31);
		result.m13 = -fInvDet * (m11 * m23 - m13 * m21);
		result.m14 = 0.0f;
		result.m11 = fInvDet * (m21 * m32 - m22 * m31);
		result.m12 = -fInvDet * (m11 * m32 - m12 * m31);
		result.m13 = fInvDet * (m11 * m22 - m12 * m21);
		result.m14 = 0.0f;
		result.m11 = -fInvDet * (m41 * result.m11 + m42 * result.m21 + m43 * result.m31);
		result.m12 = -fInvDet * (m41 * result.m12 + m42 * result.m22 + m43 * result.m32);
		result.m13 = -fInvDet * (m41 * result.m13 + m42 * result.m23 + m43 * result.m33);
		result.m14 = 1.0f;
		return result;
	}

	Matrix Transponsed()
	{
		return Matrix(m11, m21, m31, m41,
			m12, m22, m32, m42,
			m13, m23, m33, m43,
			m14, m24, m34, m44
			);
	}

} Matrix;

typedef struct Square
{
	Vector a;
	Vector b;
	Vector c;
	Vector d;
	Vector center;
	real32 rotation;

	Square()
	{
		rotation = 0;
	}

	Square(Vector a, Vector b)
	{
		this->a = a;
		this->b = b;
		calculatePositions();
		rotation = 0;
	}

	Square(Vector a, Vector b, Vector c, Vector d, real32 rotation)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->rotation = rotation;
		center = a + ((b - a) * 0.5f);
	}
	
	void rotate(Vector angle, real32 degree)
	{
		a = angle + a.Rotate(angle, degree);
		b = angle + b.Rotate(angle, degree);
		c = angle + c.Rotate(angle, degree);
		d = angle + d.Rotate(angle, degree);
		center = a + ((b - a) * 0.5f);
	}

	void calculatePositions()
	{
		this->c = Vector(a.x, b.y);
		this->d = Vector(b.x, a.y);
		center = a + ((b - a) * 0.5f);
	}

	Square transform(Vector position)
	{
		Square tmp = Square(a + position, b + position, c + position, d + position, rotation);
		return tmp;
	}

};

typedef struct Color
{
	uint8 blue;
	uint8 green;
	uint8 red;
	uint8 alpha;

	Color(uint8 blue, uint8 green, uint8 red, uint8 alpha)
	{
		this->blue = blue;
		this->red = red;
		this->green = green;
		this->alpha = alpha;
	}
};

void InitRender(Render* render)
{
	render->screenBuffer.memory = 0;
	render->screenBuffer.bytesPerPixel = 4;
	render->screenBuffer.info.bmiHeader.biSize = sizeof(render->screenBuffer.info.bmiHeader);
	render->screenBuffer.info.bmiHeader.biPlanes = 1;
	render->screenBuffer.info.bmiHeader.biBitCount = 32;
	render->screenBuffer.info.bmiHeader.biCompression = BI_RGB;
	render->screenBuffer.info.bmiHeader.biSizeImage = 0;
	render->screenBuffer.info.bmiHeader.biXPelsPerMeter = 0;
	render->screenBuffer.info.bmiHeader.biYPelsPerMeter = 0;
}

void ResizeDIBSection(Render* render, int32 width, int32 height)
{
	if (render->screenBuffer.memory)
	{
		VirtualFree(render->screenBuffer.memory, 0, MEM_RELEASE);
	}

	render->screenBuffer.info.bmiHeader.biWidth = width;
	render->screenBuffer.info.bmiHeader.biHeight = height;
	render->screenBuffer.width = width;
	render->screenBuffer.height = height;
	render->screenBuffer.pitch = render->screenBuffer.width * render->screenBuffer.bytesPerPixel;
	render->screenBuffer.memory = VirtualAlloc(0, render->screenBuffer.bytesPerPixel * width * height, MEM_COMMIT, PAGE_READWRITE);
	render->screenBuffer.width = width;
	render->screenBuffer.height = height;
}

void RenderClientArea(Render* render, WindowSize windowSize, HDC context)
{
	StretchDIBits(context, 0, 0, windowSize.width, windowSize.height, 0, 0, render->screenBuffer.width, render->screenBuffer.height, render->screenBuffer.memory, &(render->screenBuffer.info), DIB_RGB_COLORS, SRCCOPY);
}


void DrawPixel(Render* render, Color color, int32 x, int32 y)
{
	int32 yPitch = render->screenBuffer.pitch * y;
	int32 xPitch = sizeof(uint8) * 4 * x;
	uint8* row = (uint8*)render->screenBuffer.memory + yPitch + xPitch;
	if (x >= 0 && x < render->screenBuffer.width && y >= 0 && y < render->screenBuffer.height)
	{
		*row = color.blue;
		row++;
		*row = color.green;
		row++;
		*row = color.red;
		row++;
		*row = color.alpha;
	}
}

void DrawLine(Render* render, Color color, Vector source, Vector target)
{
	int32 yPitch = render->screenBuffer.pitch * source.y;
	uint8* row = (uint8*)render->screenBuffer.memory + yPitch;
	Vector direction = target - source;
	Vector nDirection = direction.Normalized();
	for (int index = 0; index < direction.Length(); index++)
	{
		Vector point = source + (nDirection * (real32) index);
		point.Convert2D();
		DrawPixel(render, color, point.x, point.y);
	}
}

void ClearScreen(Render* render)
{
	memset(render->screenBuffer.memory, 0, render->screenBuffer.width * render->screenBuffer.height * render->screenBuffer.bytesPerPixel);
}



void RenderGradient(Render* render, int32 xOffset, int32 yOffset)
{
	local Square squares[20];
	local bool created = false;
	local int32 test = 0;
	local int32 direction = 1;
	local int32 limit = 0;
	//ClearScreen(render);
	for (int32 i = 0; i < 20; i++)
	{
		if (!created)
		{
			Vector start = Vector(rand() % 2000,rand() % 1100, 1);
			int32 width = rand() % 100 + 50;
			int32 height = rand() % 100 + 50;
			squares[i] = Square(start, Vector(start.x + width, start.y + height, 1));
			squares[i].rotate(Vector(0, 0, 1), rand() % 90);
		}
		squares[i].rotate(Vector(render->screenBuffer.width / 4, render->screenBuffer.height / 3,1), test / 2);
		squares[i].rotate(squares[i].d, test / 2);
		Square square = squares[i].transform(Vector(0, 0, 0));
		DrawLine(render, Color(limit - 20, limit - 40, limit - 60, limit), square.a, square.c);
		DrawLine(render, Color(limit - 80, limit - 100, limit - 120, limit), square.c, square.b);
		DrawLine(render, Color(limit - 140, limit - 160, limit - 180, limit), square.b, square.d);
		DrawLine(render, Color(limit - 200, limit - 220, limit - 240, limit), square.d, square.a);
	}
	created = true;
	limit++;
	test += direction;
	if (test == 2)
	{
		direction = -1;
	}
	if (test == 0)
	{
		direction = 1;
	}

}
