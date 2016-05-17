#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

class LTexture
{
	public:
		LTexture();


        ~LTexture();

		bool loadFromFile( const char* path );

		void free();

		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		void setBlendMode( SDL_BlendMode blending );

		void setAlpha( Uint8 alpha );

		void render( int x, int y, SDL_Rect* clip = NULL );
        void renderbg(int x, int y);

		int getWidth();
		int getHeight();

	private:
		SDL_Texture* mTexture;

		int mWidth;
		int mHeight;
};

enum foos
{
    gStandFoo,
    gRightFoo,
    gLeftFoo,
    gTotalFoo
};

enum enemys
{
    gRightEnemy,
    gLeftEnemy,
    gRightStandEnemy,
	gLeftStandEnemy,
    gTotalEnemy
};
const int WALKING_ANIMATION_FRAMES = 4;

SDL_Rect gSpriteClipsRight[ WALKING_ANIMATION_FRAMES ];
SDL_Rect gSpriteClipsLeft[ WALKING_ANIMATION_FRAMES ];


SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

LTexture foos[gTotalFoo];
LTexture enemys[gTotalEnemy];
LTexture gCurrentFoo;
LTexture gCurrentEnemy;
LTexture gBackground;

LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile( const char* path )
{
	free();

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
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface( loadedSurface );
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

int checkCollision(float x1, float x2)
{
    int collision = 0;
    if ( x2 <= x1 + 60 && x2 >= x1 || x2 >= x1 - 60  && x2 <= x1)
    {
        collision = 1;
    }
    return collision;
}

int checkHit(float x1, float x2, int side ) // side = 1 -> direita | side = 2 -> esquerda
{
    int hit = 0;

    if (side == 1)
    {
        if( x2 <= x1 + 100.0 && x2 >= x1+10)
        {
            hit = 1;
        }
    }
    if (side == 2)
	{
        if( x2 >= x1 - 100.0 && x2 <= x1-10)
        {
            hit = 1;
        }
    }
    return hit;
}

int init()
{
	int success = 1;

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
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

int loadMedia()
{
    int success = 1;

	if( !enemys[gLeftEnemy].loadFromFile( "imagens/leftenemy.png" ) )
	{
		printf( "Failed to load left enemy animation texture!\n" );
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

    if( !enemys[gRightEnemy].loadFromFile( "imagens/rightenemy.png" ) )
	{
		printf( "Failed to load right enemy animation texture!\n" );
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
	if( !enemys[gRightStandEnemy].loadFromFile("imagens/rightstandenemy.png"))
    {
        printf( "Failed to load stand enemy texture!\n" );
		success = 0;
    }
	if( !enemys[gLeftStandEnemy].loadFromFile("imagens/leftstandenemy.png"))
    {
        printf( "Failed to load stand enemy texture!\n" );
		success = 0;
    }
    if(!foos[gStandFoo].loadFromFile("imagens/standfoo.png"))
    {
        printf( "Failed to load stand foo texture!\n" );
		success = 0;
    }

	if( !gBackground.loadFromFile( "imagens/background.png" ) )
	{
		printf( "Failed to load background texture image!\n" );
		success = 0;
	}

	return success;
}

int close()
{
    for( int i = 0; i < gTotalFoo; i++)
    {
        foos[i].free();
    }

    for( int i = 0; i < gTotalEnemy; i++)
    {
        enemys[i].free();
    }

	gBackground.free();

    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;

    SDL_DestroyWindow( gWindow );
	gWindow = NULL;

    IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf("SDL_Init failed: %s\n", SDL_GetError());
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

            int frame = 0;
			
			float speed = 10.0;

            float xf = 600.0;
            float xe = 0.0;
			
			int left = 0;
			int right = 1;           	
			
            gCurrentFoo = foos[gStandFoo];
            gCurrentEnemy = enemys[gRightEnemy];
			
	        while( !quit )
			{              	
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = 1;
					}       
					switch( e.key.keysym.sym )
                	{
		                case SDLK_LEFT:
		                    if( xe >= xf - 100.0 && xe <= xf - 10 )
		                    {   
								
								right = 0;
								left = 1;								
								xe = 1150;
		                    }
		                break;
		                
						case SDLK_RIGHT:	                    
							if( xe <= xf + 100.0 && xe >= xf + 10 )
		                    {   
								right = 1;
								left = 0;
								xe = 0;
		                    }
		                break;
                    }
                }

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				gBackground.render( 0, 0, NULL );
                gCurrentFoo.render(xf, 450, NULL);

                if(checkCollision(xf , xe))
                {
                    if(right)
					{
						gCurrentEnemy = enemys[gRightStandEnemy];
						gCurrentEnemy.render( xe , 450, NULL);
					}
					if(left)
					{
						gCurrentEnemy = enemys[gLeftStandEnemy];
						gCurrentEnemy.render( xe, 450, NULL);
					}
                }
                else
                {                    
					if(right)
					{
						gCurrentEnemy = enemys[gRightEnemy];					
						SDL_Rect* currentClip = &gSpriteClipsRight[ frame / 4 ];
                    	gCurrentEnemy.render( xe, 450, currentClip );
                    	++frame;
                   		if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                   		{
                    	    frame = 0;
                    	}
                    	xe += speed;	
					}
					if(left)
					{
						gCurrentEnemy = enemys[gLeftEnemy];	
						SDL_Rect* currentClip = &gSpriteClipsLeft[ frame / 4 ];
                    	gCurrentEnemy.render( xe, 450, currentClip );
                    	++frame;
                   		if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                   		{
                    	    frame = 0;
                    	}
                    	xe -= speed;
					}
                }

				SDL_RenderPresent( gRenderer );
            }
        }
    }

    close();

    return 0;
}
