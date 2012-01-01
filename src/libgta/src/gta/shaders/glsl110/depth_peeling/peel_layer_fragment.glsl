/*
	Copyright 2010-2012 David "Alemarius Nexus" Lerch

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

vec4 ShadeFragment();


uniform sampler2D PeelDepthTex;
uniform sampler2D OpaqueDepthTex;
uniform ivec2 TexDimensions;


void main()
{
	vec2 texCoord = vec2(gl_FragCoord.x / float(TexDimensions.x), gl_FragCoord.y / float(TexDimensions.y));

	float opaqueDepth = texture2D(OpaqueDepthTex, texCoord).r;
	
	if (gl_FragCoord.z >= opaqueDepth+0.00002) {
		discard;
	}
	
	float peelDepth = texture2D(PeelDepthTex, texCoord).r;
	
	if (gl_FragCoord.z <= peelDepth+0.00002) {
		discard;
	}
	
	gl_FragColor = ShadeFragment();
}