/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_VECTOR_H
#define __GLAZIERY_VECTOR_H


namespace glaziery
{
	/**
	 * The vector pairs an X and a Y value.
	 * This is helpful for defining positions and sizes.
	 * <b>Note:</b> There is also a long version of the Vector,
	 * e.g. to accumulate values over a long time.
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class Vector : public CObject
	#else
		class Vector
	#endif
	{
		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Vector);
			#endif

		public:

			/**
			 * The X value.
			 */
			int x;

			/**
			 * The Y value.
			 */
			int y;

			/**
			 * Creates a new vector for the coordinates (0, 0).
			 */
			Vector();

			/**
			 * Creates a new vector for given coordinates.
			 * @param x The X value.
			 * @param y The Y value.
			 */
			Vector(int x, int y);

			/**
			 * Copy constructor: Creates a new vector from a given one.
			 * @param vector The vector template.
			 */
			Vector(const Vector & vector);

			/**
			 * Transformation constructor: Creates a new vector from a given long vector.
			 * @param vector The vector template.
			 */
			Vector(const LongVector & vector);

			/**
			 * Returns this vector.
			 * @return This vector.
			 */
			Vector operator +() const;

			/**
			 * Returns the sum of this vector and another one.
			 * @param vector The other vector.
			 * @return The sum.
			 */
			Vector operator +(const Vector & vector) const;

			/**
			 * Adds another vector into this one.
			 * @param vector The other vector.
			 */
			void operator +=(const Vector & vector);

			/**
			 * Returns the negation of this vector.
			 * @return The negation of this vector.
			 */
			Vector operator -() const;

			/**
			 * Returns the difference of this vector and another one.
			 * @param vector The other vector.
			 * @return The difference.
			 */
			Vector operator -(const Vector & vector) const;

			/**
			 * Subtracts another vector from this one (into this one).
			 * @param vector The other vector.
			 */
			void operator -=(const Vector & vector);

			/**
			 * Returns the scalar product of this vector and a scalar value.
			 * @param scale The scalar vector.
			 * @return The product.
			 */
			Vector operator *(int scale) const;

			/**
			 * Returns the scalar product of this vector and a scalar value.
			 * @param scale The scalar vector.
			 * @return The product.
			 */
			Vector operator *(double scale) const;

			/**
			 * Sets the scalar product of this vector and a scalar value into this vector.
			 * @param scale The scalar vector.
			 */
			void operator *=(int scale);

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
			Vector operator /(int scale) const;

			/**
			 * Returns the scalar quotient of this vector and a scalar value.
			 * @param scale The scalar vector.
			 * @return The quotient.
			 */
			Vector operator /(double scale) const;

			/**
			 * Sets the scalar quotient of this vector and a scalar value into this vector.
			 * @param scale The scalar vector.
			 */
			void operator /=(int scale);

			/**
			 * Sets the scalar quotient of this vector and a scalar value into this vector.
			 * @param scale The scalar vector.
			 */
			void operator /=(double scale);

			/**
			 * Sets another vector's value into this vector.
			 * @param vector The vector to be set.
			 */
			void operator =(const Vector & vector);

			/**
			 * Returns whether the X and Y value of another vector equals with one's.
			 * @param vector The other vector.
			 * @return Whether both vectors are equal.
			 */
			bool operator ==(const Vector & vector) const;

			/**
			 * Returns whether the X or Y value of another vector differs from one's.
			 * @param vector The other vector.
			 * @return Whether both vectors are not equal.
			 */
			bool operator !=(const Vector & vector) const;

			/**
			 * Returns whether the X and Y value of this vector
			 * are larger than those of another vector.
			 * @param vector The other vector.
			 * @return Whether both coordinates of this vector are larger.
			 */
			bool operator >(const Vector & vector) const;

			/**
			 * Returns whether the X and Y value of this vector
			 * are larger than or equal to those of another vector.
			 * @param vector The other vector.
			 * @return Whether both coordinates of this vector are larger or equal.
			 */
			bool operator >=(const Vector & vector) const;

			/**
			 * Returns whether the X and Y value of this vector
			 * are smaller than those of another vector.
			 * @param vector The other vector.
			 * @return Whether both coordinates of this vector are smaller.
			 */
			bool operator <(const Vector & vector) const;

			/**
			 * Returns whether the X and Y value of this vector
			 * are smaller than or equal to those of another vector.
			 * @param vector The other vector.
			 * @return Whether both coordinates of this vector are smaller or equal.
			 */
			bool operator <=(const Vector & vector) const;

			/**
			 * Returns a vector with absolute X and Y values.
			 * @return A vector with absolute X and Y values.
			 */
			Vector absolute() const;

			/**
			 * Constrains the vector to a given range. The from and to parameters to not need to be ordered.
			 * @param from One corner of the allowed range, including the value itself.
			 * @param to One corner of the allowed range, including the value itself.
			 * @return Whether the vector was adjusted to lay in the range.
			 */
			bool constrain(Vector from, Vector to);

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
