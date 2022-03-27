#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//int ecart(fleur, neurone);
//FILE * fopen(const char* 'iris.csv', const char* 'r');// fonction de l'ouverture de fichier

typedef struct vec {
	double *v;
	double norme;
	char *etiquette;
}vec;
//struct neurone
typedef struct node{
	double *weight;
	double act;
	char id;
}node;
//struct bmu
typedef struct bmu{
	int c;
	int l;
	struct bmu *next;
}bmu;

/* exemple utilisation "." et "->" 
struct vec vecteur;

vecteur.norme = ... ;

struct vec *vecteur;

vecteur->norme ; 
/*
FILE init_des_fleurs(const char* , const char* ){
    FILE* fichier = NULL;
    char chaine[TAILLE_MAX] = ""; // Chaîne vide de taille TAILLE_MAX
 
    fichier = fopen("iris.csv", "r");
 
 
    if (fichier != NULL)
    {
        fgets(chaine, TAILLE_MAX, fichier); // On lit maximum TAILLE_MAX caractères du fichier, on stocke le tout dans "chaine"
        printf("%s", chaine); // On affiche la chaîne
 
        fclose(fichier);
    }
 	return chaine;
}
*/
//struct reseaux som
typedef struct net{
	int nb_colonne;
	int nb_ligne;
	int nb_node;
	struct bmu *best_unit; // ne reconnait pas bmu
	struct node** map;
	int nb_iteration;
	double alpha;
	int taille_voisinnage;
}net;
/*
int ecart(fleur , neurone)
{
	s = 0
	for(i=0;i < 4 ; i++){
		s = s +(fleur[i] - neurone[i])**2
		return sqrt(np.sqrt(s))
						}                    	
}
*/
/*
int ppv(T, fleur){
	// n,m,k = T.shape
	ppneurone = T[0][0];
	ppecart = ecart(fleur, T[0][0])
	//pos = (0,0)
	for(i = 0; i< n; i++){
		for(j = 0; i< m; i ++){
			if(ecart(fleur, T[i][j]) < ppecart){
				ppneurone = T[i][j]
				ppecart = ecart(fleur, T[i][j])
				pos = (i, j)
			}
		}
	}
	return (ppneurone, pos)
}
*/
double ecart(struct vec *fleur, struct node *neurone)
{	int s=0,k = 0 ;
	for (k = 0 ; k < 4 ; k++)
	{
		s = s+ pow(*(fleur->v + k*8) - *(neurone->weight + k*8) , 2);
	}
	
	
}


double NHD(int ligne,int colonne,struct bmu noeud, double sigma)
{
	return(exp(-(pow((noeud.c - colonne),2  ) + pow(noeud.l - ligne,2))  / (2*sigma*sigma) )  );
	
}



struct bmu ppv(struct net *reseau, struct vec *fleur)
{	struct bmu resultat;
	int i = 0, j = 0,  ecartmin = 0;
	ecartmin = ecart(fleur, *reseau->map );
	for (i=0 ; i <6 ; i++) {
		for (j=0 ; j < 10 ; j++) {
				if (ecart(fleur, (*(reseau->map + i*8) + j)) < ecartmin )
				{
					ecartmin = ecart(fleur, (*(reseau->map + i*8) + j));
					resultat.c = i;
					resultat.l = j;
				}
		}
}
	return resultat ;
}


double calculenorme(double *tab) 
{
	int i;
	double s;
    s = 0;
	for(i=0;i < 4 ; i++){
		//s = s +pow(*(tab + i), 2  );
		s = s + pow(*(tab + i*8 ),2);
		//	s = s + tab[i] * tab[i]; 
		//s += tab[i] * tab[i]; // comprend pas pk on passe un int alors qu'il faut un double
    }
    return sqrt(s);
}

int main(int argc, char *argv[]){
	
	struct net *reseau;
	int nb_colonne = 10;
	int nb_ligne= 6;
	int noeuds = 60;
	
	
	
	
	struct vec tableau[150]; // tableau de structure 150 structure
	double norme;
	char *lignei;
	int i,j, k ;
	FILE *fichier;
	fichier = fopen("iris.csv", "r");
	for(i=0 ; i < 150; i++){
		fgets(lignei, 150, fichier);
		
		struct vec newelement;
		char *temp;
		strncpy(temp,lignei , 3) ;
		*(newelement.v +0 ) = atof(temp); 
		strncpy(temp,lignei + 4, 3) ;
		*(newelement.v+8) = atof(temp);
		strncpy(temp,lignei + 8, 3) ;
		*(newelement.v+16) = atof(temp);
		strncpy(temp,lignei + 12, 3) ;
		*(newelement.v+24) = atof(temp);
		
		strncpy(temp,lignei + 16, 15) ;
		*(newelement.etiquette) = *temp ;
		norme = calculenorme(newelement.v);
	}
	for(i = 0 ; i< 150; i++){
		printf("ligne numéro %d : " , i ); 
		for(j=0; j<4 ; j++){
			printf(" %s ", *(tableau[i].v + 8*j ));
			}
	}
	
	srand((unsigned int)time(NULL));
	reseau->nb_colonne = 10;
	reseau-> nb_ligne = 6 ;
	reseau-> nb_node = 60;
	for (i=0 ; i <6 ; i++) {
		for (j=0 ; j < 10 ; j++) {
			struct node neurone; // neurone
			for (k = 0 ; k < 4 ; k++) {
				*(neurone.weight + k)= (float)rand()/(float)(RAND_MAX/6) ;

			}
		neurone.id = rand()%3	;	
		*(*(reseau->map + i*8) + j)= neurone ;       // Attention ! *8 si on est en 64 bits : les adresses sont stockées sur 8 octets //
		}
	}
	reseau->nb_iteration = 0; 
	reseau->alpha = 0.8;
	reseau->taille_voisinnage = 30;
	
	int epochs = 2000, lr = 0.9, ligne = 0, colonne = 0 , t= 0; 
	double sigma = 1 ;
	for (t=0 ; i <epochs ; i++) {
		j = rand() % 150 ;
		struct bmu vainqueur;
		vainqueur = ppv(reseau, &tableau[j]);
		lr = 0.9 * (t/ 2000) ;
		sigma = t / 2000 ;
		for (i=0 ; i < 6 ; i++) {
			for (j=0 ; j < 10 ; i++) {	
				for (k = 0 ; k < 4 ; k++){
				
				*((*(reseau->map + i*8) + j)->weight + k*8) = *((*(reseau->map + i*8) + j)->weight + k*8) + lr * NHD(i,j, vainqueur, sigma)* ( *(tableau[j].v + k*8)  - *((*(reseau->map + i*8) + j)->weight + k*8) );
		
		
				}
			}
		}
	}
	    return 0;
} 
