// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include <stdio.h>
#pragma warning(disable: 4996)
#pragma warning(disable: 4661)
#pragma warning(disable: 4267)

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <array>
#include <unordered_map>
#include <list>
#include <queue>
#include <string>
#include <random>
#include <algorithm>
#include <iterator>
#include <climits>
#include <chrono>
#include <atomic>
#include <thread>
#include <sstream>
#include <functional>
#include <iostream>
#include <fstream> 
#include <condition_variable>
#include <mutex>
#include<iomanip>
#include<stdexcept>

#include <ilopl/iloopl.h>
#include <sstream>

#include "BibGrafos.h"
#include "BibValidador.h"

//#include <BibGrafos>

#ifndef DATADIR
#ifdef ILO_WINDOWS
#define DIRSEP "\\"
#else
#define DIRSEP "/"
#endif
#define DATADIR ".." DIRSEP ".."  DIRSEP ".." DIRSEP ".." DIRSEP "opl" DIRSEP
#endif

using namespace std;
using namespace BibGrafos;
using namespace BibValidador;

#include "VDDirectedPath.h"
#include "VDGrafoSplitIndiferenca.h"
#include "VDGrafo2Cliques.h"
