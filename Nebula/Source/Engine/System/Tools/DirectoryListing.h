/**
The DirectoryListing is a storage location for a series of name to directory mappings. The
listing is intended to reduce the maintainability issues associated with modifying the
path to a directory when it is used in multiple places.

Example mapping:
"data_dir", ".\\data"

@date edited 24/11/2016
@date authored 24/11/2016

@author Nathan Sainsbury */

#ifndef DIRECTORY_LISTING_H
#define DIRECTORY_LISTING_H

#include <string>
#include "Engine/System/Tools/Map.h"

class DirectoryListing
{
	private:
		std::string m_sDefaultDirectory;
		Map<std::string, std::string> m_mappings;

	protected:

	public:
		/**
		Constructs a DirectoryListing. */
		DirectoryListing();

		/**
		Adds a directory mapping. If the given name was already in use by another mapping,
		this function will fail. Empty names are disallowed.
		@param sName The name by which the directory will be identified
		@param sPath The directory
		@return True if the mapping was added, false if it was not */
		bool addDir(const std::string& sName, const std::string& sPath);

		/**
		Removes a directory mapping. If the mapping did not exist, no action is taken.
		@param sName The name of the mapping
		@return True if a mapping was removed, false otherwise */
		bool removeDir(const std::string& sName);

		/**
		Retrieves the directory associated with the given name. If no such mapping existed,
		a default directory is returned instead.
		@param sName The name of the directory to get
		@return The directory, or a default directory */
		const std::string& getDir(const std::string& sName);

		/**
		Queries the existence of a directory mapping with the given name.
		@param sName The name of the directory mapping
		@return True if the directory existed, false if it did not */
		bool exists(const std::string& sName);

		/**
		Clears all mappings. */
		void clear();
};

#endif