# Objetivo (Céu):

## SDL com C/C++

## Preparando o ambiente para SDL_ttf no Ubuntu 14.04 - 15.10

Para trabalharmos com renderização de texto, é necessário a biblioteca SDL_ttf. Para instalar essa biblioteca, abra o terminal e digite:

```
sudo apt-get install libsdl2-ttf-dev
```

No início de todos os códigos dos programas que usarem essa biblioteca, inclua o `SDL_ttf` com essa linha de comando, junto das outras inclusões:

```
 #include <SDL2/SDL_ttf.h>
```

## Renderizando texto na tela com o SDL_ttf

É necessário ter a pasta `imagens-tutorial` com a imagem `lazy.ttf` para esse tutorial (ttf.cpp). Veja os outros tutoriais para entender comandos e etapas já utilizadas. Você irá renderizar texto vindo de uma imagem `.ttf`. Usaremos sintaxe de C, pois usaremos struct, no lugar de classes de C++.

Inclua as bibliotecas:

```
 #include <SDL2/SDL_ttf.h>
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>
 #include <stdio.h>
```

Determine a resolução:

```
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800; 
```

Crie a struct `LTexture` com um ponteiro do tipo `SDL_Texture` sendo `NULL`, e os atributos do tipo `int` que guardarão, respectivamente, a largura da imagem, a altura, e o endereço. Utilize um `typedef` para a struct, para chamarmos ela somente por `LTexture` :

```
struct LTexture{
    SDL_Texture* mTexture = NULL;
	int mWidth;
    int mHeight;
	int imgPath;
};
typedef struct LTexture LTexture;
```

Crie o ponteiro global do tipo `SDL_Window` que será renderizado, e o  ponteiro global do tipo `SDL_Renderer` que coterá as renderizações, e o ponteiro do tipo `TTF_Font` quer armazena o tipo fonte que o texto está renderizando:

```
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;
```

Declare a função `loadFromFile()`, que recebe a struct `LTexture` e o  como parâmetro que carrega a imagem passada, ignorando o fundo desta imagem. Primeiramente, declare o ponteiro do tipo `SDL_Texture` que conterá textura que será passada para o atributo da struct que vai contê-la, depois, declare o ponteiro do tipo `SDL_Surface` para carregar a imagem passada ao método:

```
int loadFromFile(LTexture* s, const char* path )
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path );
	
	if( loadedSurface == NULL )
	{
		printf( "Não pôde carregar imagem %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
```

Com a imagem carregada, utilize a função `SDL_SetColorKey()` para determinar a cor que deverá ser ignorada, nesse caso, a mistura de verde e azul. Passe a textura da imagem para o ponteiro que irá ser passado à struct posteriormente:

```
	else
	{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Não foi possível criar textura de %s! SDL Error: %s\n", path, SDL_GetError() );
		}
```

Com a imagem passada, passe as dimenções da imagem para os atributos de classe que devem contê-las, e depois, dealoque a imagem antiga que não será mais utilizada:

```
		else
		{
			s->mWidth = loadedSurface->w;
			s->mHeight = loadedSurface->h;
		}

		SDL_FreeSurface( loadedSurface );
	}
```

Por fim, passe a imagem final para o atributo de struct `mTexture` e retorne este sendo diferente de NULL:

```
	s->mTexture = newTexture;
	return s->mTexture != NULL;
}
```

Crie a função `loadFromRenderedText()` que trabalha como o ponteiro de struct `LTexture`, o texto desejado e a cor, passados no parâmetro. Essa função cria a textura do texto que vamos renderizar com a fonte. A função `TTF_RenderText_Solid()` recebe a fonte, o texto e a cor, e passa para o ponteiro `SDL_Surface` que irá ser atribuido ao mTexture da struct:

```
	int loadFromRenderedText(LTexture* s, const char* textureText, SDL_Color textColor )
	{
		SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText, textColor );
		if( textSurface == NULL )
		{
			printf( "Não foi possível renderizar o text! SDL_ttf Error: %s\n", TTF_GetError() );
		}
		else
		{
			s->mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
			if( s->mTexture == NULL )
			{
				printf( "Não foi possível criar a textura do texto renderizado! SDL Error: %s\n", SDL_GetError() );
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
```

Declare a função `render()`, que rendeizará o texto na tela:

```
void render(LTexture* s, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	SDL_Rect renderQuad = { x, y,  s->mWidth,  s->mHeight };

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx( gRenderer,  s->mTexture, clip, &renderQuad, angle, center, flip );
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

Crie a função `init()` do mesmo jeito de sempre, mas, junto da incialização do `SDL_image`, inicialize o `SDL_ttf`

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

				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf não pôde ser inicializado! SDL_ttf Error: %s\n", TTF_GetError() );
					success = 0;
				}
			}
		}
	}

	return success;
}
```

Crie a função `loadMediaText()`, que determinará a fonte com a função `TTF_OPenFont()`, que recebe o endereço da imagem `.ttf` e o tamanho da fonte. Em seguida, carregará o texto, setando-o no atributo `imgPath` da struct:

```
int loadMediaText(LTexture* s)
{
	int success = 1;
	gFont = TTF_OpenFont( "imagens-tutorial/lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Falha ao carregar a fonte! SDL_ttf Error: %s\n", TTF_GetError() );
		success = 0;
	}
	else
	{
		SDL_Color textColor = { 0, 0, 0 };
		if( !(s->imgPath = loadFromRenderedText(s, "Hello World!", textColor ) ) )
		{
			printf( "Falha ao renderizar a textura!\n" );
			success = 0;
		}
	}
	return success;
}
```

Crie a função `close()` que destruirá o renderizador e a window, fechará a fonte, o SDL_ttf, o SDL_image, e o SDL:


```
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
```
Crie, então, a função `main()`, que instanciará a struct e determinará seus atributos como 0. Chame a função `loadMediaText()`, posteriormente a render, passando, em ambas, o endereço da struct: 

```
int main()
{
	LTexture gTextTexture;
    gTextTexture.mHeight = 0;
    gTextTexture.mWidth = 0;

	if( !init() )
	{
		printf( "Falha ao inicializar!\n" );
	}
	else
	{
		if( !loadMediaText(&gTextTexture) )
		{
			printf( "Falha ao carregar mídia!\n" );
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
				}
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				render(&gTextTexture, (SCREEN_WIDTH - getWidth(&gTextTexture) ) / 2,( SCREEN_HEIGHT - getHeight(&gTextTexture) ) / 2 , NULL, 0, NULL, SDL_FLIP_NONE);

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
$ gcc ttf.cpp -o ttf -lSDL2 -lSDL2_image -lSDL2_ttf
$ ./ttf
```






