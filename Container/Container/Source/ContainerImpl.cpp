#include <Container/Exception.h>
#include <Private/ContainerImpl.h>
#include <Private/DataStructs.h>
#include <cassert>

using namespace container::internal;

namespace container
{
	ContainerImpl::ContainerImpl(
		std::unordered_map<internal::Type::Id, ComponentData>&& components,
		std::unordered_map<internal::Type::Id, InternalComponentData>&& internalComponents,
		std::unordered_map<internal::Type::Id, internal::Type::Id>&& interfaces)
	: m_components(std::move(components))
	, m_internalComponents(std::move(internalComponents))
	, m_interfaces(std::move(interfaces))
	{
		{
			uint32_t offset = 0;
			for (auto& p : m_internalComponents)
			{
				m_internalComponents[p.first].offset = offset;
				offset += p.second.size;
			}

			m_buffer = std::make_unique<uint8_t[]>(offset);
		}
		{
			m_componentOrder.reserve(m_internalComponents.size());

			for (auto& p : m_internalComponents)
			{
				getOrCreate(p.first);
			}
		}
	}

	ContainerImpl::~ContainerImpl()
	{
		for (auto it = m_componentOrder.rbegin(); it != m_componentOrder.rend(); ++it)
		{
			const auto& destructor = m_internalComponents[*it].destructor;
			const auto& componentPtr = m_components[*it].componentPtr;
			destructor(componentPtr);
		}
	}

	void* ContainerImpl::getOrCreate(Type::Id id)
	{
		auto componentIt = m_components.find(id);

		if (componentIt == m_components.cend())
		{
			throw ContainerException(
				ContainerException::Code::MissingDependency,
				std::string("Component <") + id + "> is required as dependency, but it was not found!");
		}
			
		{
			void* component = componentIt->second.componentPtr;
			if (component != nullptr) return component;
		}
		{
			void* component = m_internalComponents[id].constructor(*this, getAddress(id));
			m_components[id].componentPtr = component;
			m_componentOrder.push_back(id);
			return component;
		}
	}

	void* ContainerImpl::getOrCreateConstSafe(Type::Id id, bool bIsParamConst)
	{
		Type::Id implId = getImplementationId(id);

		void* child = getOrCreate(implId);

		if (!bIsParamConst && isConst(implId))
		{
			throw ContainerException(
				ContainerException::Code::DependencyConstMismatch,
				std::string("Component <") + implId + "> was added as const, but it is required as non-const dependency!");
		}

		return child;
	}

	bool ContainerImpl::isConst(Type::Id type) const
	{
		auto it = m_components.find(type);
		assert(it != m_components.cend());
		return it->second.bIsConst;
	}

	void* ContainerImpl::getAddress(Type::Id id) const
	{
		auto it = m_internalComponents.find(id);
		assert(it != m_internalComponents.cend());
		return m_buffer.get() + it->second.offset;
	}

	Type::Id ContainerImpl::getImplementationId(Type::Id id) const
	{
		auto it = m_interfaces.find(id);
		if (it == m_interfaces.cend()) return id;
		return it->second;
	}

	void* ContainerImpl::get(internal::Type::Id paramId, bool bIsParamConst) const
	{
		auto id = getImplementationId(paramId);
		auto res = m_components.find(id);
		if (res == m_components.cend())
		{
			throw ContainerException(
				ContainerException::Code::GetMissingComponent,
				paramId == id
				? std::string("Component <") + id + "> not found!"
				: std::string("Component implementing Interface <") + paramId + "> not found!");
		}

		if (!bIsParamConst && isConst(id))
		{
			throw ContainerException(
				ContainerException::Code::GetConstMismatch,
				paramId == id
				? std::string("Component <") + id + "> was constructed as const, but trying to get it as non-const!"
				: std::string("The Component <") + id + "> implementing Inteface <" + paramId + ">was constructed as const, but trying to get it as non - const!");
		}

		return res->second.componentPtr;
	}
}