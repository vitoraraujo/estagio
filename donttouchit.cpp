#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

struct LTexture{
    SDL_Texture* mTexture = NULL;
	int mWidth;
    int mHeight;
};
typedef struct LTexture LTexture;

struct Mouse{
    SDL_Point mPosition;
};
typedef struct Mouse Mouse;

Mouse mouse;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font *gFont = NULL;

int loadFromFile(LTexture* s, const char* path )
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path );
	assert( loadedSurface != NULL );

    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    assert( newTexture != NULL );

    s->mWidth = loadedSurface->w;
    s->mHeight = loadedSurface->h;

	SDL_FreeSurface( loadedSurface );

    if (s->mTexture != NULL) {
		SDL_DestroyTexture(s->mTexture);
	}
	s->mTexture = newTexture;

	return s->mTexture != NULL;
}

int loadFromRenderedText(LTexture* s, const char* textureText, SDL_Color textColor )
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

void init()
{
  	SDL_Init( SDL_INIT_VIDEO );

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

    gWindow = SDL_CreateWindow( "SDL GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    assert( gWindow != NULL );

    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	assert( gRenderer != NULL );

    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    int imgFlags = IMG_INIT_PNG;
    assert( IMG_Init( imgFlags ) & imgFlags );

    assert( TTF_Init() != -1 );
    gFont = TTF_OpenFont( "imagens/lazy.ttf", 50 );
    assert( gFont != NULL );
}

void close()
{
    TTF_CloseFont( gFont );
	gFont = NULL;

    TTF_Quit();

    IMG_Quit();

    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

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

    Mouse mouse;
    mouse.mPosition.x = 0;
    mouse.mPosition.y = 0;

    init();

    SDL_Color textColor = { 0, 0, 0 };

    assert(loadFromFile(&gBoy, "imagens/boy.png"));
    assert(gBoy.mTexture != NULL);

    assert(loadFromFile(&gGirl, "imagens/girl.png"));
    assert(gGirl.mTexture != NULL);

    assert(loadFromRenderedText(&gTextTimeTexture, "Tempo:", textColor ) );
    assert(gTextTimeTexture.mTexture != NULL);

    char vc[2];
    sprintf(vc, "%d", 0);
    assert(loadFromRenderedText(&gTextTime, vc , textColor ) );
    assert(gTextTime.mTexture != NULL);

    assert(loadFromFile(&gBackground, "imagens/cozinha.png"));
    assert(gBackground.mTexture != NULL);

    assert(loadFromFile(&gDifButton1, "imagens/button1.png"));
    assert(gDifButton1.mTexture != NULL);

    assert(loadFromFile(&gDifButton2, "imagens/button2.png"));
    assert(gDifButton2.mTexture != NULL);

    assert(loadFromRenderedText(&gTextTitle, "Don't Touch It!" , textColor ) );
    assert(gTextTitle.mTexture != NULL);

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

    float button1x1 = SCREEN_WIDTH / 3.75;
    float button1y1 = SCREEN_HEIGHT / 3.75;
    float button1x2 = button1x1 + 550;
    float button1y2 = button1y1 + 200;

    float button2x1 = SCREEN_WIDTH / 3.75;
    float button2y1 = (SCREEN_HEIGHT / 3.75) + 250.0;
    float button2x2 = button2x1 + 550;
    float button2y2 = button2y1 + 200;

    Uint32 oldTime = 0;
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

            render(&gTextTitle, (SCREEN_WIDTH - gTextTitle.mWidth ) / 2,( SCREEN_HEIGHT - gTextTitle.mHeight ) / 10 , NULL, 0, NULL, SDL_FLIP_NONE);

            render(&gDifButton1, button1x1 , button1y1, NULL, 0, NULL, SDL_FLIP_NONE);
            render(&gDifButton2, button2x1 , button2y1, NULL, 0, NULL, SDL_FLIP_NONE);

            render(&gTextTimeTexture, (SCREEN_WIDTH - gTextTimeTexture.mWidth ) / 2.3 ,( SCREEN_HEIGHT - gTextTimeTexture.mHeight ) / 1.05 , NULL, 0, NULL, SDL_FLIP_NONE);
            render(&gTextTime, (SCREEN_WIDTH - gTextTime.mWidth ) / 1.8,( SCREEN_HEIGHT - gTextTime.mHeight ) / 1.05, NULL, 0, NULL, SDL_FLIP_NONE);

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
                placeB = rand() % 6;
                if(placeB == placeG)
                {
                    placeB = rand() % 6;
                }
            }
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
                    placeG = rand() % 6;
                    if(placeG == placeB)
                    {
                        placeG = rand() % 6;
                    }
                }
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
                sprintf(vc, "%d", time );
                assert( loadFromRenderedText(&gTextTime, vc , textColor ) );
            }
            render(&gTextTimeTexture, (SCREEN_WIDTH - gTextTimeTexture.mWidth ) / 2.3 ,( SCREEN_HEIGHT - gTextTimeTexture.mHeight ) / 1.05 , NULL, 0, NULL, SDL_FLIP_NONE);

            render(&gTextTime, (SCREEN_WIDTH - gTextTime.mWidth ) / 1.8,( SCREEN_HEIGHT - gTextTime.mHeight ) / 1.05, NULL, 0, NULL, SDL_FLIP_NONE);

            SDL_RenderPresent( gRenderer );
        }

    currentTime = SDL_GetTicks();

    }

    SDL_DestroyTexture(gBoy.mTexture);
    SDL_DestroyTexture(gGirl.mTexture);
    SDL_DestroyTexture(gBackground.mTexture);
    SDL_DestroyTexture(gDifButton1.mTexture);
    SDL_DestroyTexture(gDifButton2.mTexture);
    SDL_DestroyTexture(gTextTimeTexture.mTexture);
    SDL_DestroyTexture(gTextTime.mTexture);
    SDL_DestroyTexture(gTextTitle.mTexture);

    close();

	return 0;
}


