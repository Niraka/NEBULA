/**
A random number generator built on top of the standard library random tools.

@date edited 26/03/2017
@date authored 26/03/2017

@author Nathan Sainsbury */

#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include <random>

#include "Bounds.h"

template <typename T>
class RandomNumberGenerator
{
	public:
		static_assert(std::_Is_RealType<T>::value || std::_Is_IntType<T>::value,
			"Random number generator must a real or integer type (\"char\" also disallowed).");

		typedef std::conditional_t<std::_Is_RealType<T>::value,
			std::uniform_real_distribution<T>,
			std::uniform_int_distribution<T>> Distributor;

		/**
		Constructs a random number generator with default 0 to 100 bounds and a seed of 0. */
		RandomNumberGenerator() :
			m_generator(0),
			m_distributor(0, 100)
		{
		}

		/**
		Constructs a random number generator with the given bounds and a default seed of 0.
		@param bounds The bounds */
		RandomNumberGenerator(Bounds<T> bounds) :
			m_generator(0),
			m_distributor(bounds.lower, bounds.upper)
		{
		}

		/**
		Constructs a random number generator with the given bounds and seed.
		@param bounds The bounds
		@param uiSeed The seed */
		RandomNumberGenerator(Bounds<T> bounds, std::uint64_t uiSeed) :
			m_generator(uiSeed),
			m_distributor(bounds.lower, bounds.upper)
		{
		}

		/**
		Sets the random number generator bounds.
		@param bounds The bounds */
		void setBounds(Bounds<T> bounds)
		{
			m_distributor = Distributor(bounds.lower, bounds.upper);
		}

		/**
		Retrieves the current bounds.
		@return The bounds */
		Bounds<T> getBounds() const
		{
			return Bounds<T>(m_distributor.a(), m_distributor.b());
		}

		/**
		Generates the next random number and advances the sequence.
		@return The next number in the sequence */
		T next()
		{
			return m_distributor(m_generator);
		}

		/**
		Seeds the random number generator.
		@param uiSeed The seed */
		void seed(std::uint64_t uiSeed)
		{
			m_generator.seed(uiSeed);
		}

	protected:

	private:
		std::mt19937_64 m_generator;
		Distributor m_distributor;
};

typedef RandomNumberGenerator<int> RandomIntGenerator;
typedef RandomNumberGenerator<float> RandomFloatGenerator;
typedef RandomNumberGenerator<double> RandomDoubleGenerator;

typedef RandomNumberGenerator<std::int32_t> RandomInt32Generator;
typedef RandomNumberGenerator<std::int64_t> RandomInt64Generator;
typedef RandomNumberGenerator<std::uint32_t> RandomUint32Generator;
typedef RandomNumberGenerator<std::uint64_t> RandomUint64Generator;

#endif