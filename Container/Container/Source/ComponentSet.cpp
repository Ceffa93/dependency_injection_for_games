#include <Container/ComponentSet.h>
#include <Private/ComponentSetImpl.h>

using namespace container::internal;

namespace container
{
	ComponentSet::ComponentSet()
	: m_impl(std::make_unique<ComponentSetImpl>())
	{
	}

	ComponentSet::~ComponentSet() = default;

	void ComponentSet::addServiceInfo(internal::Type::Id id, bool bIsConst, void* componentPtr)
	{
		return m_impl->addServiceInfo(id, bIsConst, componentPtr);
	}

	void ComponentSet::addInternalServiceInfo(internal::Type::Id id, void* (*constructor) (ContainerImpl&, void*), void (*destructor) (void*),	uint32_t size)
	{
		return m_impl->addInternalServiceInfo(id, constructor, destructor, size);
	}

	InterfaceRecorder::InterfaceRecorder(ComponentSetImpl& componentSetImpl, Type::Id implementationId)
	: m_componentSetImpl(componentSetImpl)
	, m_implementationId(implementationId)
	{
	}

	void InterfaceRecorder::registerInterface(Type::Id interfaceId, Type::Id implementationId)
	{
		m_componentSetImpl.registerInterface(interfaceId, implementationId);
	}
}