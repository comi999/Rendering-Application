#pragma once
#include <vector>
#include <entt/entt.hpp>

#include "Window.hpp"
#include "Object.hpp"

class Application
{
private:

	struct Indexer
	{
		static size_t Next()
		{
			static size_t Index = 0;
			return Index++;
		}

		template < typename T >
		static size_t Index()
		{
			static size_t Index = Next();
			return Index;
		}
	};

public:

	typedef void( *Updater )( Application*, float );

	Application( const char* a_Title, glm::uvec2 a_Size );
	virtual ~Application();
	bool Running() const;
	void Run();
	void Quit();
	const Window& GetWindow() const { return m_Window; }
	Object Create();
	void Destroy( Object a_Object );

	template < typename T >
	T* AddComponent( Object a_Object );

	template < typename T >
	T* GetComponent( Object a_Object );

	template < typename T >
	void DestroyComponent( Object a_Object );

protected:

	virtual void OnStart() = 0;
	virtual void OnTick( float a_DeltaTime ) = 0;
	virtual void OnStop() = 0;

private:

	template < typename T > static void UpdateComponents( Application* a_Application, float a_DeltaTime );
	template < typename T > Updater AssureUpdater();
	void UpdateAllComponents( float a_DeltaTime );

	bool                       m_Running;
	Window                     m_Window;
	entt::registry             m_Registry;
	std::vector< Updater >     m_Updaters;
};

template < typename T >
T* Application::AddComponent( Object a_Object )
{
	AssureUpdater< T >();
	T* New = &m_Registry.emplace_or_replace< T >( a_Object );
	New->OnCreate( this, a_Object );
	return New;
}

template < typename T >
T* Application::GetComponent( Object a_Object )
{
	return m_Registry.try_get< T >( a_Object );
}

template < typename T >
void Application::DestroyComponent( Object a_Object )
{
	T* Old = m_Registry.try_get< T >( a_Object );
	Old->OnDestroy( this, a_Object );
	m_Registry.destroy( a_Object );
}

template < typename T >
void Application::UpdateComponents( Application* a_Application, float a_DeltaTime )
{
	a_Application->m_Registry.view< T >().each( [&]( Object a_Object, T& a_Component )
	{
		a_Component.OnTick( a_Application, a_Object, a_DeltaTime );
	} );
}

template < typename T >
Application::Updater Application::AssureUpdater()
{
	if ( !( Application::Indexer::Index< T >() < m_Updaters.size() ) )
	{
		m_Updaters.resize( Application::Indexer::Index< T >() + 1u );
	}

	m_Updaters[ Application::Indexer::Index< T >() ] = Application::UpdateComponents< T >;
	return Application::UpdateComponents< T >;
}