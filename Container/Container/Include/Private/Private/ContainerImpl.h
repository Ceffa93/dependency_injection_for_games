#pragma once
#include <Container/Internal/Type.h>
#include <unordered_map>
#include <memory>
#include <vector>

namespace container
{
	struct ComponentData;
	struct InternalComponentData;

	class ContainerImpl
	{
	public:
		ContainerImpl(
			std::unordered_map<internal::Type::Id, ComponentData>&& components,
			std::unordered_map<internal::Type::Id, InternalComponentData>&& internalComponents,
			std::unordered_map<internal::Type::Id, internal::Type::Id>&& interfaces);

		~ContainerImpl();

		void* get(internal::Type::Id paramId, bool bIsParamConst) const;
		void* getAddress(internal::Type::Id) const;
		void* getOrCreateConstSafe(internal::Type::Id, bool bIsParamConst);

	private:
		void* getOrCreate(internal::Type::Id);
		bool isConst(internal::Type::Id type) const;
		internal::Type::Id getImplementationId(internal::Type::Id) const;

		std::unordered_map<internal::Type::Id, ComponentData> m_components;
		std::unordered_map<internal::Type::Id, InternalComponentData> m_internalComponents;
		std::unordered_map<internal::Type::Id, internal::Type::Id> m_interfaces;

		std::vector<internal::Type::Id> m_componentOrder;
		std::unique_ptr<uint8_t[]> m_buffer;
	};
}
