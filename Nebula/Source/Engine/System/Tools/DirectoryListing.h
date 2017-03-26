/**
The DirectoryListing is a storage location for a series of key to directory mappings. The listing 
is intended to reduce the maintainability issues associated with modifying the path to a directory 
when it is used in multiple places.

Some example mappings could be (depending on key type):
"data_dir", "./data/"
"textures_sub_dir", "textures/"
1, "./data/"

@date edited 26/03/2017
@date authored 24/11/2016

@author Nathan Sainsbury */

#ifndef DIRECTORY_LISTING_H
#define DIRECTORY_LISTING_H

#include <map>
#include <set>
#include <string>

struct DirectoryListingSearchResult
{
	/**
	True if the search successfully found its target directory. */
	bool bFound; 
	
	/**
	The directory. Addresses the default directory if the search failed to find the target
	directory. */
	const std::string& sDirectory;

	DirectoryListingSearchResult(bool bFound,
		const std::string& sDirectory) :
		bFound(bFound),
		sDirectory(sDirectory)
	{
	}

	DirectoryListingSearchResult(const DirectoryListingSearchResult& other) :
		bFound(other.bFound),
		sDirectory(other.sDirectory)
	{
	}
};

template <typename KeyType = std::string>
class DirectoryListing
{
	private:
		typedef typename std::map<KeyType, std::string>::const_iterator ConstIter;

	public:
		typedef DirectoryListingSearchResult SearchResult;

		enum class OpResults
		{
			/**
			The operation was successful. */
			SUCCESS,

			/**
			The operation failed because the given key was illegal. */
			FAIL_ILLEGAL_KEY,

			/**
			The operation failed because the given directory was illegal. */
			FAIL_ILLEGAL_DIRECTORY
		};

		/**
		Constructs a directory listing with no additional illegal keys or directories. */
		DirectoryListing() :
			m_sDefaultDirectory("./default/")
		{
			m_illegalDirectories.insert("./default/");
		}
		
		/**
		Constructs a directory listing with the given illegal keys and directories.
		@param illegalKeys The keys to disallow
		@param illegalDirectories The directories to disallow */
		DirectoryListing(const std::set<KeyType>& illegalKeys, 
			const std::set<std::string&> illegalDirectories) :
			m_sDefaultDirectory("./default/"),
			m_illegalKeys(illegalKeys),
			m_illegalDirectories(illegalDirectories)
		{
			m_illegalDirectories.insert("./default/");
		}

		/**
		Adds a directory to the listing. Overwrites if the key was already in use. Both the key and 
		directory must be legal values as defined by the illegal keys and directories given in the 
		directory listing constructor.
		@param key The key
		@param sDirectory The directory
		@return A code indicating the result of the operation */
		OpResults add(const KeyType& key, const std::string& sDirectory)
		{
			if (isIllegalKey(key))
			{
				return OpResults::FAIL_ILLEGAL_KEY;
			}
			if (isIllegalDirectory(sDirectory))
			{
				return OpResults::FAIL_ILLEGAL_DIRECTORY;
			}

			m_mappings[key] = sDirectory;
			return OpResults::SUCCESS;
		}

		/**
		Removes a directory from the listing.
		@param key The key */
		void remove(const KeyType& key) 
		{
			m_mappings.erase(key);
		}

		/**
		Searches for and returns the directory mapped to the given key. If no such directory 
		existed the default directory is returned instead.
		@param key The key 
		@return The result of the search */
		SearchResult find(const KeyType& key) const
		{
			ConstIter it = m_mappings.find(key);
			if (it != m_mappings.cend())
			{
				return SearchResult(true, it->second);
			}
			else
			{
				return SearchResult(false, m_sDefaultDirectory);
			}
		}

		/**
		Clears all entries. */
		void clear()
		{
			m_mappings.clear();
		}

	protected:

	private:
		std::string m_sDefaultDirectory;
		std::map<KeyType, std::string> m_mappings;
		std::set<KeyType> m_illegalKeys;
		std::set<std::string> m_illegalDirectories;

		/**
		Queries whether a key is illegal.
		@param key The key to query
		@return True if the key is illegal, false if it is not */
		bool isIllegalKey(const KeyType& key) const
		{
			return m_illegalKeys.find(key) != m_illegalKeys.cend();
		}

		/**
		Queries whether a directory is illegal.
		@param sDirectory The directory to query
		@return True if the directory is illegal, false if it is not. */
		bool isIllegalDirectory(const std::string& sDirectory) const
		{
			return m_illegalDirectories.find(sDirectory) != m_illegalDirectories.cend();
		}
};

#endif