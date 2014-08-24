/*
-- Copyright (c) 2013, Juha Turunen (turunen@iki.fi)
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions are met:
--
-- 1. Redistributions of source code must retain the above copyright notice, this
--    list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright notice,
--    this list of conditions and the following disclaimer in the documentation
--    and/or other materials provided with the distribution.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
-- ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
-- WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
-- DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
-- ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
-- (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
-- LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
-- ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
-- (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
-- SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "mathutils.h"
#include <math.h>
#include <stdlib.h>
#include <QDebug>

MathUtils::MathUtils()
{
}

const double MathUtils::dEpsilon = 3e-5;
const float MathUtils::fEpsilon = 3e-5f;
const QVector4D MathUtils::v4Epsilon = QVector4D(MathUtils::dEpsilon, MathUtils::dEpsilon, MathUtils::dEpsilon, 0.0);

bool MathUtils::solveQuadratic(const double a, const double b, const double c, double &t1, double &t2)
{
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false;

    double root = sqrt(discriminant);
    t1 = (-b + root) / (2 * a);
    t2 = (-b - root) / (2 * a);
    return true;
}

QVector4D MathUtils::reflect(const QVector4D &incident, const QVector4D& normal)
{
    return incident - 2 * QVector4D::dotProduct(incident, normal) * normal;
}

bool MathUtils::refract(const QVector4D &incident, const QVector4D &normal,
                             const float nOutside, const float nInside, QVector4D &refracted, bool fromInside)
{
    double n = nInside / nOutside;
    double nInv = 1.0 / n;
    double cosPhiI = -QVector4D::dotProduct(incident, normal);

    double cosPhiISq = cosPhiI * cosPhiI;
    double nInvSq = nInv * nInv;
    double cosPhiT = 1.0 - (nInvSq * (1.0 - cosPhiISq));
    if (cosPhiT <= 0.0)
        return true;       // total internal reflection
    cosPhiT = sqrt(cosPhiT);
    refracted = nInv * incident - (cosPhiT - nInv * cosPhiI) * normal;

    return false;
}

float MathUtils::randomf()
{
    return (float) random() / (float) RAND_MAX;
}




/*
 *  Roots3And4.c
 *
 *  Utility functions to find cubic and quartic roots,
 *  coefficients are passed like this:
 *
 *      c[0] + c[1]*x + c[2]*x^2 + c[3]*x^3 + c[4]*x^4 = 0
 *
 *  The functions return the number of non-complex roots and
 *  put the values into the s array.
 *
 *  Author:         Jochen Schwarze (schwarze@isa.de)
 *
 *  Jan 26, 1990    Version for Graphics Gems
 *  Oct 11, 1990    Fixed sign problem for negative q's in SolveQuartic
 *  	    	    		(reported by Mark Podlipec),
 *  	    	    		Old-style function definitions,
 *  	    	    		IsZero() as a macro
 *  Nov 23, 1990  Some systems do not declare acos() and cbrt() in
 *                <math.h>, though the functions exist in the library.
 *                If large coefficients are used, EQN_EPS should be
 *                reduced considerably (e.g. to 1E-30), results will be
 *                correct but multiple roots might be reported more
 *                than once.
 */

 
#ifndef M_PI
#define M_PI PI
#endif

// You may have to experiment with EQN_EPS
// The original was 1e-9, but I use 1e-90  KS Dec 3, 2007

//#define     EQN_EPS     1e-9  
//#define     EQN_EPS     1e-30
//#define     EQN_EPS     1e-60
#define     EQN_EPS     1e-90

#define	IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)

#ifndef CBRT
#define     cbrt(x)  ((x) > 0.0 ? pow((double)(x), 1.0/3.0) : \
			  		 ((x) < 0.0 ? -pow((double)-(x), 1.0/3.0) : 0.0))
#endif



int MathUtils::solveCubic(double c[4], double s[3]) {
    int     i, num;
    double  sub;
    double  A, B, C;
    double  sq_A, p, q;
    double  cb_p, D;

    /* normal form: x^3 + Ax^2 + Bx + C = 0 */

    A = c[ 2 ] / c[ 3 ];
    B = c[ 1 ] / c[ 3 ];
    C = c[ 0 ] / c[ 3 ];

    /*  substitute x = y - A/3 to eliminate quadric term:
	x^3 +px + q = 0 */

    sq_A = A * A;
    p = 1.0/3 * (- 1.0/3 * sq_A + B);
    q = 1.0/2 * (2.0/27 * A * sq_A - 1.0/3 * A * B + C);

    /* use Cardano's formula */

    cb_p = p * p * p;
    D = q * q + cb_p;

    if (IsZero(D)) {
		if (IsZero(q)) { /* one triple solution */
		    s[ 0 ] = 0;
		    num = 1;
		}
	else { /* one single and one double solution */
	    double u = cbrt(-q);
	    s[ 0 ] = 2 * u;
	    s[ 1 ] = - u;
	    num = 2;
	}
    }
    else if (D < 0) { /* Casus irreducibilis: three real solutions */
		double phi = 1.0/3 * acos(-q / sqrt(-cb_p));
		double t = 2 * sqrt(-p);

		s[ 0 ] =   t * cos(phi);
		s[ 1 ] = - t * cos(phi + M_PI / 3);
		s[ 2 ] = - t * cos(phi - M_PI / 3);
		num = 3;
    }
    else { /* one real solution */
		double sqrt_D = sqrt(D);
		double u = cbrt(sqrt_D - q);
		double v = - cbrt(sqrt_D + q);

		s[ 0 ] = u + v;
		num = 1;
    }

    /* resubstitute */

    sub = 1.0/3 * A;

    for (i = 0; i < num; ++i)
	s[ i ] -= sub;

    return num;
}


int
SolveQuadric(double c[3], double s[2]) {
    double p, q, D;

    /* normal form: x^2 + px + q = 0 */

    p = c[ 1 ] / (2 * c[ 2 ]);
    q = c[ 0 ] / c[ 2 ];

    D = p * p - q;

    if (IsZero(D)) {
    s[ 0 ] = - p;
    return 1;
    }
    else if (D > 0) {
    double sqrt_D = sqrt(D);

    s[ 0 ] =   sqrt_D - p;
    s[ 1 ] = - sqrt_D - p;
    return 2;
    }
    else /* if (D < 0) */
        return 0;
}


int MathUtils::solveQuartic(double c[5], double s[4]) {
    double  coeffs[4];
    double  z, u, v, sub;
    double  A, B, C, D;
    double  sq_A, p, q, r;
    int     i, num;

    /* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */

    A = c[ 3 ] / c[ 4 ];
    B = c[ 2 ] / c[ 4 ];
    C = c[ 1 ] / c[ 4 ];
    D = c[ 0 ] / c[ 4 ];

    /*  substitute x = y - A/4 to eliminate cubic term:
	x^4 + px^2 + qx + r = 0 */

    sq_A = A * A;
    p = - 3.0/8 * sq_A + B;
    q = 1.0/8 * sq_A * A - 1.0/2 * A * B + C;
    r = - 3.0/256*sq_A*sq_A + 1.0/16*sq_A*B - 1.0/4*A*C + D;

    if (IsZero(r)) {
		/* no absolute term: y(y^3 + py + q) = 0 */

		coeffs[ 0 ] = q;
		coeffs[ 1 ] = p;
		coeffs[ 2 ] = 0;
		coeffs[ 3 ] = 1;

        num = MathUtils::solveCubic(coeffs, s);

		s[ num++ ] = 0;
    }
    else {
		/* solve the resolvent cubic ... */

		coeffs[ 0 ] = 1.0/2 * r * p - 1.0/8 * q * q;
		coeffs[ 1 ] = - r;
		coeffs[ 2 ] = - 1.0/2 * p;
		coeffs[ 3 ] = 1;
		
        (void) MathUtils::solveCubic(coeffs, s);

		/* ... and take the one real solution ... */

		z = s[ 0 ];

		/* ... to build two quadric equations */

		u = z * z - r;
		v = 2 * z - p;

		if (IsZero(u))
		    u = 0;
		else if (u > 0)
		    u = sqrt(u);
		else
		    return 0;

		if (IsZero(v))
		    v = 0;
		else if (v > 0)
		    v = sqrt(v);
		else
		    return 0;

		coeffs[ 0 ] = z - u;
		coeffs[ 1 ] = q < 0 ? -v : v;
		coeffs[ 2 ] = 1;

		num = SolveQuadric(coeffs, s);

		coeffs[ 0 ]= z + u;
		coeffs[ 1 ] = q < 0 ? v : -v;
		coeffs[ 2 ] = 1;

		num += SolveQuadric(coeffs, s + num);
	}

    /* resubstitute */

    sub = 1.0/4 * A;

    for (i = 0; i < num; ++i)
		s[ i ] -= sub;

    return num;
}
