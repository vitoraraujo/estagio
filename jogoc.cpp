#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int WALKING_ANIMATION_FRAMES = 4;

const int BUTTON_HEIGHT = 150;
const int BUTTON_WIDTH = 300;

struct LTexture{
    SDL_Texture* mTexture = NULL;
	int mWidth;
    int mHeight;
	int imgPath;
};
typedef struct LTexture LTexture;

struct Mouse{
    SDL_Point mPosition;
};

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Rect gSpriteClipsRight[ WALKING_ANIMATION_FRAMES ];
SDL_Rect gSpriteClipsLeft[ WALKING_ANIMATION_FRAMES ];

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

int clickButton(Mouse* m, float x1, float y1, float x2, float y2)
{
    if ( m->mPosition.x >= x1 && m->mPosition.x <= x2 && m->mPosition.y >= y1 && m->mPosition.y <= y2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int checkCollision(float x1, float x2, float y1, float y2)
{
    int collision = 0;
    if ( x2 <= x1 + 60 && x2 >= x1 && y2 <= y1+100 && y2 >= y1 - 50  || x2 >= x1 - 60  && x2 <= x1 && y2 <= y1 + 100 && y2 >= y1 - 50)
    {
        collision = 1;
    }
    return collision;
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

int loadMediaLeftEnemy(LTexture* s)
{
	int success = 1;

	if( !(s->imgPath = loadFromFile( s, "imagens/leftenemy.png" )) )
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

int loadMediaRightEnemy(LTexture* s)
{
	int success = 1;

	if( !(s->imgPath = loadFromFile( s, "imagens/rightenemy.png" )) )
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

int loadMediaRightStandEnemy(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/rightstandenemy.png")) )
    {
        printf( "Failed to load stand animation texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaLeftStandEnemy(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/leftstandenemy.png")) )
    {
        printf( "Failed to load stand animation texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaLeftPunch(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/punchleft.png")) )
    {
        printf( "Failed to load left punch animation texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaRightPunch(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/punchright.png")) )
    {
        printf( "Failed to load right punch animation texture!\n" );
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

    LTexture gCurrentEnemy;
    gCurrentEnemy.mHeight = 0;
    gCurrentEnemy.mWidth = 0;

    LTexture gRightStandEnemy;
    gRightStandEnemy.mHeight = 0;
    gRightStandEnemy.mWidth = 0;

    LTexture gLeftStandEnemy;
    gLeftStandEnemy.mHeight = 0;
    gLeftStandEnemy.mWidth = 0;

    LTexture gRightEnemy;
    gLeftStandEnemy.mHeight = 0;
    gLeftStandEnemy.mWidth = 0;

    LTexture gLeftEnemy;
    gLeftStandEnemy.mHeight = 0;
    gLeftStandEnemy.mWidth = 0;

    LTexture gLeftPunch;
    gLeftPunch.mHeight = 0;
    gLeftPunch.mWidth = 0;

    LTexture gRightPunch;
    gRightPunch.mHeight = 0;
    gRightPunch.mWidth = 0;

    LTexture gBackground;
    gBackground.mHeight = 0;
    gBackground.mWidth = 0;

    Mouse mouse;
    mouse.mPosition.x = 0;
    mouse.mPosition.y = 0;

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
        if( !loadMediaRightEnemy(&gRightEnemy) )
		{
			printf( "Failed to load right enemy media!\n" );
		}
		if( !loadMediaLeftEnemy(&gLeftEnemy) )
		{
			printf( "Failed to load left enemy media!\n" );
		}
        if( !loadMediaRightStandEnemy(&gRightStandEnemy) )
		{
			printf( "Failed to load right stand enemy media!\n" );
		}
		if( !loadMediaRightPunch(&gRightPunch) )
		{
			printf( "Failed to load right stand enemy media!\n" );
		}
		if( !loadMediaLeftPunch(&gLeftPunch) )
		{
			printf( "Failed to load right stand enemy media!\n" );
		}
		if( !loadMediaLeftStandEnemy(&gLeftStandEnemy) )
		{
			printf( "Failed to load left stand enemy media!\n" );
		}

		else
		{
			int quit = 0;

			SDL_Event e;

			int frame = 0;

            int startGame = 0;

            int r = 0;

			float xf = 600;
            float yf = 450;
            float xe = 0.0;
            float ye = 450;

            float speedenemy = 0;
            float speedfoo = 0;
            float speedjump = 0;

            int rightenemy = 1;
            int leftenemy = 0;
            int rightpunch = 0;
            int leftpunch = 0;

            int jumpfoo = 0;
            int standfoo = 1;
            int rightfoo = 0;
            int leftfoo = 0;

			gCurrentFoo = gStandFoo;
            gCurrentEnemy = gRightEnemy;

            Uint32 jumptime = 0;
            Uint32 oldTime = 0;
            Uint32 currentTime = 0;
            currentTime = SDL_GetTicks();

            float x1 = SCREEN_WIDTH / 4;
            float y1 = SCREEN_HEIGHT / 4;
            float x2 = SCREEN_WIDTH / 2;
            float y2 = SCREEN_HEIGHT / 2;

            int mx = 0;
            int my = 0;

			while( !quit )
			{
                if( startGame == 0)
                {
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        if( e.type == SDL_QUIT )
                        {
                            quit = 1;
                        }
                        if (e.type == SDL_MOUSEBUTTONUP)
                        {
                            SDL_GetMouseState(&mx, &my);
                            mouse.mPosition.x = mx;
                            mouse.mPosition.y = my;
                            startGame = clickButton(&mouse, x1, y1, x2, y2);
                        }
                    }

                    SDL_Rect fillRect = { x1, y1, x2, y2 };
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                    SDL_RenderFillRect( gRenderer, &fillRect );
                    SDL_RenderPresent( gRenderer );

                }
                else
                {
                    speedenemy = ((currentTime - oldTime)/1000.0) * 600.0;
                    speedfoo = ((currentTime - oldTime)/1000.0) * 500.0;
                    oldTime = currentTime;

                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        if( e.type == SDL_QUIT )
                        {
                            quit = 1;
                        }
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_a:

                                leftpunch = 1;
                                rightpunch = 0;

                                if( xe >= xf - 80.0 && xe <= xf - 10 && ye == yf)
                                {
                                    r = rand() % 2;

                                    switch (r)
                                    {
                                        case 0:
                                        rightenemy = 0;
                                        leftenemy = 1;
                                        xe = 1150;
                                        break;

                                        case 1:
                                        rightenemy = 1;
                                        leftenemy = 0;
                                        xe = 0;
                                        break;
                                    }
                                }
                            break;

                            case SDLK_d:

                                rightpunch = 1;
                                leftpunch = 0;
                                if( xe <= xf + 100.0 && xe >= xf + 10 && ye == yf )
                                {
                                    r = rand() % 2;

                                    switch (r)
                                    {
                                        case 0:
                                        rightenemy = 0;
                                        leftenemy = 1;
                                        xe = 1150;
                                        break;

                                        case 1:
                                        rightenemy = 1;
                                        leftenemy = 0;
                                        xe = 0;
                                        break;
                                    }
                                }
                            break;
                        }
                    }

                    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

                    if( jumpfoo == 0 )
                    {
                        if( keystate[ SDL_SCANCODE_UP])
                        {
                            jumptime = currentTime;
                            jumpfoo = 1;
                            leftpunch = 0;
                            rightpunch = 0;
                        }
                    }
                    if( keystate[ SDL_SCANCODE_RIGHT ])
                    {
                        gCurrentFoo = gRightFoo;
                        rightfoo = 1;
                        leftfoo = 0;
                        standfoo = 0;
                        leftpunch = 0;
                        rightpunch = 0;
                    }
                    else if( keystate[ SDL_SCANCODE_LEFT ] )
                    {
                        gCurrentFoo = gLeftFoo;
                        leftfoo = 1;
                        rightfoo = 0;
                        standfoo = 0;
                        leftpunch = 0;
                        rightpunch = 0;
                    }
                    else if(rightpunch)
                    {
                        gCurrentFoo = gRightPunch;
                        standfoo = 1;
                        rightfoo = 0;
                        leftfoo = 0;
                        leftpunch = 0;
                    }
                    else if(leftpunch)
                    {
                        gCurrentFoo = gLeftPunch;
                        standfoo = 1;
                        rightfoo = 0;
                        leftfoo = 0;
                        rightpunch = 0;
                    }

                    else
                    {
                        gCurrentFoo = gStandFoo;
                        standfoo = 1;
                        rightfoo = 0;
                        leftfoo = 0;
                        rightpunch = 0;
                        leftpunch = 0;
                    }

                    if ( xf >= 1150 )
                    {
                        xf = 1150;
                    }
                    if (  xf <= -10 )
                    {
                        xf = -10;
                    }

                    if ( xe > 1150)
                    {
                        rightenemy = 0;
                        leftenemy = 1;
                        xe = 1150;
                    }
                    if (  xe < 0 )
                    {
                        rightenemy = 1;
                        leftenemy = 0;
                        xe = 0;
                    }


                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderClear( gRenderer );

                    render(&gBackground, 0 , 0, NULL);

                    if(jumpfoo)
                    {
                        speedjump = 30;
                        speedjump -= 0.0008*((currentTime - jumptime))*((currentTime - jumptime));
                        yf -= speedjump;
                        if(speedjump <= 0)
                        {
                            speedjump += 0.0008*((currentTime - jumptime))*((currentTime - jumptime));
                            if( yf >= 450)
                            {
                                yf = 450;
                                jumpfoo = 0;
                            }
                        }
                    }
                    if(standfoo || rightpunch || leftpunch)
                    {
                        render(&gCurrentFoo, xf, yf, NULL);
                    }
                    if(rightfoo)
                    {
                        SDL_Rect* currentClip = &gSpriteClipsRight[ frame / 4 ];
                        render(&gCurrentFoo, xf, yf, currentClip );
                        ++frame;
                        if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                        {
                            frame = 0;
                        }
                        xf += speedfoo;
                    }
                    if(leftfoo)
                    {
                        SDL_Rect* currentClip = &gSpriteClipsLeft[ frame / 4 ];
                        render(&gCurrentFoo, xf, yf, currentClip );
                        ++frame;
                        if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                        {
                            frame = 0;
                        }
                        xf -= speedfoo;

                    }
                    if(checkCollision(xf , xe, yf, ye))
                    {
                        if(rightenemy)
                        {
                            gCurrentEnemy = gRightStandEnemy;
                            render(&gCurrentEnemy, xe , ye, NULL);
                            if(xf <= xe + 20)
                            {
                                xf += speedfoo;
                            }
                        }
                        if(leftenemy)
                        {
                            gCurrentEnemy = gLeftStandEnemy;
                            render(&gCurrentEnemy, xe, ye, NULL);
                            if(xf >= xe - 20)
                            {
                                xf -= speedfoo;
                            }
                        }
                    }
                    else
                    {
                        if(rightenemy)
                        {
                            gCurrentEnemy = gRightEnemy;
                            SDL_Rect* currentClip = &gSpriteClipsRight[ frame / 4 ];
                            render(&gCurrentEnemy, xe, ye, currentClip );
                            ++frame;
                            if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                            {
                                frame = 0;
                            }
                            xe += speedenemy;
                        }
                        if(leftenemy)
                        {
                            gCurrentEnemy = gLeftEnemy;
                            SDL_Rect* currentClip = &gSpriteClipsLeft[ frame / 4 ];
                            render(&gCurrentEnemy, xe, ye, currentClip );
                            ++frame;
                            if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                            {
                                frame = 0;
                            }
                            xe -= speedenemy;
                        }
                    }


                    SDL_RenderPresent( gRenderer );
                    currentTime = SDL_GetTicks();


                }
			}
		}
	}
	close();

	return 0;
}
