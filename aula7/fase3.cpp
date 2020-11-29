// [Lição   de   casa   1   da   aula   5] 
// Faça   um   programa   fase3.cpp   que   lê   um   vídeo   240x320 
// pixels(capturado*.avi), um modelo de placa quadrado.png
// e localiza a placa-modelo nos seus quadros,
// toda vez que a placa estiver entre aproximadamente 30 e 150
// cm da câmera, gerando o vídeo desaída com a localização da placa
// (figura 1).
// Calcule quantos quadros/segundo o seu programa
// consegue processar


#include <cekeikon.h>

void printQuadrado( Mat_<COR>& img, Mat_<FLT> quadrado){				
	// Funcao para imprimir o quadrado na imagem
	Mat_<FLT> imgFLT(img.size());
	Mat_<FLT> result;
	
	#pragma omp parallel for
	for (int ii = 0; ii  < img.rows; ii++)
		for (int jj = 0; jj  < img.cols; jj++)
			imgFLT(ii,jj) = (float)(img(ii,jj)[0])/256;

	double minVal; double maxVal;
	Point minLoc; Point maxLoc; Point matchLoc;

	int tamanho; float aux = 0.0;
	
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
    if( tamanho > 20 && maxVal > 0.3f){
		line( img, Point( matchLoc.x + quadrado.cols/2 , matchLoc.y + quadrado.rows/2 + 0.12*quadrado.rows ), 
			Point( matchLoc.x + quadrado.cols/2 , matchLoc.y + quadrado.rows/2 - 0.12*quadrado.rows ), Scalar(0,255,0), 1, 8);
		line( img, Point( matchLoc.x + quadrado.cols/2 + 0.12*quadrado.rows , matchLoc.y + quadrado.rows/2 ), 
			Point( matchLoc.x + quadrado.cols/2 - 0.12*quadrado.rows , matchLoc.y + quadrado.rows/2 ), Scalar(0,255,0), 1, 8);
		rectangle( img, matchLoc, Point( matchLoc.x + quadrado.cols , matchLoc.y + quadrado.rows ), Scalar(0,255,0), 1.5, 8);
	}
}
	
int main( int argc, char** argv )
{

	// Read user input
	if (argc != 3) 
		erro("\nErro: invalid number of arguments");
	// Read input video
	string videoFile = argv[1];
    // Output video name
    string outputFile = argv[2];

	if (!existeArq(videoFile)) 
		erro("\nEro: Video not found \n");
	
	VideoCapture vi(videoFile);

	float fps = vi.get(CV_CAP_PROP_FPS);
	int nc = vi.get(CV_CAP_PROP_FRAME_WIDTH);
	int nl = vi.get(CV_CAP_PROP_FRAME_HEIGHT);
	int frames = vi.get(CV_CAP_PROP_FRAME_COUNT);

	// Create output video
	videoFile.erase(videoFile.end()-4, videoFile.end());
	// string videoName = videoFile;
	// videoFile += "-fase3.avi";
	VideoWriter vo(outputFile, CV_FOURCC('X','V','I','D'), fps, Size(nc,nl));

	//Create matrix
	Mat_<COR> src;
	Mat_<FLT> quadrado; 
	le( quadrado, "quadrado.png" );

	printf("\nAguarde... O video esta sendo processado\n");

	//Frame analysis
	for (int i = 1; i <= frames; i++) 
	{
		//Save frame in image
		vi >> src;
		if (i == 450) imp( src, outputFile); 
		
		printQuadrado(src, quadrado); 
		if (i == 450) imp( src, outputFile); 

		//Show results
		namedWindow("Template Matching Video", 0);
		imshow("Template Matching Video", src);
		if (waitKey(30) >= 0) 
			break;

		//Save image in output video
		vo << src;
	}

	cout << "\n\t" + outputFile + " gerado com sucesso\n";
	destroyAllWindows();
}