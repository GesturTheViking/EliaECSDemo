/*
* BitArray
*
* Container class for easily storing bit values.
*
* Requirements: C++17
*/

#if !defined(BITARRAY_H_)
#define BITARRAY_H_

#pragma once

#include <stdint.h>
#include <string.h>

#include <assert.h>

template <size_t size>
class BitArray
{
	using dataType = uint64_t;

	static_assert(size > 0, "Attempting to create an empty bitarray.");
public:
	/* Constructors & Destructor */
	BitArray()
	{
		memset(myData, 0, dataCount * ourSizeOfTypeBytes);
	}
	~BitArray() = default;

	BitArray(const BitArray<size>&aBitArray)
	{
		memcpy(myData, aBitArray.myData, dataCount * ourSizeOfTypeBytes);
	}

	BitArray(dataType init)
	{
		memset(myData, 0, dataCount * ourSizeOfTypeBytes);
		myData[0] = init;
	}

	BitArray(BitArray<size> && aBitArray) noexcept
	{
		memcpy(myData, aBitArray.myData, dataCount * ourSizeOfTypeBytes);
	}

	BitArray& operator=(const BitArray<size>&aBitArray)
	{
		memcpy(myData, aBitArray.myData, dataCount * ourSizeOfTypeBytes);

		return *this;
	}

	BitArray& operator=(BitArray<size> && aBitArray) noexcept
	{
		memcpy(myData, aBitArray.myData, dataCount * ourSizeOfTypeBytes);

		return *this;
	}

	/* Interface */

	/* Getters */
	size_t Size() const
	{
		return size;
	}

	bool Test(size_t anIndex) const
	{
		assert(anIndex < size && "Index out of range.");

		return (myData[anIndex / ourSizeOfType] >> (anIndex % ourSizeOfType)) & 1;
	}

	bool All() const
	{
		for (size_t index = 0; index < dataCount; ++index)
		{
			if (!myData[index])
			{
				return false;
			}
		}

		return true;
	}

	bool Any() const
	{
		for (size_t index = 0; index < dataCount; ++index)
		{
			if (myData[index])
			{
				return true;
			}
		}

		return false;
	}

	bool None() const
	{
		for (size_t index = 0; index < dataCount; ++index)
		{
			if (myData[index])
			{
				return false;
			}
		}

		return true;
	}

	size_t Count() const
	{
		size_t count = 0U;

		for (size_t index = 0; index < size; ++index)
		{
			if (Test(index))
			{
				++count;
			}
		}

		return count;
	}

	/* Setters */
	void Set(size_t anIndex)
	{
		assert(anIndex < size && "Index out of range.");

		myData[anIndex / ourSizeOfType] |= (uint64_t(1U) << (anIndex % ourSizeOfType));
	}

	void Set(size_t anIndex, bool aValue)
	{
		assert(anIndex < size && "Index out of range.");

		if (aValue) Set(anIndex);
		else		Reset(anIndex);
	}

	void Reset(size_t anIndex)
	{
		assert(anIndex < size && "Index out of range.");

		myData[anIndex / ourSizeOfType] &= ~(uint64_t(1U) << (anIndex % ourSizeOfType));
	}

	void Flip(size_t anIndex)
	{
		assert(anIndex < size && "Index out of range.");

		myData[anIndex / ourSizeOfType] ^= (uint64_t(1U) << (anIndex % ourSizeOfType));
	}

	void SetAll()
	{
		memset(myData, (uint8_t)-1, dataCount * ourSizeOfTypeBytes);
	}

	void ResetAll()
	{
		memset(myData, 0, dataCount * ourSizeOfTypeBytes);
	}

	void FlipAll()
	{
		for (size_t index = 0; index < dataCount; ++index)
		{
			myData[index] = ~myData[index];
		}
	}

	/* Operators */
	bool operator==(const BitArray<size>&anotherArray)
	{
		for (size_t index = 0; index < dataCount; ++index)
		{
			if (myData[index] != anotherArray.myData[index])
			{
				return false;
			}
		}

		return true;
	}

	bool operator!=(const BitArray<size>&anotherArray)
	{
		return !operator==(anotherArray);
	}

	bool operator[] (size_t anIndex) const
	{
		assert(anIndex < size && "Index out of range.");

		return (myData[anIndex / ourSizeOfType] >> (anIndex % ourSizeOfType)) & 1;
	}

	BitArray<size> operator | (const BitArray<size>&anotherBitArray) const
	{
		BitArray<size> a = *this;

		for (size_t index = 0; index < dataCount; ++index)
		{
			a.myData[index] |= anotherBitArray.myData[index];
		}

		return a;
	}

	BitArray<size>& operator |= (const BitArray<size>&anotherBitArray)
	{
		return *this = *this | anotherBitArray;
	}

	BitArray<size> operator & (const BitArray<size>&anotherBitArray) const
	{
		BitArray<size> a = *this;

		for (size_t index = 0; index < dataCount; ++index)
		{
			a.myData[index] &= anotherBitArray.myData[index];
		}

		return a;
	}

	BitArray<size>& operator &= (const BitArray<size>& anotherBitArray)
	{
		return *this = *this & anotherBitArray;
	}

	BitArray<size> operator ^ (const BitArray<size>&anotherBitArray) const
	{
		BitArray<size> a = *this;

		for (size_t index = 0; index < dataCount; ++index)
		{
			a.myData[index] ^= anotherBitArray.myData[index];
		}

		return a;
	}

	BitArray<size>& operator ^= (const BitArray<size>& anotherBitArray)
	{
		return *this = *this ^ anotherBitArray;
	}

private:
	static constexpr size_t ourSizeOfTypeBytes = sizeof(dataType);
	static constexpr size_t ourSizeOfType = ourSizeOfTypeBytes * 8U;
	static constexpr size_t dataCount = (size - 1) / ourSizeOfType + 1;

	dataType myData[dataCount];
};

#endif // BITARRAY_H_