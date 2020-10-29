#include "projeto.hpp"

int main(int argc,char*argv[]){
    xdebug;
    if(argc!=2) erro("client6 servidorIpAddr\n");
    
    CLIENT client(argv[1]);
    const int n=100000;
    BYTE buf[n];
    
    // Cliente recebe bytes 111 do servidor
    client.receiveBytes(n,buf);
    if(testaBytes(buf,111,n)) printf("Recebeu corretamente %d bytes %d\n",n,111);
    else printf("Erro na recepcao de %d bytes %d\n",n,111);
    
    // Cliente envia bytes 214 para servidor, que vai enviar de volta
    memset(buf,214,n); //insere 214 em n bytes a partir do endereço buf
    client.sendBytes(n,buf);

    // Cliente recebe bytes 214 do servidor
    client.receiveBytes(n,buf);
    if(testaBytes(buf,214,n)) printf("Recebeu corretamente %d bytes %d\n",n,214);
    else printf("Erro na recepcao de %d bytes %d\n",n,214);
    ;
}