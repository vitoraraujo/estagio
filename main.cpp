#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

enum teclas
{
    tecla_default,
    tecla_cima,
    tecla_baixo,
    tecla_esquerda,
    tecla_direita,
    tecla_total
};

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* teclas[ tecla_total ];
SDL_Surface* gCurrentSurface = NULL;
void *__gxx_personality_v0;

SDL_Surface* loadSurface( const char* path )
{
    SDL_Surface* optimizedSurface = NULL;
    SDL_Surface* loadedSurface = IMG_Load( path );
    if( loadedSurface == NULL )
    {
        printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
    }
   else
	{
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
		}
		SDL_FreeSurface( loadedSurface );
	}
	return optimizedSurface;
}

int init()
{
	int success = 1;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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
            int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				success = 0;
			}
			else
			{
                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
		}
	}
	return success;
}

int loadMedia()
{
    int success = 1;
    teclas[ tecla_default ] = loadSurface( "imagens/press.png" );
    if( teclas[ tecla_default ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = 0;
    }
    teclas[ tecla_cima ] = loadSurface( "imagens/up.png" );
    if(teclas[tecla_cima] == NULL)
    {
        printf( "Failed to load up image!\n" );
        success = 0;
    }
    teclas[ tecla_baixo ] = loadSurface( "imagens/down.png" );
    if(teclas[tecla_baixo] == NULL)
    {
        printf("Failed to load down image!\n");
    }
    teclas[ tecla_esquerda ] = loadSurface( "imagens/left.png" );
    if( teclas[ tecla_esquerda ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = 0;
    }
    teclas[ tecla_direita ] = loadSurface( "imagens/right.png" );
    if(teclas[tecla_direita] == NULL)
    {
        printf("Failed to load right image!\n");
        success = 0;
    }
    return success;
}

void close()
{
    for( int i = 0; i < tecla_total; i++ )
        {
            SDL_FreeSurface( teclas[ i ] );
            teclas[ i ] = NULL;
        }

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	SDL_Quit();

}

int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			int quit = 0;
			SDL_Event e;
            gCurrentSurface = teclas[ tecla_default ];
			while( !quit )
			{
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = 1;
					}
                    else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_UP:
                            gCurrentSurface = teclas[ tecla_cima ];
                            break;

                            case SDLK_DOWN:
                            gCurrentSurface = teclas[ tecla_baixo ];
                            break;

                            case SDLK_LEFT:
                            gCurrentSurface = teclas[ tecla_esquerda];
                            break;

                            case SDLK_RIGHT:
                            gCurrentSurface = teclas[ tecla_direita ];
                            break;

                            default:
                            gCurrentSurface = teclas[ tecla_default ];
                            break;
                        }

                    }

                }
                SDL_Rect stretchRect;
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;
				SDL_BlitScaled( gCurrentSurface, NULL, gScreenSurface, &stretchRect );

				SDL_UpdateWindowSurface( gWindow );
            }
        }
    }
	close();

	return 0;
}
