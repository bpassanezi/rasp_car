#include <cekeikon.h>
#include <wiringPi.h>
#include <softPwm.h>

class CONTROLE{
    public:
    CONTROLE(){
        wiringPiSetup();
        if(softPwmCreate(0,0,100)) erro("erro");
        if(softPwmCreate(1,0,100)) erro("erro");
        if(softPwmCreate(2,0,100)) erro("erro");
        if(softPwmCreate(3,0,100)) erro("erro");
    }
    ~CONTROLE(){

    }
    void anda(uint32_t estado){
        // Esquerda re
        if (estado == 1){
            // roda direita para tras
            softPwmWrite(0,0);
            softPwmWrite(1,90);
            // roda esquerda para tras
            softPwmWrite(2,0);
            softPwmWrite(3,20);
        }
        // Da re
        else if (estado == 2){
            // roda direita para tras
            softPwmWrite(0,0);
            softPwmWrite(1,60);
            // roda esquerda para tras
            softPwmWrite(2,0);
            softPwmWrite(3,60);
        }
        // Direita re
        else if (estado == 3){
            // roda direita para tras
            softPwmWrite(0,0);
            softPwmWrite(1,20);
            // roda esquerda para tras
            softPwmWrite(2,0);
            softPwmWrite(3,90);
        }
        // Virada acentuada direita
        else if (estado == 6){
            // roda direita para frente
            softPwmWrite(0,0);
            softPwmWrite(1,80);
        }
        else if (estado == 5){
            // Para roda direita
            softPwmWrite(0,0);
            softPwmWrite(1,0);
            // Para roda esquerda
            softPwmWrite(2,0);
            softPwmWrite(3,0);
        }
        // Virada acentuada esquerda
        else if (estado == 4){
            // roda direita para frente
            softPwmWrite(1,0);
            softPwmWrite(0,80);
        }
        // Vira direita
        else if (estado == 9){
            // roda esquerda para frente
            softPwmWrite(2,90);
            softPwmWrite(3,0);
            // roda direita para frente
            softPwmWrite(1,0);
            softPwmWrite(0,20);
        }
		// Vira esquerda
        else if (estado == 7){
            // roda esquerda para frente
            softPwmWrite(2,20);
            softPwmWrite(3,0);
            // roda direita para frente
            softPwmWrite(1,0);
            softPwmWrite(0,90);
        }
        // Reto
        else if (estado == 8){
			// roda esquerda para frente
            softPwmWrite(2,60);
            softPwmWrite(3,0);
            // roda direita para frente
            softPwmWrite(1,0);
            softPwmWrite(0,60);
        }
        else{
            // Para roda direita
            softPwmWrite(0,0);
            softPwmWrite(1,0);
            // Para roda esquerda
            softPwmWrite(2,0);
            softPwmWrite(3,0);
        }
    }
};