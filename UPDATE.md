# 9/06

## O que eu ia fazer durante a semana passada?

## O que eu realmente fiz durante a semana passada?
* Passei o sistema de dano pro jogoc.cpp. Acabei esquecendo de dar git add no dano.cpp na semana passada
* Fiz imagens para o soco dos inimigos
* Pensei em um jeito de fazer o inimigo socar quando estiver em contato e em um jeito de parecer um pouco mais realista (o soco estava durando 1 frame)
* Soco do inimigo pronto
* Põe titulo no início do jogo
* Coloquei o score no início do jogo
* Procurei por bugs na mecanica do jogo

## O que eu vou fazer durante a próxima semana?


## Quantas horas de trabalho?
* 2 hora e meia

09:45

=======
# 02/06

## O que eu ia fazer durante a semana passada?
* TTF
* Sistema de dano
* Consertar o modo punch do jogoc.cpp
* Melhorar a distancia do soco esquerdo
* Consertar o botão da tela de transicao inicial

## O que eu realmente fiz durante a semana passada?
* Consertei a distancia do soco esquerdo, agora o boneco não se move para tras quando soca pra esquerda
* Consertei o botão da tela de transicao inicial
* Consertar o modo punch do jogoc.cpp, agora ele volta pro stand quando não está socando
* Pesquisei ainda mais como colocar o SDL2_TTF na máquina e finalmente achei
* Passei o ttf para C, para usar no jogo (ttf.cpp)
* Implementei no jogoc.cpp
* Pesquisei como usar o ttf para colocar escrever variaveis como vida e score, já que é obrigatório o uso de const char* no tff
* Coloquei contador de vida e score, e score incrementa 1 a cada soco acertado
* Fiz o TUTORIAL5.md sobre SDL_ttf, junto com um código de apoio
* Pensei em como contar o tempo quando o inimigo se encontra com o personagem, para fazer o sistema de dano
* Sistema de dano pronto (dano.cpp)
* Procurei por bugs e achei que as vezes, depois de dar restart no jogo, a imagem do lazy.ttf não consegue ser aberta

## O que eu vou fazer durante a próxima semana?


## Quantas horas de trabalho?
* 8 horas e meia

=======
# 27/05

## O que eu ia fazer durante a semana passada?
* Telas de transição (inicio, jogo e fim)
* Randomizar o lado que o inimigo nasce
* TTF
* Sistema de dano

## O que eu realmente fiz durante a semana passada?
* Procurei como instalar ttf na máquina e como utiliza-lo (ainda nao consegui)
* Sistema para ler evento do mouse
* Adicionei pagina de transição, porém, está com bug (só pode clicar na parte superior esquerda do botao vermelho para começar)
* Os bonecos nascem de lados aleatórios
* Pensei em um sistema de pulo 
* Coloquei a velocidade no movimento.cpp relativa ao fps e vou trabalhar o sistema de pulo neste código
* Sistema de pulo pronto no movimento.cpp
* Coloca pulo no jogoc.cpp e altera os casos de colisões e hits 
* O personagem não atravessa mais os inimigos
* Pesquisei e tentei instalar mais vezes o ttf com .tar.gz, make install e etc, mas ainda não deu certo
* Fiz imagem punchright e punchleft
* Implementei os socos no jogoc.cpp, porém, falta tirar do modo punch depois que soltar o botao

## O que eu vou fazer durante a próxima semana?
* TTF
* Sistema de dano
* Consertar o modo punch do jogoc.cpp
* Melhorar a distancia do soco esquerdo
* Consertar o botão da tela de transicao inicial

## Quantas horas de trabalho?
* 8 horas

=======

# 19/05

## O que eu ia fazer durante a semana passada?
* Aprimorar os tutoriais
* Usar millis
* Passar pra C
* TTF
* Sistema de dano
* Telas de transição (inicio, jogo e fim)

## O que eu realmente fiz durante a semana passada?
* Aprimorei o código jogo.cpp
* Passei o boneco.cpp para C
* Aprimorei os tutoriais
* Coloquei o millis, mas ainda vou ver como usa-lo para movimento
* Passa movimento.cpp para C, em funcionamento
* Pesquisei sobre o funcinamento do millis
* Coloquei o millis para fazer o movimento no jogo.cpp
* Passei o jogo.cpp para C e botei no código jogoc.cpp
* pensei em uma nova ideia do jogo
* No jogoc.cpp, comecei a trabalhar na nova ideia do jogo

## O que eu vou fazer durante a próxima semana?
* TTF
* Sistema de dano
* Telas de transição (inicio, jogo e fim)
* Randomizar o lado que o inimigo nasce

## Quantas horas de trabalho?
* 8 horas

=======

# 12/05

## O que eu ia fazer durante a semana passada?
* passar o jogo.cpp para C (preciso ir à PUC tirar dúvidas)
* fazer mais tutoriais

## O que eu realmente fiz durante a semana passada?
* Tutorial 4 (colocando duas imagens na interface, com o uso de colorkey)
* Alteração nos 2 ultimos tutoriais
* Alteração no codigo jogo.cpp para assim q o botao estiver apertado o boneco anda com uso de keystate, e travas nas laterais da tela
* Alteração no código jogo.cpp para, quando não apertar nenhum botão, ficar no modo stand 
* Resolver alguns bugs acima 
* passei o codigo de jogo.cpp para movimento.cpp, para ter salvo a mecanica de movimento, e poder aprimorar o jogo em si no codigo jogo.cpp
* Criei imagem para inimigo
* Evento ao colidir (não ultrapassa o boneco)
* passei o codigo de jogo.cpp, com colisao, para colisao.cpp, para ter salvo a mecanica de colisao, e poder aprimorar o jogo em si no codigo jogo.cpp
* Começa o jogo. quando aperta as teclas para o lado, o boneco vermelho some (não terminado)
* Debugar para boneco sumir (ainda falta)

## O que eu vou fazer durante a próxima semana?
* Usar millis
* Passar pra C
* TTF
* Sistema de dano
* Telas de transição (inicio, jogo e fim)

## Quantas horas de trabalho?
* 8 horas e meia

# 21/04

## O que eu ia fazer durante a semana passada?
* Escrever o TUTORIAL.md
* criar eventos caso aconteça choque na animação

## O que eu realmente fiz durante a semana passada?
* Começar o TUTORIAL.md
* TUTORIAL1.md (iniciar uma inteface de sdl)
* TUTORIAL2.md (colocar imagem na tela e fechar no x)
* TUTORIAL3.md (ler teclado, imagem .png e ajustar imagem para tamanho da tela)

## O que eu vou fazer durante a próxima semana?
* passar o jogo.cpp para C (preciso ir à PUC tirar dúvidas)
* fazer mais tutoriais

## Quantas horas de trabalho?
4 horas

=======

# 14/04

## O que eu ia fazer durante a semana passada?
* Ler comandos do teclado e animar
* Animar figuras
  * criar eventos caso aconteça choque

## O que eu realmente fiz durante a semana passada?
* Movimenta o boneco conforme as setas do teclado para direita e esquerda e
* Faz imagem para quando o boneco tiver fora de movimento ( a ser melhorada)
* Estraguei tudo dando checkout no lugar de rm no git, desculpa..
* Refiz o código, porém bem mais rápido
* Pensar sobre e melhorar a ideia do jogo

## O que eu vou fazer durante a próxima semana?
* Escrever o TUTORIAL.md
* criar eventos caso aconteça choque na animação

## Quantas horas de trabalho?
5 horas

=======

# 7/04

## O que eu ia fazer durante a semana passada?
* Juntar fundo com figura na interface
* Animar figuras
  * criar eventos caso aconteça choque

## O que eu realmente fiz durante a semana passada?
* consertar bugs do test.cpp e jogo.cpp
* consertar o bug do driver na máquina
* Tentar converter o código de animação de C++ para C (boneco.cpp). Falta debug.
* Juntar imagem e animação

## O que eu vou fazer durante a próxima semana?
* Ler comandos do teclado e animar
* Animar figuras
  * criar eventos caso aconteça choque

## Quantas horas de trabalho?
5 horas e meia

=======

# 31/03

## O que eu ia fazer durante a semana passada?
* Juntar fundo com figura na interface
* Animar figuras
  * criar eventos caso aconteça choque

## O que eu realmente fiz durante a semana passada?
* consertar bugs do test.cpp e jogo.cpp
* Começar a por duas imagens diferentes na tela 
* Resolver bugs para rodar o código no lablua
* Tentar converter o código de animação de C++ para C (boneco.cpp)

## O que eu vou fazer durante a próxima semana?
* Juntar fundo com figura na interface
* Animar figuras
  * criar eventos caso aconteça choque

## Quantas horas de trabalho?
5 horas e meia

=======

# 24/03

## O que eu ia fazer durante a semana passada?
* Começar um jogo simples.
  * exibição de uma figura na tela
  * animação desta figura

## O que eu realmente fiz durante a semana passada?
* Incrementar otimização da resolução da interface
* Possibilitar o uso de imagens .png
* exibição de uma figura geometrica na tela
* Criar uma interface básica com uma figura e um plano de fundo (a ser corrigido)
* Exibição de uma figura animada na tela
* Criar um interface básica com uma figura animada e um plano de fundo (a ser corrigido)
* Tentar converter o código de animação de C++ para C (boneco.cpp)
* Instalar drivers e reboots na máquina de casa para efetuar execuções de códigos com problemas de " No hardware accelerated renderers available "

## O que eu vou fazer durante a próxima semana?
* Juntar fundo com figura na interface
* Animar figuras
  * criar eventos caso aconteça choque

## Quantas horas de trabalho?
11 horas e meia

=======

# 17/03

## O que eu ia fazer durante a semana passada
* Começar um jogo simples.
  * exibição de uma figura na tela
  * animação desta figura

## O que eu realmente fiz durante a semana passada?
* Colocar um plano de fundo na interface
* Adicionar o evento para desligar no botão x
* Adicionar leitor e evento para uso do teclado

## O que eu vou fazer durante a próxima semana?
* Exibir uma figura na tela
* Animar a figura

## Quantas horas de trabalho?
8 horas

=======

# 10/03

## O que eu ia fazer durante a semana passada?
Nada.

## O que eu realmente fiz durante a semana passada?
Nada.

## O que eu vou fazer durante a próxima semana?

* Começar um jogo simples.
  * exibição de uma figura na tela
  * animação desta figura
