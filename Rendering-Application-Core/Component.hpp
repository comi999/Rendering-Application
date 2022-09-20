#pragma once
#include <entt/entt.hpp>

#include "Application.hpp"
#include "Object.hpp"


template < typename T >
class ComponentHandle
{
private:

	ComponentHandle( entt::registry& a_Registry, Object a_Object ) : m_Handle( a_Registry, a_Object ) { }

public:

	ComponentHandle() = default;
	ComponentHandle( const ComponentHandle& a_ComponentHandle ) : m_Handle( a_ComponentHandle.m_Handle ) { }
	ComponentHandle( T* a_Component ) { m_Handle = a_Component ? entt::handle( a_Component->GetApplication()->GetRegistry(), a_Component->GetObject() ) : entt::handle(); }
	ComponentHandle& operator=( Object a_Object ) { m_Handle = entt::handle( *m_Handle.registry(), a_Object ); return *this; }
	ComponentHandle& operator=( T& a_Component ) { m_Handle = entt::handle( a_Component.GetApplication()->GetRegistry(), a_Component.GetObject() ); return *this; }
	ComponentHandle& operator=( T* a_Component ) { m_Handle = a_Component ? entt::handle( a_Component->GetApplication()->GetRegistry(), a_Component->GetObject() ) : entt::handle(); return *this; }
	inline bool operator==( const ComponentHandle& a_ComponentHandle ) const { return m_Handle == a_ComponentHandle.m_Handle; }
	inline bool operator!=( const ComponentHandle& a_ComponentHandle ) const { return !( *this == a_ComponentHandle ); }
	inline bool operator==( Object a_Object ) const { return m_Handle.entity() == a_Object; }
	inline bool operator!=( Object a_Object ) const { return !( *this == a_ComponentHandle ); }
	inline bool operator==( std::nullptr_t ) const { return m_Handle.entity() == NullObject; }
	inline bool operator!=( std::nullptr_t ) const { return !( *this == a_ComponentHandle ); }
	const T& operator*() const { return m_Handle.get< T >(); }
	T& operator*() { return m_Handle.get< T >(); }
	const T* operator->() const { return m_Handle.registry() ? m_Handle.try_get< T >() : nullptr; }
	T* operator->() { return m_Handle.registry() ? m_Handle.try_get< T >() : nullptr; }
	operator Object() const { return m_Handle.entity(); }
	operator bool() const { return static_cast< bool >( m_Handle ); }
	operator T*() const { return m_Handle.registry() ? m_Handle.try_get< T >() : nullptr; }
	operator const T*() const { return m_Handle.registry() ? m_Handle.try_get< T >() : nullptr; }

private:

	entt::handle m_Handle;
};

class Component
{
public:

	Object GetObject() const { return m_Object; }
	Application* GetApplication() const { return m_Application; }

private:

	friend class Application;

	Object       m_Object;
	Application* m_Application;
};