#include "fem.h"




double geoSize(double x, double y){

    femGeo* theGeometry = geoGetGeometry();
    
    double h = theGeometry->h;
    double x0 = theGeometry->xNotch;
    double y0 = theGeometry->yNotch;
    double r0 = theGeometry->rNotch;
    double h0 = theGeometry->hNotch;
    double d0 = theGeometry->dNotch;
  
    
    double x1 = theGeometry->xHole;
    double y1 = theGeometry->yHole;
    double r1 = theGeometry->rHole;
    double h1 = theGeometry->hHole;
    double d1 = theGeometry->dHole;

    h=5;
    return h;
    double hfinal = h;
    double d = sqrt((x-x0)*(x-x0) + (y-y0)*(y-y0)) - r0;
    if (d < d0) {
        double a = (-2*h + 2*h0)/(d0*d0*d0);
        double b = (3*h  - 3*h0)/(d0*d0);
        double c = 0;
        hfinal = a*d*d*d + b*d*d + c*d + h0; }
        
    d = sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1)) - r1;
    if (d < d1) {
        double a = (-2*h + 2*h1)/(d1*d1*d1);
        double b = (3*h  - 3*h1)/(d1*d1);
        double c = 0;
        hfinal = fmin(hfinal,a*d*d*d + b*d*d + c*d + h1); }
        
    return hfinal;
    

}


void geoMeshGenerate() {

    femGeo* theGeometry = geoGetGeometry();

    double w = theGeometry->LxPlate;
    double h = theGeometry->LyPlate;
     
    double x0 = theGeometry->xNotch;
    double y0 = theGeometry->yNotch;
    double r0 = theGeometry->rNotch;
    
    
    double x1 = theGeometry->xHole;
    double y1 = theGeometry->yHole;
    double r1 = theGeometry->rHole;
 
//
//  -1- Construction de la géométrie avec OpenCascade
//      On crée le rectangle
//      On crée les deux cercles
//      On soustrait les cercles du rectangle :-)
//

    int ierr;
    int idbottomcircle = gmshModelOccAddDisk(0,200,0,15.75,15.75,-1,NULL,0,NULL,0,&ierr);
    int idholebottomcircle = gmshModelOccAddDisk(0,200,0,10,10,-1,NULL,0,NULL,0,&ierr);
    int idheadcircle = gmshModelOccAddDisk(0,0,0,22.15,22.15,-1,NULL,0,NULL,0,&ierr);
    int idhandle = gmshModelOccAddRectangle(-7.5,20,0, 15,170 ,-1, 0.0,&ierr);
    int idbolt = gmshModelOccAddRectangle(-8.5,0,0.0,17,-50,-1,1,&ierr); // on veut les bords arrondis 
    
    
    int bottomcircle[] = {2,idbottomcircle};
    int holebottomcircle[] = {2,idholebottomcircle};
    int headcircle[]  = {2,idheadcircle};
    int handle[] = {2,idhandle}; // manche de la clé à molette
    int bolt[] = {2,idbolt}; // trou du boulon
    gmshModelOccRotate(bolt,2,0,5,0,0,0,1,0.2,&ierr);
    gmshModelOccCut(headcircle,2,bolt,2,NULL,NULL,NULL,NULL,NULL,-1,1,1,&ierr); 
    gmshModelOccCut(bottomcircle,2,holebottomcircle ,2,NULL,NULL,NULL,NULL,NULL,-1,1,1,&ierr); 
    gmshModelOccFuse(handle,2,bottomcircle,2,NULL,NULL,NULL,NULL,NULL,-1,1,1,&ierr);
    gmshModelOccFuse(handle,2,headcircle,2,NULL,NULL,NULL,NULL,NULL,-1,1,1,&ierr);

//
//  -2- Définition de la fonction callback pour la taille de référence
//      Synchronisation de OpenCascade avec gmsh
//      Génération du maillage (avec l'option Mesh.SaveAll :-)
                  
    geoSetSizeCallback(geoSize);   
    gmshModelOccSynchronize(&ierr);  
    gmshOptionSetNumber("Mesh.SaveAll", 1, &ierr);
    gmshModelMeshGenerate(2, &ierr);  
    
//
//  Generation de quads (avec quelques triangles...) :-)
//
//    gmshOptionSetNumber("Mesh.SaveAll", 1, &ierr);
//    gmshOptionSetNumber("Mesh.RecombineAll", 1, &ierr);
//    gmshOptionSetNumber("Mesh.Algorithm", 8, &ierr);  chk(ierr);
//    gmshOptionSetNumber("Mesh.RecombinationAlgorithm", 1.0, &ierr);  chk(ierr);
//    gmshModelGeoMeshSetRecombine(2,1,45,&ierr);  chk(ierr);
//    gmshModelMeshGenerate(2, &ierr);  
   
 
//
//  Plot avec Fltk et vous avez accès au graphique de gmsh :-)
//
//   gmshFltkInitialize(&ierr);
//   gmshFltkRun(&ierr);  chk(ierr);
//
    
}