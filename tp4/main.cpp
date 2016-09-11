/* --------------------------------------------------------------------------
Introduction à OpenCV, projet encadre IVI
-------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------
Inclure les fichiers d'entete
-------------------------------------------------------------------------- */
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

#include <cv.h>
#include <highgui.h>


using namespace std;
using namespace cv;

/* --------------------------------------------------------------------------
Fonction d'affichage d'un histogramme monodimensionnel
-------------------------------------------------------------------------- */
void displayHist1D( CvHistogram* hist )
{
    // Taille (en pixels) réservée à la graduation et aux bords
    const uchar GRAD_SIZE = 20;
    // Définition de la couleur des barres (blanc) et du texte (noir)
    const CvScalar BLACK = CV_RGB( 0, 0, 0 );
    const CvScalar WHITE = CV_RGB( 255, 255, 255 );
    // Étiquette de graduation
    char graduation[4];
    // Création d'une fonte pour afficher du texte sur l'image
    CvFont font;
    cvInitFont( &font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5 );

    // Nombre de cellules de l'histogramme
    int h_bins = cvGetDimSize( hist->bins, 0 );
    // Échelle de représentation (largeur d'une cellule, en pixels)
    int scale = 2 * 255 / h_bins;

    // Créer une image (couleur) de visualisation de l'histogramme
    CvSize h_imSize = cvSize( (h_bins+10)*scale + 2*GRAD_SIZE, 255 + GRAD_SIZE );
    IplImage* hist_img = cvCreateImage( h_imSize, IPL_DEPTH_8U, 3 );
    // Initialiser à WHITE l'image d'histogramme
    cvZero( hist_img );
    cvAddS( hist_img, WHITE, hist_img );

    // Déterminer le maximum pour pouvoir normaliser l'histogramme
    float max_value = 0;
    cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );

    // Représentation de l'histogramme par des barres rectangulaires (orientées vers le bas)
    for( int h = 0; h < h_bins; h++ )
    {
        float bin_val = cvQueryHistValue_1D( hist, h );
        int intensity = cvRound( bin_val * 255 / max_value );
        CvPoint ptHG = cvPoint( h*scale+GRAD_SIZE, GRAD_SIZE );
        CvPoint ptBD = cvPoint( (h+1)*scale-1+GRAD_SIZE, intensity+GRAD_SIZE );
        cvRectangle( hist_img, ptHG, ptBD, BLACK, CV_FILLED );
    }
    // Retourner l'image autour de l'axe x
    cvFlip( hist_img, NULL );
    // Créer la graduation
    for( int h = 0; h <= h_bins; h++ )
    {
        if ( h % 50 == 0 )
        {
            int x_tick = h*scale+GRAD_SIZE;
            sprintf( graduation, "%d", h*256/h_bins );
            cvLine ( hist_img, cvPoint( x_tick, 258 ), cvPoint ( x_tick, 255 ), BLACK );
            // Écrire le texte centré en déterminant sa taille
            CvSize gradSize;
            int baseline;
            cvGetTextSize( graduation, &font, &gradSize, &baseline );
            CvPoint ptGrad = cvPoint( h*scale-gradSize.width/2+GRAD_SIZE, 270 );
            cvPutText ( hist_img, graduation, ptGrad , &font, BLACK );
        }
    }
    // Afficher la fenêtre d'histogramme
    cvNamedWindow( "Histogram", 1 );
    cvShowImage( "Histogram", hist_img );
    cvSaveImage("hist.png", hist_img);

}

void  afficheHisto()
{

    int sizes[] = { 256 };
    float range0[] = { 0, 255 };
    float* ranges[] = { range0 };

    CvHistogram* hist = cvCreateHist(1, sizes, CV_HIST_ARRAY, ranges, 1);
    IplImage* images[] = { cvLoadImage( "phare.png", CV_LOAD_IMAGE_UNCHANGED ) };

    //cvCalcHist(images, 1, 0, t, hist, 1, sizes, ranges );
    cvCalcHist(images, hist, 0, NULL);
    displayHist1D( hist );
}

/* --------------------------------------------------------------------------
Programme principal
-------------------------------------------------------------------------- */
int main ()
{

    afficheHisto();
    /*const std::string fenetreSrc = "source";
    const std::string fenetreDest = "destination";
    const std::string nomsImage = "phare_contraste.png";
    const std::string nomsImageDest = "phare_mat_huit.png";
    IplImage* image = cvLoadImage( nomsImage.data(), CV_LOAD_IMAGE_GRAYSCALE);
    IplImage* dest = cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 1);

    if ( !image ) {
        cout << "Impossible de charger l'image " + nomsImage << endl;
        exit( EXIT_FAILURE );
    }

    //crée une matrice 3x3 non initialisé
    float kernel[] = {0,    1,  0,
                      1,   -4,  1,
                      0,    1,  0};
    CvMat masque = cvMat( 3, 3, CV_32FC1, kernel );

    //initialisation à 0
    //cvSetZero ( masque );
    //initialise le masque avec la valeur 1
    //cvAddS( masque, cvScalar(1.0), masque );

    //augmentation du contraste
    //cvEqualizeHist(image, dest);

    //normalisation de l'image de destination
    //cvNormalize( masque, masque, 1.0, 0.0, CV_L1 );
    cvNormalize( &masque, &masque, 1.0, 0.0, CV_L1 );

    //applique le filtre sur l'image
    cvFilter2D(image, dest, &masque);

    // applique un filtre median
    //cvSmooth(image, dest, CV_MEDIAN, 3, 3);

    //sauvegarde de l'image
    cvSaveImage(nomsImageDest.data(), dest);

    //affichage des images
    cvNamedWindow( fenetreSrc.data() );
    cvNamedWindow( fenetreDest.data() );
    cvShowImage(fenetreSrc.data(), image);
    cvShowImage(fenetreDest.data(), dest);

    // Sur appui d'une touche, fermer la fenêtre et libérer la mémoire image
    cvWaitKey( 0 );
    //cvDestroyWindow( nomFenetre );

    //destruction de la fenetre
    cvDestroyAllWindows();
    cvReleaseImage( &image );*/
    return (0);
}

/* Q1 -> l'image de destination devient presque totalement blanche
    l'addition place tous les pixel à une valeur supérieur ou egale a 255 */

/* Q2 -> flou moyenneur => réduit le bruit et atténue les details */

/* Q3 -> accentuation du flou car la moyenne est faite sur un plus grand nombre
    de pixel */

/* Q4 -> grosse reduction du bruit */

/* Q5 -> apparition d'un flou */

/* Q6 -> le filtre met en avant les grosse variation de couleur dans l'image
    et donc des contours sur les objets.*/

/*  Q7 -> le filtre ne garde que les contours contrairement au filtre précédent
    qui gardait quelque details */

