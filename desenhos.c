#include "desenhos.h"

void desenhaCirculo(Ponto centro, FILE *comandos, Imagem img, Pixel cor){
	/*
	esta função é reponsável por desenhar círculos e foi baseada no algoritmo do Ponto Médio 
	de Bresenham
 	*/
	int raio;
	fscanf(comandos, " %d", &raio);

	int x, y, d=1-raio;
	y=raio;
	x=0;

	while(y > x){
		/*
		algoritmo de Simetria de Ordem 8.
		como estamos calculando com x=0 e y=raio, um arco de 45º graus com centro na origem,
		devemos somar os pontos centrais de x e y para colocarmos o círculo na posição correta
		*/
		if(x+centro.x>=0 && y+centro.y>=0 && x+centro.x<img.largura && y+centro.y<img.altura)
			img.matrizImagem[x+centro.x][y+centro.y] = cor;

		if(y+centro.x>=0 && x+centro.y>=0 && y+centro.x<img.largura && x+centro.y<img.altura)
			img.matrizImagem[y+centro.x][x+centro.y] = cor;

		if(-x+centro.x>=0 && -y+centro.y>=0 && -x+centro.x<img.largura && -y+centro.y<img.altura)
			img.matrizImagem[-x+centro.x][-y+centro.y] = cor;

		if(-y+centro.x>=0 && -x+centro.y>=0 && -y+centro.x<img.largura && -x+centro.y<img.altura)
			img.matrizImagem[-y+centro.x][-x+centro.y] = cor;

		if(-x+centro.x>=0 && y+centro.y>=0 && -x+centro.x<img.largura && y+centro.y<img.altura)
			img.matrizImagem[-x+centro.x][y+centro.y] = cor;

		if(x+centro.x>=0 && -y+centro.y>=0 && x+centro.x<img.largura && -y+centro.y<img.altura)
			img.matrizImagem[x+centro.x][-y+centro.y] = cor;

		if(-y+centro.x>=0 && x+centro.y>=0 && -y+centro.x<img.largura && x+centro.y<img.altura)
			img.matrizImagem[-y+centro.x][x+centro.y] = cor;

		if(y+centro.x>=0 && -x+centro.y>=0 && y+centro.x<img.largura && -x+centro.y<img.altura)
			img.matrizImagem[y+centro.x][-x+centro.y] = cor;
		
		//cálculo do Ponto Médio
		if(d<0)
			d+= 2*x + 3;
		else{
			d+= 2*(x-y) + 5;
			y--;
		}
		x++;
	}
}

void desenhaRetangulo(Ponto p, FILE *comandos, Imagem *img, Pixel cor, int TAM){
	/*
	esta função desenha retângulos a partir do ponto superior esquerdo 
	que é passado como parâmetro
	*/
    int largura, altura;
    fscanf(comandos, " %d %d", &largura, &altura);

    Ponto i = p, f;

    f.x = p.x + largura;
    desenhaReta(i, f, img, cor, TAM);
    i.x+= largura;
    f.y-= altura;
    desenhaReta(i, f, img, cor, TAM);
    i.y-= altura;
    f.x-= largura;
    desenhaReta(i, f, img, cor, TAM);
    i.x-= largura;
    f.y+= altura;
    desenhaReta(i, f, img, cor, TAM);
}

void desenhaPoligono(FILE *comandos, Imagem *img, Pixel cor, int TAM){
	//esta função é reponsável por desenhar polígonos a partir dos pontos passados
	int N, i;
	fscanf(comandos, " %d", &N);
	Ponto pontos[N];
	
	for(i=0; i<N; i++){
		fscanf(comandos, " %d %d", &pontos[i].x, &pontos[i].y);
	}

	for(i=0; i<N-1; i++){
		desenhaReta(pontos[i], pontos[i+1], img, cor, TAM);
	}
	
	//liga último ponto ao primeiro para fechar a figura
	desenhaReta(pontos[0], pontos[N-1], img, cor, TAM);
}

void desenhaPoligono3D(FILE *comandos, Imagem *img, Pixel cor, int TAM){
	//esta função é reponsável por desenhar polígonos 3D a partir de pontos de um polígono plano passados
	int P, N, i;
	//lê a profundidade(P) e quantidade de pontos(N)
	fscanf(comandos, " %d %d", &P, &N);
	Ponto pontos[N], aux[N];
	
	//lê os pontos do polígono plano e armazena no vetor de pontos,
	//armazena os pontos também em um vetor auxiliar, no qual os pontos serão modificados
	for(i=0; i<N; i++){
		fscanf(comandos, " %d %d", &pontos[i].x, &pontos[i].y);
		aux[i].x = pontos[i].x;
		aux[i].y = pontos[i].y;
	}
	
	aux[0].x += P;
	aux[0].y += P;
	//somamos a profundida aos pontos do vetor aux, criando assim dois polígonos 'paralelos',
	//depois interligamos os vértices dos polígonos formando um polígono 3D
	for(i=0; i<N-1; i++){
		aux[i+1].x += P;
		aux[i+1].y += P;
		desenhaReta(pontos[i], pontos[i+1], img, cor, TAM);
		desenhaReta(aux[i], aux[i+1], img, cor, TAM);
		desenhaReta(pontos[i], aux[i], img, cor, TAM);
	}
	
	desenhaReta(aux[0], aux[N-1], img, cor, TAM);
	desenhaReta(pontos[0], pontos[N-1], img, cor, TAM);
	desenhaReta(pontos[N-1], aux[N-1], img, cor, TAM);
}

void desenhaReta(Ponto inicial, Ponto final, Imagem *img, Pixel cor, int TAM){
	//esta função é reponsável por desenhar retas/linhas e foi baseada no algoritmo de Bresenham
	float inclinacao = 0;
	int dx, dy;
	char posicaoReta;
	/*
	a variável aux seŕa responsável por incrementar, se inicial.y < final.y, ou
	decrementar, se inicial.y > final.y, o valor de inicial.y.
	as variáveis menorY e maiorY são responsáveis por armazenar, respectivamente,
	o valor do menor y e o valor do maior y, e, são usadas em um while. Elas são importantes pois
	há casos em que inicial.x < final.x e inicial.y > final.y, logo não são cabíveis de inversão.
	*/
	int aux=1, menorY=inicial.y, maiorY=final.y;

	if(inicial.x > final.x){
		inverteCoordenadas(&inicial.x, &final.x);
		inverteCoordenadas(&inicial.y, &final.y);
	}
	if(inicial.y > final.y){
		inverteCoordenadas(&maiorY, &menorY);
		aux = -1;
	}

	//dx é a diferença de final.x e inicial.x, enquanto dy é a diferença de final.y e inicial.y
	dx = final.x-inicial.x;
	dy = (final.y-inicial.y)*aux;
	posicaoReta = verificaPosicaodaReta(dx, dy);

	if(dx == dy){
		inicial.x -= TAM/2;
		final.x += TAM/2;
		menorY -= TAM/2;
		maiorY += TAM/2;
		//se inicial.y <= final.y, então inicial.y irá decrementar e final.y irá incrementar
		//se inicial.y > final.y, então inicial.y irá incrementar e final.y irá decrementar
		inicial.y -= TAM/2*aux;
		final.y += TAM/2*aux;
	}
	else if(posicaoReta == 'X'){ //dx == 0
		menorY -= TAM/2;
		maiorY += TAM/2;
		inicial.y -= TAM/2*aux;
		final.y += TAM/2*aux;
	}
	else if(posicaoReta == 'Y'){ //dy == 0
		inicial.x -= TAM/2;
		final.x += TAM/2;
	}

	//com os novos valores de x e y, calculemos a nova diferença das coordenadas
	dx = final.x-inicial.x;
	dy = (final.y-inicial.y)*aux;

	if(dx >= dy){
		while(inicial.x <= final.x){
			pintaPixeldaReta(TAM, img, cor, inicial, posicaoReta);

			inclinacao += dy*1.0/dx; 
			if(inclinacao >= 1){
				//ao atingir 1, devemos decrementar a inclinção para repitir o processo
				inclinacao--;
				/*
				ao incrementar ou decrementar inicial.y, o que dependerá da variável aux,
				devemos decrementar dy, já que a diferença entre final.y e inicial.y ficará menor
				*/
				inicial.y+= aux;
				dy--;
			}
			//ao incrementar inicial.x devemos decrementar dx, 
			//já que a diferença entre final.x e inicial.x ficará menor
			inicial.x++;
			dx--;
		}
	}
	//se dy for maior que dx, faremos os mesmos passos anteriores
	//invertendo apenas os papéis de dx e dy, e, x e y nas operações
	else{
		while(menorY <= maiorY){
			pintaPixeldaReta(TAM, img, cor, inicial, posicaoReta);

			inclinacao += dx*1.0/dy;
			if(inclinacao >= 1){
				inclinacao--;
				inicial.x++;
				dx--;
			}
			inicial.y+= aux;
			menorY++;
			dy--;
		}
	}
}

void desenhaCurva(FILE *comandos, Imagem *img, Pixel cor){
	//esta função é reponsável por desenhar curvas de Bézier
	int i, j, K;
	//lê quantidade de pontos
	fscanf(comandos, " %d", &K);
	//matriz para o triângulo de Pascal
	int T[K][K];

	Ponto o, f, p[K];
	//lê os pontos de controle e armazena no vetor p
	for(i=0; i<K; i++){
		fscanf(comandos, " %d %d", &p[i].x, &p[i].y);
	}

	//definimos n como 30 vezes a quantidade de pontos
	int n=K*30;
	//t é um valor de parametrização para percorrer a curva
	float t, dt=1.0/n, x, y;

	f.x = p[0].x;
	f.y = p[0].y;

	//fazemos o triângulo de Pascal de acordo com a quantidade de pontos usados para curva
	//já que a curva baseia seu cálculo no Binômio de Newton para a resolução de seus coeficientes 
	T[0][0] = 1;
	for (i = 1; i < K; i++){
		T[i][0] = 1;
		T[i][i] = 1;
		for (j = 1; j < i; j++){
			T[i][j] = T[i-1][j] + T[i-1][j-1];
		}
	}

	for(i=0; i<=n; i++){
		o.x = f.x;
		o.y = f.y;
		t = i*dt;
		x = 0;
		y = 0;
		for(j=0; j<K; j++){
			//realizamos o cálculo para os pontos de acordo com o resultado da fórmula (t+(t-1))^(K-1)
			//que é dado por um somatório, por isso o for
			x += T[K-1][j]*pow(t, j)*pow(1-t, K-1-j)*p[j].x;
			y += T[K-1][j]*pow(t, j)*pow(1-t, K-1-j)*p[j].y;
		}
		f.x = x;
		f.y = y;
		desenhaReta(o, f, img, cor, 1);
	}
}