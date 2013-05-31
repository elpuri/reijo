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

#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <QVector4D>

#define MIN3(a, b, c) a < b ? (a < c ? a : c) : (b < c ? b : c)

class MathUtils
{
public:
    static bool solveQuadratic(const double a, const double b, const double c,
                               double &t1, double &t2);
    static int solveCubic(double coeffs[], double solutions[]);
    static int solveQuartic(double coeffs[], double solutions[]);
    static QVector4D reflect(const QVector4D& incident, const QVector4D& normal);
    static bool refract(const QVector4D& incident, const QVector4D& normal,
                        const float nOutside, const float nInside, QVector4D& refracted, bool fromInside);
    static const double dEpsilon;
    static const float fEpsilon;
    static const QVector4D v4Epsilon;
    static float randomf();

private:
    MathUtils();
};

#endif // MATHUTILS_H
