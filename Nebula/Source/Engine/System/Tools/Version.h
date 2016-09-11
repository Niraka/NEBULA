/**
A version structure provides a convenient method of storing version information.

@date edited 11/09/2016
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef VERSION_H
#define VERSION_H

#include <string>

struct Version
{
	/* The major release counter */
	unsigned int uiMajor;

	/* The minor release counter */
	unsigned int uiMinor;

	/* The build counter */
	unsigned int uiBuild;

	/**
	Constructs a version object. */
	Version()
	{
	}

	/**
	Constructs a version object with the given values.
	@param uiMajor The major version
	@param uiMinor The minor version
	@param uiBuild The build number */
	Version(unsigned int uiMajor, unsigned int uiMinor, unsigned int uiBuild)
	{
		this->uiMajor = uiMajor;
		this->uiMinor = uiMinor;
		this->uiBuild = uiBuild;
	}

	/**
	Checks whether two version objects are equal.
	@param other The version to compare with
	@return True if the versions were equal, false otherwise */
	bool operator==(const Version& other) const
	{
		return uiMajor == other.uiMajor &&
			uiMinor == other.uiMinor &&
			uiBuild == other.uiBuild;
	}

	/**
	Returns a string representation of the version.
	@return A string representation */
	std::string ToString() const
	{
		return std::to_string(uiMajor) + "." +
			std::to_string(uiMinor) + "." +
			std::to_string(uiBuild);
	}
};

#endif