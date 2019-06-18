/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_LONGVECTOR_H
#define __GLAZIERY_LONGVECTOR_H


namespace glaziery
{
	/**
	 * The vector pairs an X and a Y long value.
	 * This is helpful for defining positions and sizes.
	 * <b>Note:</b> This is a special-case vector to handle with big values.
	 * Usually, you work with Vector instances.
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class LongVector : public CObject
	#else
		class LongVector
	#endif
	{
		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(LongVector);
			#endif

		public:

			/**
			 * The X value.
			 */
			long x;

			/**
			 * The Y value.
			 */
			long y;

			/**
			 * Creates a new long vector for the coordinates (0, 0).
			 */
			LongVector();

			/**
			 * Creates a new long vector for given coordinates.
			 * @param x The X value.
			 * @param y The Y value.
			 */
			LongVector(long x, long y);

			/**
			 * Transformation constructor: Creates a new long vector from a given regular vector.
			 * @param vector The vector template.
			 */
			LongVector(const Vector & vector);

			/**
			 * Copy constructor: Creates a new vector from a given one.
			 * @param vector The vector template.
			 */
			LongVector(const LongVector & vector);

			/**
			 * Returns this vector.
			 * @return This vector.
			 */
			LongVector operator +() const;

			/**
			 * Returns the sum of this vector and another one.
			 * @param vector The other vector.
			 * @return The sum.
			 */
			LongVector operator +(const LongVector & vector) const;

			/**
			 * Adds another vector into this one.
			 * @param vector The other vector.
			 */
			void operator +=(const LongVector & vector);

			/**
			 * Returns the negation of this vector.
			 * @return The negation of this vector.
			 */
			LongVector operator -() const;

			/**
			 * Returns the difference of this vector and another one.
			 * @param vector The other vector.
			 * @return The difference.
			 */
			LongVector operator -(const LongVector & vector) const;

			/**
			 * Subtracts another vector from this one (into this one).
			 * @param vector The other vector.
			 */
			void operator -=(const LongVector & vector);

			/**
			 * Returns the scalar product of this vector and a scalar value.
			 * @param scale The scalar vector.
			 * @return The product.
			 */
			LongVector operator *(long scale) const;

			/**
			 * Returns the scalar product of this vector and a scalar value.
			 * @param scale The scalar vector.
			 * @return The product.
			 */
			LongVector operator *(double scale) const;

			/**
			 * Sets the scalar product of this vector and a scalar value into this vector.
			 * @param scale The scalar vector.
			 */
			void operator *=(long scale);

			/**
			 * Sets the scalar product of this vector and a scalar value into this vector.
			 * @param scale The scalar vector.
			 */
			void operator *=(double scale);

			/**
			 * Returns the scalar quotient of this vector and a scalar value.
			 * @param scale The scalar vector.
			 * @return The quotient.
			 */
			LongVector operator /(long scale) const;

			/**
			 * Returns the scalar quotient of this vector and a scalar value.
			 * @param scale The scalar vector.
			 * @return The quotient.
			 */
			LongVector operator /(double scale) const;

			/**
			 * Sets the scalar quotient of this vector and a scalar value into this vector.
			 * @param scale The scalar vector.
			 */
			void operator /=(long scale);

			/**
			 * Sets the scalar quotient of this vector and a scalar value into this vector.
			 * @param scale The scalar vector.
			 */
			void operator /=(double scale);

			/**
			 * Sets another vector's value into this vector.
			 * @param vector The vector to be set.
			 */
			void operator =(const LongVector & vector);

			/**
			 * Returns whether the X and Y value of another vector equals with one's.
			 * @param vector The other vector.
			 * @return Whether both vectors are equal.
			 */
			bool operator ==(const LongVector & vector) const;

			/**
			 * Returns whether the X or Y value of another vector differs from one's.
			 * @param vector The other vector.
			 * @return Whether both vectors are not equal.
			 */
			bool operator !=(const LongVector & vector) const;

			/**
			 * Returns whether the X and Y value of this vector
			 * are larger than those of another vector.
			 * @param vector The other vector.
			 * @return Whether both coordinates of this vector are larger.
			 */
			bool operator >(const LongVector & vector) const;

			/**
			 * Returns whether the X and Y value of this vector
			 * are larger than or equal to those of another vector.
			 * @param vector The other vector.
			 * @return Whether both coordinates of this vector are larger or equal.
			 */
			bool operator >=(const LongVector & vector) const;

			/**
			 * Returns whether the X and Y value of this vector
			 * are smaller than those of another vector.
			 * @param vector The other vector.
			 * @return Whether both coordinates of this vector are smaller.
			 */
			bool operator <(const LongVector & vector) const;

			/**
			 * Returns whether the X and Y value of this vector
			 * are smaller than or equal to those of another vector.
			 * @param vector The other vector.
			 * @return Whether both coordinates of this vector are smaller or equal.
			 */
			bool operator <=(const LongVector & vector) const;

			/**
			 * Returns a vector with absolute X and Y values.
			 * @return A vector with absolute X and Y values.
			 */
			LongVector absolute() const;

			/**
			 * Constrains the vector to a given range. The from and to parameters to not need to be ordered.
			 * @param from One corner of the allowed range, including the value itself.
			 * @param to One corner of the allowed range, including the value itself.
			 * @return Whether the vector was adjusted to lay in the range.
			 */
			bool constrain(LongVector from, LongVector to);

			/**
			 * Returns the length of the vector.
			 * @return The length of the vector.
			 */
			double getLength();

			/**
			 * Returns whether both X and Y are zero.
			 * @return Whether both coordinates are zero.
			 */
			bool isZero() const;

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			String toString();
	};
}


#endif
