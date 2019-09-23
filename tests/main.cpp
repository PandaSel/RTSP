#include "Server.h"
#include <chrono>
#include <thread>
#include "resource.h"

int main(int argc, char *argv[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::unique_ptr<Server> mServer = std::make_unique<Server>(9554);

	char c;
	while (c = mygetch() != 'q') {
#ifdef WIN32
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
#else
		usleep(10000);
#endif
	}


	return 0;
}
