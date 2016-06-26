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

    LTexture gBoy;
    gBoy.mHeight = 0;
    gBoy.mWidth = 0;

    LTexture gBackground;
    gBackground.mHeight = 0;
    gBackground.mWidth = 0;

    Mouse mouse;
    mouse.mPosition.x = 0;
    mouse.mPosition.y = 0;

    LTexture gTextTimeTexture;
    gTextTimeTexture.mHeight = 0;
    gTextTimeTexture.mWidth = 0;

    LTexture gTextTime;
    gTextTime.mHeight = 0;
    gTextTime.mWidth = 0;

    if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
    if( !loadMediaBoy(&gBoy) )
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
    else
    {
        int quit = 0;

        SDL_Event e;

        int time = 0;

        float x1 = 600;
        float y1 = 450;
        float x2 = 664;
        float y2 = 600;

        int mouseX = 0;
        int mouseY = 0;

        int mouseOver = 0;

        gCurrentFoo = gBoy;

        Uint32 oldTime = 0;
        Uint32 mouseOverTime = 0;
        Uint32 currentTime = 0;
        Uint32 countTime = 0;
        currentTime = SDL_GetTicks();

        while( !quit )
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
                if (e.type == SDL_MOUSEBUTTONDOWN && checkMouse(&mouse, x1, y1, x2, y2))
                {
                    quit = 1;
                }
            }

            if(checkMouse(&mouse, x1, y1, x2, y2))
            {
                if(mouseOver == 0)
                {
                    mouseOver = 1;
                    mouseOverTime = oldTime;
                }
                if( currentTime - mouseOverTime >= 15 )
                {
                    x1 = rand() % 1100;
                    x2 = x1 + 64;
                    mouseOver = 0;
                }
            }
            if(!(checkMouse(&mouse, x1, y1, x2, y2)))
            {
                mouseOverTime = oldTime;
                mouseOver = 0;
            }

            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            render(&gBackground, 0 , 0, NULL, 0, NULL, SDL_FLIP_NONE);

            render(&gCurrentFoo, x1, y1, NULL, 0, NULL, SDL_FLIP_NONE);

            if(currentTime - countTime >= 1000)
            {
                countTime = currentTime;
                time +=1;
                if( !loadMediaTime(&gTextTime, time) )
                {
                    printf( "Failed to load text media!\n" );
                }
            }

            render(&gTextTimeTexture, (SCREEN_WIDTH - getWidth(&gTextTimeTexture) ) / 2.2 ,( SCREEN_HEIGHT - getHeight(&gTextTimeTexture) ) / 1.05 , NULL, 0, NULL, SDL_FLIP_NONE);
            render(&gTextTime, (SCREEN_WIDTH - getWidth(&gTextTime) ) / 1.9,( SCREEN_HEIGHT - getHeight(&gTextTime) ) / 1.05, NULL, 0, NULL, SDL_FLIP_NONE);




            SDL_RenderPresent( gRenderer );
            currentTime = SDL_GetTicks();
            printf("%d\n", currentTime);
        }
    }

    close();

	return 0;
}
