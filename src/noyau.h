/*******************************************************************************

          fichier include : noyau.h
          role            : declarations communes pour l'ensemble des fichiers C
					                  relevant du noyau.
														Ne doit pas �tre inclus pour les programmes utilisateurs

********************************************************************************/

#ifndef __noyau_h
#define __noyau_h

/*-----------------------------------------------------------------------------
			Include
------------------------------------------------------------------------------*/

#include <stdint.h>

#include "constC.h"

#include "LPC17xx.h"
#include "core_cm3.h"

/*-----------------------------------------------------------------------------
			D�finition de type
------------------------------------------------------------------------------*/

typedef uint32_t tRegistre;  // on d�finit un type registre (32bits non sign�)

typedef int (*tPP) ();			//  pointeur sur une fonction, servira pour la proc�dure utlisateur principale

typedef int tPid;						// pour voir que l'on parle d'un descripteur de processus

typedef int tSid;						// pour voir que l'on parle d'un descripteur de semaphore

/*-----------------------------------------------------------------------------
			D�finition des fonctions publiques
------------------------------------------------------------------------------*/

// � compl�ter au fur et � mesure de l'�criture des primitives

//====================== dans p_noyau.c ==================

IMPORT tPid p_creeProcessus(int priorite, tPP procedurePrincipale);

//====================== dans files.c ==================

IMPORT void insere_selonPrioriteDansFile(tPid,tPid*);
IMPORT void retire_deFile(tPid,tPid*);    

//====================== dans affichage.c ==================

IMPORT void initialiseAffichage(void);
IMPORT void affiche(const char *);
IMPORT void afficheNombre(int);
IMPORT void afficheChaine(const char*);
IMPORT void passeALaLigneSuivante(void); 
IMPORT void revientEnPremiereColonne(void); 

//====================== divers  ==================

IMPORT void detruitProcessusAppelant(void);	  // dans libGestionProcessUser.c

/*-----------------------------------------------------------------------------
			Dimensionnement du systeme
------------------------------------------------------------------------------*/

#define	NB_MAX_PROCESSUS 	10  
#define	NB_MAX_SEMAPHORES	10
#define	TAILLE_PILE			 256     // de la taille d'un registre soit 256 mots de 4 octets

/*-----------------------------------------------------------------------------
			Constantes du processeur
------------------------------------------------------------------------------*/

#define CONTROLE_THREAD_NP 0x1		// valeur pour controle avec mode thread non privil�gi�
																	// pour la pile c'est lors de retour d'exceprion qu'elle est positionn�e

#define XPSR_INITIAL 0x01000000		// xPSR avec juste Thumb � 1, le reste est � 0 (comme suite � un reset)

#define LR_THREAD_PSP 0xFFFFFFFD	// valeur de la pseudo @ de retour pour une exception en revenant en mode
																	// thread avec utilisation de la pile PSP

/*-----------------------------------------------------------------------------
			Marqueurs systeme
------------------------------------------------------------------------------*/

#define	AUCUN_SEMAPHORE_BLOCAGE (-1)


/*-----------------------------------------------------------------------------
			D�finition de la structure de donn�es
			correspondant � l'�tat m�moire propos�.
			
			Vous pouvez bien sur chang� si vous voulez, ou rajouter des �lements dans la
			structure (il faudra d'ailleurs le faire dans des �tapes ult�rieurs.
			Attention dans ce cas � l'impact possible sur les autres fonctions (gestion
			des listes par exemple).
------------------------------------------------------------------------------*/

// descripteur de etatCalcul

typedef struct {						
	tRegistre controle;			    //  controle
	tRegistre futur_R[8];				// R04..R11				
	tRegistre *PSP;							// pile PSP
	tRegistre *MSP;							// pile MSP
  tRegistre LR_Handler;	    	// LR en mode handler
	}
	tDescEtatCalcul;

// descripteur de processus

typedef struct {
	tPid		succ;													// code un pId=indice dans tableau
	int 		priorite;											// dans 0..255 (0=min)
	tDescEtatCalcul* 	etatCalcul;					// pointe le bloc contenant valeur registres pour future ex�cution
	tSid 		semBlocage;										// sid et AUCUN_SEMAPHORE_BLOCAGE si non bloqu�
	}
	tDescProc;

// descripteur de semaphore

typedef struct {
	enum {DESC_SEM_ALLOUE,DESC_SEM_LIBRE} etat;
	int compteur;													// si <0, |compteur|=taille(fileProcessusBloques)
	tPid	fileBloques;											// contient pId du 1er de la file */
	}
	tDescSem;

/*-----------------------------------------------------------------------------
			Importation des structures de donn�es globales
------------------------------------------------------------------------------*/

// tableau des processus

IMPORT tDescProc descProc[];

// tableau des s�maphores

IMPORT tDescSem	descSem[];

// Pointeurs divers
	
IMPORT tPid fileEligibles;			// index dans descProc[] du premier descripteur de processus �ligible
IMPORT tPid elu;								// index du processus qui a le cpu
IMPORT tPid fileDescProcLibre;	// premier descripteur de processus libre

/*-----------------------------------------------------------------------------
			Importation des structures de donn�es globales
			
			Pour ces trois structures, comme elles ne seront acc�d�es directement  que par creerProcessus
      il est possible de les d�finir uniquement en statique ( mot cl� LOCAL) dans le fichier contenant
      p_creeProcessus (p_noyau.c si vous suivez notre convention.

			Dans ce cas, il faut supprimer ces deux lignes
------------------------------------------------------------------------------*/


IMPORT tDescEtatCalcul descEtatCalcul[];
IMPORT tRegistre pilesPSP[];
IMPORT tRegistre pilesMSP[];

#endif /* __noyau_h */
