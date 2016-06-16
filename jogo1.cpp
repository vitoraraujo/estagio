#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

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

TTF_Font *gFont = NULL;

Mix_Music *gMusic = NULL;

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
    if ( x2 <= x1 + 60 && x2 >= x1 && y2 <= y1+100 && y2 >= y1 - 60  || x2 >= x1 - 80  && x2 <= x1 && y2 <= y1 + 100 && y2 >= y1 - 50)
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

				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = 0;
				}
			}
		}
	}

	return success;
}

int loadMediaMusic()
{
    int success = 1;

    gMusic = Mix_LoadMUS( "sons/backsound.wav" );
	if( gMusic == NULL )
	{
		printf( "Falha ao carregar a música! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}
    return success;
}

int loadMediaHealthText(LTexture* s)
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
		if( !(s->imgPath = loadFromRenderedText(s, "HP:", textColor ) ) )
		{
			printf( "Failed to render text texture!\n" );
			success = 0;
		}
	}
	return success;
}


int loadMediaHealth(LTexture* s, int hp)
{
	int success = 1;

    char vc[2];
    sprintf(vc, "%d", hp );

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

int loadMediaScoreText(LTexture* s)
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
		if( !(s->imgPath = loadFromRenderedText(s, "Score:", textColor ) ) )
		{
			printf( "Failed to render hp text texture!\n" );
			success = 0;
		}
	}
	return success;
}

int loadMediaScore(LTexture* s, int score)
{
	int success = 1;

    char vc[2];
    sprintf(vc, "%d", score );

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
			printf( "Failed to render score text texture!\n" );
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
		if( !(s->imgPath = loadFromRenderedText(s, "Fist Rambo!" , textColor ) ) )
		{
			printf( "Failed to render title text texture!\n" );
			success = 0;
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

int loadMediaLeftPunchfoo(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/punchleftfoo.png")) )
    {
        printf( "Failed to load left punch foo animation texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaRightPunchfoo(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/punchrightfoo.png")) )
    {
        printf( "Failed to load right punch foo animation texture!\n" );
        success = 0;
    }
    return success;
}
int loadMediaLeftPunchEnemy(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/punchleftenemy.png")) )
    {
        printf( "Failed to load left punch enemy animation texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaRightPunchEnemy(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/punchrightenemy.png")) )
    {
        printf( "Failed to load right punch enemy animation texture!\n" );
        success = 0;
    }
    return success;
}

int loadMediaStartButton(LTexture* s)
{
    int success = 1;

    if(!(s->imgPath = loadFromFile(s, "imagens/start.png")) )
    {
        printf( "Failed to load background texture!\n" );
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

    TTF_CloseFont( gFont );
	gFont = NULL;

    TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{

    LTexture gTextHealthTexture;
    gTextHealthTexture.mHeight = 0;
    gTextHealthTexture.mWidth = 0;

    LTexture gTextScoreTexture;
    gTextScoreTexture.mHeight = 0;
    gTextScoreTexture.mWidth = 0;

    LTexture gTextHealth;
    gTextHealth.mHeight = 0;
    gTextHealth.mWidth = 0;

    LTexture gTextScore;
    gTextScore.mHeight = 0;
    gTextScore.mWidth = 0;

    LTexture gTextTitle;
    gTextTitle.mHeight = 0;
    gTextTitle.mWidth = 0;


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

    LTexture gLeftPunchfoo;
    gLeftPunchfoo.mHeight = 0;
    gLeftPunchfoo.mWidth = 0;

    LTexture gRightPunchfoo;
    gRightPunchfoo.mHeight = 0;
    gRightPunchfoo.mWidth = 0;

    LTexture gLeftPunchenemy;
    gLeftPunchenemy.mHeight = 0;
    gLeftPunchenemy.mWidth = 0;

    LTexture gRightPunchenemy;
    gRightPunchenemy.mHeight = 0;
    gRightPunchenemy.mWidth = 0;

    LTexture gBackground;
    gBackground.mHeight = 0;
    gBackground.mWidth = 0;

    LTexture gStartButton;
    gStartButton.mHeight = 0;
    gStartButton.mWidth = 0;

    Mouse mouse;
    mouse.mPosition.x = 0;
    mouse.mPosition.y = 0;

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if (!loadMediaMusic())
		{
            printf("Falha ao carregar a musica!\n");
		}
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
		if( !loadMediaLeftStandEnemy(&gLeftStandEnemy) )
		{
			printf( "Failed to load left stand enemy media!\n" );
		}
		if( !loadMediaRightPunchfoo(&gRightPunchfoo) )
		{
			printf( "Failed to load right punch foo media!\n" );
		}
		if( !loadMediaLeftPunchfoo(&gLeftPunchfoo) )
		{
			printf( "Failed to load left punch foo media!\n" );
		}
        if( !loadMediaRightPunchEnemy(&gRightPunchenemy) )
		{
			printf( "Failed to load right punch enemy media!\n" );
		}
		if( !loadMediaLeftPunchEnemy(&gLeftPunchenemy) )
		{
			printf( "Failed to load right stand enemy media!\n" );
		}
		if( !loadMediaStartButton(&gStartButton) )
		{
			printf( "Failed to load start button media!\n" );
		}
        if( !loadMediaHealthText(&gTextHealthTexture) )
		{
			printf( "Failed to load Health text media!\n" );
		}
		if( !loadMediaScoreText(&gTextScoreTexture) )
		{
			printf( "Failed to load Score text media!\n" );
		}
		if( !loadMediaHealth(&gTextHealth, 10) )
        {
            printf( "Failed to load health media!\n" );
        }
        if( !loadMediaScore(&gTextScore, 0) )
        {
            printf( "Failed to load score media!\n" );
        }
        if( !loadMediaTitle(&gTextTitle) )
        {
            printf( "Failed to load text media!\n" );
        }

		else
		{
			int quit = 0;

			SDL_Event e;

			int frame = 0;

            int startGame = 0;
            int music = 1;

            int r = 0;

            int standTime = 0;
            int enemypunchTime = 0;

            int rightenemy = 1;
            int leftenemy = 0;

            float xf = 600;
            float yf = 450;
            float xe = 0.0;
            float ye = 450;

            float speedenemy = 0;
            float speedfoo = 0;
            float speedjump = 0;

            int rightpunchfoo = 0;
            int leftpunchfoo = 0;

            int rightpunchenemy = 0;
            int leftpunchenemy = 0;

            int jumpfoo = 0;
            int standfoo = 1;
            int rightfoo = 0;
            int leftfoo = 0;

			gCurrentFoo = gStandFoo;
            gCurrentEnemy = gRightEnemy;

            Uint32 jumpTime = 0;
            Uint32 oldTime = 0;
            Uint32 currentTime = 0;

            currentTime = SDL_GetTicks();

            float x1 = SCREEN_WIDTH / 3.75;
            float y1 = SCREEN_HEIGHT / 3.75;
            float x2 = x1+550;
            float y2 = y1+450;

            int hp = 10;
            int score = 0;

            int mx = 0;
            int my = 0;

			while( !quit )
			{
                if( startGame == 0 )
                {
                    if( Mix_PlayingMusic() == 1 )
                    {
                        Mix_PauseMusic();
                    }

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
                            if(startGame == 1)
                            {
                                xf = 600;
                                yf = 450;
                                xe = 0.0;
                                ye = 450;

                                hp = 10;
                                score = 0;
                            }
                        }
                    }

                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderClear( gRenderer );

                    render(&gTextTitle, (SCREEN_WIDTH - getWidth(&gTextTitle) ) / 2,( SCREEN_HEIGHT - getHeight(&gTextTitle) ) / 10 , NULL, 0, NULL, SDL_FLIP_NONE);

                    render(&gStartButton, x1 , y1, NULL, 0, NULL, SDL_FLIP_NONE);

                    render(&gTextScoreTexture, (SCREEN_WIDTH - getWidth(&gTextScoreTexture) ) / 2.2 ,( SCREEN_HEIGHT - getHeight(&gTextScoreTexture) ) / 1.05 , NULL, 0, NULL, SDL_FLIP_NONE);
                    render(&gTextScore, (SCREEN_WIDTH - getWidth(&gTextScore) ) / 1.8,( SCREEN_HEIGHT - getHeight(&gTextScore) ) / 1.05, NULL, 0, NULL, SDL_FLIP_NONE);

                    SDL_RenderPresent( gRenderer );

                    rightenemy = 1;
                    leftenemy = 0;



                    if( !loadMediaHealth(&gTextHealth, hp) )
                    {
                        printf( "Failed to load health media!\n" );
                    }
                    if( !loadMediaScore(&gTextScore, score) )
                    {
                        printf( "Failed to load score media!\n" );
                    }
                }
                else
                {
                    if( Mix_PlayingMusic() == 0 && startGame == 1)
                    {
                        Mix_PlayMusic( gMusic, -1 );
                    }
                    if( Mix_PausedMusic() == 1 )
                    {
                        Mix_ResumeMusic();
                    }

                    speedenemy = ((currentTime - oldTime)/1000.0) * 720.0;
                    speedfoo = ((currentTime - oldTime)/1000.0) * 720.0;
                    oldTime = currentTime;

                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        if( e.type == SDL_QUIT )
                        {
                            quit = 1;
                        }
                    }

                    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

                    if( jumpfoo == 0 )
                    {
                        if( keystate[ SDL_SCANCODE_UP])
                        {
                            jumpTime = currentTime;
                            jumpfoo = 1;
                            leftpunchfoo = 0;
                            rightpunchfoo = 0;
                        }
                    }
                    if( keystate[ SDL_SCANCODE_RIGHT ])
                    {
                        gCurrentFoo = gRightFoo;
                        rightfoo = 1;
                        leftfoo = 0;
                        standfoo = 0;
                        leftpunchfoo = 0;
                        rightpunchfoo = 0;
                    }
                    else if( keystate[ SDL_SCANCODE_LEFT ] )
                    {
                        gCurrentFoo = gLeftFoo;
                        leftfoo = 1;
                        rightfoo = 0;
                        standfoo = 0;
                        leftpunchfoo = 0;
                        rightpunchfoo = 0;
                    }
                    else if( keystate[ SDL_SCANCODE_A ] )
                    {
                        leftpunchfoo = 1;
                        rightpunchfoo = 0;
                        if( xe >= xf - 80.0 && xe <= xf - 10 && ye == yf)
                        {
                            r = rand() % 2;
                            score += 1;
                            if( !loadMediaScore(&gTextScore, score) )
                            {
                                printf( "Failed to load score media!\n" );
                            }
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


                        gCurrentFoo = gLeftPunchfoo;
                        standfoo = 0;
                        rightfoo = 0;
                        leftfoo = 0;

                    }
                    else if( keystate[ SDL_SCANCODE_D ] )
                    {
                        rightpunchfoo = 1;
                        leftpunchfoo = 0;
                        if( xe <= xf + 100.0 && xe >= xf + 10 && ye == yf )
                        {
                            r = rand() % 2;
                            score += 1;
                            if( !loadMediaScore(&gTextScore, score) )
                            {
                                printf( "Failed to load score media!\n" );
                            }
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

                        gCurrentFoo = gRightPunchfoo;
                        standfoo = 0;
                        rightfoo = 0;
                        leftfoo = 0;
                    }

                    else
                    {
                        gCurrentFoo = gStandFoo;
                        standfoo = 1;
                        rightfoo = 0;
                        leftfoo = 0;
                        rightpunchfoo = 0;
                        leftpunchfoo = 0;
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

                    //render(&gBackground, 0 , 0, NULL, 0, NULL, SDL_FLIP_NONE);

                    render(&gTextHealthTexture, (SCREEN_WIDTH - getWidth(&gTextHealthTexture) ) / 20,( SCREEN_HEIGHT - getHeight(&gTextHealthTexture) ) / 15 , NULL, 0, NULL, SDL_FLIP_NONE);
                    render(&gTextScoreTexture, (SCREEN_WIDTH - getWidth(&gTextScoreTexture) ) / 20,( SCREEN_HEIGHT - getHeight(&gTextScoreTexture) ) / 10 , NULL, 0, NULL, SDL_FLIP_NONE);
                    render(&gTextHealth, (SCREEN_WIDTH - getWidth(&gTextHealth) ) / 10,( SCREEN_HEIGHT - getHeight(&gTextHealth) ) / 15 , NULL, 0, NULL, SDL_FLIP_NONE);
                    render(&gTextScore, (SCREEN_WIDTH - getWidth(&gTextScore) ) / 7,( SCREEN_HEIGHT - getHeight(&gTextScore) ) / 10 , NULL, 0, NULL, SDL_FLIP_NONE);



                    if(jumpfoo)
                    {
                        speedjump = 30;
                        speedjump -= (96*((currentTime - jumpTime))*((currentTime - jumpTime)))/100000;
                        yf -= speedjump;
                        if(speedjump <= 0)
                        {
                            speedjump += 0.0008*((currentTime - jumpTime))*((currentTime - jumpTime));
                            if( yf >= 450)
                            {
                                yf = 450;
                                jumpfoo = 0;
                            }
                        }
                    }
                    if(standfoo || rightpunchfoo )
                    {
                        render(&gCurrentFoo, xf, yf, NULL, 0, NULL, SDL_FLIP_NONE);
                    }
                    if (leftpunchfoo)
                    {
                        render(&gCurrentFoo, xf-50, yf, NULL, 0, NULL, SDL_FLIP_NONE);
                    }
                    if(rightfoo)
                    {
                        SDL_Rect* currentClip = &gSpriteClipsRight[ frame / 4 ];
                        render(&gCurrentFoo, xf, yf, currentClip, 0, NULL, SDL_FLIP_NONE );
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
                        render(&gCurrentFoo, xf, yf, currentClip, 0, NULL, SDL_FLIP_NONE );
                        ++frame;
                        if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                        {
                            frame = 0;
                        }
                        xf -= speedfoo;

                    }

                    if(checkCollision(xf , xe, yf, ye))
                    {
                        standTime +=1;
                        if(standTime == 10 )
                        {
                            standTime = 0;
                            hp -= 1;
                        }
                        if(standTime > 6 )
                        {
                            if(leftenemy)
                            {
                                gCurrentEnemy = gLeftPunchenemy;
                                render(&gCurrentEnemy, xe-50, ye, NULL, 0, NULL, SDL_FLIP_NONE);
                            }

                            if(rightenemy)
                            {
                                gCurrentEnemy = gRightPunchenemy;
                                render(&gCurrentEnemy, xe, ye, NULL, 0, NULL, SDL_FLIP_NONE);
                            }

                            if( !loadMediaHealth(&gTextHealth, hp) )
                            {
                                printf( "Failed to load health media!\n" );
                            }
                            if( hp == 0)
                            {
                                startGame = 0;
                            }
                        }
                        else if(rightenemy)
                        {
                            gCurrentEnemy = gRightStandEnemy;
                            render(&gCurrentEnemy, xe , ye, NULL, 0, NULL, SDL_FLIP_NONE);
                            if(xf <= xe + 50)
                            {
                                xf += speedfoo;
                            }
                        }
                        else if(leftenemy)
                        {
                            gCurrentEnemy = gLeftStandEnemy;
                            render(&gCurrentEnemy, xe, ye, NULL, 0, NULL, SDL_FLIP_NONE);
                            if(xf >= xe - 50)
                            {
                                xf -= speedfoo;
                            }
                        }
                    }


                    else
                    {

                        standTime = 0;
                        if(rightenemy)
                        {
                            gCurrentEnemy = gRightEnemy;
                            SDL_Rect* currentClip = &gSpriteClipsRight[ frame / 4 ];
                            render(&gCurrentEnemy, xe, ye, currentClip, 0, NULL, SDL_FLIP_NONE );
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
                            render(&gCurrentEnemy, xe, ye, currentClip, 0, NULL, SDL_FLIP_NONE );
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
                    float mili = currentTime-oldTime;
                    //printf("%f\n", mili);
                }
			}
		}
	}
	close();

	return 0;
}
