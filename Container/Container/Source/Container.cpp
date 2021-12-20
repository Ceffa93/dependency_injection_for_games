#include <Container/Container.h>
#include <Container/ComponentSet.h>
#include <Private/ContainerImpl.h>
#include <Private/ComponentSetImpl.h>
#include <memory>
	
using namespace container::internal;

namespace container
{
	Container::Container(ComponentSet& componentSet)
	: m_impl(
		std::make_unique<ContainerImpl>(
			std::move(componentSet.m_impl->m_components),
			std::move(componentSet.m_impl->m_internalComponents),
			std::move(componentSet.m_impl->m_interfaces)))
	{
	}

	Container::~Container() = default;

	void* Container::get(internal::Type::Id paramId, bool bIsParamConst) const
	{
		return m_impl->get(paramId, bIsParamConst);
	}
}