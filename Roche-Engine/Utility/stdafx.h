#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#if _DEBUG
#include <imgui/imgui.h>
#endif

#include <memory>
#include <algorithm>
#include <functional>

// Data Structures
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>

// Windows API
#include <Windows.h>
#include <d3d11_1.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#define _XM_NO_INTRINSICS_
using namespace DirectX;

// Classes
#include "ErrorLogger.h"
#include "EventSystem.h"
#include "Vector2f.h"
#include "Timer.h"

#endif