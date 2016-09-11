/* --------------------------------------------------------------------------
Acquisition sur camera IDS uEye, projet encadre IVI
Copyright (C) 2009  Universite Lille 1

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------
Inclure les fichiers d'entete
-------------------------------------------------------------------------- */
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "uEye.h"

/* --------------------------------------------------------------------------
Macros
-------------------------------------------------------------------------- */
// Message d'erreur si necessaire apres une fonction IDS
#define IFIDSERROR(x) if (iRet != IS_SUCCESS) \
{ \
    fprintf (stderr, x " (Code %d)\n", iRet); \
    is_ExitCamera (hCam); \
    return (-1); \
}

/* --------------------------------------------------------------------------
Programme principal
-------------------------------------------------------------------------- */
int main ()
{

    // Definir les variables locales
    HIDS hCam = (HIDS) 0; // handle de la camera
    int iRet; // code de retour des fonctions uEye

    // Initialiser la camera
    iRet = is_InitCamera (&hCam, NULL);
    // Succes ?
    if (iRet == IS_SUCCESS && hCam != (HIDS) 0)
    {

        // Definir les variables locales
        int iBitsPerPixel; // Nombre de bits par pixel
        int iChannels; // Nombre de canaux couleur
        int iMaxSizeX, iSizeX; // Largeur d'image
        int iMaxSizeY, iSizeY; // Hauteur d'image
        int iMemorySizeX; // Longueur d'une ligne en memoire
        int iMemorySizeY; // Nombre de lignes en memoire
        int iPosX, iPosY; // Position du debut d'image
        int iPixelClock; // Frequence de l'horloge pixel (MHz)
        int iBinningX, iBinningY; // Position du debut d'image
        SENSORINFO sInfo; // Structure pour le format de capteur
        char* pcMemoryImage; // Zone memoire (buffer) d'acquisition
        int iIdImage; // ID du buffer d'acquisition
        int iKey = -1; // Touche
        IplImage* cvImage; // Structure image pour OpenCV
        double dFrameRate; // Cadence d'acquisition (im/s)
        double dExposureTime; // Temps d'exposition (ms)

        // Recuperer les informations du capteur de la camera
        is_GetSensorInfo (hCam, &sInfo);
        iMaxSizeX = sInfo.nMaxWidth;
        iMaxSizeY = sInfo.nMaxHeight;

        /* ------------------------------------------------------------------
        Definir les parametres d'acquisition
        ------------------------------------------------------------------ */

        // Definir le mode d'acquisition (n&b ou couleur)
        iRet = is_SetColorMode (hCam, IS_CM_BGR8_PACKED);
        IFIDSERROR ("Erreur lors du reglage du type d'image")
        iBitsPerPixel = 24;
        iChannels = 3;

        // Definir la taille de la region d'interet (AOI)
        iSizeX = iMaxSizeX;
        iSizeY = iMaxSizeY;
        IS_SIZE_2D sSizeAOI = { iSizeX, iSizeY };
        iRet = is_AOI(hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&sSizeAOI, sizeof(sSizeAOI));
        IFIDSERROR ("Impossible de fixer la taille de la zone d'interet")

        // Definir la position de la region d'interet (AOI)
        iPosX = 0;
        iPosY = 0;
        IS_POINT_2D sPointAOI = { iPosX, iPosY };
        iRet = is_AOI(hCam, IS_AOI_IMAGE_SET_POS, (void*)&sPointAOI, sizeof(sPointAOI));
        IFIDSERROR ("Impossible de fixer la position de la zone d'interet")

        // Mode de binning vertical et/ou horizontal
        iRet = is_SetBinning (hCam, IS_BINNING_2X_HORIZONTAL);
        IFIDSERROR ("Impossible de fixer les facteurs de binning")
        iBinningX = 2;
        iBinningY = 1;

        // Message d'information
        printf ("Taille d'image %dx%d, position (%d,%d), %d bits par pixel\n",
                iSizeX / iBinningX, iSizeY / iBinningY, iPosX, iPosY,
                iBitsPerPixel);

        // Definir l'horloge pixel (MHz)
        iPixelClock = 40;
        iRet = is_PixelClock(hCam, IS_PIXELCLOCK_CMD_SET, (void*)&iPixelClock, sizeof(iPixelClock));
        IFIDSERROR ("Erreur lors du reglage de l'horloge pixel")

        // Definir la cadence d'acquisition (im/s)
        iRet = is_SetFrameRate (hCam, 25.0, &dFrameRate);
        IFIDSERROR ("Impossible de fixer la cadence d'acquisition")
        printf ("Cadence d'acquisition fixee a %lf images/seconde\n",
                dFrameRate);

        // Definir le temps d'integration (ms)
        dExposureTime = 20.0;
        iRet = is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, &dExposureTime, 8);
        IFIDSERROR ("Erreur lors du reglage du temps d'integration")
        printf ("Temps d'integration fixe a %lf ms\n", dExposureTime);

        // Definir les gains, principal et des composantes couleur
        iRet = is_SetHardwareGain (hCam, 100, 20, 10, 20);
        IFIDSERROR ("Erreur lors du reglage des gains")

        // Creer une image uEye et allocation memoire
        iMemorySizeX = (iSizeX / iBinningX + 7) & 0xFFFFFFF8;
        iMemorySizeY = iSizeY / iBinningY;
        iRet = is_AllocImageMem (hCam, iMemorySizeX, iMemorySizeY,
                                 iBitsPerPixel, &pcMemoryImage, &iIdImage);
        IFIDSERROR ("Erreur d'allocation memoire IDS uEye")
        printf ("Memoire allouee pour le buffer %d, adresse 0x%08X\n",
                iIdImage, (int) pcMemoryImage);

        // Activer cette memoire (l'image acquise va y etre stockee)
        iRet = is_SetImageMem (hCam, pcMemoryImage, iIdImage);
        IFIDSERROR ("Erreur d'association de la memoire")

        // Creer l'image openCV
        cvImage = cvCreateImageHeader (cvSize (iMemorySizeX, iMemorySizeY),
                                       IPL_DEPTH_8U, iChannels);
        // Les valeurs des pixels sont dans le buffer de la camera IDS
        cvImage->imageData = pcMemoryImage;

        // Creer la fenêtre d'affichage de l'image
        cvNamedWindow ("Acquisition", CV_WINDOW_AUTOSIZE);

        // Boucler pour afficher les images
        while (iKey != 27)
        {
            // Acquérir l'image
            iRet = is_FreezeVideo (hCam, IS_WAIT);
            //IFIDSERROR ("Erreur d'acquisition de l'image")
            // Afficher l'image
            cvShowImage ("Acquisition", cvImage);
            // Attendre 20 ms que l'utilisateur appuie sur une touche
            iKey = cvWaitKey (20);
        }

        // Liberer la memoire et finir sans erreur
        cvDestroyWindow ("Acquisition");
        cvImage->imageData = NULL;
        cvReleaseImageHeader (&cvImage);
        is_FreeImageMem (hCam, pcMemoryImage, iIdImage);
        is_ExitCamera (hCam);
        return (0);
    }

    // Erreur lors de l'ouverture de la camera IDS
    else
    {
        // Afficher un message d'erreur
        printf ("Erreur lors de l'ouverture de la camera IDS uEye" \
                " (verifier la connexion sur l'USB)");
        return (-1);
    }
}
