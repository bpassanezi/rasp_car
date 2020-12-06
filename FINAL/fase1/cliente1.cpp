#include "projeto.hpp"
#include <cekeikon.h>
#include <stdio.h>

uint32_t estado;
void on_mouse(int event, int c, int l, int flags, void* userdata) {
    if (event==EVENT_LBUTTONDOWN) {
        if      (200<=l && l<300 &&   0<=c && c<100) estado=1;
        else if (200<=l && l<300 && 100<=c && c<200 ) estado=2;
        else if (200<=l && l<300 && 200<=c && c<300) estado=3;
        else if (100<=l && l<200 &&   0<=c && c<100) estado=4;
        else if (100<=l && l<200 && 100<=c && c<200 ) estado=0;
        else if (100<=l && l<200 && 200<=c && c<300) estado=6;
        else if (0<=l && l<100 &&   0<=c && c<100) estado=7;
        else if (0<=l && l<100 && 100<=c && c<200 ) estado=8;
        else if (0<=l && l<100 && 200<=c && c<300) estado=9;
        else estado=0;

        cout << "estado " << estado << endl;
    }
    else if (event==EVENT_LBUTTONUP) {
        estado=0;
    }
}


int main(int argc, char *argv[]){

    printf("OI \n");

    // Read user input
    if (argc != 2)
        erro("client1 servidorIpAddr\n");


    Mat_<COR> teclas;

    le(teclas, "setas.png");
    namedWindow("janela",WINDOW_NORMAL);
    resize(teclas, teclas, Size(320, 320));
    setMouseCallback("janela", on_mouse);

    CLIENT client(argv[1]);
    vector<BYTE> vb;
    Mat_<COR> camera_display(240, 320);
    Mat_<COR> tela_com_camera;

    int ch=-1;
    client.sendUint(estado);

    while (ch<0) {
        // recebe imagem
        client.receiveImgComp(camera_display);
        resize(camera_display, camera_display, Size(240, 320));

        // envia estado
        client.sendUint(estado);

        // mostra na janela do PC
        tela_com_camera = grudaH(teclas, camera_display);
        imshow("janela", tela_com_camera);

        ch=waitKey(30);
    }
    return 0;
}