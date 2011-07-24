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

	Additional permissions are granted, which are listed in the file
	GPLADDITIONS.
 */

#ifndef CACHEPOINTER_H_
#define CACHEPOINTER_H_

#include "SharedCachePointer.h"
#include <boost/smart_ptr/shared_ptr.hpp>



class CachePointer {
public:
	CachePointer() {}
	CachePointer(const CachePointer& other) : sharedPtr(other.sharedPtr) {}
	bool isValid() const { return sharedPtr; }
	CacheEntry* getEntry() { return sharedPtr->getEntry(); }
	void release() { return sharedPtr->release(); }

private:
	CachePointer(SharedCachePointer* sharedPtr) : sharedPtr(sharedPtr) {}

private:
	shared_ptr<SharedCachePointer> sharedPtr;

private:
	friend class ResourceCache;
};

#endif /* CACHEPOINTER_H_ */
