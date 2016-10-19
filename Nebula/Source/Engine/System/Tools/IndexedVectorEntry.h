/**
An indexed vector entry.

@see IndexedVector

@date edited 25/09/2016
@date authored 15/09/2016

@author Nathan Sainsbury */

#ifndef INDEXED_VECTOR_ENTRY_H
#define INDEXED_VECTOR_ENTRY_H

#include "Engine/System/Tools/Id.h"

template <class ElementType, class IdType>
struct IndexedVectorEntry
{
	/** 
	The data. */
	ElementType data;

	/**
	The version. */
	IdType version;

	/**
	True if the entry is in use. */
	bool bActive;
};

#endif