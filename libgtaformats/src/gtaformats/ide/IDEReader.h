/*
	Copyright 2010 David Lerch

	This file is part of gtaformats.

	gtaformats is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	gtaformats is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with gtaformats.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IDEREADER_H_
#define IDEREADER_H_

#include "../gf_config.h"
#include <istream>
#include <cstring>
#include <cstdlib>
#include "IDEEntity.h"

using std::istream;


class IDEReader
{
private:
	enum SectionType {
		NONE,
		OBJS
	};

public:
	IDEReader(istream* stream, bool deleteStream = true);
	IDEReader(const char* filename);
	~IDEReader();
	IDEEntity* readEntity();

private:
	char* nextString(char* str = NULL) { return strtok(str, ","); }
	int32_t nextInt(char* str = NULL) { return atoi(nextString(str)); }
	float nextFloat(char* str = NULL) { return (float) atof(nextString(str)); }

private:
	istream* stream;
	SectionType currentSection;
	bool deleteStream;
};

#endif /* IDEREADER_H_ */
