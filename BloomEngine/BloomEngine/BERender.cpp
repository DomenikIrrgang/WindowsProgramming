#include "BERender.h"
#include <math.h>

Render defaultRender;

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
}

void RenderClientArea(Render* render, WindowSize windowSize, HDC context)
{
	StretchDIBits(context, 0, 0, windowSize.width, windowSize.height, 0, 0, render->screenBuffer.width, render->screenBuffer.height, render->screenBuffer.memory, &(render->screenBuffer.info), DIB_RGB_COLORS, SRCCOPY);
}


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

	real32 length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	Vector normalized()
	{
		real32 length = this->length();
		return *this / length;
	}

	Vector convert2D()
	{
		return Vector(x / z, y / z);
	}

	Vector rotate(Vector angle, real32 degree)
	{
		real32 radian = degree * pi32 / 180.0f;
		Vector newVector = *this - angle;
		real32 newX = newVector.x * cosf(radian) - newVector.y * sinf(radian);
		real32 newY = newVector.y * cosf(radian) + newVector.x * sinf(radian);
		return Vector(newX, newY, 0);
	}
} Vector;

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

	Square(Vector a, Vector b, Vector c, Vector d)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		center = a + ((b - a) * 0.5f);
	}
	
	void rotate(Vector angle, real32 degree)
	{
		a = angle + a.rotate(angle, degree);
		b = angle + b.rotate(angle, degree);
		c = angle + c.rotate(angle, degree);
		d = angle + d.rotate(angle, degree);
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
		Square tmp = Square(a + position, b + position, c + position, d + position);
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

void drawPixel(Render* render, Color color, int32 x, int32 y)
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

void drawLine(Render* render, Color color, Vector source, Vector target)
{
	int32 yPitch = render->screenBuffer.pitch * source.y;
	uint8* row = (uint8*)render->screenBuffer.memory + yPitch;
	Vector direction = target - source;
	Vector nDirection = direction.normalized();
	for (int index = 0; index < direction.length(); index++)
	{
		Vector point = source + (nDirection * (real32) index);
		point.convert2D();
		drawPixel(render, color, point.x, point.y);
	}
}

void RenderGradient(Render* render, int32 xOffset, int32 yOffset)
{
	uint8* row = (uint8*) render->screenBuffer.memory;
	for (int32 y = 0; y < render->screenBuffer.height; y++)
	{
		uint8* pixel = (uint8*)row;
		for (int32 x = 0; x < render->screenBuffer.width; x++)
		{

			// BLUE
			*pixel = 0;
			++pixel;
			// GREEN
			*pixel = 0;
			++pixel;
			// RED
			*pixel = 0;
			++pixel;
			*pixel = 0;
			++pixel;
		}
		row += render->screenBuffer.pitch;
	}
	local Square squares[2];
	local bool created = false;
	local int32 test = 0;
	local int32 direction = 1;
	local int32 limit = 0;
	for (int32 i = 0; i < 1; i++)
	{
		if (!created)
		{
			Vector start = Vector(100,100, 1);
			int32 width = rand() % 100 + 50;
			int32 height = rand() % 100 + 50;
			squares[i] = Square(start, Vector(200, 200, 1));
			squares[i].rotate(Vector(0, 0, 1), 45);
		}
		squares[i].rotate(Vector(0,0,1), test);
		
		squares[i].rotate(squares[i].center, test * 2);
		Square square = squares[i].transform(Vector(0, 0, 0));
		drawLine(render, Color(limit - 20, limit - 40, limit - 60, limit), square.a, square.c);
		drawLine(render, Color(limit - 80, limit - 100, limit - 120, limit), square.c, square.b);
		drawLine(render, Color(limit - 140, limit - 160, limit - 180, limit), square.b, square.d);
		drawLine(render, Color(limit - 200, limit - 220, limit - 240, limit), square.d, square.a);
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
