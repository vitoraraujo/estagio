#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <assert.h>

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
Mix_Chunk *gButtonSound = NULL;

SDL_Texture* newTexture = NULL;


int loadFromFile(LTexture* s, const char* path )
{

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
    gFont = TTF_OpenFont( "imagens/lazy.ttf", 40 );
    assert( gFont != NULL );

    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
}

void loadMediaSound()
{
    gMusic = Mix_LoadMUS( "sons/backsong.wav" );
	assert( gMusic != NULL );

	gButtonSound = Mix_LoadWAV( "sons/buttonsound.wav" );
	assert( gButtonSound != NULL );
}

void close()
{
    SDL_DestroyTexture(newTexture);

    Mix_FreeMusic( gMusic );
	gMusic = NULL;
	Mix_FreeChunk( gButtonSound );
	gButtonSound = NULL;
    Mix_Quit();

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

    LTexture gLeftPunchfoo;
    gLeftPunchfoo.mHeight = 0;
    gLeftPunchfoo.mWidth = 0;

    LTexture gRightPunchfoo;
    gRightPunchfoo.mHeight = 0;
    gRightPunchfoo.mWidth = 0;

    LTexture gCurrentEnemy1;
    gCurrentEnemy1.mHeight = 0;
    gCurrentEnemy1.mWidth = 0;

    LTexture gRightStandEnemy1;
    gRightStandEnemy1.mHeight = 0;
    gRightStandEnemy1.mWidth = 0;

    LTexture gLeftStandEnemy1;
    gLeftStandEnemy1.mHeight = 0;
    gLeftStandEnemy1.mWidth = 0;

    LTexture gRightEnemy1;
    gLeftStandEnemy1.mHeight = 0;
    gLeftStandEnemy1.mWidth = 0;

    LTexture gLeftEnemy1;
    gLeftStandEnemy1.mHeight = 0;
    gLeftStandEnemy1.mWidth = 0;

    LTexture gLeftPunchenemy1;
    gLeftPunchenemy1.mHeight = 0;
    gLeftPunchenemy1.mWidth = 0;

    LTexture gRightPunchenemy1;
    gRightPunchenemy1.mHeight = 0;
    gRightPunchenemy1.mWidth = 0;

    LTexture gCurrentEnemy2;
    gCurrentEnemy2.mHeight = 0;
    gCurrentEnemy2.mWidth = 0;

    LTexture gRightStandEnemy2;
    gRightStandEnemy2.mHeight = 0;
    gRightStandEnemy2.mWidth = 0;

    LTexture gLeftStandEnemy2;
    gLeftStandEnemy2.mHeight = 0;
    gLeftStandEnemy2.mWidth = 0;

    LTexture gRightEnemy2;
    gLeftStandEnemy2.mHeight = 0;
    gLeftStandEnemy2.mWidth = 0;

    LTexture gLeftEnemy2;
    gLeftStandEnemy2.mHeight = 0;
    gLeftStandEnemy2.mWidth = 0;

    LTexture gLeftPunchenemy2;
    gLeftPunchenemy2.mHeight = 0;
    gLeftPunchenemy2.mWidth = 0;

    LTexture gRightPunchenemy2;
    gRightPunchenemy2.mHeight = 0;
    gRightPunchenemy2.mWidth = 0;

    LTexture gBackground;
    gBackground.mHeight = 0;
    gBackground.mWidth = 0;

    LTexture gDifButton1;
    gDifButton1.mHeight = 0;
    gDifButton1.mWidth = 0;

    LTexture gDifButton2;
    gDifButton2.mHeight = 0;
    gDifButton2.mWidth = 0;

    LTexture gMedikit;
    gMedikit.mHeight = 0;
    gMedikit.mWidth = 0;

    Mouse mouse;
    mouse.mPosition.x = 0;
    mouse.mPosition.y = 0;

	init();

    loadMediaSound();

    SDL_Color textColor = { 0, 0, 0 };

    char vc[2];

    SDL_Rect gSpriteClipsRight[ WALKING_ANIMATION_FRAMES ];
    SDL_Rect gSpriteClipsLeft[ WALKING_ANIMATION_FRAMES ];

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


    assert(loadFromFile(&gRightFoo, "imagens/rightfoo.png" ));
    assert(gRightFoo.mTexture != NULL);

    assert(loadFromFile(&gLeftFoo, "imagens/leftfoo.png" ));
    assert(gLeftFoo.mTexture != NULL);

    assert(loadFromFile(&gStandFoo, "imagens/standfoo.png" ));
    assert(gStandFoo.mTexture != NULL);

    assert(loadFromFile(&gBackground, "imagens/background.png" ));
    assert(gBackground.mTexture != NULL);

    assert(loadFromFile(&gRightEnemy1, "imagens/rightenemy.png" ));
    assert(gRightEnemy1.mTexture != NULL);

    assert(loadFromFile(&gLeftEnemy1, "imagens/leftenemy.png" ));
    assert(gLeftEnemy1.mTexture != NULL);

    assert(loadFromFile(&gRightEnemy2, "imagens/rightenemy.png" ));
    assert(gRightEnemy2.mTexture != NULL);

    assert(loadFromFile(&gLeftEnemy2, "imagens/leftenemy.png" ));
    assert(gLeftEnemy2.mTexture != NULL);

    assert(loadFromFile(&gRightStandEnemy1, "imagens/rightstandenemy.png" ));
    assert(gRightStandEnemy1.mTexture != NULL);

    assert(loadFromFile(&gLeftStandEnemy1, "imagens/leftstandenemy.png" ));
    assert(gLeftStandEnemy1.mTexture != NULL);

    assert(loadFromFile(&gRightStandEnemy2, "imagens/rightstandenemy.png" ));
    assert(gRightStandEnemy2.mTexture != NULL);

    assert(loadFromFile(&gLeftStandEnemy2, "imagens/leftstandenemy.png" ));
    assert(gLeftStandEnemy2.mTexture != NULL);

    assert(loadFromFile(&gRightPunchfoo, "imagens/punchrightfoo.png" ));
    assert(gRightPunchfoo.mTexture != NULL);

    assert(loadFromFile(&gLeftPunchfoo, "imagens/punchleftfoo.png" ));
    assert(gLeftPunchfoo.mTexture != NULL);

    assert(loadFromFile(&gLeftPunchenemy1, "imagens/punchleftenemy.png" ));
    assert(gLeftPunchenemy1.mTexture != NULL);

    assert(loadFromFile(&gRightPunchenemy1, "imagens/punchrightenemy.png" ));
    assert(gRightPunchenemy1.mTexture != NULL);

    assert(loadFromFile(&gLeftPunchenemy2, "imagens/punchleftenemy.png" ));
    assert(gLeftPunchenemy2.mTexture != NULL);

    assert(loadFromFile(&gRightPunchenemy2, "imagens/punchrightenemy.png" ));
    assert(gRightPunchenemy2.mTexture != NULL);

    assert(loadFromFile(&gDifButton1, "imagens/button1.png"));
    assert(gDifButton1.mTexture != NULL);

    assert(loadFromFile(&gDifButton2, "imagens/button2.png"));
    assert(gDifButton2.mTexture != NULL);

    assert(loadFromRenderedText(&gTextHealthTexture, "HP:", textColor ) );
    assert(gTextHealthTexture.mTexture != NULL);

    assert(loadFromRenderedText(&gTextScoreTexture, "Score:", textColor ) );
    assert(gTextScoreTexture.mTexture != NULL);

    sprintf(vc, "%d", 10);
    assert(loadFromRenderedText(&gTextHealth, vc , textColor ) );
    assert(gTextHealth.mTexture != NULL);

    sprintf(vc, "%d", 0);
    assert(loadFromRenderedText(&gTextScore, vc , textColor ) );
    assert(gTextScore.mTexture != NULL);


    assert(loadFromRenderedText(&gTextTitle, "Fist Rambo!", textColor ) );
    assert(gTextTitle.mTexture != NULL);

    assert(loadFromFile(&gMedikit, "imagens/medikit.png"));
    assert(gMedikit.mTexture != NULL);


    int quit = 0;

    SDL_Event e;

    int frame = 0;

    int medikittempo = 0;

    int startGame = 0;

    int dificuldade = 0;

    int r = 0;

    int enemyStandTime1 = 0;
    int enemyStandTime2 = 0;

    int rightenemy1 = 1;
    int leftenemy1 = 0;
    int rightenemy2 = 1;
    int leftenemy2 = 0;

    float xf = 600;
    float yf = 450;
    float xe1 = 0.0;
    float ye1 = 450;
    float xe2 = 1150.0;
    float ye2 = 450;

    float speedenemy = 0;
    float speedfoo = 0;
    float speedjump = 0;

    int rightpunchfoo = 0;
    int leftpunchfoo = 0;

    int rightpunchenemy1 = 0;
    int leftpunchenemy1 = 0;
    int rightpunchenemy2 = 0;
    int leftpunchenemy2 = 0;

    int jumpfoo = 0;
    int standfoo = 1;
    int rightfoo = 0;
    int leftfoo = 0;

    int medikit = 0;
    int medikitx = 0;

    gCurrentFoo = gStandFoo;
    gCurrentEnemy1 = gRightEnemy1;
    gCurrentEnemy2 = gLeftEnemy1;

    Uint32 jumpTime = 0;
    Uint32 oldTime = 0;
    Uint32 currentTime = 0;

    currentTime = SDL_GetTicks();

    float button1x1 = SCREEN_WIDTH / 3.75;
    float button1y1 = SCREEN_HEIGHT / 3.75;
    float button1x2 = button1x1 + 550;
    float button1y2 = button1y1 + 200;

    float button2x1 = SCREEN_WIDTH / 3.75;
    float button2y1 = (SCREEN_HEIGHT / 3.75) + 250.0;
    float button2x2 = button2x1 + 550;
    float button2y2 = button2y1 + 200;


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
                Mix_HaltMusic();
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
                    if(clickButton(&mouse, button1x1, button1y1, button1x2, button1y2))
                    {
                        Mix_PlayChannel( -1, gButtonSound, 0 );

                        startGame = 1;
                        xf = 600;
                        yf = 450;
                        xe1 = 0.0;
                        ye1 = 450;
                        xe2 = 1150.0;
                        ye2 = 450;

                        hp = 10;
                        score = 0;

                        rightenemy1 = 1;
                        leftenemy1 = 0;
                        rightenemy2 = 0;
                        leftenemy2 = 1;

                        dificuldade = 1;
                    }
                    else if(clickButton(&mouse, button2x1, button2y1, button2x2, button2y2))
                    {
                        Mix_PlayChannel( -1, gButtonSound, 0 );

                        startGame = 1;
                        xf = 600;
                        yf = 450;
                        xe1 = 0.0;
                        ye1 = 450;
                        xe2 = 1150.0;
                        ye2 = 450;

                        hp = 10;
                        score = 0;

                        rightenemy1 = 1;
                        leftenemy1 = 0;
                        rightenemy2 = 0;
                        leftenemy2 = 1;

                        dificuldade = 2;
                    }
                }
            }

            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            render(&gTextTitle, (SCREEN_WIDTH - gTextTitle.mWidth ) / 2,( SCREEN_HEIGHT - gTextTitle.mHeight ) / 10 , NULL, 0, NULL, SDL_FLIP_NONE);

            render(&gDifButton1, button1x1 , button1y1, NULL, 0, NULL, SDL_FLIP_NONE);
            render(&gDifButton2, button2x1 , button2y1, NULL, 0, NULL, SDL_FLIP_NONE);

            render(&gTextScoreTexture, (SCREEN_WIDTH - gTextScoreTexture.mWidth ) / 2.2 ,( SCREEN_HEIGHT - gTextScoreTexture.mHeight ) / 1.05 , NULL, 0, NULL, SDL_FLIP_NONE);
            render(&gTextScore, (SCREEN_WIDTH - gTextScore.mWidth ) / 1.8,( SCREEN_HEIGHT - gTextScore.mHeight ) / 1.05, NULL, 0, NULL, SDL_FLIP_NONE);

            SDL_RenderPresent( gRenderer );

        }
        else
        {
            if( Mix_PlayingMusic() == 0 && startGame == 1)
            {
                Mix_PlayMusic( gMusic, -1 );
            }

            speedenemy = ((currentTime - oldTime)/1000.0) * 600.0;
            speedfoo = ((currentTime - oldTime)/1000.0) * 600.0;
            oldTime = currentTime;

            while( SDL_PollEvent( &e ) != 0 && startGame == 1)
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
                if( xe1 >= xf - 80.0 && xe1 <= xf - 10 && ye1 == yf)
                {
                    r = rand() % 2;
                    score += 1;

                    sprintf(vc, "%d", score);
                    assert(loadFromRenderedText(&gTextScore, vc , textColor ) );
                    assert(gTextScore.mTexture != NULL);
                    switch (r)
                    {
                        case 0:
                        rightenemy1 = 0;
                        leftenemy1 = 1;
                        xe1 = 1150;
                        break;

                        case 1:
                        rightenemy1 = 1;
                        leftenemy1 = 0;
                        xe1 = 0;
                        break;
                    }
                }
                if( xe2 >= xf - 80.0 && xe2 <= xf - 10 && ye2 == yf)
                {
                    r = rand() % 2;
                    score += 1;

                    sprintf(vc, "%d", score);
                    assert(loadFromRenderedText(&gTextScore, vc , textColor ) );
                    assert(gTextScore.mTexture != NULL);

                    switch (r)
                    {
                        case 0:
                        rightenemy2 = 0;
                        leftenemy2 = 1;
                        xe2 = 1150;
                        break;

                        case 1:
                        rightenemy2 = 1;
                        leftenemy2 = 0;
                        xe2 = 0;
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
                if( xe1 <= xf + 100.0 && xe1 >= xf + 10 && ye1 == yf )
                {
                    r = rand() % 2;
                    score += 1;

                    sprintf(vc, "%d", score);
                    assert(loadFromRenderedText(&gTextScore, vc , textColor ) );
                    assert(gTextScore.mTexture != NULL);

                    switch (r)
                    {
                        case 0:
                        rightenemy1 = 0;
                        leftenemy1 = 1;
                        xe1 = 1150;
                        break;

                        case 1:
                        rightenemy1 = 1;
                        leftenemy1 = 0;
                        xe1 = 0;
                        break;
                    }
                }
                if( xe2 <= xf + 100.0 && xe2 >= xf + 10 && ye2 == yf )
                {
                    r = rand() % 2;
                    score += 1;

                    sprintf(vc, "%d", score);
                    assert(loadFromRenderedText(&gTextScore, vc , textColor ) );
                    assert(gTextScore.mTexture != NULL);

                    switch (r)
                    {
                        case 0:
                        rightenemy2 = 0;
                        leftenemy2 = 1;
                        xe2 = 1150;
                        break;

                        case 1:
                        rightenemy2 = 1;
                        leftenemy2 = 0;
                        xe2 = 0;
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

            if ( xe1 > 1150 )
            {
                rightenemy1 = 0;
                leftenemy1 = 1;
                xe1 = 1150;
            }
            if (  xe1 < 0 )
            {
                rightenemy1 = 1;
                leftenemy1 = 0;
                xe1 = 0;
            }
            if ( xe2 > 1150 )
            {
                rightenemy2 = 0;
                leftenemy2 = 1;
                xe2 = 1150;
            }
            if (  xe2 < 0 )
            {
                rightenemy2 = 1;
                leftenemy2 = 0;
                xe2 = 0;
            }


            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            render(&gBackground, 0 , 0, NULL, 0, NULL, SDL_FLIP_NONE);

            SDL_Rect fillRect = { 50, 50 , 180 , 55};
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderFillRect( gRenderer, &fillRect );

            render(&gTextHealthTexture, (SCREEN_WIDTH - gTextHealthTexture.mWidth ) / 20,( SCREEN_HEIGHT - gTextHealthTexture.mHeight ) / 15 , NULL, 0, NULL, SDL_FLIP_NONE);
            render(&gTextScoreTexture, (SCREEN_WIDTH - gTextScoreTexture.mWidth ) / 20,( SCREEN_HEIGHT - gTextScoreTexture.mHeight ) / 10 , NULL, 0, NULL, SDL_FLIP_NONE);
            render(&gTextHealth, (SCREEN_WIDTH - gTextHealth.mWidth ) / 9,( SCREEN_HEIGHT - gTextHealth.mHeight ) / 15 , NULL, 0, NULL, SDL_FLIP_NONE);
            render(&gTextScore, (SCREEN_WIDTH - gTextScore.mWidth ) / 6.8,( SCREEN_HEIGHT - gTextScore.mHeight ) / 10 , NULL, 0, NULL, SDL_FLIP_NONE);

            if(medikittempo == 1000 && medikit == 0)
            {
                medikit = 1;
                medikitx = rand() % 1151;
            }
            if(medikit == 1)
            {
                render(&gMedikit, medikitx, 500, NULL, 0, NULL, SDL_FLIP_NONE);
            }
            if(checkCollision(xf, medikitx, yf, 450))
            {
                medikit = 0;
                medikittempo = 0;
                hp = 10;

                sprintf(vc, "%d", hp);
                assert(loadFromRenderedText(&gTextHealth, vc , textColor ) );
                assert(gTextHealth.mTexture != NULL);
            }

            if(jumpfoo)
            {
                speedjump = 30;
                speedjump -= (80*((currentTime - jumpTime))*((currentTime - jumpTime)))/100000;
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

            if(checkCollision(xf , xe1, yf, ye1))
            {
                enemyStandTime1 +=1;
                if(enemyStandTime1 == 10 )
                {
                    enemyStandTime1 = 0;
                    hp -= 1;
                }
                if(enemyStandTime1 > 6 )
                {
                    if(leftenemy1)
                    {
                        gCurrentEnemy1 = gLeftPunchenemy1;
                        render(&gCurrentEnemy1, xe1-50, ye1, NULL, 0, NULL, SDL_FLIP_NONE);
                    }

                    if(rightenemy1)
                    {
                        gCurrentEnemy1 = gRightPunchenemy1;
                        render(&gCurrentEnemy1, xe1, ye1, NULL, 0, NULL, SDL_FLIP_NONE);
                    }

                    sprintf(vc, "%d", hp);
                    assert(loadFromRenderedText(&gTextHealth, vc , textColor ) );
                    assert(gTextHealth.mTexture != NULL);
                    if( hp <= 0)
                    {
                        startGame = 0;
                    }
                }
                else if(rightenemy1)
                {
                    gCurrentEnemy1 = gRightStandEnemy1;
                    render(&gCurrentEnemy1, xe1 , ye1, NULL, 0, NULL, SDL_FLIP_NONE);
                    if(xf <= xe1 + 50)
                    {
                        xf += speedfoo;
                    }
                }
                else if(leftenemy1)
                {
                    gCurrentEnemy1 = gLeftStandEnemy1;
                    render(&gCurrentEnemy1, xe1, ye1, NULL, 0, NULL, SDL_FLIP_NONE);
                    if(xf >= xe1 - 50)
                    {
                        xf -= speedfoo;
                    }
                }
            }
            else
            {
                enemyStandTime1 = 0;
                if(rightenemy1)
                {
                    gCurrentEnemy1 = gRightEnemy1;
                    SDL_Rect* currentClip = &gSpriteClipsRight[ frame / 4 ];
                    render(&gCurrentEnemy1, xe1, ye1, currentClip, 0, NULL, SDL_FLIP_NONE );
                    ++frame;
                    if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                    {
                        frame = 0;
                    }
                    xe1 += speedenemy;
                }
                if(leftenemy1)
                {
                    gCurrentEnemy1 = gLeftEnemy1;
                    SDL_Rect* currentClip = &gSpriteClipsLeft[ frame / 4 ];
                    render(&gCurrentEnemy1, xe1, ye1, currentClip, 0, NULL, SDL_FLIP_NONE );
                    ++frame;
                    if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                    {
                        frame = 0;
                    }
                    xe1 -= speedenemy;
                }
            }
            if(dificuldade == 2)
            {
                if(checkCollision(xf , xe2, yf, ye2))
                {
                    enemyStandTime2 +=1;
                    if(enemyStandTime2 == 10 )
                    {
                        enemyStandTime2 = 0;
                        hp -= 1;
                    }
                    if(enemyStandTime2 > 6 )
                    {
                        if(leftenemy2)
                        {
                            gCurrentEnemy2 = gLeftPunchenemy2;
                            render(&gCurrentEnemy2, xe2-50, ye2, NULL, 0, NULL, SDL_FLIP_NONE);
                        }

                        if(rightenemy2)
                        {
                            gCurrentEnemy2 = gRightPunchenemy2;
                            render(&gCurrentEnemy2, xe2, ye2, NULL, 0, NULL, SDL_FLIP_NONE);
                        }

                        SDL_DestroyTexture(gTextHealth.mTexture);
                        sprintf(vc, "%d", hp);
                        assert(loadFromRenderedText(&gTextHealth, vc , textColor ) );
                        assert(gTextHealth.mTexture != NULL);
                        if( hp <= 0)
                        {
                            startGame = 0;
                        }
                    }
                    else if(rightenemy2)
                    {
                        gCurrentEnemy2 = gRightStandEnemy2;
                        render(&gCurrentEnemy2, xe2 , ye2, NULL, 0, NULL, SDL_FLIP_NONE);
                        if(xf <= xe2 + 50)
                        {
                            xf += speedfoo;
                        }
                    }
                    else if(leftenemy2)
                    {
                        gCurrentEnemy2 = gLeftStandEnemy2;
                        render(&gCurrentEnemy2, xe2, ye2, NULL, 0, NULL, SDL_FLIP_NONE);
                        if(xf >= xe2 - 50)
                        {
                            xf -= speedfoo;
                        }
                    }
                }
                else
                {
                    enemyStandTime2 = 0;
                    if(rightenemy2)
                    {
                        gCurrentEnemy2 = gRightEnemy2;
                        SDL_Rect* currentClip = &gSpriteClipsRight[ frame / 4 ];
                        render(&gCurrentEnemy2, xe2, ye2, currentClip, 0, NULL, SDL_FLIP_NONE );
                        ++frame;
                        if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                        {
                            frame = 0;
                        }
                        xe2 += speedenemy;
                    }
                    if(leftenemy2)
                    {
                        gCurrentEnemy2 = gLeftEnemy2;
                        SDL_Rect* currentClip = &gSpriteClipsLeft[ frame / 4 ];
                        render(&gCurrentEnemy2, xe2, ye2, currentClip, 0, NULL, SDL_FLIP_NONE );
                        ++frame;
                        if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                        {
                            frame = 0;
                        }
                        xe2 -= speedenemy;
                    }
                }
            }
            SDL_RenderPresent( gRenderer );
            currentTime = SDL_GetTicks();
            //float mili = currentTime-oldTime;
            //printf("%f\n", mili);
            medikittempo+=1;
		}
	}

	SDL_DestroyTexture(gTextHealthTexture.mTexture);
	SDL_DestroyTexture(gTextScoreTexture.mTexture);
	SDL_DestroyTexture(gTextHealth.mTexture);
	SDL_DestroyTexture(gTextScore.mTexture);
	SDL_DestroyTexture(gTextTitle.mTexture);

    SDL_DestroyTexture(gStandFoo.mTexture);
	SDL_DestroyTexture(gRightFoo.mTexture);
	SDL_DestroyTexture(gLeftFoo.mTexture);
	SDL_DestroyTexture(gLeftPunchfoo.mTexture);
	SDL_DestroyTexture(gRightPunchfoo.mTexture);

	SDL_DestroyTexture(gRightEnemy1.mTexture);
	SDL_DestroyTexture(gLeftEnemy1.mTexture);
	SDL_DestroyTexture(gLeftStandEnemy1.mTexture);
	SDL_DestroyTexture(gRightStandEnemy1.mTexture);
	SDL_DestroyTexture(gRightPunchenemy1.mTexture);
	SDL_DestroyTexture(gLeftPunchenemy1.mTexture);

	SDL_DestroyTexture(gRightEnemy2.mTexture);
	SDL_DestroyTexture(gLeftEnemy2.mTexture);
	SDL_DestroyTexture(gLeftStandEnemy2.mTexture);
	SDL_DestroyTexture(gRightStandEnemy2.mTexture);
	SDL_DestroyTexture(gRightPunchenemy2.mTexture);
	SDL_DestroyTexture(gLeftPunchenemy2.mTexture);

	SDL_DestroyTexture(gDifButton1.mTexture);
	SDL_DestroyTexture(gDifButton2.mTexture);
	SDL_DestroyTexture(gBackground.mTexture);
	SDL_DestroyTexture(gMedikit.mTexture);

	close();

	return 0;
}
