#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int WALKING_ANIMATION_FRAMES = 4;

struct LTexture{
    SDL_Texture* mTexture = NULL;
	int mWidth;
    int mHeight;
	int imgPath;
};
typedef struct LTexture LTexture;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Rect gSpriteClipsRight[ WALKING_ANIMATION_FRAMES ];
SDL_Rect gSpriteClipsLeft[ WALKING_ANIMATION_FRAMES ];

LTexture gCurrentFoo;
LTexture gBackground;

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

void render(LTexture* s, int x, int y, SDL_Rect* clip )
{
	SDL_Rect renderQuad = { x, y, s->mWidth, s->mHeight };

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy( gRenderer, s->mTexture, clip, &renderQuad );
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

		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
			}
		}
	}

	return success;
}

int loadMediaLeftFoo(LTexture* s)
{
	int success = 1;

	if( !(s->imgPath = loadFromFile( s, "imagens/leftfoo.png" )) )
	{
		printf( "Failed to load left walking animation texture!\n" );
		success = 0;
	}
	else
	{
		gSpriteClipsLeft[ 0 ].x =   0;
		gSpriteClipsLeft[ 0 ].y =   0;
		gSpriteClipsLeft[ 0 ].w =  64;
		gSpriteClipsLeft[ 0 ].h = 205;

		gSpriteClipsLeft[ 1 ].x =  64;
		gSpriteClipsLeft[ 1 ].y =   0;
		gSpriteClipsLeft[ 1 ].w =  64;
		gSpriteClipsLeft[ 1 ].h = 205;

		gSpriteClipsLeft[ 2 ].x = 128;
		gSpriteClipsLeft[ 2 ].y =   0;
		gSpriteClipsLeft[ 2 ].w =  64;
		gSpriteClipsLeft[ 2 ].h = 205;

		gSpriteClipsLeft[ 3 ].x = 196;
		gSpriteClipsLeft[ 3 ].y =   0;
		gSpriteClipsLeft[ 3 ].w =  64;
		gSpriteClipsLeft[ 3 ].h = 205;
	}

	return success;
}

int loadMediaRightFoo(LTexture* s)
{
	int success = 1;

	if( !(s->imgPath = loadFromFile( s, "imagens/rightfoo.png" )) )
	{
		printf( "Failed to load right walking animation texture!\n" );
		success = 0;
	}
	else
	{
        gSpriteClipsRight[ 0 ].x = 196;
		gSpriteClipsRight[ 0 ].y =   0;
		gSpriteClipsRight[ 0 ].w =  64;
		gSpriteClipsRight[ 0 ].h = 205;

		gSpriteClipsRight[ 1 ].x = 128;
		gSpriteClipsRight[ 1 ].y =   0;
		gSpriteClipsRight[ 1 ].w =  64;
		gSpriteClipsRight[ 1 ].h = 205;

		gSpriteClipsRight[ 2 ].x =  64;
		gSpriteClipsRight[ 2 ].y =   0;
		gSpriteClipsRight[ 2 ].w =  64;
		gSpriteClipsRight[ 2 ].h = 205;

		gSpriteClipsRight[ 3 ].x =   0;
		gSpriteClipsRight[ 3 ].y =   0;
		gSpriteClipsRight[ 3 ].w =  64;
		gSpriteClipsRight[ 3 ].h = 205;
	}

	return success;
}

int loadMediaStandFoo(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/standfoo.png")) )
    {
        printf( "Failed to load stand animation texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaBackground(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/background.png")) )
    {
        printf( "Failed to load background texture!\n" );
        success = 0;
    }
    return success;
}

void close()
{
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{

    LTexture gCurrentFoo;
    gCurrentFoo.mHeight = 0;
    gCurrentFoo.mWidth = 0;

    LTexture gRightFoo;
    gRightFoo.mHeight = 0;
    gRightFoo.mWidth = 0;

    LTexture gStandFoo;
    gStandFoo.mHeight = 0;
    gStandFoo.mWidth = 0;

    LTexture gLeftFoo;
    gLeftFoo.mHeight = 0;
    gLeftFoo.mWidth = 0;

    LTexture gBackground;
    gBackground.mHeight = 0;
    gBackground.mWidth = 0;

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if( !loadMediaRightFoo(&gRightFoo) )
		{
			printf( "Failed to load right foo media!\n" );
		}
		if( !loadMediaLeftFoo(&gLeftFoo) )
		{
			printf( "Failed to load left foo media!\n" );
		}
		if( !loadMediaStandFoo(&gStandFoo) )
		{
			printf( "Failed to load stand foo media!\n" );
		}
		if( !loadMediaBackground(&gBackground) )
		{
			printf( "Failed to load background media!\n" );
		}
		else
		{
			int quit = 0;

			SDL_Event e;

			int frame = 0;

			float x = 600;

            int stand = 1;
            int right = 0;
            int left = 0;

            Uint32 oldTime = 0;
            Uint32 currentTime = 0;
            currentTime = SDL_GetTicks();

            float speedfoo = 0;

			gCurrentFoo = gStandFoo;

			while( !quit )
			{
                speedfoo = ((currentTime - oldTime)/1000.0) * 500.0;
                oldTime = currentTime;
				while( SDL_PollEvent( &e ) != 0 )
				{
                    if( e.type == SDL_QUIT )
					{
						quit = 1;
					}
				}

				const Uint8 *keystate = SDL_GetKeyboardState(NULL);

                if( keystate[ SDL_SCANCODE_RIGHT ])
                {
                    gCurrentFoo = gRightFoo;
                    right = 1;
                    left = 0;
                    stand = 0;
                }
                else if( keystate[ SDL_SCANCODE_LEFT ] )
                {
                    gCurrentFoo = gLeftFoo;
                    left = 1;
                    right = 0;
                    stand = 0;
                }

                else{
                    gCurrentFoo = gStandFoo;
                    stand = 1;
                    right = 0;
                    left = 0;
                }
                if ( x >= 1150)
                {
                    x = 1150;
                }
                if (  x <= -10 )
                {
                    x = -10;
                }

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

                render(&gBackground, 0 , 0, NULL);

                if(stand)
                {
                    render(&gCurrentFoo, x, 450, NULL);
                }
                if(right)
                {
                    SDL_Rect* currentClip = &gSpriteClipsRight[ frame / 4 ];
                    render(&gCurrentFoo, x, 450, currentClip );
                    ++frame;
                    if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                    {
                        frame = 0;
                    }
                    x += speedfoo;
                }
                if(left)
                {
                    SDL_Rect* currentClip = &gSpriteClipsLeft[ frame / 4 ];
                    render(&gCurrentFoo, x, 450, currentClip );
                    ++frame;
                    if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                    {
                        frame = 0;
                    }
                    x -= speedfoo;
                }
				SDL_RenderPresent( gRenderer );
                currentTime = SDL_GetTicks();
			}
		}
	}
	close();

	return 0;
}
