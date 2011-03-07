/*
	Copyright 2010-2011 David "Alemarius Nexus" Lerch

	This file is part of libgta.

	libgta is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	libgta is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with libgta.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GLException.h"
#include "gl.h"
#include <cstring>
#include <cstdio>



void GLException::checkError(const char* msg)
{
	GLenum error = glGetError();

	if (error != GL_NO_ERROR) {
#ifdef GTA_USE_OPENGL_ES
		char errname[32];

		switch (error) {
		case GL_INVALID_ENUM:
			sprintf(errname, "GL_INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:
			sprintf(errname, "GL_INVALID_VALUE");
			break;
		case GL_INVALID_OPERATION:
			sprintf(errname, "GL_INVALID_OPERATION");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			sprintf(errname, "GL_INVALID_FRAMEBUFFER_OPERATION");
			break;
		case GL_OUT_OF_MEMORY:
			sprintf(errname, "GL_OUT_OF_MEMORY");
			break;
		default:
			sprintf(errname, "[UNKNOWN]");
		}

		char errmsg[128];
		sprintf(errmsg, "OpenGL error %s (%u)", errname, error);
		throw GLException(errmsg, __FILE__, __LINE__);
#else
		const char* errstr = (const char*) gluErrorString(error);

		char* errmsg;

		if (msg) {
			errmsg = new char[strlen(errstr) + strlen(msg) + 64];
			strcpy(errmsg, msg);
			strcat(errmsg, ": ");
		} else {
			errmsg = new char[strlen(errstr) + 64];
			errmsg[0] = '\0';
		}

		strcat(errmsg, "OpenGL error: ");
		strcat(errmsg, errstr);

		GLException ex(errmsg, __FILE__, __LINE__);
		delete[] errmsg;
		throw ex;
#endif
	}
}
