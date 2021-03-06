/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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
#include "filesystemstd.h"
#include <salt/fileclasses.h>

using namespace boost;
using namespace salt;
using namespace std;

// constructor
FileSystemSTD::FileSystemSTD()
    : FileSystem()
{
}

// destructor
FileSystemSTD::~FileSystemSTD()
{
}

bool FileSystemSTD::SetPath(const string& inPath)
{
    // release memory for old path
    mPath = inPath;

    return true;
}

//
// This function is really simple. It appends inName to mPath and
// tries to open the combined name as a readonly file.
//
shared_ptr<salt::RFile> FileSystemSTD::Open(const string& inName)
{
    shared_ptr<salt::RFile> file(new StdFile());
    std::string fileName = mPath + inName;

    if(! file->Open(fileName.c_str()))
        {
            file.reset();
        }

    return file;
}

int FileSystemSTD::ForEachFile(const string& expression, TCallback callback,
                               void* param)
{
    // todo
    return 0;
}
