//raspcam_ocv.cpp
//compila raspcam_ocv -ocv (usando "compila" do Cekeikon)
// OU //g++ raspcam_ocv.cpp -o raspcam_ocv `pkg-config opencv --libs --cflags` -O3 -s

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(){  
    VideoCapture w(0);
    if(!w.isOpened()){
            printf("Erro: Abertura de webcam 0.\n");
            exit(0);
        }
    w.set(CV_CAP_PROP_FRAME_WIDTH,320); 
    w.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    Mat a;
    namedWindow("janela");
    while(true){
        w >> a;//  get  a  new  frame  from  camera
        imshow("janela",a);
        int ch=(signed char)(waitKey(30));// E necessario (signed char)
        if(ch>=0) break;
    }
}