#include <stdio.h>
#include <SDL2/SDL.h>
#include <assert.h>
#include <SDL2/SDL_ttf.h>

SDL_Window* gWindow = NULL;

TTF_Font *gFont = NULL;

SDL_Renderer* gRenderer = NULL;

struct LTexture{
    SDL_Texture* mTexture = NULL;
	int mWidth;
    int mHeight;
};

void loadFromRenderedText(LTexture* s, const char* textureText, SDL_Color textColor )
{

	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText, textColor );
	assert( textSurface != NULL );

    if (s->mTexture != NULL) {
		SDL_DestroyTexture(s->mTexture);
	}
	s->mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
	assert( s->mTexture != NULL );

	s->mWidth = textSurface->w;
	s->mHeight = textSurface->h;

	SDL_FreeSurface( textSurface );
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

void loadMediaTime(LTexture* s, int time)
{
	char vc[2];
    sprintf(vc, "%d", time );

	SDL_Color textColor = { 0, 0, 0 };
	loadFromRenderedText(s, vc , textColor );
}

int main( int argc, char* args[] )
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Init( SDL_INIT_VIDEO );

	gWindow = SDL_CreateWindow( "SDL Valgrind", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	assert(gWindow != NULL );

	gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	assert ( gRenderer != NULL );

	assert( TTF_Init() != -1 );
	gFont = TTF_OpenFont( "imagens/lazy.ttf", 35 );
	assert( gFont != NULL );

	LTexture gTextTime;
    gTextTime.mHeight = 0;
    gTextTime.mWidth = 0;

	loadMediaTime(&gTextTime, 0) ;

	int quit = 0;
	SDL_Event e;
	int time = 0;

	Uint32 currentTime = 0;
    Uint32 countTime = 0;



	while( !quit )
   	{
		while( SDL_PollEvent( &e ) != 0 )
       	{
       		if( e.type == SDL_QUIT )
            {
                quit = 1;
            }
		}

		if(currentTime - countTime >= 100)
        {
        	countTime = currentTime;
            time +=1;

			loadMediaTime(&gTextTime, time);
        }

		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

		render(&gTextTime, (SCREEN_WIDTH - 20) / 2,( SCREEN_HEIGHT) / 2, NULL, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent( gRenderer );

		currentTime = SDL_GetTicks();
	}

	TTF_CloseFont( gFont );
	gFont = NULL;

	TTF_Quit();

	SDL_DestroyRenderer( gRenderer );
	gRenderer = NULL;

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	SDL_Quit();
	printf("OK\n");

	return 0;
}
