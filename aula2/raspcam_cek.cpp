//raspcam_cek.cpp
//compila raspicam_cek -c

#include <cekeikon.h>

int main(){  
	VideoCapture w(0);
	if(!w.isOpened()) erro("Erro: Abertura de webcam 0."); 
	w.set(CV_CAP_PROP_FRAME_WIDTH,320); 
	w.set(CV_CAP_PROP_FRAME_HEIGHT,240); 
	Mat_<COR> a;
	namedWindow("janela");
	while(true){
	    w >> a;//  get  a  new  frame  from  camera
	    imshow("janela",a);
	    int ch=(signed char)(waitKey(30));// E necessario (signed char)
	    if(ch>=0) break;
	}
}
