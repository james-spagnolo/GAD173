#include "App.h"

int main()
{

	srand(time(NULL));


	App game("Level Editor", 800, 800, 32);

	if (!game.Init())
	{
		printf("Game could not be started!");
		return 1;
	}
	else
	{
		game.Run();
	}
	return 0;
}