#if !defined(MISC_H_)
#define MISC_H_

#pragma once

#include <string.h>
#include <stdint.h>

struct StringWrapper32
{
    char str[32]{'\0'};

	bool operator== (const StringWrapper32& anotherStr)
	{
		return strcmp(str, anotherStr.str) == 0;
	}
};

struct HashSW32
{
    uint64_t operator () (const StringWrapper32& aStr)
    {
        const uint64_t length = strlen(aStr.str);
		uint64_t i = 0;
		uint64_t hash = 0;
		while (i != length) {
			hash += aStr.str[i++];
			hash += hash << 10;
			hash ^= hash >> 6;
		}
		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
		return hash;
    }
};

struct HashInt
{
	uint64_t operator () (int anInt)
    {
        constexpr uint64_t pattern = 0x55555555;
		constexpr uint64_t constant = 1610612741;

		return constant * (anInt ^ (pattern * (anInt ^ (anInt >> 16) >> 16)));
    }
};

#endif // MISC_H_
