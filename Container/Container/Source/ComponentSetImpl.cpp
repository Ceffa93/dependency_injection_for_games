#include <Container/Exception.h>
#include <Private/ComponentSetImpl.h>

using namespace container::internal;

namespace container
{
	void ComponentSetImpl::registerInterface(Type::Id interfaceId, Type::Id implementationId)
	{
		auto it = m_interfaces.find(interfaceId);
		if (it != m_interfaces.cend())
		{
			throw ComponentSetException(
				ComponentSetException::Code::InterfaceAlreadyImplemented,
				std::string("Interface <") + interfaceId + "> is already implemented by <" + it->second + ">.");
		}
		m_interfaces[interfaceId] = implementationId;
	}

	void ComponentSetImpl::addServiceInfo(internal::Type::Id id, bool bIsConst, void* componentPtr)
	{
		if (m_components.find(id) != m_components.end())
		{
			throw ComponentSetException(
				ComponentSetException::Code::ComponentAlreadyAdded,
				std::string("Component <") + id + "> was already added!");
		}

		m_components[id] = { componentPtr, bIsConst };
	}

	void ComponentSetImpl::addInternalServiceInfo(internal::Type::Id id, void* (*constructor) (ContainerImpl&, void*), void (*destructor) (void*),	uint32_t size)
	{
		m_internalComponents[id] = { constructor, destructor, size };
	}
}