#pragma once
#include <exception>
#include <string>

namespace container
{
	class ComponentSetException : public std::exception
	{
	public:
		enum class Code
		{
			ComponentAlreadyAdded,
			InterfaceAlreadyImplemented,
		};

		ComponentSetException(Code code, const std::string& message) 
		: m_code(code)
		, m_message(message)
		{}

		Code code() { return m_code; }

		const char* what() const noexcept override { return m_message.c_str(); }

	private:
		Code m_code;
		std::string m_message;
	};

	class ContainerException : public std::exception
	{
	public:
		enum class Code
		{
			MissingDependency,
			DependencyConstMismatch,
			GetConstMismatch,
			GetMissingComponent,
			AccessInterfaceAsReference,
		};

		ContainerException(Code code, const std::string& message)
		: m_code(code)
		, m_message(message)
		{}

		Code code() { return m_code; }

		const char* what() const noexcept override { return m_message.c_str(); }

	private:
		Code m_code;
		std::string m_message;
	};
}
