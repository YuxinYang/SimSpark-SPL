/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: planecollider.cpp 108 2009-11-25 10:20:10Z a-held $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <oxygen/physicsserver/ode/odeplanecollider.h>

using namespace oxygen;
using namespace salt;

ODEPlaneCollider::ODEPlaneCollider() : ODECollider()
{
}

void ODEPlaneCollider::SetPlaneParams(float a, float b, float c, float d)
{
    dGeomPlaneSetParams(mODEGeom, a, b, c, d);
}

void ODEPlaneCollider::CreatePlane()
{
    // a plane with normal pointing up, going through the origin
    mODEGeom = dCreatePlane(0, 0, 1, 0, 0);
    mGeomID = (long) mODEGeom;
}

void ODEPlaneCollider::SetParams(const salt::Vector3f& pos, salt::Vector3f normal)
{
    normal.Normalize();
    float d = pos.Dot(normal);
    dGeomPlaneSetParams(mODEGeom, normal.x(), normal.y(), normal.z(), d);
}

float ODEPlaneCollider::GetPointDepth(const Vector3f& pos)
{
    return dGeomPlanePointDepth
        (mODEGeom,pos[0],pos[1],pos[2]);
}

long ODEPlaneCollider::GetGeomID(){
    return mGeomID;
}