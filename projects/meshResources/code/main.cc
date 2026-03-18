#include "config.h"
#include "meshResourcesSim.h"


int
main(int argc, const char** argv)
{
	MeshResourceSim::MeshResourceSimApp app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();	

	return app.ExitCode();
}