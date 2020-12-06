#include "projeto.hpp"
#include "visao.hpp"
#include <cekeikon.h>
#include <stdio.h>

uint32_t estado;
uint32_t novo_estado;
uint32_t number;
uint32_t novo_estado1 = 0;
uint32_t novo_estado2 = 0;
uint32_t novo_estado3 = 0;
uint32_t novo_estado4 = 0;
uint32_t novo_estado5 = 0;
uint32_t novo_estado6 = 0;
uint32_t novo_estado7 = 0;
uint32_t novo_estado8 = 0;
uint32_t novo_estado9 = 0;



int main(int argc, char *argv[]){

    printf("OI \n");

    // Read user input
    if (argc != 2)
        erro("client6 servidorIpAddr\n");

    Mat_<FLT> quadrado;
	le( quadrado, "quadrado.png" );

    CLIENT client(argv[1]);
    VISAO visao;
    vector<BYTE> vb;
    Mat_<COR> camera_display(320, 240);
    Mat_<COR> tela_com_camera;

    MNIST mnist(28, true, true);
	mnist.le(".");
	flann::Index ind(mnist.ax,flann::KDTreeIndexParams(4));


    int ch=-1;
    estado = 0;
    novo_estado = 0;
    novo_estado1 = 0;
    novo_estado2 = 0;
    novo_estado3 = 0;
    novo_estado4 = 0;

    client.sendUint(estado);

    while (ch<0) {
        // recebe imagem
        client.receiveImgComp(camera_display);
        resize(camera_display, camera_display, Size(320, 240));

        cv:Rect rect;
        rect = visao.achaQuadrado(camera_display, quadrado);

        for(int i = 0; i < 10; i++){
            if(i == 0){
                novo_estado = visao.achaNumero(ind, mnist, camera_display, rect);
            }
            else if(i == 1){
                novo_estado1 = visao.achaNumero(ind, mnist, camera_display, rect);
            }
            else if(i == 2){
                novo_estado2 = visao.achaNumero(ind, mnist, camera_display, rect);
            }
            else if(i == 3){
                novo_estado3 = visao.achaNumero(ind, mnist, camera_display, rect);
            }
            else if(i == 4){
                novo_estado4 = visao.achaNumero(ind, mnist, camera_display, rect);
            }
            else if(i == 5){
                novo_estado5 = visao.achaNumero(ind, mnist, camera_display, rect);
            }
            else if(i == 6){
                novo_estado6 = visao.achaNumero(ind, mnist, camera_display, rect);
            }
            else if(i == 7){
                novo_estado7 = visao.achaNumero(ind, mnist, camera_display, rect);
            }
            else if(i == 8){
                novo_estado8 = visao.achaNumero(ind, mnist, camera_display, rect);
            }
            else if(i == 9){
                novo_estado9 = visao.achaNumero(ind, mnist, camera_display, rect);
            }
        }

        number = visao.achaNumero(ind, mnist, camera_display, rect);

        if (number == -1){
                novo_estado = estado;
                novo_estado1 = novo_estado;
                novo_estado2 = novo_estado;
                novo_estado3 = novo_estado;
                novo_estado4 = novo_estado;
                novo_estado5 = novo_estado;
                novo_estado6 = novo_estado;
                novo_estado7 = novo_estado;
                novo_estado8 = novo_estado;
                novo_estado9 = novo_estado;
            }
        else if ((number == novo_estado) && (number == novo_estado1) &&
                 (number == novo_estado2) && (number == novo_estado3) &&
                 (number == novo_estado4) && (number == novo_estado5) &&
                 (number == novo_estado6) && (number == novo_estado7) &&
                 (number == novo_estado8) && (number == novo_estado9)
                 && ((number == 9) || (number == 4) || (number == 7) || (number == 1))){
                novo_estado = number;
                estado = novo_estado;
        }

        // envia estado
        client.sendUint(novo_estado);

        // mostra na janela do PC
        imshow("janela", camera_display);

        ch=waitKey(30);
    }
    return 0;
}