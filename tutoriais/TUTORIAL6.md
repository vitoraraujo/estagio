# Objetivo (Céu):

## SDL com C/C++

## Preparando o ambiente para SDL_mixer no Ubuntu 14.04 - 15.10

Para trabalharmos com som em SDL, é necessário a biblioteca SDL_mixer. Para instalar essa biblioteca, abra o terminal e digite:

```
sudo apt-get install libsdl2-mixer-dev
```

No início de todos os códigos dos programas que usarem essa biblioteca, inclua o `SDL_mixer` com essa linha de comando, junto das outras inclusões:

```
 #include <SDL2/SDL_ttf.h>
```

## Renderizando uma imagem na tela e sons com SDL_mixer

É necessário ter a pasta `imagens-tutorial` com a imagem `somtela.png` para esse tutorial (som.cpp) e uma pasta `sons-tutorial`. Veja os outros tutoriais para entender comandos e etapas já utilizadas. Você irá renderizar uma imagem um arquivo `.png` e sons de audios `.wav` . Usaremos sintaxe de C, pois usaremos struct, no lugar de classes de C++.

Inclua as bibliotecas:

```
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>
 #include <SDL2/SDL_mixer.h>
 #include <stdio.h>
```

Determine a resolução:

```
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480; 
```

Crie a struct `LTexture`:

```
struct LTexture{
    SDL_Texture* mTexture = NULL;
	int mWidth;
    int mHeight;
	int imgPath;
};
typedef struct LTexture LTexture;
```

Crie o ponteiro global do tipo `SDL_Window`, o  ponteiro global do tipo `SDL_Renderer`:

```
SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;
```

Crie uma váriavel do tipo `Mix_Music` que conterá o áudio de música, e as variáveis do tipo `Mix_Chunk` que conterão o áudio de sons rápidos, e torne seus conteúdos como `NULL`:

```
Mix_Music *gMusic = NULL;

Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;
```

Declare a função `loadFromFile()`:

```
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
```

Declare a função `render()`:

```
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
```

Declare os getters:

```
int getWidth(LTexture* s)
{
	return s->mWidth;
}

int getHeight(LTexture* s)
{
	return s->mHeight;
}
```

Crie a função `init()` do mesmo jeito de sempre, mas, junto da incialização do `SDL_image`, inicialize o `SDL_mixer`

```
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
			printf( "Cuidado: Filtro de textura linear não ativado!" );
		}

		gWindow = SDL_CreateWindow( "SDL TTF", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window não pôde ser criada! SDL Error: %s\n", SDL_GetError() );
			success = 0;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderizador não pôde ser criado! SDL Error: %s\n", SDL_GetError() );
				success = 0;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image não pôde ser inicializado! SDL_image Error: %s\n", IMG_GetError() );
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
```

Crie a função `loadMediaTela()` para carregar a imagem que ficará na tela:

```
int loadMediaTela(LTexture* s)
{
	int success = 1;

	if( !(s->imgPath = loadFromFile( s, "imagens-tutorial/somtela.png" )) )
	{
		printf( "Falha para carregar imagem da tela!\n" );
		success = 0;
	}
	return success;
}
```

Crie a função `loadMediaSound()` que carregará os áudios da música com `Mix_LoadMUS` e dos chunks `Mix_LoadWAV`:

```
int loadMediaSound()
{
    int success = 1;

    gMusic = Mix_LoadMUS( "sons-tutorial/music.wav" );
	if( gMusic == NULL )
	{
		printf( "Falha ao carregar a música! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}
	gScratch = Mix_LoadWAV( "sons-tutorial/scratch.wav" );
	if( gScratch == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}

	gHigh = Mix_LoadWAV( "sons-tutorial/high.wav" );
	if( gHigh == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}

	gMedium = Mix_LoadWAV( "sons-tutorial/medium.wav" );
	if( gMedium == NULL )
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}

	gLow = Mix_LoadWAV( "sons-tutorial/low.wav" );
	if( gLow == NULL )
	{
		printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = 0;
	}

    return success;
}
```

Crie a função `close()` que dealocará a música e os chunks e os borá como `NULL`, e fechará o `SDL_mixer` :

```
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
```

Crie a `int main()`:

```
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
						
```

E faça um `switch` para ler as teclas e tocar os sons de acordo com elas e renderize a tela:

```
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
```

Por fim, finalize o programa com a função `close()`:

```
    close();

	return 0;
}
```

Para compilar e executar, basta escrever os seguintes comandos no terminal:

```
$ gcc som.cpp -o som -lSDL2 -lSDL2_image -lSDL2_mixer
$ ./ttf
```
