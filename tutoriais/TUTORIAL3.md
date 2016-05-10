# Objetivo (Céu)

## SDL com C/C++

## Preparando o ambiente para SDL_image no ubuntu 14.04 - 15.10

Para trabalharmos com imagens .png, é necessário utilizar a biblioteca sdl_image. Para instalar essa biblioteca, abra o terminal e digite:

```
sudo apt-cache search libsdl2-image
sudo apt-get install libsdl2-image-dev
```

No início de todos os códigos dos programas que usarem essa biblioteca, inclua o SDL_image com essa linha de comando, junto das outras inclusões:

```
 #include<SDL2/SDL_image.h>
```

## Lendo eventos do teclado e desenhando imagens .png, que se optimizam conforme o tamanho da tela, relativas à estes eventos (botoes.cpp)

É necessário ter a pasta "imagens-tutorial" com as imagens "press.png, up.png, down.png, left.png e right.png" para esse tutorial (botoes.cpp). Veja o TUTORIAL1.md e o TUTORIAL2.md para enteder comandos e etapas já utilizadas. Você irá ler comandos do teclado, utilizar imagens .png e alterar o formato da imagem conforme o tamanho da tela

Inclua as bibliotecas:

```
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>
 #include <stdio.h>
```

Determine a resolução:

```
const int SCREEN_WIDTH = 1200; 
const int SCREEN_HEIGHT = 800; 
```

Crie um enum para guardar as teclas que serão utilizadas:

```
enum teclas
{
    tecla_default,
    tecla_cima,
    tecla_baixo,
    tecla_esquerda,
    tecla_direita,
    tecla_total
};
```

Crie o ponteiro global do tipo "SDL_Window" que será renderizado, o ponteiro global do tipo "SDL_Surface" que será a superfice que a window conterá, declare o enum como um enum de ponteiros globais do tipo "SDL_Surface", e, já que haveram diferentes imagens conforme as teclas, deve-se declarar uma surperfície que estará guardando a imagem utilizada no momento: 

```
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* teclas[ tecla_total ];
SDL_Surface* gCurrentSurface = NULL;
```

Crie uma função "loadSurface("endereço da imagem")" que retornará uma superfície com uma imagem .png carregada e optimizada. Para isso, declare uma superfície que terá imagens optimizadas "optimizedSurface" e uma que carregará a imagem desejada "loadedSurface", e checaremos se há erros:

```
SDL_Surface* loadSurface( const char* path )
{
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load( path );
	if( loadedSurface == NULL )
    {
        printf("Não foi possível carregar imagem %s! SDL Error: %s\n", path, SDL_GetError());
    }
```

Caso não ocorra erro, optimize a imagem, deloque "loadedSurface" para que está possa receber mais imagens e não haja conflito, e retorne a optimizada:

```
	else
	{
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Não foi possível optimizar a imagem %s! SDL Error: %s\n", path, SDL_GetError() );
		}
		SDL_FreeSurface( loadedSurface );
	}
	return optimizedSurface;
}
```

Crie a função "int init()":

```
int init()
{
	int success = 1;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL não pôde ser inicializado! SDL_Error: %s\n", SDL_GetError() );
		success = 0;
	}
	else
	{
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window não pôde ser criada! SDL_Error: %s\n", SDL_GetError() );
			success = 0;
		}
```		

Inicie o SDL_image, e pegue a surface gWindow :

```
		else
		{
            int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				printf( "SDL_image não pôde iniciar! SDL_image Error: %s\n", IMG_GetError() );
				success = 0;
			}
			else
			{
                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
		}
	}
	return success;
}
```

Crie a função "int loadMedia()", que irá chamar a função "loadSurface("endereço da imagem")" para cada tecla:

```
int loadMedia()
{
    int success = 1;
    teclas[ tecla_default ] = loadSurface( "imagens/press.png" );
    if( teclas[ tecla_default ] == NULL )
    {
        printf( "Falha ao carregar a imagem default!\n" );
        success = 0;
    }
    teclas[ tecla_cima ] = loadSurface( "imagens/up.png" );
    if(teclas[tecla_cima] == NULL)
    {
        printf( "Falha ao carregar a imagem cima!\n" );
        success = 0;
    }
    teclas[ tecla_baixo ] = loadSurface( "imagens/down.png" );
    if(teclas[tecla_baixo] == NULL)
    {
        printf("Falha ao carregar a imagem baixo!\n");
    }
    teclas[ tecla_esquerda ] = loadSurface( "imagens/left.png" );
    if( teclas[ tecla_esquerda ] == NULL )
    {
        printf( "Falha ao carregar a imagem esquerda!\n" );
        success = 0;
    }
    teclas[ tecla_direita ] = loadSurface( "imagens/right.png" );
    if(teclas[tecla_direita] == NULL)
    {
        printf("Falha ao carregar a imagem direita!\n");
        success = 0;
    }
    return success;
}
```

Crie a função "void close()", que irá delocar tecla por tecla:

```
void close()
{
    for( int i = 0; i < tecla_total; i++ )
        {
            SDL_FreeSurface( teclas[ i ] );
            teclas[ i ] = NULL;
        }

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	SDL_Quit();

}
```

Crie a "int main()" e inicie a "gCurrentSurface" como tecla_default: 

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
			printf( "Falha ao carregar a mídia!\n" );
		}
		else
		{
			int quit = 0;
			SDL_Event e;
			gCurrentSurface = teclas[ tecla_default ];
			while( !quit )
			{
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = 1;
					}
					
```

Utilize o leitor de eventos "e" para ler as teclas do teclado. Utilize o switch para fazer as condições das teclas:

```
					else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_UP:
                            gCurrentSurface = teclas[ tecla_cima ];
                            break;

                            case SDLK_DOWN:
                            gCurrentSurface = teclas[ tecla_baixo ];
                            break;

                            case SDLK_LEFT:
                            gCurrentSurface = teclas[ tecla_esquerda];
                            break;

                            case SDLK_RIGHT:
                            gCurrentSurface = teclas[ tecla_direita ];
                            break;

                            default:
                            gCurrentSurface = teclas[ tecla_default ];
                            break;
                        }
					}
				}
```

Declare "stretchRect" do tipo "SDL_Rect" que irá conter o tamanho ta tela e irá forncer à imagem. Carregue a imagem e atualize a tela:

```
                SDL_Rect stretchRect;
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;
				SDL_BlitScaled( gCurrentSurface, NULL, gScreenSurface, &stretchRect );
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
$ gcc botoes.cpp -o botoes -lSDL2 -lSDL2_image
$ ./botoes
```



