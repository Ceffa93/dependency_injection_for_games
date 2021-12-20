#pragma once
#include <Container/Internal/Type.h>
#include <Container/Internal/Traits.h>
#include <memory>

namespace container
{
	class InterfaceRecorder;
	class ComponentSetImpl;

	class ComponentSet 
	{
		friend class Container;
		friend class InterfaceRecorder;

	public:
		ComponentSet();
		~ComponentSet();

		template <class ComponentType>
		InterfaceRecorder add()
		{
			using RawComponentType = std::remove_const_t<ComponentType>;
			auto id = internal::Type::id<RawComponentType>();
			auto constructor = &internal::ConstructorFactory<RawComponentType>::Create;
			auto destructor = &internal::DestructorFactory<RawComponentType>::Destroy;
			uint32_t size = sizeof(RawComponentType);

			addInternalServiceInfo(id, constructor, destructor, size);

			return add<ComponentType>(nullptr);
		}

		template <class ComponentType>
		InterfaceRecorder add(ComponentType& externalComponent)
		{
			return add(&externalComponent);
		}

		template <class ComponentType>
		InterfaceRecorder add(ComponentType* externalComponent)
		{
			using RawComponentType = std::remove_const_t<ComponentType>;
			auto id = internal::Type::id<RawComponentType>();
			bool bIsConst = std::is_const_v<ComponentType>;
			void* componentPointer = reinterpret_cast<void*>(const_cast<RawComponentType*>(externalComponent));

			addServiceInfo(id, bIsConst, componentPointer);

			return InterfaceRecorder(*m_impl, id);
		}

	private:
		void addServiceInfo(internal::Type::Id id, bool bIsConst, void* componentPtr);
		void addInternalServiceInfo(internal::Type::Id id, void* (*constructor) (ContainerImpl&, void*), void (*destructor) (void*), uint32_t size);

		std::unique_ptr<ComponentSetImpl> m_impl;
	};


	class InterfaceRecorder
	{
	public:
		InterfaceRecorder(ComponentSetImpl&, internal::Type::Id);

		template <class Interface>
		void implements()
		{
			auto interfaceId = internal::Type::id<Interface>();
			registerInterface(interfaceId, m_implementationId);
		}

	private:
		void registerInterface(internal::Type::Id interfaceId, internal::Type::Id implementationId);

		ComponentSetImpl& m_componentSetImpl;
		internal::Type::Id m_implementationId;
	};
}
