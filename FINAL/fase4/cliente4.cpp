#include "projeto.hpp"
#include <cekeikon.h>
#include <stdio.h>

uint32_t estado;

void achaQuadrado( Mat_<COR>& img, Mat_<FLT> quadrado){
	// Funcao para imprimir o quadrado na imagem
	Mat_<FLT> imgFLT(img.size());
	Mat_<FLT> result;

	#pragma omp parallel for
	for (int ii = 0; ii  < img.rows; ii++)
		for (int jj = 0; jj  < img.cols; jj++)
			imgFLT(ii,jj) = (float)(img(ii,jj)[0])/256;

	double minVal; double maxVal;
	Point minLoc; Point maxLoc; Point matchLoc;

	int tamanho; float aux = 0.0; int proportion;

	// procura o tamanho do quadrado que melhor se encaixa na imagem
    for ( int s = 15; s < 60; s=s+5){
		resize( quadrado, quadrado, Size(s,s), 0, 0, INTER_AREA );
		quadrado = somaAbsDois( dcReject( quadrado ) );
		matchTemplate( imgFLT, quadrado, result, CV_TM_CCORR );
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );
		if ( maxVal > aux){
			aux = maxVal;
			tamanho = s;
			matchLoc = maxLoc;
		}
	}
	// encontra o quadrado com o tamanho certo na imagem
    resize( quadrado, quadrado, Size(tamanho,tamanho), 0, 0, INTER_AREA );
	quadrado = somaAbsDois( dcReject( quadrado ) );
	matchTemplate( imgFLT, quadrado, result, CV_TM_CCORR );
	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );


	// imprime o quadrado no video
    if(maxVal > 0.3f){
		line( img, Point( matchLoc.x + quadrado.cols/2 , matchLoc.y + quadrado.rows/2 + 0.12*quadrado.rows ),
			Point( matchLoc.x + quadrado.cols/2 , matchLoc.y + quadrado.rows/2 - 0.12*quadrado.rows ), Scalar(0,255,0), 1, 8);
		line( img, Point( matchLoc.x + quadrado.cols/2 + 0.12*quadrado.rows , matchLoc.y + quadrado.rows/2 ),
			Point( matchLoc.x + quadrado.cols/2 - 0.12*quadrado.rows , matchLoc.y + quadrado.rows/2 ), Scalar(0,255,0), 1, 8);
		rectangle( img, matchLoc, Point( matchLoc.x + quadrado.cols , matchLoc.y + quadrado.rows ), Scalar(0,255,0), 1.5, 8);

        // calcula proporcao do quadrado em relacao ao tamanho do video
        proportion = round(((matchLoc.x)*100)/(img.cols));
        cout << proportion << endl;
        if (proportion > 75) {
            // se está  muito perto, para o carrinho
            estado = 0;
        }
        else if (proportion < 75) {
            // se está longe, anda em direção ao quadrado
            estado = 8;
        }
	}
    else{
        estado = 0;
    }
}


int main(int argc, char *argv[]){

    printf("OI \n");

    // Read user input
    if (argc != 2)
        erro("client6 servidorIpAddr\n");


    Mat_<FLT> quadrado;
	le( quadrado, "quadrado.png" );

    CLIENT client(argv[1]);
    vector<BYTE> vb;
    Mat_<COR> camera_display(240, 320);
    Mat_<COR> tela_com_camera;

    int ch=-1;
    estado = 0;
    client.sendUint(estado);

    while (ch<0) {
        // recebe imagem
        client.receiveImgComp(camera_display);
        resize(camera_display, camera_display, Size(240, 320));

        achaQuadrado(camera_display, quadrado);

        // envia estado
        client.sendUint(estado);

        // mostra na janela do PC
        imshow("janela", camera_display);

        ch=waitKey(30);
    }
    return 0;
}