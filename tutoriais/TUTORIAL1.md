# Objetivo (Céu)

## SDL com C/C++

## Preparando o ambiente para SDL no ubuntu 14.04 - 15.10

Para isso, é necessário instalar alguns arquivos da biblioteca de SDL. Abra o terminal e digite os seguintes comandos:

```
sudo apt-cache search libsdl2
sudo apt-get install libsdl2-dev
```

No início de todos os códigos dos programas, inclua a biblioteca do SDL com essa linha de comando:

```
 #include<SDL2/SDL.h>
```

### Primeiro programa

Primeiro precisamos fazer o mais simples: criar uma interface e compilar um exemplo de SDL somente com C (main.cpp):

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

Inicie a main, crie o ponteiro do tipo "SDL_Window" que será renderizado, o ponteiro do tipo "SDL_Surface" que será a superfice que a window conterá:

```
int main( int argc, char* args[] ) 
{
	SDL_Window* window = NULL;
	
	SDL_Surface* screenSurface = NULL;
```

Para iniciar o SDL, crie exceções (if / else) para se organizar melhor e saber os erros dentro do código. A função "SDL_INIT" iniciará o SDL, e retornará -1 caso ocorra algum erro. Inicializando o SDL:

```
	if( SDL_Init( SDL_INIT_VIDEO ) < 0) 
	{ 
		printf("SDL não pode inicializar. SDL_Error: %s\n", SDL_GetError());
	}
```

Caso não ocorra erro na função, cria-se a window através da função "SDL_CreateWindow" e checa se "window" a contem:

```
	else
	{
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Não foi possível criar a window. SDL_Error: %s\n", SDL_GetError() ); 
		}
```

Com "window" criada, pega-se a superfície da window, utilizando o comando "SDL_GetWindowSurface", para que possamos desenhar nela. Depois, tornamos a tela em branco. Por ultimo, deve-se atualizar a tela, para que possamos ver os desenhos feitos e espera-se 2000 milisegundo (2 segundos) para sair do else:

```
		else
		{
			screenSurface = SDL_GetWindowSurface( window );
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
			SDL_UpdateWindowSurface( window );
			delay(2000);
		}
	}

```

Por fim, deve-se destruir a window e sair do SDL, para finalizar o programa:

```
	SDL_DestroyWindow( window );
	SDL_Quit();
	return 0;
}
```


Para compilar e executar, basta escrever os seguintes comandos no terminal:

```
$ gcc main.cpp -o main -lSDL2
$ ./main
```



