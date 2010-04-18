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

#include "IMGArchive.h"
#include "IMGException.h"
#include <string>
#include <cstring>
#include <vector>
#include "../gf_filetype.h"

using std::string;
using std::vector;
using std::ifstream;




IMGArchive::IMGArchive(istream* stream, bool randomAccess)
		: stream(stream), randomAccess(randomAccess), autoGeneratedStream(false)
{
	readHeader(stream);
}


IMGArchive::IMGArchive(const char* filename)
		: stream(NULL), randomAccess(true), autoGeneratedStream(true)
{
	string fname(filename);

	if (isValidDIRFilename(fname)) {
		string imgFile(fname);
		size_t index = imgFile.find_last_of('.');
		imgFile = imgFile.substr(0, index).append(".img");

		ifstream* dirStream = openStream(filename);

		if (!dirStream) {
			throw IMGException(IMGException::UnableToOpen, "Unable to open DIR file");
		}

		stream = openStream(imgFile.c_str());

		if (!stream) {
			char errmsg[2048];
			sprintf(errmsg, "Unable to open corresponding IMG file %s for the given DIR file",
					imgFile.c_str());
			throw IMGException(IMGException::UnableToOpen, errmsg);
		}

		readHeader(dirStream);

		dirStream->close();
		delete dirStream;
	} else if (isValidIMGFilename(fname)) {
		if (guessIMGVersion(filename) == VER2) {
			stream = openStream(filename);

			if (!stream) {
				throw IMGException(IMGException::UnableToOpen, "Unable to open VER2 IMG file");
			}

			readHeader(stream);
		} else {
			stream = openStream(filename);

			if (!stream) {
				throw IMGException(IMGException::UnableToOpen, "Unable to open VER1 IMG file");
			}

			string dirFile(fname);
			size_t index = dirFile.find_last_of('.');
			dirFile = dirFile.substr(0, index).append(".dir");

			ifstream* dirStream = openStream(dirFile.c_str());

			if (!dirStream) {
				char errmsg[2048];
				sprintf(errmsg, "Unable to open corresponding DIR file %s for the given IMG file",
						dirFile.c_str());
				throw IMGException(IMGException::UnableToOpen, errmsg);
			}

			readHeader(dirStream);
			dirStream->close();
			delete dirStream;
		}
	} else {
		throw IMGException(IMGException::UnknownFileType, "File name is neither an IMG nor a DIR file");
	}
}


IMGArchive::IMGArchive(istream* dirStream, istream* imgStream, bool randomAccess)
		: stream(imgStream), randomAccess(randomAccess), autoGeneratedStream(false)
{
	readHeader(dirStream);
}


IMGArchive::IMGArchive(const char* dirName, const char* imgName)
		: stream(NULL), randomAccess(true), autoGeneratedStream(true)
{
	stream = openStream(imgName);
	ifstream* dirStream = openStream(dirName);

	readHeader(dirStream);

	dirStream->close();
	delete dirStream;
}


IMGArchive::~IMGArchive()
{
	for (int32_t i = 0 ; i < numEntries ; i++) {
		delete entries[i];
	}

	delete[] entries;

	if (autoGeneratedStream  &&  stream) {
		((ifstream*) stream)->close();
		delete stream;
	}
}


bool IMGArchive::isValidDIRFilename(const std::string& filename)
{
	return GFGuessFileType(filename) == GF_TYPE_DIR;
}


bool IMGArchive::isValidIMGFilename(const std::string& filename)
{
	return GFGuessFileType(filename) == GF_TYPE_IMG;
}


IMGArchive::IMGVersion IMGArchive::guessIMGVersion(istream* stream, bool returnToStart)
{
	char fourCC[4];
	stream->read(fourCC, 4);

	if (stream->eof()) {
		return VER1;
	}

	if (returnToStart) {
		stream->seekg(-4, istream::cur);

		if (stream->fail()) {
			throw IMGException(IMGException::IOError, "Unable to return to file start");
		}
	}

	if (fourCC[0] == 'V'  &&  fourCC[1] == 'E'  &&  fourCC[2] == 'R'  &&  fourCC[3] == '2') {
		return VER2;
	} else {
		return VER1;
	}
}


IMGArchive::IMGVersion IMGArchive::guessIMGVersion(const char* filename)
{
	ifstream stream(filename, ifstream::in | ifstream::binary);

	if (stream.fail()) {
		throw IMGException(IMGException::UnableToOpen, "Unable to open file for guessing IMG version");
	}

	return guessIMGVersion(&stream, false);
}


istream* IMGArchive::gotoEntry(IMGEntry* entry) {
	long long start = entry->offset*IMG_BLOCK_SIZE;

	long long cpos;

	if (randomAccess) {
		cpos = stream->tellg();

		if (cpos == -1) {
			throw IMGException(IMGException::MayNotSeek, "Attempt to tellg() failed!");
		}
	} else {
		cpos = bytesRead;
	}

	if (start > bytesRead  &&  !randomAccess) {
		char skipBuffer[4096];
		SkipBytes(stream, start - cpos, skipBuffer, sizeof(skipBuffer));
		bytesRead = start;
	} else {
		if (randomAccess) {
			stream->seekg(start-cpos, istream::cur);

			if (stream->fail()) {
				throw IMGException(IMGException::IOError, "Could not reach entry");
			}
		} else {
			string msg = "Entry ";
			msg += entry->name;
			msg += " is before current position, but may not seek!";
			throw IMGException(IMGException::MayNotSeek, msg.c_str());
		}
	}

	return stream;
}

istream* IMGArchive::gotoEntry(const char* name) {
	IMGEntry* entry = getEntryByName(name);

	if (!entry) {
		return NULL;
	}

	return gotoEntry(entry);
}

void IMGArchive::visit(IMGVisitor* visitor, IMGEntry* entry) {
	void* udata = NULL;

	if (visitor->readHeader(entry, udata)) {
		gotoEntry(entry);
		visitor->readEntry(entry, stream, udata);
	}
}

void IMGArchive::visitAll(IMGVisitor* visitor) {
	for (int i = 0 ; i < numEntries ; i++) {
		visit(visitor, entries[i]);
	}
}

IMGEntry* IMGArchive::getEntryByName(const char* name) {
	for (int32_t i = 0 ; i < numEntries ; i++) {
		if (strcmp(entries[i]->name, name) == 0) {
			return entries[i];
		}
	}

	return NULL;
}

/*bool IMGArchive::isValidFile(const char* filename) {
	char ver[4];
	std::ifstream stream(filename, std::ifstream::in | std::ifstream::binary);
	stream.read(ver, 4);
	stream.close();
	return strcmp(ver, "VER2") == 0;
}*/


void IMGArchive::readHeader(istream* stream)
{
	int32_t firstBytes;

	stream->read((char*) &firstBytes, 4);

	if (stream->eof()) {
		if (stream->gcount() == 0) {
			// This is an empty file: VER1 DIR files can be completely empty, so we assume this one is
			// such a file.

			version = VER1;
			numEntries = 0;
			entries = new IMGEntry*[0];
			return;
		} else {
			throw IMGException(IMGException::SyntaxError, "Premature end of file");
		}
	}

	char* fourCC = (char*) &firstBytes;

	if (fourCC[0] == 'V'  &&  fourCC[1] == 'E'  &&  fourCC[2] == 'R'  &&  fourCC[3] == '2') {
		version = VER2;
		stream->read((char*) &numEntries, 4);

		if (stream->fail()) {
			throw IMGException(IMGException::SyntaxError, "Premature end of file");
		}

		if (numEntries < 0) {
			throw IMGException(IMGException::SyntaxError, "Number of entries in the header is < 0!");
		}

		entries = new IMGEntry*[numEntries];

		for (int32_t i = 0 ; i < numEntries ; i++) {
			entries[i] = new IMGEntry;
			stream->read((char*) entries[i], sizeof(IMGEntry));

			if (stream->fail()) {
				throw IMGException(IMGException::SyntaxError, "Premature end of file");
			}
		}

		bytesRead = 8 + numEntries*sizeof(IMGEntry);
	} else {
		version = VER1;

		vector<IMGEntry*> entryVector;

		IMGEntry* firstEntry = new IMGEntry;
		firstEntry->offset = firstBytes;
		stream->read((char*) firstEntry+4, sizeof(IMGEntry)-4);
		entryVector.push_back(firstEntry);

		if (stream->fail()) {
			throw IMGException(IMGException::SyntaxError, "Premature end of file");
		}

		stream->exceptions(istream::goodbit);

		numEntries = 0;

		while (!stream->eof()) {
			IMGEntry* entry = new IMGEntry;
			stream->read((char*) entry, sizeof(IMGEntry));
			entryVector.push_back(entry);
			numEntries++;
		}

		stream->clear();
		stream->exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);

		entries = new IMGEntry*[entryVector.size()];

		vector<IMGEntry*>::iterator it;
		int i = 0;
		for (it = entryVector.begin() ; it != entryVector.end() ; it++) {
			entries[i++] = *it;
		}

		bytesRead = 0;
	}

	for (int32_t i = 0 ; i < numEntries ; i++) {
		IMGEntry* entry = entries[i];

		if (entry->offset < 0) {
			throw IMGException(IMGException::SyntaxError, "Entry offset is < 0!");
		}
		if (entry->size < 0) {
			throw IMGException(IMGException::SyntaxError, "Entry size is < 0!");
		}
	}
}

ifstream* IMGArchive::openStream(const char* filename)
{
	ifstream* stream = new ifstream;
	stream->open(filename, ifstream::in | ifstream::binary);

	if (stream->fail()) {
		return NULL;
	}

	//stream->exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);
	return stream;
}


void IMGArchive::reposition(int offset)
{
	bytesRead += offset;
}
