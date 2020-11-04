#include "projeto.hpp"
#include <cekeikon.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    // Read user input
    if (argc != 2) 
        erro("client6 servidorIpAddr\n");

    CLIENT client(argv[1]);
    vector<BYTE> vb;
    Mat_<COR> camera_display(480, 640);
    
    int ch=-1;
    uint32_t estado=0;
    client.sendUint(estado);        

    while (ch<0) {
        // recebe imagem
        client.receiveImg(camera_display);
        
        // envia estado
        estado += 1;
        client.sendUint(estado);        

        // mostra na janela do PC
        imshow("camera", camera_display);
        ch=waitKey(30);
    }
}