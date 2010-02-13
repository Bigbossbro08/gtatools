/*
 * IMGArchive.h
 *
 *  Created on: 21.01.2010
 *      Author: alemariusnexus
 */

#ifndef IMGARCHIVE_H_
#define IMGARCHIVE_H_

#include "../config.h"
#include "IMGVisitor.h"
#include <istream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>

using std::istream;
using std::ifstream;


#define IMG_BLOCK_SIZE 2048



class IMGVisitor;

struct IMGEntry {
	int32_t offset;
	int32_t size;
	char name[24];
};


class IMGArchive {
public:
	enum IMGVersion {
		VER1, // GTA3, GTA VC
		VER2 // GTA SA
	};

	static bool isValidIMGFilename(const std::string& filename);
	static bool isValidDIRFilename(const std::string& filename);
	static bool isValidFile(const char* filename);
	static IMGVersion guessIMGVersion(const char* filename);
	static IMGVersion guessIMGVersion(istream* stream, bool returnToStart = true);

public:
	IMGArchive(const char* filename);
	IMGArchive(istream* stream, bool randomAccess = true);
	IMGArchive(istream* dirStream, istream* imgStream, bool randomAccess = true);
	IMGArchive(const char* dirName, const char* imgName);
	~IMGArchive();
	istream* gotoEntry(IMGEntry* entry);
	istream* gotoEntry(const char* name);
	void visit(IMGVisitor* visitor, IMGEntry* entry);
	void visitAll(IMGVisitor* visitor);
	IMGEntry* getEntryByName(const char* name);

	IMGEntry** getEntries() {
		return entries;;
	}
	int32_t getEntryCount() {
		return numEntries;
	}
	istream* getStream() {
		return stream;
	}
	IMGVersion getVersion()
	{
		return version;
	}

private:
	void assertNoEOF() {}
	ifstream* openStream(const char* filename);
	void readHeader(istream* stream);

private:
	istream* stream;
	IMGEntry** entries;
	int32_t numEntries;
	IMGVersion version;

	istream::streamoff bytesRead;
	bool randomAccess;
	bool autoGeneratedStream;
};

#endif /* IMGARCHIVE_H_ */