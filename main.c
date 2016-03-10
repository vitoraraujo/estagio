#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL; 
SDL_Surface* gHelloWorld = NULL; 

int init()
{
	int success = 1;
	if (SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() ); 
		success = 0; 	
	}
	else
	{
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{ 
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() ); 
			success = 0; 
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}
		return success;
}

int loadMedia()
{
	int success = 1;
	gHelloWorld = SDL_LoadBMP( "hello_world.bmp" ); 
	if( gHelloWorld == NULL)
	{
		printf( "FAILED LOADING IMAGE! %s\n", SDL_GetError() );
		success = 0; 	
	}
	return success;
}

void close()
{
	SDL_FreeSurface( gHelloWorld ); 
	gHelloWorld = NULL; 

	SDL_DestroyWindow( gWindow ); 
	gWindow = NULL;

	SDL_Quit();
}

int main (int argc, char* args[]) 
{
	if (init() == 0)
	{
   		printf("Failed initialize!\n");
    }
	else 
	{
		if( loadMedia() == 0 )
		{
			printf( "Failed loading media!\n"); 
		}
		else
		{
			SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
			SDL_UpdateWindowSurface( gWindow );
			SDL_Delay( 2000 );
		}
	}
	
	close();  	

	return 0;
}

