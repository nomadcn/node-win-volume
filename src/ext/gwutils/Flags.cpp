#include "Flags.h"

GW_NAMESPACE_BEGIN

Flags::Flags(void)
{
	m_uFlags = 0u;
}

Flags::Flags(uint32_t uFlags)
{
	m_uFlags = uFlags;
}

Flags::~Flags(void)
{
}

uint32_t Flags::SetFlags(uint32_t uFlags, bool bValue)
{
	if(bValue)
	{
		m_uFlags |= uFlags;
	}
	else
	{
		m_uFlags &= (uFlags ^ 0xFFFFFFFFu);
	}

    return m_uFlags;
}

uint32_t Flags::SetFlags(uint32_t uFlags)
{
	m_uFlags = uFlags;
    return m_uFlags;
}

uint32_t Flags::GetFlags(uint32_t uMask)
{
	return m_uFlags & uMask;
}

uint32_t Flags::ToggleFlags(uint32_t uFlags)
{
	uint32_t uValue = m_uFlags & uFlags;
	return SetFlags(uFlags, !uValue);
}

GW_NAMESPACE_END
