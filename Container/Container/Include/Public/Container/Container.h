#pragma once
#include <Container/Internal/Type.h>
#include <Container/Internal/Traits.h>
#include <memory>

namespace container
{
	class ComponentSet;
	class ContainerImpl;

	class Container
	{
	public:
		Container(ComponentSet& componentSet);
		~Container();

		template <class ComponentPtrType, typename = std::enable_if_t<std::is_pointer_v<ComponentPtrType>>>
		ComponentPtrType get() const
		{
			using ComponentType = std::remove_pointer_t<ComponentPtrType>;
			auto paramId = internal::Type::id<std::remove_const_t<ComponentType>>();
			bool bIsParamConst = std::is_const_v<ComponentType>;

			void* component = get(paramId, bIsParamConst);

			return reinterpret_cast<ComponentPtrType>(component);
		}

		template <class ComponentType, typename = std::enable_if_t<!std::is_pointer_v<ComponentType>>>
		ComponentType& get() const
		{
			return *get<ComponentType*>();
		}

	private:
		void* get(internal::Type::Id paramId, bool bIsParamConst) const;

		std::unique_ptr<ContainerImpl> m_impl;
	};
}
