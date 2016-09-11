/* --------------------------------------------------------------------------
   Introduction à OpenCV, projet encadre IVI
   -------------------------------------------------------------------------- */



/* --------------------------------------------------------------------------
   Inclure les fichiers d'entete
   -------------------------------------------------------------------------- */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>

#include <cv.h>
#include <highgui.h>

#define CANAL_ROUGE 2
#define CANAL_VERT 1
#define CANAL_BLEU 0


using namespace std;

uchar getMax(IplImage* image)
{
    int row, col;
    uchar max_val = 0;
    uchar niveau;

    for(col=0; col<image->height; col++)
    {
        for(row=0; row<image->width; row++)
        {
            niveau = (uchar)(image->imageData[ col*image->widthStep + row ]);
            max_val = max(max_val, niveau);
        }
    }

    return max_val;
}

void seuiller(IplImage* imageSrc, IplImage* imageDst, uchar seuil)
{
    int row, col;
    uchar niveau;

    if(imageSrc->imageSize!=imageDst->imageSize ||
            imageSrc->depth!=8 || imageDst->depth!=8 ||
            imageSrc->nChannels!=1 || imageDst->nChannels!=1)
    {
        imageDst = NULL;
        return;
    }

    for(col=0; col<imageSrc->height; col++)
    {
        for(row=0; row<imageSrc->width; row++)
        {
            niveau = (uchar)(imageSrc->imageData[ col*imageSrc->widthStep + row ]);

            if(niveau>seuil)
            {
                imageDst->imageData[ col*imageDst->widthStep + row ] = 255;
            }
            else
            {
                imageDst->imageData[ col*imageDst->widthStep + row ] = 0;
            }

        }
    }
}

void getCanal(IplImage* imageSrc, IplImage* imageDst, uchar canal){
    int row, col;

    // la position dans le tableau imageData
    // la position se caracterise par une ligne, une colonne et un canal
    int position;

    if(imageSrc->imageSize!=imageDst->imageSize ||
       imageSrc->depth!=8 || imageDst->depth!=8 ||
       imageSrc->nChannels!=3 || imageDst->nChannels!=3)
    {
        imageDst = NULL;
        return;
    }

    cvSet(imageDst, CV_RGB(0,0,0));

    for(col=0; col<imageSrc->height; col++)
    {
        for(row=0; row<imageSrc->width*3; row=row+3)
        {
            position = col*imageSrc->widthStep + row + canal;
            imageDst->imageData[position] = imageSrc->imageData[position];
        }
    }
}

/* --------------------------------------------------------------------------
   Programme principal
   -------------------------------------------------------------------------- */
int main (int argc, char* argv[])
{

    // Noms du fichier image et de la fenêtre d'affichage
    IplImage* image = NULL;
    IplImage* bin_image = NULL;
    IplImage* image_cmp = NULL;
    IplImage* bin_gimp1 = NULL;
    IplImage* bin_gimp2 = NULL;
    string nomImage = "gateaux1.png";
    string nomFenetre = "pjE3";
    vector<string> *noms;
    vector<string> *nomsfen;


    // le meilleur threshold est de 30 pour gateaux1
    double t = 30;
    int t_max = 255;

    /*
     * Parties:
     *
     * 1. Lecture et affichage d'une image
     * 2. Transformations ponctuelles d'une image
     * 3. Parcours d'une image
     * 4. Dématriçage par interpolation
     */
    int partie = 3;
    int question = 4;

    switch(partie)
    {

    case 1: // PARTIE 1

        switch(question)
        {

        case 1: // QUESTION 1.1

            // Lire le fichier image
            image = cvLoadImage( nomImage.data(), CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image %s\n", nomImage.data() );
                exit( EXIT_FAILURE );
            }

            // Créer une fenêtre et y afficher l'image
            cvNamedWindow( nomFenetre.data() );
            cvShowImage( nomFenetre.data(), image );

            // Afficher des proprietes de l'image sur la console
            printf( "Infos sur l'image %s\n", nomImage.data() );
            // Definition (largeur x hauteur)
            printf( "Definition \t: %dx%d pixels\n", image->width, image->height );
            printf( "Taille \t\t: %d octets\n", image->imageSize );
            printf( "Profondeur \t: %d bits\n", image->depth );
            printf( "Nb de canaux \t: %d\n", image->nChannels );
            printf( "Appuyer sur une touche pour terminer.\n" );

            // Sur appui d'une touche, fermer la fenêtre et libérer la mémoire image
            cvWaitKey( 0 );
            cvDestroyWindow( nomFenetre.data() );
            break;

        case 2: // QUESTION 1.2

            noms = new vector<string>();
            noms->push_back("gateaux1.png");
            noms->push_back("gateaux2.png");
            noms->push_back("gateaux3.png");
            noms->push_back("lena.png");

            nomsfen = new vector<string>();
            nomsfen->push_back("Fenetre gateaux1");
            nomsfen->push_back("Fenetre gateaux2");
            nomsfen->push_back("Fenetre gateaux3");
            nomsfen->push_back("Fenetre lena");

            for(int i=0; i<noms->size(); i++)
            {

                // Lire le fichier image
                image = cvLoadImage( (*noms)[i].data(), CV_LOAD_IMAGE_UNCHANGED );
                if ( !image )
                {
                    printf( "Impossible de charger l'image %s\n", (*noms)[i].data() );
                    exit( EXIT_FAILURE );
                }

                // Créer une fenêtre et y afficher l'image
                cvNamedWindow( (*nomsfen)[i].data() );
                cvShowImage( (*nomsfen)[i].data(), image );

                // Afficher des propriétés de l'image sur la console
                printf( "Infos sur l'image %s\n", (*noms)[i].data() );
                printf( "Definition \t: %dx%d pixels\n", image->width, image->height );
                printf( "Taille \t\t: %d octets\n", image->imageSize );
                printf( "Profondeur \t: %d\n", image->depth );
                printf( "Nb de canaux \t: %d\n", image->nChannels );
                printf( "Appuyer sur une touche pour terminer.\n" );

                // Sur appui d'une touche, fermer la fenêtre et libérer la mémoire image
                cvWaitKey( 0 );
                cvDestroyWindow( (*nomsfen)[i].data() );

                printf("\n");

            }

            break;

        case 3: // QUESTION 1.3

            noms = new vector<string>();
            noms->push_back("gateaux1.png");
            noms->push_back("gateaux2.png");
            noms->push_back("gateaux3.png");
            noms->push_back("lena.png");

            nomsfen = new vector<string>();
            nomsfen->push_back("Fenetre gateaux1");
            nomsfen->push_back("Fenetre gateaux2");
            nomsfen->push_back("Fenetre gateaux3");
            nomsfen->push_back("Fenetre lena");

            for(int i=0; i<noms->size(); i++)
            {

                // Lire le fichier image
                image = cvLoadImage( (*noms)[i].data(), CV_LOAD_IMAGE_GRAYSCALE );
                if ( !image )
                {
                    printf( "Impossible de charger l'image %s\n", (*noms)[i].data() );
                    exit( EXIT_FAILURE );
                }

                // Créer une fenêtre et y afficher l'image
                cvNamedWindow( (*nomsfen)[i].data() );
                cvShowImage( (*nomsfen)[i].data(), image );

                // Afficher des propriétés de l'image sur la console
                printf( "Infos sur l'image %s\n", (*noms)[i].data() );
                printf( "Definition \t: %dx%d pixels\n", image->width, image->height );
                printf( "Taille \t\t: %d octets\n", image->imageSize );
                printf( "Profondeur \t: %d\n", image->depth );
                printf( "Nb de canaux \t: %d\n", image->nChannels );
                printf( "Appuyer sur une touche pour terminer.\n" );

                // Sur appui d'une touche, fermer la fenêtre et libérer la mémoire image
                cvWaitKey( 0 );
                cvDestroyWindow( (*nomsfen)[i].data() );

                printf("\n");

            }

            break;

        }

        break;

    case 2: //PARTIE 2

        switch(question)
        {

        case 2: // QUESTION 2.2

            double minVal, maxVal;
            CvPoint minLoc, maxLoc;

            // Lire le fichier image
            image = cvLoadImage( nomImage.data(), CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image %s\n", nomImage.data() );
                exit( EXIT_FAILURE );
            }

            cvMinMaxLoc(image, &minVal, &maxVal, &minLoc, &maxLoc);

            printf("Info sur l'histogramme de %s:\n", nomImage.data() );
            printf("\t- valeur la plus somble: %lf\n", minVal);
            printf("\t- valeur la plus clair : %lf\n", maxVal);
            printf("\t- position du pixel le plus somble:\n");
            printf("\t\t- x: %d\n", minLoc.x);
            printf("\t\t- y: %d\n", minLoc.y);
            printf("\t- position du pixel le plus clair:\n");
            printf("\t\t- x: %d\n", maxLoc.x);
            printf("\t\t- y: %d\n", maxLoc.y);


            break;

        case 3: // QUESTION 2.3

            image = cvLoadImage( nomImage.data(), CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image %s\n", nomImage.data() );
                exit( EXIT_FAILURE );
            }

            bin_image = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);


            // Seuil binaire
            // les pixels > t seront initialisé a max
            // sinon zero
            // voir doc
            cvThreshold(image, bin_image, t, t_max, cv::THRESH_BINARY);

            cvNamedWindow(nomFenetre.data());
            cvShowImage(nomFenetre.data(), bin_image);
            cvWaitKey(0);
            cvDestroyWindow(nomFenetre.data());

            break;

        case 4: // QUESTION 2.4

            image = cvLoadImage( nomImage.data(), CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image %s\n", nomImage.data() );
                exit( EXIT_FAILURE );
            }

            bin_image = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
            image_cmp = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);

            // Seuil binaire
            // les pixels > t seront initialisé a max
            // sinon zero
            // voir doc
            cvThreshold(image, bin_image, t, t_max, cv::THRESH_BINARY);

            image = cvLoadImage( "gateaux1_bin_gimp_T.png", CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image gateaux1_bin_gimp_T.png\n");
                exit( EXIT_FAILURE );
            }

            cvCmp(bin_image, image, image_cmp, cv::CMP_NE);

            cvNamedWindow("T'=T");
            cvShowImage("T'=T", image_cmp);
            cvWaitKey(0);
            cvDestroyWindow("T'=T");

            printf("La distance des image avec les seuils T'=T: %d\n", cvCountNonZero(image_cmp));

            // Avec le meme seuil on remarque un legere different (quelques pixels près)
            // entre le seuil d'OpenCV et se lui de Gimp



            image = cvLoadImage( "gateaux1_bin_gimp_T2.png", CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image gateaux1_bin_gimp_T2.png\n" );
                exit( EXIT_FAILURE );
            }

            cvCmp(bin_image, image, image_cmp, cv::CMP_NE);

            cvNamedWindow("T'=2T");
            cvShowImage("T'=2T", image_cmp);
            cvWaitKey(0);
            cvDestroyWindow("T'=2T");

            printf("La distance des image avec les seuils T'=2T: %d\n", cvCountNonZero(image_cmp));

            // Avec un seuil à 60 sur gimp et à 30 sur OpenCV
            // la difference est visible sur les bords des gateaux

            break;

        case 5: // QUESTION 2.5

            string fen1 = "Thresh trunc";
            string fen2 = "Thresh binary avec otsu";

            image = cvLoadImage( nomImage.data(), CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image %s\n", nomImage.data() );
                exit( EXIT_FAILURE );
            }

            bin_image = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);

            printf("%s:\n", fen1.data());
            printf("T fixe a %f\n", t);
            cvThreshold(image, bin_image, t, t_max, cv::THRESH_TRUNC);

            cvNamedWindow(fen1.data());
            cvShowImage(fen1.data(), bin_image);
            cvWaitKey(0);
            cvDestroyWindow(fen1.data());

            printf("\n");


            printf("%s:\n", fen2.data());
            t = cvThreshold(image, bin_image, 0, t_max, cv::THRESH_BINARY | cv::THRESH_OTSU);
            printf("T calcule a %f\n", t);
            cvNamedWindow(fen2.data());
            cvShowImage(fen2.data(), bin_image);
            cvWaitKey(0);
            cvDestroyWindow(fen2.data());

            printf("\n");


            break;
        }

        break;

    case 3:

        switch(question)
        {

        case 1:
            int row, col;
            int max_row, max_col;
            uchar niveau;

            // Lire le fichier image
            image = cvLoadImage( nomImage.data(), CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image %s\n", nomImage.data() );
                exit( EXIT_FAILURE );
            }

            max_row = (image->width<4)?image->width:4;
            max_col = (image->height<4)?image->height:4;


            for(col=0; col<max_col; col++)
            {
                for(row=0; row<max_row; row++)
                {
                    niveau = (uchar)( image->imageData[ col*image->widthStep + row ] );
                    printf("%d\t", (int)niveau);
                }
                printf("\n");
            }

            break;

        case 2:
            uchar niveau_max;

            // Lire le fichier image
            image = cvLoadImage( nomImage.data(), CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image %s\n", nomImage.data() );
                exit( EXIT_FAILURE );
            }

            niveau_max = getMax(image);

            printf("Niveau max: %d\n", (int)niveau_max);

            break;

        case 3:
            IplImage *dst;

            // Lire le fichier image
            image = cvLoadImage( nomImage.data(), CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image %s\n", nomImage.data() );
                exit( EXIT_FAILURE );
            }

            dst = cvCreateImage( cvGetSize(image), 8, 1 );
            if ( !dst )
            {
                printf( "Impossible de charger l'image %s\n", nomImage.data() );
                exit( EXIT_FAILURE );
            }

            seuiller(image, dst, 30);

            if ( !dst )
            {
                printf( "erreur lors du calcul du seuil\n" );
                exit( EXIT_FAILURE );
            }

            cvNamedWindow("Seuil");
            cvShowImage("Seuil", dst);
            cvWaitKey(0);
            cvDestroyWindow("Seuil");

            break;

        case 4:

            IplImage *dsti;
            string fen = "Canal";

            image = cvLoadImage( "lena.png", CV_LOAD_IMAGE_UNCHANGED );
            if ( !image )
            {
                printf( "Impossible de charger l'image lena\n" );
                exit( EXIT_FAILURE );
            }

            dsti = cvCreateImage( cvGetSize(image), 8, 3 );
            if ( !dsti )
            {
                printf( "Impossible de creer l'image dsti\n");
                exit( EXIT_FAILURE );
            }

            getCanal(image, dsti, CANAL_VERT);

            cvNamedWindow(fen.data());
            cvShowImage(fen.data(), dsti);
            cvWaitKey(0);
            cvDestroyWindow(fen.data());

            break;
        }
        break;
    }


    // Terminer
    cvDestroyAllWindows();
    cvReleaseImage( &image );
    exit(EXIT_SUCCESS);
}
