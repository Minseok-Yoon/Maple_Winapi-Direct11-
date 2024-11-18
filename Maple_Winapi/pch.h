#pragma once

using namespace std;

#include <Windows.h>
#include <assert.h>
#include <iostream>

#include <algorithm>
using std::sort;
#include <string>
using std::string;
using std::wstring;
#include <vector>
using std::vector;
using std::erase_if;
#include <list>
using std::list;
#include <stack>
using std::stack;
#include <queue>
using std::queue;
#include <map>
using std::map;
using std::make_pair;
#include <unordered_map>
using std::unordered_map;
#include <utility>
using std::pair;

#include <bitset>
#include <functional>
using std::function;
using std::bind;
#include <filesystem>
using namespace std::filesystem;
#include <memory>
using std::unique_ptr;
using std::make_unique;
#include <mutex>
#include <thread>

#include <stdexcept>
#include <locale>
#include <codecvt>
#include <ShlObj.h>
#include <crtdbg.h>

#include <mmsystem.h>
#include <dinput.h>

#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "winmm.lib")

#include "Define.h"
#include "Enum.h"
#include "CMath.h"
#include "Struct.h"
#include "Resource.h"