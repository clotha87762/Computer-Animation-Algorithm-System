#ifndef _VECTOR_3D_H_
#define _VECTOR_3D_H_

#pragma once
#include <Math.h>
#include <iostream>
#include <assert.h>

class Vector3d
{
public:
    union {
        struct {
            double x, y, z;
        };
        double val[3];
    };

public:
    inline Vector3d()
		: x( 0 ), y( 0 ), z( 0 )
    {
    }

    inline Vector3d( const double fX, const double fY, const double fZ )
        : x( fX ), y( fY ), z( fZ )
    {
    }

    inline explicit Vector3d( const double afCoordinate[3] )
        : x( afCoordinate[0] ),
          y( afCoordinate[1] ),
          z( afCoordinate[2] )
    {
    }

    //inline explicit Vector3d( const int afCoordinate[3] )
    //{
    //    x = (double)afCoordinate[0];
    //    y = (double)afCoordinate[1];
    //    z = (double)afCoordinate[2];
    //}

    inline explicit Vector3d( double* const r )
        : x( r[0] ), y( r[1] ), z( r[2] )
    {
    }

    inline explicit Vector3d( const double scalar )
        : x( scalar )
        , y( scalar )
        , z( scalar )
    {
    }


    inline Vector3d( const Vector3d& rkVector )
        : x( rkVector.x ), y( rkVector.y ), z( rkVector.z )
    {
    }

	inline double operator [] ( const size_t i ) const
    {
        assert( i < 3 );

        return *(&x+i);
    }

	inline double& operator [] ( const size_t i )
    {
        assert( i < 3 );

        return *(&x+i);
    }

    /** Assigns the value of the other vector.
        @param
            rkVector The other vector
    */
    inline Vector3d& operator = ( const Vector3d& rkVector )
    {
        x = rkVector.x;
        y = rkVector.y;
        z = rkVector.z;

        return *this;
    }

    inline Vector3d& operator = ( const double fScalar )
    {
        x = fScalar;
        y = fScalar;
        z = fScalar;

        return *this;
    }

    inline bool operator == ( const Vector3d& rkVector ) const
    {
        return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
    }

    inline bool operator != ( const Vector3d& rkVector ) const
    {
        return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
    }

    // arithmetic operations
    inline Vector3d operator + ( const Vector3d& rkVector ) const
    {
        Vector3d kSum;

        kSum.x = x + rkVector.x;
        kSum.y = y + rkVector.y;
        kSum.z = z + rkVector.z;

        return kSum;
    }

    inline Vector3d operator - ( const Vector3d& rkVector ) const
    {
        Vector3d kDiff;

        kDiff.x = x - rkVector.x;
        kDiff.y = y - rkVector.y;
        kDiff.z = z - rkVector.z;

        return kDiff;
    }

    inline Vector3d operator * ( const double fScalar ) const
    {
        Vector3d kProd;

        kProd.x = fScalar*x;
        kProd.y = fScalar*y;
        kProd.z = fScalar*z;

        return kProd;
    }

    inline Vector3d operator * ( const Vector3d& rhs) const
    {
        Vector3d kProd;

        kProd.x = rhs.x * x;
        kProd.y = rhs.y * y;
        kProd.z = rhs.z * z;

        return kProd;
    }

    inline Vector3d operator / ( const double fScalar ) const
    {
        assert( fScalar != 0.0f );

        Vector3d kDiv;

        double fInv = 1.0f / fScalar;
        kDiv.x = x * fInv;
        kDiv.y = y * fInv;
        kDiv.z = z * fInv;

        return kDiv;
    }

    inline Vector3d operator / ( const Vector3d& rhs) const
    {
        Vector3d kDiv;

        kDiv.x = x / rhs.x;
        kDiv.y = y / rhs.y;
        kDiv.z = z / rhs.z;

        return kDiv;
    }


    inline Vector3d operator - () const
    {
        Vector3d kNeg;

        kNeg.x = -x;
        kNeg.y = -y;
        kNeg.z = -z;

        return kNeg;
    }

    // overloaded operators to help Vector3d
    inline friend Vector3d operator * ( const double fScalar, const Vector3d& rkVector )
    {
        Vector3d kProd;

        kProd.x = fScalar * rkVector.x;
        kProd.y = fScalar * rkVector.y;
        kProd.z = fScalar * rkVector.z;

        return kProd;
    }

    inline friend Vector3d operator + (const Vector3d& lhs, const double rhs)
    {
        Vector3d ret(rhs);
        return ret += lhs;
    }

    inline friend Vector3d operator + (const double lhs, const Vector3d& rhs)
    {
        Vector3d ret(lhs);
        return ret += rhs;
    }

    inline friend Vector3d operator - (const Vector3d& lhs, const double rhs)
    {
        return lhs - Vector3d(rhs);
    }

    inline friend Vector3d operator - (const double lhs, const Vector3d& rhs)
    {
        Vector3d ret(lhs);
        return ret -= rhs;
    }

    // arithmetic updates
    inline Vector3d& operator += ( const Vector3d& rkVector )
    {
        x += rkVector.x;
        y += rkVector.y;
        z += rkVector.z;

        return *this;
    }

    inline Vector3d& operator += ( const double fScalar )
    {
        x += fScalar;
        y += fScalar;
        z += fScalar;
        return *this;
    }

    inline Vector3d& operator -= ( const Vector3d& rkVector )
    {
        x -= rkVector.x;
        y -= rkVector.y;
        z -= rkVector.z;

        return *this;
    }

    inline Vector3d& operator -= ( const double fScalar )
    {
        x -= fScalar;
        y -= fScalar;
        z -= fScalar;
        return *this;
    }

    inline Vector3d& operator *= ( const double fScalar )
    {
        x *= fScalar;
        y *= fScalar;
        z *= fScalar;
        return *this;
    }

    inline Vector3d& operator *= ( const Vector3d& rkVector )
    {
        x *= rkVector.x;
        y *= rkVector.y;
        z *= rkVector.z;

        return *this;
    }

    inline Vector3d& operator /= ( const double fScalar )
    {
        assert( fScalar != 0.0f );

        double fInv = 1.0f / fScalar;

        x *= fInv;
        y *= fInv;
        z *= fInv;

        return *this;
    }

    inline Vector3d& operator /= ( const Vector3d& rkVector )
    {
        x /= rkVector.x;
        y /= rkVector.y;
        z /= rkVector.z;

        return *this;
    }


    /** Returns the length (magnitude) of the vector.
        @warning
            This operation requires a square root and is expensive in
            terms of CPU operations. If you don't need to know the exact
            length (e.g. for just comparing lengths) use squaredLength()
            instead.
    */
    inline double Length () const
    {
        return sqrt( x * x + y * y + z * z );
    }
    inline double Magnitude () const
    {
        return sqrt( x * x + y * y + z * z );
    }
    /** Returns the square of the length(magnitude) of the vector.
        @remarks
            This  method is for efficiency - calculating the actual
            length of a vector requires a square root, which is expensive
            in terms of the operations required. This method returns the
            square of the length of the vector, i.e. the same as the
            length but before the square root is taken. Use this if you
            want to find the longest / shortest vector without incurring
            the square root.
    */
    inline double SquaredLength () const
    {
        return x * x + y * y + z * z;
    }

    /** Calculates the dot (scalar) product of this vector with another.
        @remarks
            The dot product can be used to calculate the angle between 2
            vectors. If both are unit vectors, the dot product is the
            cosine of the angle; otherwise the dot product must be
            divided by the product of the lengths of both vectors to get
            the cosine of the angle. This result can further be used to
            calculate the distance of a point from a plane.
        @param
            vec Vector with which to calculate the dot product (together
            with this one).
        @returns
            A double representing the dot product value.
    */
    inline double DotProduct(const Vector3d& vec) const
    {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    inline double AngleBetween(const Vector3d& vec) const
    {
		double dot = DotProduct(vec);
		double len = Length() + vec.Length();
		double cos = dot / len;
		double angle = acos(cos);
        return angle;
    }

    /** Normalizes the vector.
        @remarks
            This method Normalizes the vector such that it's
            length / magnitude is 1. The result is called a unit vector.
        @note
            This function will not crash for zero-sized vectors, but there
            will be no changes made to their components.
        @returns The previous length of the vector.
    */
    inline double Normalize()
    {
        double fLength = sqrt( x * x + y * y + z * z );

        // Will also work for zero-sized vectors, but will change nothing
        if ( fLength > 1e-08 )
        {
            double fInvLength = 1.0f / fLength;
            x *= fInvLength;
            y *= fInvLength;
            z *= fInvLength;
        }

        return fLength;
    }

	inline Vector3d RotatedCopy(double angle, const Vector3d &axis)
	{
		Vector3d ret = *this;
		ret.Rotate(angle, axis);
		return ret;
	}

	inline void Rotate(double angle, const Vector3d &axisP)
	{
		//
		//  Compute the length of the rotation axis.
		//
		Vector3d axis = axisP;
		if(axis.SquaredLength() != 1)
			axis.Normalize();

		//
		//  Compute the dot product of the vector and the rotation axis.
		//
		double dot = DotProduct(axis);
		//
		//  Compute the parallel component of the vector.
		//
		Vector3d xp = dot * axis;
		//
		//  Compute the normal component of the vector.
		//
		Vector3d xn = *this - xp;
		double normn = xn.Length();
		xn.Normalize();

		//
		//  Compute a second vector, lying in the plane, perpendicular
		//  to (X1,Y1,Z1), and forming a right-handed system...
		//
		Vector3d xn2 = axis.CrossProduct(*this);
		xn2.Normalize();

		//
		//  Rotate the normal component by the angle.
		//
		Vector3d xr = normn * (cos(angle) * xn + sin(angle) * xn2);
		//
		//  The rotated vector is the parallel component plus the rotated
		//  component.
		//
		*this = xp + xr;
	}

    /** Calculates the cross-product of 2 vectors, i.e. the vector that
        lies perpendicular to them both.
        @remarks
            The cross-product is normally used to calculate the normal
            vector of a plane, by calculating the cross-product of 2
            non-equivalent vectors which lie on the plane (e.g. 2 edges
            of a triangle).
        @param
            vec Vector which, together with this one, will be used to
            calculate the cross-product.
        @returns
            A vector which is the result of the cross-product. This
            vector will <b>NOT</b> be Normalized, to maximise efficiency
            - call Vector3d::Normalize on the result if you wish this to
            be done. As for which side the resultant vector will be on, the
            returned vector will be on the side from which the arc from 'this'
            to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z)
            = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
			This is because OGRE uses a right-handed coordinate system.
        @par
            For a clearer explanation, look a the left and the bottom edges
            of your monitor's screen. Assume that the first vector is the
            left edge and the second vector is the bottom edge, both of
            them starting from the lower-left corner of the screen. The
            resulting vector is going to be perpendicular to both of them
            and will go <i>inside</i> the screen, towards the cathode tube
            (assuming you're using a CRT monitor, of course).
    */
    inline Vector3d CrossProduct( const Vector3d& rkVector ) const
    {
        Vector3d kCross;

        kCross.x = y * rkVector.z - z * rkVector.y;
        kCross.y = z * rkVector.x - x * rkVector.z;
        kCross.z = x * rkVector.y - y * rkVector.x;

        return kCross;
    }

    /** Returns true if the vector's scalar components are all greater
        that the ones of the vector it is compared against.
    */
    inline bool operator < ( const Vector3d& rhs ) const
    {
        if( x < rhs.x && y < rhs.y && z < rhs.z )
            return true;
        return false;
    }

    /** Returns true if the vector's scalar components are all smaller
        that the ones of the vector it is compared against.
    */
    inline bool operator > ( const Vector3d& rhs ) const
    {
        if( x > rhs.x && y > rhs.y && z > rhs.z )
            return true;
        return false;
    }

    /** As Normalize, except that this vector is unaffected and the
        Normalized vector is returned as a copy. */
    inline Vector3d NormalizedCopy(void) const
    {
        Vector3d ret = *this;
        ret.Normalize();
        return ret;
    }

    /** Calculates a reflection vector to the plane with the given normal .
    @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
    */
    inline Vector3d Reflect(const Vector3d& normal) const
    {
        return Vector3d( *this - ( 2 * this->DotProduct(normal) * normal ) );
    }
	
    static const Vector3d ZERO;
    static const Vector3d UNIT_X;
    static const Vector3d UNIT_Y;
    static const Vector3d UNIT_Z;
};

std::ostream &operator<<(
    std::ostream &a_kOstream,
    const Vector3d &a_kInput
    );

#endif