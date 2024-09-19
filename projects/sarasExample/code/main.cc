#include "config.h"
#include "sarasExampleApp.h"

int
main(int argc, const char** argv)
{
	Example::ExampleApp app;
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();	

	return app.ExitCode();
}