/*
	Copyright 2010-2014 David "Alemarius Nexus" Lerch

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

	Additional permissions are granted, which are listed in the file
	GPLADDITIONS.
 */

uniform mat4 MVPMatrix;

ATTRIBUTE vec4 Vertex;
ATTRIBUTE vec4 Color;
ATTRIBUTE vec2 TexCoord;

VARYING vec4 FragColor;
VARYING vec2 FragTexCoord;


void main()
{
	gl_Position = MVPMatrix * Vertex;
	FragColor = Color;
	FragTexCoord = TexCoord;
}
