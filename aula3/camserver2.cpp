#include "projeto.hpp"
#include <cekeikon.h>

int main(void)
{
    SERVER server;
    server.waitConnection();

    uint32_t estado = 0;
    uint32_t novo_estado = 0;

    VideoCapture w(0);
	if(!w.isOpened()) erro("Erro: Abertura de webcam 0."); 
	w.set(CV_CAP_PROP_FRAME_WIDTH,640); 
	w.set(CV_CAP_PROP_FRAME_HEIGHT,480); 
	Mat_<COR> a;
	namedWindow("janela");

	while(true){
        // recebe novo estado
        server.receiveUint(novo_estado);
        estado = novo_estado;
        cout << estado << endl;

	    w >> a;//  get  a  new  frame  from  camera

        server.sendImgComp(a);
	    imshow("janela",a);
	    int ch=(signed char)(waitKey(30));// E necessario (signed char)
	    if(ch>=0) break;
	}

}