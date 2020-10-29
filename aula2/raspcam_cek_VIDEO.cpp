//raspcam_cek.cpp
//compila raspicam_cek -c

#include <cekeikon.h>

int main(int argc, char** argv){  
	
	VideoCapture w(0);
	VideoWriter vo(argv[1], CV_FOURCC('X','V','I','D'), 
				   30, Size(320,240)); 

	cout << argv[1];

	if(!w.isOpened()) erro("Erro: Abertura de webcam 0."); 
	
	w.set(CV_CAP_PROP_FRAME_WIDTH,320); 
	w.set(CV_CAP_PROP_FRAME_HEIGHT,240); 
	
	Mat_<COR> a;
	namedWindow("janela");
	while(true){
	    w >> a;//  get  a  new  frame  from  camera
	    imshow("janela",a); // mostra imagem na tela
		vo << a; // escreve no video
	    int ch=(signed char)(waitKey(30));// E necessario (signed char)
	    if(ch>=0) break;
	}
}
