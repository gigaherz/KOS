#pragma once

#define DECLARE_RING(name, type, size) \
	type name##_buffer[size]; \
	UInt32 name##_readPos; \
	UInt32 name##_writePos;

#define RING_INIT(name) do { \
	name##_readPos = 0; \
	name##_writePos = 0; \
	} while(0)

#define RING_ENQ(name, item) do { \
	name##_buffer[name##_writePos] = item; \
	name##_writePos = (name##_writePos + 1) % ARRAYSIZE(name##_buffer); \
	} while(0)

#define RING_DEQ(name, dest) do { \
	dest = name##_buffer[name##_readPos]; \
	name##_readPos = (name##_readPos + 1) % ARRAYSIZE(name##_buffer); \
	} while(0)

#define RING_USED(name) ((name##_writePos + ARRAYSIZE(name##_buffer) - name##_readPos) % ARRAYSIZE(name##_buffer))

#define RING_FREE(name) (ARRAYSIZE(name##_buffer) - RING_USED(name) - 1)