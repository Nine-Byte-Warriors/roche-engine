#pragma once
#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <thread>

class ThreadManager
{
public:
	static void CreateThread( std::function<void()> thread );
	static void WaitForAllThreads();
private:
	static std::vector<std::thread*> m_pThreads;
};

#endif