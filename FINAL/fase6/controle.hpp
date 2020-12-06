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
        // Virada 90 graus direita
        if (estado == 8 || estado == 9){
            // roda direita para frente
            softPwmWrite(0,0);
            softPwmWrite(1,60);

            wait(1200);

            // roda esquerda para frente
            softPwmWrite(2,40);
            softPwmWrite(3,0);
            // roda direita para frente
            softPwmWrite(1,0);
            softPwmWrite(0,40);
            wait(200);
        }
        // Vira 90 graus esquerda
        else if (estado == 7 || estado == 6){
            // roda direita para frente
            softPwmWrite(1,0);
            softPwmWrite(0,60);

            wait(1500);

            // roda esquerda para frente
            softPwmWrite(2,40);
            softPwmWrite(3,0);
            // roda direita para frente
            softPwmWrite(1,0);
            softPwmWrite(0,40);

            wait(200);
        }
        // Reto
        else if (estado == 4){
			// roda esquerda para frente
            softPwmWrite(2,40);
            softPwmWrite(3,0);
            // roda direita para frente
            softPwmWrite(1,0);
            softPwmWrite(0,40);
        }
        else if ((estado == 0) || (estado == 1)){
            // Para roda direita
            softPwmWrite(0,0);
            softPwmWrite(1,0);
            // Para roda esquerda
            softPwmWrite(2,0);
            softPwmWrite(3,0);
        }
    }
};