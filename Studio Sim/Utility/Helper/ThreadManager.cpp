#include "stdafx.h"
#include "ThreadManager.h"

std::vector<std::thread*> ThreadManager::m_pThreads;

void ThreadManager::CreateThread( std::function<void()> thread )
{
	std::thread* newThread = new std::thread( thread );
	m_pThreads.emplace_back( newThread );
}

void ThreadManager::WaitForAllThreads()
{
	for ( auto& thread : m_pThreads )
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}
	m_pThreads.clear();
}