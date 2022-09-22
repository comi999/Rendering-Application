#pragma once
#include <string>
#include <vector>
#include <any>
#include <entt/resource/resource.hpp>
#include <entt/core/hashed_string.hpp>

class Resource
{
public:

	Resource() = default;
	Resource( const std::string& a_Path ) : m_Path( a_Path ) { };
	const std::string& GetPath() const { return m_Path; }

private:

	std::string m_Path;
};

template < typename T >
class ResourceHandle
{
public:

	bool IsLoaded();
	T*   Load();
	void Release(){m_Handle.operator=() }

private:

	std::string                m_Name;
	entt::resource_cache< T >* m_Cache;
	entt::resource< T >        m_Handle;
};

class ResourceRepository
{
private:

	static size_t Next()
	{
		static size_t Value = 0;
		return Value++;
	}

	template < typename T >
	static size_t Index()
	{
		static size_t Value = Next();
		return Value;
	}

public:



private:

	template < typename T >
	std::any& Assure()
	{
		static size_t Value = Index< T >();

		if ( !( Value < m_Caches.size() ) )
		{
			m_Caches.resize( Value + 1u );
			m_Caches.back().emplace< entt::resource_cache< T > >();
		}

		return m_Caches[ Value ];
	}

	std::vector< std::any > m_Caches;
};