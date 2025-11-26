 Jogo Flappy em C 

- Esse projeto é um jogo simples inspirado no famoso Flappy Bird e foi desenvolvido TOTALMENTE em C, executado diretamente no terminal.  
- O player controla um pássaro que precisa desviar dos canos enquanto acumula pontos.  
- O jogo possui sistema de pontuação, ranking, salvamento e carregamento de progresso.



	Como jogar

- O pássaro cai automaticamente com o tempo.
- Pressione **barra de espaço** ou **ENTER** para fazer o pássaro pular.
- O jogo termina quando o personagem colide com um cano ou com o chão.
- Sua pontuação aumenta cada vez que passa entre dois canos.


	Como compilar

1 - Certifique-se de ter o GCC instalado.  
2 - Dentro da pasta do projeto, execute no terminal:

	Windows 

1 - gcc *.c -o flappy
2 - ./flappy


	Linux - Ubuntu, Mint, Debian, Arch e etc.

1 - gcc main.c game.c io.c util.c model.c -o flappy
2- ./flappy
chmod +x flappy - Caso erros de permissões.

	MacOS

1 - gcc main.c game.c io.c util.c model.c -o flappy
2 - ./flappy

	Onde ficam os arquivos criados

O jogo CRIA a pasta (Caso não exista ainda):

data/

	na mesma fica: 

- salvamento.txt		<- estado de jogo salvo 
- ranking.txt		<- Ranking com nome e pontuação (ex: 1 - Pedro 10 pontos )
- melhor_pontuacao.txt  	<- Melhor pontuação do jogador até o momento 

	caso a pasta não existir ou não puder ser criada, o jogo avisa:

-> Aviso: nao foi possível criar/abrir a pasta 'data'. Salvamento pode falhar. 

	Controles (todos os sistemas)

Começar/Confimar 	->	ENTER
Pular			->	W ou ESPACO
Sair durante o jogo	->	Q

	
