# Objetivo (Céu)

## SDL com C/C++

## Colocando imagem .bmp na tela (Hello World!) no meio mais fácil e fechando programa com o botao `x` da interface:

É necessário ter a pasta `imagens-tutorial` com o arquivo `hello_world.bmp` para esse tutorial (helloworld.cpp). Veja o TUTORIAL1.md para enteder comandos e etapas já utilizadas. Novas funções que serão utilizadas serão indispensáveis nos próximos tutoriais (`init()`, `loadMedia()` e `close()`).

Inclua as bibliotecas:

```
 #include <SDL2/SDL.h>
 #include <stdio.h>
```

Determine a resolução:

```
const int SCREEN_WIDTH = 640; 
const int SCREEN_HEIGHT = 480; 
```

Crie o ponteiro global do tipo `SDL_Window` que será renderizado, o ponteiro global do tipo `SDL_Surface` que será a superfice que a window conterá, e o ponteiro global que irá armazenar a imagem `hello_world.bmp`:

```
SDL_Window* gWindow = NULL;	
SDL_Surface* gScreenSurface = NULL;	
SDL_Surface* gHelloWorld = NULL; 
```

Crie a função `int init()`, com uma variável `success`, que irá retornar 1 caso não ocorra erro, e 0 caso ocorra, que inicializará o SDL. Crie a window e pegue a superfície da window:

```
int init()
{
	int success = 1;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL não pode inicializar. SDL_Error: %s\n", SDL_GetError() );
		success = 0;	
	}
	else
	{
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Não foi possível criar a window. SDL_Error: %s\n", SDL_GetError() ); 
			success = 0;		
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}
	return success;
}
```

Crie a função `loadMedia()`, com uma variável `success`, que irá retornar 1 caso não ocorra erro e 0 caso ocorra, para carregar a imagem na superfície `gHelloWorld` com o comando `SDL_LoadBMP("endereço da imagem.bmp")` e cheque se a imagem foi carregada:

```
int loadMedia()
{
	int success = 1;
	gHelloWorld = SDL_LoadBMP("imagens-tutorial/hello_world.bmp");	
	if (gHelloWorld == NULL)
	{
		printf( "Não foi possível carregar a imagem %s! SDL Error: %s\n", "imagens-tutorial/hello_world.bmp", SDL_GetError() );	
		success = 0;	
	}
	return success;
}
```
Crie a função `close()`, que deloca a superfíce, destroi a window e sai do SDL:

```
void close()
{
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;
	
	SDL_DestroyWindow( gWindow ); 
	gWindow = NULL;
		
	SDL_Quit();
}
```

Inicie a main, chame a função `init()`. Se não ocorrer erro, carregue a imagem com a função `loadMedia()`:

```
int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf( "Falha ao iniciar!\n" );
	}
	else
	{
		if( !loadMedia() )
		{ 
			printf( "Falha ao carregar a mídia!\n" ); 
		}
		
```

Com a imagem carregada, crie a variável do tipo int `quit` para que possa tornar 1 quando estiver na hora de sair e declare `SDL_Event e` para receber o evento (click no `x` no topo da interface):

```
		else
		{
			int quit = 0;
			SDL_Event e;
```

O programa estará rodando a leitura do `x` enquanto quit for 0, e se o evento `e` for do tipo `SDL_QUIT` (se o `x` for clicado) torna `quit = 1` para acabar o loop:

```
			while(!quit)
			{	
				while( SDL_PollEvent( &e ) != 0 )
				{ 
					if( e.type == SDL_QUIT )
					{ 
						quit = 1;
					}
				}
```

Enquanto o loop `while(!quit)` não acabar, irá desenhar a imagem na tela e dar update nesta, para que o desenho apareça, e quando acabar o loop, close será chamado:

```
			SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );		
			SDL_UpdateWindowSurface( gWindow );		
			}
		}
	}
	close();
	return 0;
}
```

Para compilar e executar, basta escrever os seguintes comandos no terminal:

```
$ gcc helloworld.cpp -o helloworld -lSDL2
$ ./helloworld
```

