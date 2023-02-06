#include "stdafx.h"
#include "LevelStateMachine.h"

LevelStateMachine::LevelStateMachine() : levels( 0 ), currentLevel( 0 ) { AddToEvent(); }

LevelStateMachine::~LevelStateMachine() { RemoveFromEvent(); }

void LevelStateMachine::Update( const float dt )
{
	if ( currentLevel )
		currentLevel->Update( dt );
}

void LevelStateMachine::Render_Start()
{
	if ( currentLevel )
	{
		currentLevel->BeginFrame();
		currentLevel->RenderFrame();
		currentLevel->EndFrame_Start();
	}
}

void LevelStateMachine::Render_End()
{
	if ( currentLevel )
	{
		currentLevel->EndFrame_End();
	}
}

uint32_t LevelStateMachine::Add( std::shared_ptr<LevelContainer> level )
{
	auto inserted = levels.insert( std::make_pair( insertedLevelID, level ) );
	insertedLevelID++;
	return insertedLevelID - 1;
}

void LevelStateMachine::Remove( uint32_t id )
{
	auto it = levels.find( id );
	if ( it != levels.end() )
		if ( currentLevel == it->second )
			currentLevel = nullptr;
	levels.erase( it );
}

void LevelStateMachine::SwitchTo( uint32_t id )
{
	auto it = levels.find( id );
	if ( it != levels.end() )
	{
		if ( currentLevel )
			currentLevel->CleanUp();

		currentLevel = it->second;
		if ( !currentLevel->GetIsCreated() )
		{
			currentLevel->OnCreate();
			currentLevel->SetIsCreated( true );
		}
		currentLevel->OnSwitch();
	}
}

void LevelStateMachine::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient( EVENTID::GameLevelChangeEvent, this );
}

void LevelStateMachine::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient( EVENTID::GameLevelChangeEvent, this );
}

void LevelStateMachine::HandleEvent( Event* event )
{
	// Switch level
	switch ( event->GetEventID() )
	{
		case EVENTID::GameLevelChangeEvent:
		{
			SwitchTo( *static_cast<int*>( event->GetData() ) );
			delete event->GetData(); // is it valid?
		}
		break;
	}
}