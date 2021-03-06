#include "projeto.hpp"
#include "controle.hpp"
#include <cekeikon.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>

using namespace cv;
using namespace std;


int main(void)
{
    SERVER server;
    server.waitConnection();

	CONTROLE control;

    uint32_t estado = 0;
    uint32_t novo_estado = 0;

    VideoCapture w(0);
	if(!w.isOpened()) erro("Erro: Abertura de webcam 0."); 
	w.set(CV_CAP_PROP_FRAME_WIDTH,320); 
	w.set(CV_CAP_PROP_FRAME_HEIGHT,240); 
	Mat_<COR> a;
	namedWindow("janela");

	while(true){
        // recebe novo estado
        server.receiveUint(novo_estado);
        estado = novo_estado;

	    w >> a;//  get  a  new  frame  from  camera

		//anda
		control.anda(estado);

		putText(a, to_string(estado), Point(5,100), FONT_HERSHEY_DUPLEX, 1, Scalar(200,200,200), 2);

        server.sendImgComp(a);
	    imshow("janela",a);
	    int ch=(signed char)(waitKey(30));// E necessario (signed char)
	    if(ch>=0) break;
	}

}