#include "config.h"
#include "meshResourcesSim.h"


int
main(int argc, const char** argv)
{
	Mesh::MeshResources app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();	

	return app.ExitCode();
}