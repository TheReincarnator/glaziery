/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


LongVector::LongVector() : x(0), y(0)
{
	ASSERTION_COBJECT(this);
}

LongVector::LongVector(long x, long y) : x(x), y(y)
{
	ASSERTION_COBJECT(this);
}

LongVector::LongVector(const LongVector & vector) : x(vector.x), y(vector.y)
{
	ASSERTION_COBJECT(this);
}

LongVector::LongVector(const Vector & vector) : x(vector.x), y(vector.y)
{
	ASSERTION_COBJECT(this);
}

LongVector LongVector::operator +() const
{
	ASSERTION_COBJECT(this);
	return LongVector(*this);
}

LongVector LongVector::operator +(const LongVector & vector) const
{
	ASSERTION_COBJECT(this);
	return LongVector(x + vector.x, y + vector.y);
}

void LongVector::operator +=(const LongVector & vector)
{
	ASSERTION_COBJECT(this);

	x += vector.x;
	y += vector.y;
}

LongVector LongVector::operator -() const
{
	ASSERTION_COBJECT(this);
	return LongVector(-x, -y);
}

LongVector LongVector::operator -(const LongVector & vector) const
{
	ASSERTION_COBJECT(this);
	return LongVector(x - vector.x, y - vector.y);
}

void LongVector::operator -=(const LongVector & vector)
{
	ASSERTION_COBJECT(this);

	x -= vector.x;
	y -= vector.y;
}

LongVector LongVector::operator *(long scale) const
{
	ASSERTION_COBJECT(this);
	return LongVector(x * scale, y * scale);
}

LongVector LongVector::operator *(double scale) const
{
	ASSERTION_COBJECT(this);
	return LongVector((long) (x * scale + 0.5), (long) (y * scale + 0.5));
}

void LongVector::operator *=(long scale)
{
	ASSERTION_COBJECT(this);

	x *= scale;
	y *= scale;
}

void LongVector::operator *=(double scale)
{
	ASSERTION_COBJECT(this);

	x = (long) (x * scale + 0.5);
	y = (long) (y * scale + 0.5);
}

LongVector LongVector::operator /(long scale) const
{
	ASSERTION_COBJECT(this);
	return LongVector(x / scale, y / scale);
}

LongVector LongVector::operator /(double scale) const
{
	ASSERTION_COBJECT(this);
	return LongVector((long) (x / scale + 0.5), (long) (y / scale + 0.5));
}

void LongVector::operator /=(long scale)
{
	ASSERTION_COBJECT(this);

	x /= scale;
	y /= scale;
}

void LongVector::operator /=(double scale)
{
	ASSERTION_COBJECT(this);

	x = (long) (x / scale + 0.5);
	y = (long) (y / scale + 0.5);
}

void LongVector::operator =(const LongVector & vector)
{
	ASSERTION_COBJECT(this);

	x = vector.x;
	y = vector.y;
}

bool LongVector::operator ==(const LongVector & vector) const
{
	ASSERTION_COBJECT(this);
	return x == vector.x && y == vector.y;
}

bool LongVector::operator !=(const LongVector & vector) const
{
	ASSERTION_COBJECT(this);
	return x != vector.x || y != vector.y;
}

bool LongVector::operator >(const LongVector & vector) const
{
	ASSERTION_COBJECT(this);
	return x > vector.x && y > vector.y;
}

bool LongVector::operator >=(const LongVector & vector) const
{
	ASSERTION_COBJECT(this);
	return x >= vector.x && y >= vector.y;
}

bool LongVector::operator <(const LongVector & vector) const
{
	ASSERTION_COBJECT(this);
	return x < vector.x && y < vector.y;
}

bool LongVector::operator <=(const LongVector & vector) const
{
	ASSERTION_COBJECT(this);
	return x <= vector.x && y <= vector.y;
}

LongVector LongVector::absolute() const
{
	ASSERTION_COBJECT(this);
	return LongVector(abs(x), abs(y));
}

bool LongVector::constrain(LongVector from, LongVector to)
{
	ASSERTION_COBJECT(this);

	if (from.x > to.x)
	{
		long buffer = from.x;
		from.x = to.x;
		to.x = buffer;
	}

	if (from.y > to.y)
	{
		long buffer = from.y;
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

double LongVector::getLength()
{
	ASSERTION_COBJECT(this);
	return sqrt((double) (x*x + y*y));
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(LongVector, CObject);
#endif

bool LongVector::isZero() const
{
	ASSERTION_COBJECT(this);
	return x == 0 && y == 0;
}

String LongVector::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("LongVector(%d,%d)", x, y);
	return string;
}
