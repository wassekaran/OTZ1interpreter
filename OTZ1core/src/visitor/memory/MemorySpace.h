#pragma once

#include<map>
#include<string>
#include "MemoryChunk.h"

typedef std::map<std::string, MemoryChunk*> memory_map;

class MemorySpace
{
protected:
	memory_map data;

public:
	void addMemoryChunk(MemoryChunk * mem)
	{
		data.insert(std::make_pair(mem->id, mem));
	}

	MemoryChunk * getMemoryChunk(std::string id)
	{
		memory_map::iterator it = data.find(id);

		if (it != data.end())
		{
			return it->second;
		}

		return nullptr;
	}
};