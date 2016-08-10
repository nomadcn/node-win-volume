#pragma once
#ifndef GW_FLAGS_H
#define GW_FLAGS_H

#include "gw.h"

GW_NAMESPACE_BEGIN

class Flags
{
protected:
	uint32_t m_uFlags;

public:
	Flags(void);
	Flags(uint32_t uFlags);
	~Flags(void);

	uint32_t SetFlags(uint32_t uFlags, bool bValue);
	uint32_t SetFlags(uint32_t uFlags);
	uint32_t GetFlags(uint32_t uMask=0xFFFFFFFFu);
	uint32_t ToggleFlags(uint32_t uFlags);
};

GW_NAMESPACE_END

#endif
