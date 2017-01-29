/**
A version structure provides a convenient method of storing version information.

@date edited 29/01/2017
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef VERSION_H
#define VERSION_H

#include <string>
#include <cstdint>

struct Version
{
	/**
	The major release value. */
	std::uint32_t uiMajor;

	/**
	The minor release value. */
	std::uint32_t uiMinor;

	/**
	The build value. */
	std::uint32_t uiBuild;

	/**
	Constructs a default version of 0, 0, 0. */
	Version()
	{
		this->uiMajor = 0;
		this->uiMinor = 0;
		this->uiBuild = 0;
	}

	/**
	Constructs a version and sets all components to the given value.
	@param uiAll The major, minor and build number */
	Version(std::uint32_t uiAll)
	{
		this->uiMajor = uiAll;
		this->uiMinor = uiAll;
		this->uiBuild = uiAll;
	}

	/**
	Constructs a version with the given values.
	@param uiMajor The major version
	@param uiMinor The minor version
	@param uiBuild The build number */
	Version(std::uint32_t uiMajor, std::uint32_t uiMinor, std::uint32_t uiBuild)
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
	Checks whether one version is less than or equal to the other.
	@param other The version to compare with
	@return True if this version was less than or equal to the other, false otherwise */
	bool operator<=(const Version& other) const
	{
		if (uiMajor < other.uiMajor)
		{
			return true;
		}
		else if (uiMajor == other.uiMajor)
		{
			if (uiMinor < other.uiMinor)
			{
				return true;
			}
			else if (uiMinor == other.uiMinor)
			{
				if (uiBuild <= other.uiBuild)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	/**
	Checks whether one version is greater than or equal to the other.
	@param other The version to compare with
	@return True if this version was greater than or equal to the other, false otherwise */
	bool operator>=(const Version& other) const
	{
		if (uiMajor > other.uiMajor)
		{
			return true;
		}
		else if (uiMajor == other.uiMajor)
		{
			if (uiMinor > other.uiMinor)
			{
				return true;
			}
			else if (uiMinor == other.uiMinor)
			{
				if (uiBuild >= other.uiBuild)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	/**
	Checks whether one version is less than the other.
	@param other The version to compare with
	@return True if this version was less than the other, false otherwise */
	bool operator<(const Version& other) const
	{
		if (uiMajor < other.uiMajor)
		{
			return true;
		}
		else if (uiMajor == other.uiMajor)
		{
			if (uiMinor < other.uiMinor)
			{
				return true;
			}
			else if (uiMinor == other.uiMinor)
			{
				if (uiBuild < other.uiBuild)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	/**
	Checks whether one version is greater than the other.
	@param other The version to compare with
	@return True if this version was greater than the other, false otherwise */
	bool operator>(const Version& other) const
	{
		if (uiMajor > other.uiMajor)
		{
			return true;
		}
		else if (uiMajor == other.uiMajor)
		{
			if (uiMinor > other.uiMinor)
			{
				return true;
			}
			else if (uiMinor == other.uiMinor)
			{
				if (uiBuild > other.uiBuild)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	/**
	Checks whether two version objects are not equal.
	@param other The version to compare with
	@return True if the versions were not equal, false otherwise */
	bool operator!=(const Version& other) const
	{
		return uiMajor != other.uiMajor ||
			uiMinor != other.uiMinor ||
			uiBuild != other.uiBuild;
	}

	/**
	Returns a string representation of the version.
	@return A string representation */
	std::string toString() const
	{
		return std::to_string(uiMajor) + "." +
			std::to_string(uiMinor) + "." +
			std::to_string(uiBuild);
	}
};

#endif