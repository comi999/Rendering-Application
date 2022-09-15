#pragma once
#include <vector>
#include <entt/entt.hpp>

#include "Window.hpp"
#include "Object.hpp"

class Camera;



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

	template < typename T >
	struct HasOnCreate
	{
		template < typename U > static char test( decltype( &U::OnCreate ) );
		template < typename U > static long test( ... );
		static constexpr bool Value = sizeof( test< T >( 0 ) ) == sizeof( char );
	};

	template < typename T >
	struct HasOnDestroy
	{
		template < typename U > static char test( decltype( &U::OnDestroy ) );
		template < typename U > static long test( ... );
		static constexpr bool Value = sizeof( test< T >( 0 ) ) == sizeof( char );
	};

	template < typename T >
	struct HasOnTick
	{
		template < typename U > static char test( decltype( &U::OnTick ) );
		template < typename U > static long test( ... );
		static constexpr bool Value = sizeof( test< T >( 0 ) ) == sizeof( char );
	};

public:

	typedef void( *Updater )( Application*, float );

	Application( const char* a_Title, glm::uvec2 a_Size );
	virtual ~Application();
	bool Running() const;
	void Run();
	void Quit();
	const Window* GetWindow() const { return m_Window; }
	Object Create();
	void Destroy( Object a_Object );
	Camera* GetMainCamera();
	void SetMainCamera( Camera* a_Camera );

	template < typename T >
	T* AddComponent( Object a_Object );

	template < typename T >
	T* GetComponent( Object a_Object );

	template < typename T >
	void DestroyComponent( Object a_Object );

	template < typename T, typename Func >
	void PatchComponents( Func a_Patcher );

protected:

	virtual void OnStart() = 0;
	virtual void OnTick( float a_DeltaTime ) = 0;
	virtual void OnStop() = 0;

private:

	template < typename T > static void UpdateComponents( Application* a_Application, float a_DeltaTime );
	template < typename T > Updater AssureUpdater();
	void UpdateAllComponents( float a_DeltaTime );

	bool                       m_Running;
	Window*                    m_Window;
	Object                     m_MainCamera;
	entt::registry             m_Registry;
	std::vector< Updater >     m_Updaters;
};

template < typename T >
T* Application::AddComponent( Object a_Object )
{
	AssureUpdater< T >();
	T* New = &m_Registry.emplace_or_replace< T >( a_Object );
	New->m_Application = this;
	New->m_Object = a_Object;

	if constexpr ( HasOnCreate< T >::Value )
	{
		New->OnCreate();
	}

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

	if constexpr ( HasOnDestroy< T >::Value )
	{
		Old->OnDestroy();
	}

	m_Registry.destroy( a_Object );
}

template < typename T, typename Func >
void Application::PatchComponents( Func a_Patcher )
{
	m_Registry.view< T >().each( a_Patcher );
}

template < typename T >
void Application::UpdateComponents( Application* a_Application, float a_DeltaTime )
{
	if constexpr ( HasOnTick< T >::Value )
	{
		a_Application->m_Registry.view< T >().each( [&]( T& a_Component )
		{
			a_Component.OnTick( a_DeltaTime );
		} );
	}
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