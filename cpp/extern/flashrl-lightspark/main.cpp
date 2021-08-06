/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2008-2013  Alessandro Pignotti (a.pignotti@sssup.it)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#ifdef __MINGW32__
    #ifndef PATH_MAX
    #define PATH_MAX _MAX_PATH
    #endif
    #define realpath(N,R) _fullpath((R),(N),_MAX_PATH)
#endif


#ifdef _WIN32
const char* localedir = getExectuablePath();
#else
const char* localedir = "/usr/share/locale";
#endif
#include <fstream>
#include <filesystem>



int main([[maybe_unused]] int argc, char* argv[])
{











}
