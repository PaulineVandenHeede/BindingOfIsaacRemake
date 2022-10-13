#include "pch.h"
#include "Core.h"
#include <ctime>
#if defined(DEBUG) | defined (_DEBUG)
#include <vld.h>
#endif

void StartHeapControl();
void DumpMemoryLeaks();

int main( int argc, char *argv[] )
{
	srand(int(time(nullptr)));
	{ // Make sure stack objects go out of scope before memory leaks are dumped
		// StartHeapControl();

		Core core{ Window{ "Binding of Isaac: Rebirth - Vanden Heede, Pauline - 1DAE21", 1130.f, 740.f } };
		core.Run();
	}
	// DumpMemoryLeaks();
	return 0;
}


void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc( 156 );
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}


