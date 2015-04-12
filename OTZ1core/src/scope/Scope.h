#pragma once

#include<map>
#include "ScopeEntry.h"
#include<string>
#include<iostream>

typedef std::map<std::string, ScopeEntry*> scope_entry_map;

class Scope
{
protected:
	Scope * parent;
	scope_entry_map entries;

public:
	Scope(Scope * parent) : parent(parent) {}
	Scope() : parent(nullptr) {}

	void declare(std::string id)
	{
		if (reference(id) == nullptr)
		{
			entries.insert(std::make_pair(id, new ScopeEntry(id)));
		}
		else
		{
			std::cout << "Scope entry already defined" << std::endl;
		}
	}

	ScopeEntry * reference(std::string id)
	{
		scope_entry_map::iterator it = entries.find(id);

		if (it != entries.end())
		{
			return it->second;
		}

		if (parent != nullptr)
		{
			return parent->reference(id);
		}

		return nullptr;
	}

	inline Scope * getParent() { return parent; }
};