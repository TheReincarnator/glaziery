/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Vector::Vector() : x(0), y(0)
{
	ASSERTION_COBJECT(this);
}

Vector::Vector(int x, int y) : x(x), y(y)
{
	ASSERTION_COBJECT(this);
}

Vector::Vector(const Vector & vector) : x(vector.x), y(vector.y)
{
	ASSERTION_COBJECT(this);
}

Vector::Vector(const LongVector & vector) : x((int) vector.x), y((int) vector.y)
{
	ASSERTION_COBJECT(this);
}

Vector Vector::operator +() const
{
	ASSERTION_COBJECT(this);
	return Vector(*this);
}

Vector Vector::operator +(const Vector & vector) const
{
	ASSERTION_COBJECT(this);
	return Vector(x + vector.x, y + vector.y);
}

void Vector::operator +=(const Vector & vector)
{
	ASSERTION_COBJECT(this);

	x += vector.x;
	y += vector.y;
}

Vector Vector::operator -() const
{
	ASSERTION_COBJECT(this);
	return Vector(-x, -y);
}

Vector Vector::operator -(const Vector & vector) const
{
	ASSERTION_COBJECT(this);
	return Vector(x - vector.x, y - vector.y);
}

void Vector::operator -=(const Vector & vector)
{
	ASSERTION_COBJECT(this);

	x -= vector.x;
	y -= vector.y;
}

Vector Vector::operator *(int scale) const
{
	ASSERTION_COBJECT(this);
	return Vector(x * scale, y * scale);
}

Vector Vector::operator *(double scale) const
{
	ASSERTION_COBJECT(this);
	return Vector((int) (x * scale + 0.5), (int) (y * scale + 0.5));
}

void Vector::operator *=(int scale)
{
	ASSERTION_COBJECT(this);

	x *= scale;
	y *= scale;
}

void Vector::operator *=(double scale)
{
	ASSERTION_COBJECT(this);

	x = (int) (x * scale + 0.5);
	y = (int) (y * scale + 0.5);
}

Vector Vector::operator /(int scale) const
{
	ASSERTION_COBJECT(this);
	return Vector(x / scale, y / scale);
}

Vector Vector::operator /(double scale) const
{
	ASSERTION_COBJECT(this);
	return Vector((int) (x / scale + 0.5), (int) (y / scale + 0.5));
}

void Vector::operator /=(int scale)
{
	ASSERTION_COBJECT(this);

	x /= scale;
	y /= scale;
}

void Vector::operator /=(double scale)
{
	ASSERTION_COBJECT(this);

	x = (int) (x / scale + 0.5);
	y = (int) (y / scale + 0.5);
}

void Vector::operator =(const Vector & vector)
{
	ASSERTION_COBJECT(this);

	x = vector.x;
	y = vector.y;
}

bool Vector::operator ==(const Vector & vector) const
{
	ASSERTION_COBJECT(this);
	return x == vector.x && y == vector.y;
}

bool Vector::operator !=(const Vector & vector) const
{
	ASSERTION_COBJECT(this);
	return x != vector.x || y != vector.y;
}

bool Vector::operator >(const Vector & vector) const
{
	ASSERTION_COBJECT(this);
	return x > vector.x && y > vector.y;
}

bool Vector::operator >=(const Vector & vector) const
{
	ASSERTION_COBJECT(this);
	return x >= vector.x && y >= vector.y;
}

bool Vector::operator <(const Vector & vector) const
{
	ASSERTION_COBJECT(this);
	return x < vector.x && y < vector.y;
}

bool Vector::operator <=(const Vector & vector) const
{
	ASSERTION_COBJECT(this);
	return x <= vector.x && y <= vector.y;
}

Vector Vector::absolute() const
{
	ASSERTION_COBJECT(this);
	return Vector(abs(x), abs(y));
}

bool Vector::constrain(Vector from, Vector to)
{
	ASSERTION_COBJECT(this);

	if (from.x > to.x)
	{
		int buffer = from.x;
		from.x = to.x;
		to.x = buffer;
	}

	if (from.y > to.y)
	{
		int buffer = from.y;
		from.y = to.y;
		to.y = buffer;
	}

	bool changed = false;

	if (x > to.x)
	{
		x = to.x;
		changed = true;
	}

	if (y > to.y)
	{
		y = to.y;
		changed = true;
	}

	if (x < from.x)
	{
		x = from.x;
		changed = true;
	}

	if (y < from.y)
	{
		y = from.y;
		changed = true;
	}

	return changed;
}

double Vector::getLength()
{
	ASSERTION_COBJECT(this);
	return sqrt((double) (x*x + y*y));
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Vector, CObject);
#endif

bool Vector::isZero() const
{
	ASSERTION_COBJECT(this);
	return x == 0 && y == 0;
}

String Vector::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Vector(%d,%d)", x, y);
	return string;
}
