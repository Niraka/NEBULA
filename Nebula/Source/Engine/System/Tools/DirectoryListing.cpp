#include "Engine/System/Tools/DirectoryListing.h"

DirectoryListing::DirectoryListing()
{
	m_sDefaultDirectory = "./NoDirectoryMapping/";
}		

bool DirectoryListing::addDir(const std::string& sName, const std::string& sPath)
{
	if (sName == "")
	{
		return false;
	}

	return m_mappings.insert(sName, sPath);
}

bool DirectoryListing::removeDir(const std::string& sName)
{
	return m_mappings.remove(sName);
}

const std::string& DirectoryListing::getDir(const std::string& sName)
{
	if (m_mappings.exists(sName))
	{
		return m_mappings.get(sName);
	}
	else
	{
		return m_sDefaultDirectory;
	}
}

bool DirectoryListing::exists(const std::string& sName)
{
	return m_mappings.exists(sName);
}

void DirectoryListing::clear()
{
	m_mappings.clear();
}