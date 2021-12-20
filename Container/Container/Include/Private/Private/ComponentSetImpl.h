#pragma once
#include <Container/Internal/Type.h>
#include <Private/DataStructs.h>
#include <memory>
#include <unordered_map>
#include <exception>

namespace container
{
	struct ComponentData;
	struct InternalComponentData;

	class ComponentSetImpl
	{
	public:
		void addServiceInfo(internal::Type::Id id, bool bIsConst, void* componentPtr);
		void addInternalServiceInfo(internal::Type::Id id, void* (*constructor) (ContainerImpl&, void*), void (*destructor) (void*), uint32_t size);
		void registerInterface(internal::Type::Id interfaceId, internal::Type::Id implementationId);

		std::unordered_map<internal::Type::Id, ComponentData> m_components;
		std::unordered_map<internal::Type::Id, InternalComponentData> m_internalComponents;
		std::unordered_map<internal::Type::Id, internal::Type::Id> m_interfaces;
	};
}
