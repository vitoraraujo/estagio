# Objetivo (Céu)

## SDL com C++

## Renderizando multiplas imagens ao mesmo tempo (plano de fundo e boneco), com imagens com fundo transparente, ignorando o entorno (boneco)

É necessário ter a pasta `imagens-tutorial` com os arquivos `boneco.png` e `planodefundo.png` para para esse tutorial (colorkey.cpp). Caso queira expandir a resolução, utilize o ultimo tutorial. Veja os outros tutoriais para entender comandos e etapas já utilizadas. A partir de agora, Usa-se a linguagem C++ para os programas, pois usará classes, atributos de classe e métodos.

Inclua as bibliotecas:

```
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>
 #include <stdio.h>
```

Determine a resolução:

```
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480; 
```

Crie o ponteiro global do tipo `SDL_Window` que será renderizado, e o  ponteiro global do tipo `SDL_Renderer` que coterá as renderizações:

```
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
```

Crie a classe `LTexture`, que conterá, como públicos, o contrutor, o descontrutor, os métodos  `loadFromFile()`, `free()`, `render()`, e getters para as dimenções da imagem:

```
class LTexture
{
	public:
		LTexture();

		~LTexture();

		bool loadFromFile( const char* path );

		void free();

		void render( int x, int y );

		int getWidth();
		int getHeight();
```

E, como privados, o ponteiro do tipo `SDL_Texture` que conterá a textura renderizada, e os atributos das dimensões da imagem:

```
private:
		SDL_Texture* mTexture;

		int mWidth;
		int mHeight;
};
```

Inicialize a classe para cada imagem:

```
LTexture gBonecoTexture;
LTexture gFundoTexture;
```

Declare o construtor, que irá incializar os atributos:

```
LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}
```

Declare o desconstrutor que decolará a imagem:

```
LTexture::~LTexture()
{
	free();
}
```

Declare o método `loadFromFile()`, que carrega a imagem passada, ignorando o fundo desta imagem. Primeiramente, deloque qualquer textura que já estivesse antes, para prevenir conflitos, declare o ponteiro do tipo `SDL_Texture` que conterá textura que será passada para o atributo de classe que deve contê-la, depois, declare o ponteiro do tipo `SDL_Surface` para carregar a imagem passada ao método:

```
bool LTexture::loadFromFile( const char* path )
{
	free();

	SDL_Texture* newTexture = NULL;
	
	SDL_Surface* loadedSurface = IMG_Load( path );
	if( loadedSurface == NULL )
	{
		printf( "Não pôde carregar imagem %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
```

Com a imagem carregada, utilize a função `SDL_SetColorKey()` para determinar a cor que deverá ser ignorada, nesse caso, a mistura de verde e azul. Passe a textura da imagem para o ponteiro que irá ser passado à classe posteriormente:

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

Com a imagem passada, passe as dimenções da imagem para os atributos de classe que devem contê-las, e depois, deloque a imagem antiga que não será mais utilizada:

```
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface( loadedSurface );
	}
```

Por fim, passe a imagem final para o atributo de classe `mTexture` e retorne este sendo diferente de NULL:

```
	mTexture = newTexture;
	return mTexture != NULL;
}
```

Declare o método `free()`, que deloca a textura da memória caso exista:

```
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
```

Declare a função `render()`, que rendeizará a imagem na tela:

```
void LTexture::render( int x, int y )
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}
```

Declare os getters :

```
int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}
```

Crie a função `init()`, que botará o filtro da textura de modo linear: 

```
int init()
{
	int success = 1;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL não pôde ser inicializado! SDL Error: %s\n", SDL_GetError() );
		success = 0;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Cuidado: Filtro de textura linear não ativado!" );
		}
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window não pôde ser criada! SDL Error: %s\n", SDL_GetError() );
			success = 0;
		}			
```

Com isso, crie o renderizador para a window, e inicie o `SDL_Image`:

```
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
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
			}
		}
	}

	return success;
}
```

Crie a função `loadMedia()`, que carregará o boneco e o plano de fundo:

```
int loadMedia()
{
	int success = 1;

	if( !gBonecoTexture.loadFromFile( "imagens-tutorial/boneco.png" ) )
	{
		printf( "Falha ao carregar o boneco!\n" );
		success = 0;
	}

	if( !gFundoTexture.loadFromFile( "imagens-tutorial/planodefundo.png" ) )
	{
		printf( "Falha ao carregar o plano de fundo!\n" );
		success = 0;
	}

	return success;
}
```
Crie a função `close()` que delocará as imagens, destruirá o renderizador e a window, e fechará o SDL:

```
void close()
{
	gBonecoTexture.free();
	gFundoTexture.free();

	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}
```

Crie a função `main()`:

```
int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf( "Falha ao inicializar!\n" );
	}
	else
	{
		if( !loadMedia() )
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
```

Limpe a tela e chame os métodos `render()` para cada imagem, e atualize a tela:

```
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				gFundoTexture.render( 0, 0 );

				gBonecoTexture.render( 340, 190 );

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
$ g++ colorkey.cpp -o colorkey -lSDL2 -lSDL2_image
$ ./colorkey
```


