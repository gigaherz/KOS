#pragma once

#include "Common.h"

class Memory
{
private:

public:
	bool Initialize();
	void Destroy();

	// Allocates a memory block in the address space of the process.
	void* Alloc  (void* desired_address, u32 size);	

	// Maps the specified address to point to a previously allocated block.
	void* Map    (void* desired_address, void* existing_address);

	// Releases a memory mapping. If the address was the last (or only) map to a specific block, it also frees the block.
	void  Release(void* mapping_pointer);
};
