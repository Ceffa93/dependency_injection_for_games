#pragma once
#include <Container/Internal/Type.h>
#include <unordered_map>
#include <memory>

namespace container
{
	class ContainerImpl;

	struct ComponentData
	{
		void* componentPtr;
		bool bIsConst;
	};

	struct InternalComponentData
	{
		void* (*constructor) (ContainerImpl&, void* address);
		void (*destructor) (void* address);
		uint32_t size;
		uint32_t offset;
	};
}
