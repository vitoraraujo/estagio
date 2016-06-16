#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

struct LTexture{
    SDL_Texture* mTexture = NULL;
	int mWidth;
    int mHeight;
	int imgPath;
};
typedef struct LTexture LTexture;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

Mix_Music *gMusic = NULL;

Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

int loadFromFile(LTexture* s, const char* path )
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path );

	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
	else
	{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		}
		else
		{
			s->mWidth = loadedSurface->w;
			s->mHeight = loadedSurface->h;
		}

		SDL_FreeSurface( loadedSurface );
	}

	s->mTexture = newTexture;
	return s->mTexture != NULL;
}

void setColor(LTexture* s, Uint8 red, Uint8 green, Uint8 blue )
{
	SDL_SetTextureColorMod( s->mTexture, red, green, blue );
}

void setBlendMode(LTexture* s, SDL_BlendMode blending )
{
	SDL_SetTextureBlendMode(s->mTexture, blending );
}

void setAlpha(LTexture* s, Uint8 alpha )
{
	SDL_SetTextureAlphaMod( s->mTexture, alpha );
}

void render(LTexture* s, int x, int y, SDL_Rect* clip,  double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	SDL_Rect renderQuad = { x, y, s->mWidth, s->mHeight };

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx( gRenderer, s->mTexture, clip, &renderQuad, angle, center, flip );
}

int getWidth(LTexture* s)
{
	return s->mWidth;
}

int getHeight(LTexture* s)
{
	return s->mHeight;
}

int init()
{
	int  success = 1;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = 0;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "SDL GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = 0;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = 0;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = 0;
				}

				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

int loadMediaTela(LTexture* s)
{
	int success = 1;

	if( !(s->imgPath = loadFromFile( s, "imagens/somtela.png" )) )
	{
		printf( "Falha para carregar imagem da tela!\n" );
		success = 0;
	}
	return success;
}

int loadMediaSound()
{
    int success = 1;

    gMusic = Mix_LoadMUS( "sons/backsong.wav" );
	if( gMusic == NULL )
	{
		printf( "Falha ao carregar a música! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}
	gScratch = Mix_LoadWAV( "sons/scratch.wav" );
	if( gScratch == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}

	gHigh = Mix_LoadWAV( "sons/high.wav" );
	if( gHigh == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}

	gMedium = Mix_LoadWAV( "sons/medium.wav" );
	if( gMedium == NULL )
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}

	gLow = Mix_LoadWAV( "sons/low.wav" );
	if( gLow == NULL )
	{
		printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}

    return success;
}

void close()
{
    Mix_FreeMusic( gMusic );
	gMusic = NULL;

    Mix_FreeChunk( gScratch );
	Mix_FreeChunk( gHigh );
	Mix_FreeChunk( gMedium );
	Mix_FreeChunk( gLow );
	gScratch = NULL;
	gHigh = NULL;
	gMedium = NULL;
	gLow = NULL;

	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

    Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main()
{
    LTexture gScreen;
    gScreen.mHeight = 0;
    gScreen.mWidth = 0;

    if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
        if( !loadMediaTela(&gScreen) )
		{
			printf( "Falha ao carregar a tela!\n" );
		}
		if (!loadMediaSound())
		{
            printf("Falha ao carregar a musica!\n");
		}
        else
        {
            int quit = 0;

			SDL_Event e;

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
							case SDLK_1:
							Mix_PlayChannel( -1, gHigh, 0 );
							break;

							case SDLK_2:
							Mix_PlayChannel( -1, gMedium, 0 );
							break;

							case SDLK_3:
							Mix_PlayChannel( -1, gLow, 0 );
							break;

							case SDLK_4:
							Mix_PlayChannel( -1, gScratch, 0 );
							break;

							case SDLK_9:
							if( Mix_PlayingMusic() == 0 )
							{
								Mix_PlayMusic( gMusic, -1 );
							}
							else
							{
								if( Mix_PausedMusic() == 1 )
								{
									Mix_ResumeMusic();
								}
								else
								{
									Mix_PauseMusic();
								}
							}
							break;

							case SDLK_0:
							Mix_HaltMusic();
							break;
						}
					}
				}

                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                render(&gScreen, 0, 0, NULL, 0, NULL, SDL_FLIP_NONE);

                SDL_RenderPresent( gRenderer );
            }
        }
    }

    close();

    return 0;
}

