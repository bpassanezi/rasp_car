#include <cekeikon.h>


class VISAO{
    public:

    VISAO(){

    }
    ~VISAO(){

    }
    // Funcao responsavel por encontrar o quadrado na imagem
    cv::Rect printSQR( Mat_<COR>& img, Mat_<FLT> quadrado){				
        
        Mat_<FLT> imgFLT(img.size());
        Mat_<FLT> result;
        
        for (int ii = 0; ii  < img.rows; ii++)
            for (int jj = 0; jj  < img.cols; jj++)
                imgFLT(ii,jj) = (float)(img(ii,jj)[0])/256;
            
            
        double minVal; double maxVal;
        Point minLoc; Point maxLoc; Point matchLoc;

        int tamanho; float aux = 0.0;

        #pragma omp parallel for
        for ( int s = 25; s < 60; s = s+5){
            resize( quadrado, quadrado, Size(s,s), 0, 0, INTER_AREA );
            quadrado = somaAbsDois( dcReject( quadrado ) );
            matchTemplate( imgFLT, quadrado, result, CV_TM_CCORR );
            minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );	
            if ( maxVal > aux){
                aux = maxVal;
                tamanho = s;
                matchLoc = maxLoc;
            }
        }

        resize( quadrado, quadrado, Size(tamanho,tamanho), 0, 0, INTER_AREA );
        quadrado = somaAbsDois( dcReject( quadrado ) );
        matchTemplate( imgFLT, quadrado, result, CV_TM_CCORR );
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );

        cv::Rect rect;
        if( maxVal > 0.3f && tamanho >= 30 && tamanho <= 60){
            rectangle( img, matchLoc, Point( matchLoc.x + quadrado.cols , matchLoc.y + quadrado.rows ), Scalar(0,255,0), 1.5, 8);
            rect = Rect(matchLoc.y + tamanho*0.3 ,matchLoc.x  + tamanho*0.3, tamanho*0.4, tamanho*0.4);
            // cout << maxVal << " , " << tamanho << endl;
        }
        else
            rect = Rect(0,0,0,0);
        
        return rect;
        
    }

    // Funcao que encontra o numero no centro da imagem
    int printNumber(flann::Index& ind, MNIST& mnist, Mat_<COR>& img, Rect rec){
        
        if (rec.width < 15)
            return(-1);

        vector<int> indices(1); vector<float> dists(1);
        
        // crio a imagem quadrado  em niveis de cinza
        // contendo somente o que esta dentro
        // do quadrado identificado
        Mat_<GRY> quadrado(rec.width,rec.height);

        #pragma omp parallel for	
        for (int i=0; i<rec.width; i++){
            for (int j=0; j<rec.height; j++){
                quadrado(i,j) = (img(i+rec.x,j+rec.y)[0]
                                + img(i+rec.x,j+rec.y)[1] 
                                + img(i+rec.x,j+rec.y)[2])/3;
                if (quadrado(i,j) > 140)
                    quadrado(i,j) = 255;
                else
                    quadrado(i,j) = quadrado(i,j)/1.5;
            }
        }
        
        Mat_<GRY> dentroQuadrado = mnist.bbox(quadrado);
        
        Mat_<FLT> matToFind(1,28*28); 
        for(int i=0; i<28; i++)
            for(int j=0; j<28; j++)
                matToFind(i*28 + j) = (float)(dentroQuadrado(i,j))/255.0;
        
        ind.knnSearch(matToFind,indices,dists,1);
    
        int result = mnist.ay(indices[0]);

        if ((result == 9) || (result == 1) || (result == 4) || (result == 7)){
            cv::putText(img, std::to_string(result), 
            Point(rec.y + 2*rec.width, rec.x + 2*rec.width), FONT_HERSHEY_SCRIPT_SIMPLEX, 1, Scalar(255,0,0), 1, 8);    
            cv::putText(img, "X:" + std::to_string(rec.x), Point(rec.y - 2*rec.width, rec.x), FONT_HERSHEY_SCRIPT_SIMPLEX, 0.2, Scalar(0,0,255), 1, 8);
            cv::putText(img, "y:" + std::to_string(rec.y), Point(rec.y - 2*rec.width, rec.x + 10), FONT_HERSHEY_SCRIPT_SIMPLEX, 0.2, Scalar(0,0,255), 1, 8);

            return result;
        }
        else{
            return -1;
        }
    }
};