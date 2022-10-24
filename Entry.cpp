#include "winapi.h"
#include "Error.h"

#include "Application.h"

#ifdef NDEBUG
int WINAPI wWinMain(HINSTANCE , HINSTANCE , PWSTR , int )
#else
int main()
#endif
{
	debug_log("Start!\n");
	Application app;

	debug_log("App initalized!\n");

	int ret = app.run();

	debug_log("Exit with %d!\n", ret);
	
	return ret;
}