/**
An id is a binding of an index number and a version number that is used by indexed containers.

An invalid id is defined as an id where the version number equals 0. This is because containers
that utilise ids will never return a version number of 0.

@date edited 11/09/2016
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef ID_H
#define ID_H

template <class IntegralType = unsigned int>
struct Id
{
	private:

	protected:

	public:
		/**
		The index. */
		IntegralType index;

		/**
		The version. */
		IntegralType version;

		/**
		Constructs an id. */
		Id()
		{
		}

		/**
		Constructs an id with the given values.
		@param index The index number
		@param version The version number */
		Id(IntegralType index, IntegralType version)
		{
			this->index = index;
			this->version = version;
		}

		/* 
		Resets both the version an index number to 0. */
		void reset()
		{
			index = 0;
			version = 0;
		}

		/**
		Invalidates the id by resetting the version number to 0. The index remains unchanged. */
		void invalidate()
		{
			version = 0;
		}

		/**
		Evaluates the validity of the id. An id is considered valid if its version number is
		non-zero.
		@return True if the id is valid, false if it is not */
		bool isValid() const
		{
			return version != 0;
		}

		/**
		Constructs and returns an invalid id. 
		@return An invalid id */
		static Id<IntegralType> createInvalid()
		{
			Id<IntegralType> id;
			id.index = 0;
			id.version = 0;
			return id;
		}
};

#endif