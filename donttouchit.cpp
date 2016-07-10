#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

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

TTF_Font *gFont = NULL;

SDL_Texture* newTexture = NULL;

int loadFromFile(LTexture* s, const char* path )
{
	//SDL_Texture* newTexture = NULL;

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

int loadFromRenderedText(LTexture* s, const char* textureText, SDL_Color textColor )
{
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText, textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		s->mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( s->mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			s->mWidth = textSurface->w;
			s->mHeight = textSurface->h;
		}

		SDL_FreeSurface( textSurface );
	}

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

int checkMouse(Mouse* m, float x1, float y1, float x2, float y2)
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

				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = 0;
				}
			}
		}
	}

	return success;
}

int loadMediaBoy(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/boy.png")) )
    {
        printf( "Failed to load boy texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaGirl(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/girl.png")) )
    {
        printf( "Failed to load boy texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaTimeText(LTexture* s)
{
	int success = 1;

	gFont = TTF_OpenFont( "imagens/lazy.ttf", 35 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = 0;
	}
	else
	{
		SDL_Color textColor = { 0, 0, 0 };
		if( !(s->imgPath = loadFromRenderedText(s, "Tempo:", textColor ) ) )
		{
			printf( "Failed to render text texture!\n" );
			success = 0;
		}
	}
	return success;
}

int loadMediaTime(LTexture* s, int time)
{
	int success = 1;

    char vc[2];
    sprintf(vc, "%d", time );

	gFont = TTF_OpenFont( "imagens/lazy.ttf", 35 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = 0;
	}
	else
	{
		SDL_Color textColor = { 0, 0, 0 };
		if( !(s->imgPath = loadFromRenderedText(s, vc , textColor ) ) )
		{
			printf( "Failed to render text texture!\n" );
			success = 0;
		}
	}
	return success;
}

int loadMediaTitle(LTexture* s)
{
	int success = 1;

	gFont = TTF_OpenFont( "imagens/lazy.ttf", 80 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = 0;
	}
	else
	{
		SDL_Color textColor = { 0, 0, 0 };
		if( !(s->imgPath = loadFromRenderedText(s, "Don't Touch It!" , textColor ) ) )
		{
			printf( "Failed to render title text texture!\n" );
			success = 0;
		}
	}
	return success;
}

int loadMediaBackground(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/cozinha.png")) )
    {
        printf( "Failed to load background texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaButton1(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/button1.png")) )
    {
        printf( "Failed to load button1 texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaButton2(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/button2.png")) )
    {
        printf( "Failed to load button2 texture!\n" );
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

    TTF_CloseFont( gFont );
	gFont = NULL;

    SDL_DestroyTexture(newTexture);

    TTF_Quit();
    IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
    LTexture gBoy;
    gBoy.mHeight = 0;
    gBoy.mWidth = 0;

    LTexture gGirl;
    gGirl.mHeight = 0;
    gGirl.mWidth = 0;

    LTexture gBackground;
    gBackground.mHeight = 0;
    gBackground.mWidth = 0;

    Mouse mouse;
    mouse.mPosition.x = 0;
    mouse.mPosition.y = 0;

    LTexture gDifButton1;
    gDifButton1.mHeight = 0;
    gDifButton1.mWidth = 0;

    LTexture gDifButton2;
    gDifButton2.mHeight = 0;
    gDifButton2.mWidth = 0;

    LTexture gTextTimeTexture;
    gTextTimeTexture.mHeight = 0;
    gTextTimeTexture.mWidth = 0;

    LTexture gTextTime;
    gTextTime.mHeight = 0;
    gTextTime.mWidth = 0;

    LTexture gTextTitle;
    gTextTitle.mHeight = 0;
    gTextTitle.mWidth = 0;

    if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
    if( !loadMediaBoy(&gBoy) )
    {
        printf( "Failed to load boy media!\n" );
    }
    if( !loadMediaGirl(&gGirl) )
    {
        printf( "Failed to load boy media!\n" );
    }
    if( !loadMediaTimeText(&gTextTimeTexture) )
    {
        printf( "Failed to load text time media!\n" );
    }
    if( !loadMediaTime(&gTextTime, 0) )
    {
        printf( "Failed to load time media!\n" );
    }
    if( !loadMediaBackground(&gBackground) )
    {
        printf( "Failed to load background media!\n" );
    }
    if( !loadMediaButton1(&gDifButton1) )
    {
        printf( "Failed to load start button media!\n" );
    }
    if( !loadMediaButton2(&gDifButton2) )
    {
        printf( "Failed to load start button media!\n" );
    }
    if( !loadMediaTitle(&gTextTitle) )
    {
        printf( "Failed to load text media!\n" );
    }
    else
    {
        int quit = 0;

        SDL_Event e;

        int time = 0;

        int startGame = 0;

        int placeB = rand() % 6;
        int placeG = rand() % 6;

        int gottenChild = 0;

        int dificuldade = 0;

        float bx1 ;
        float by1 = 450;
        float bx2 = bx1 + 64;
        float by2 = by1 + 150;

        float gx1;
        float gy1 = 450;
        float gx2 = gx1 + 64;
        float gy2 = gy1 + 150;

        int mouseX = 0;
        int mouseY = 0;

        int mouseOverBoy = 0;
        int mouseOverGirl = 0;


        //gCurrentBoy = gBoy;
        //gCurrentGirl = gGirl;

        float button1x1 = SCREEN_WIDTH / 3.75;
        float button1y1 = SCREEN_HEIGHT / 3.75;
        float button1x2 = button1x1 + 550;
        float button1y2 = button1y1 + 200;

        float button2x1 = SCREEN_WIDTH / 3.75;
        float button2y1 = (SCREEN_HEIGHT / 3.75) + 250.0;
        float button2x2 = button2x1 + 550;
        float button2y2 = button2y1 + 200;

        Uint32 oldTime = 0;
        //Uint32 mouseOverBoyTime = 0;
        //Uint32 mouseOverGirlTime = 0;
        Uint32 currentTime = 0;
        Uint32 countTime = 0;
        Uint32 endGame = -1000;
        currentTime = SDL_GetTicks();

        while( !quit )
        {
            if( startGame == 0 )
            {
                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        if( e.type == SDL_QUIT )
                        {
                            quit = 1;
                        }
                        if(currentTime - endGame >= 500)
                        {
                            if (e.type == SDL_MOUSEBUTTONUP)
                            {
                                SDL_GetMouseState(&mouseX, &mouseY);
                                mouse.mPosition.x = mouseX;
                                mouse.mPosition.y = mouseY;
                                if(checkMouse(&mouse, button1x1, button1y1, button1x2, button1y2))
                                {
                                    //Mix_PlayChannel( -1, gButtonSound, 0 );

                                    startGame = 1;

                                    dificuldade = 1;

                                    gottenChild = 1;
                                    time = -1;

                                    placeB = rand() % 6;
                                    by1 = 450;
                                    by2 = by1 + 150;
                                }
                                else if(checkMouse(&mouse, button2x1, button2y1, button2x2, button2y2))
                                {
                                    //Mix_PlayChannel( -1, gButtonSound, 0 );

                                    startGame = 1;

                                    dificuldade = 2;

                                    gottenChild = 2;
                                    time = -1;

                                    placeB = rand() % 6;
                                    placeG = rand() % 6;

                                    by1 = 450;
                                    by2 = by1 + 150;

                                    gy1 = 450;
                                    gy2 = gy1 + 150;
                                }

                            }
                        }
                    }

                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderClear( gRenderer );

                    render(&gTextTitle, (SCREEN_WIDTH - getWidth(&gTextTitle) ) / 2,( SCREEN_HEIGHT - getHeight(&gTextTitle) ) / 10 , NULL, 0, NULL, SDL_FLIP_NONE);

                    render(&gDifButton1, button1x1 , button1y1, NULL, 0, NULL, SDL_FLIP_NONE);
                    render(&gDifButton2, button2x1 , button2y1, NULL, 0, NULL, SDL_FLIP_NONE);

                    render(&gTextTimeTexture, (SCREEN_WIDTH - getWidth(&gTextTimeTexture) ) / 2.2 ,( SCREEN_HEIGHT - getHeight(&gTextTimeTexture) ) / 1.05 , NULL, 0, NULL, SDL_FLIP_NONE);
                    render(&gTextTime, (SCREEN_WIDTH - getWidth(&gTextTime) ) / 1.8,( SCREEN_HEIGHT - getHeight(&gTextTime) ) / 1.05, NULL, 0, NULL, SDL_FLIP_NONE);

                    SDL_RenderPresent( gRenderer );
            }

            else if (startGame == 1)
            {
                oldTime = currentTime;
                while( SDL_PollEvent( &e ) != 0)
                {
                    if( e.type == SDL_QUIT )
                    {
                        quit = 1;
                    }
                    SDL_GetMouseState(&mouseX, &mouseY);
                    mouse.mPosition.x = mouseX;
                    mouse.mPosition.y = mouseY;
                    if (e.type == SDL_MOUSEBUTTONDOWN && checkMouse(&mouse, bx1, by1, bx2, by2))
                    {
                        gottenChild -= 1;

                        by1 = 1000;
                        by2 = 1000;

                    }
                    if (e.type == SDL_MOUSEBUTTONDOWN && checkMouse(&mouse, gx1, gy1, gx2, gy2))
                    {
                        gottenChild -= 1;

                        gy1 = 1000;
                        gy2 = 1000;

                    }

                }

                if(checkMouse(&mouse, bx1, by1, bx2, by2))
                {
                    //if(mouseOverBoy == 0)
                    //{
                        //mouseOverBoy = 1;
                        //mouseOverBoyTime = oldTime;
                    //}
                    //if( currentTime - mouseOverBoyTime >= 2 )
                    //{
                        placeB = rand() % 6;
                        if(placeB == placeG)
                        {
                            placeB = rand() % 6;
                        }
                        //mouseOverBoy = 0;
                    //}
                }
                /*if(!(checkMouse(&mouse, bx1, by1, bx2, by2)))
                {
                    mouseOverBoyTime = oldTime;
                    mouseOverBoy = 0;
                }*/

                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                render(&gBackground, 0 , 0, NULL, 0, NULL, SDL_FLIP_NONE);

                switch( placeB )
                {
                    case 0:
                    bx1 = 25;

                    break;

                    case 1:
                    bx1 = 210;
                    break;

                    case 2:
                    bx1 = 400;
                    break;

                    case 3:
                    bx1 = 600;
                    break;

                    case 4:
                    bx1 = 700;
                    break;

                    case 5:
                    bx1 = 980;
                    break;

                    case 6:
                    bx1 = 1080;
                    break;
                }

                bx2 = bx1 + 64;

                render(&gBoy, bx1, by1, NULL, 0, NULL, SDL_FLIP_NONE);

                if(dificuldade == 2)
                {
                    if(checkMouse(&mouse, gx1, gy1, gx2, gy2))
                    {
                        //if(mouseOverGirl == 0)
                        //{
                            //mouseOverGirl = 1;
                            //mouseOverGirlTime = oldTime;
                        //}
                        //if( currentTime - mouseOverGirlTime >= 1 )
                        //{
                            placeG = rand() % 6;
                            if(placeG == placeB)
                            {
                                placeG = rand() % 6;
                            }
                            //mouseOverGirl = 0;
                        //}
                    }
                    /*if(!(checkMouse(&mouse, gx1, gy1, gx2, gy2)))
                    {
                        mouseOverGirlTime = oldTime;
                        mouseOverGirl = 0;
                    }*/

                    switch( placeG )
                    {
                        case 0:
                        gx1 = 25;

                        break;

                        case 1:
                        gx1 = 210;
                        break;

                        case 2:
                        gx1 = 400;
                        break;

                        case 3:
                        gx1 = 600;
                        break;

                        case 4:
                        gx1 = 700;
                        break;

                        case 5:
                        gx1 = 980;
                        break;

                        case 6:
                        gx1 = 1080;
                        break;
                    }
                    gx2 = gx1 + 64;

                    render(&gGirl, gx1, gy1, NULL, 0, NULL, SDL_FLIP_NONE);
                }

                if( gottenChild == 0 )
                {
                    startGame = 0;
                    endGame = currentTime;
                }

                if(currentTime - countTime >= 1000)
                {
                    countTime = currentTime;
                    time +=1;
                    if( !loadMediaTime(&gTextTime, time) )
                    {
                        printf( "Failed to load time text media!\n" );
                    }
                }

                render(&gTextTimeTexture, (SCREEN_WIDTH - getWidth(&gTextTimeTexture) ) / 2.2 ,( SCREEN_HEIGHT - getHeight(&gTextTimeTexture) ) / 1.05 , NULL, 0, NULL, SDL_FLIP_NONE);
                render(&gTextTime, (SCREEN_WIDTH - getWidth(&gTextTime) ) / 1.9,( SCREEN_HEIGHT - getHeight(&gTextTime) ) / 1.05, NULL, 0, NULL, SDL_FLIP_NONE);

                SDL_RenderPresent( gRenderer );

            }
        currentTime = SDL_GetTicks();
        }
    }

    SDL_DestroyTexture(gBoy.mTexture);
    //free(gBoy.mTexture);
    SDL_DestroyTexture(gGirl.mTexture);
    //free(gGirl.mTexture);
    SDL_DestroyTexture(gBackground.mTexture);
    //free(gBackground.mTexture);
    SDL_DestroyTexture(gDifButton1.mTexture);
    //free(gDifButton1.mTexture);
    SDL_DestroyTexture(gDifButton2.mTexture);
    //free(gDifButton2.mTexture);
    SDL_DestroyTexture(gTextTimeTexture.mTexture);
    //free(gTextTimeTexture.mTexture);
    SDL_DestroyTexture(gTextTime.mTexture);
    //free(gTextTime.mTexture);
    SDL_DestroyTexture(gTextTitle.mTexture);
    //free(gTextTitle.mTexture);

    close();

	return 0;
}


