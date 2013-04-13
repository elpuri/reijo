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

MathUtils::MathUtils()
{
}

const double MathUtils::dEpsilon = 1e-5d;
const float MathUtils::fEpsilon = 1e-5f;

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

QVector4D MathUtils::reflect(const QVector4D &incident, const QVector4D normal)
{
    return incident - 2 * QVector4D::dotProduct(incident, normal) * normal;
}

QVector4D MathUtils::refract(const QVector4D &incident, const float n1, const float n2)
{

}

float MathUtils::randomf()
{
    return (float) random() / (float) RAND_MAX;
}
