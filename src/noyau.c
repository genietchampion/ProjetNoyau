#include "noyau.h"

extern void appelSvcInitNoyau(void);
extern void demoRestitution(void);

void static messageDemarrageNoyau()

  /*

  Message d�but du noyau
	
	*/

{
	
	affiche("*** D�marrage du syst�me ***");
	
	affiche("Bin�me : Romain Geniet - Ian Jeantet");
	
	affiche ("Ann�e 2015-2016");

}

void initNoyau()

  /*

  D�but de l'initialisation du noyau, on est en mode handler donc tout va bien
	
	*/

{

  // on commence par se mettre en mode indivisible puisque l'on touche au structure
	// du noyau => masquage des IT pour un mono proc ce qui est notre cas
	
	__disable_fault_irq(); // seules NMI et RESET passent

	// initialisation de l'UART
	// on commente car pas encore disponible
	
	initialiseAffichage();
	
	// message d'accueil
	
	messageDemarrageNoyau();
	
	// fin de l'initialisation pour l'�tape 1
	// on appelle demoRestitution, une fct en ASM qui va faire la restitution de contexte � partir d'une structure
	// �tat calcul statique
	
	demoRestitution();
	
  // on ne revient pas ici mais afin de parer � tout probl�me
	// vous pouvez imaginer ici un appel � une fonction panicKernel avec un syst�me d'erreur et de message
	
	while (true) ;

}

int main()

{

	/*

 	Point d'entr�e de notre programme juste apr�s l'initialisation de base
	   
	On est :
	  * on vient juste d'executer le code d'initialisation apr�s le reset
	    * init de la pile MSP
	    * init des horloges syst�mes
	    * init des zones m�moires utilis�es (� z�ro ou aux valeurs d'initialisation faite dans le code
	  * en mode thread privil�gi�, avec la pile MSP
	
	Il faut maintenant que l'on passe en mode "noyau" pour initiliaser le noyau et lancer
	notre premier processus utiisateur comme un restitution de contexte normal (via un retour
	d'exception, l'entr�e dans le noyau se faisant via une exception)
	
	Hors le cortex M3 ne veut pas faire de retour d'exception en mode thread m�me en mode
	privil�gi�, ceci g�n�re une faute mat�rielle, il faut donc faire une exception pour prendre la
	main dans le bon mode
	
	Plusieurs solutions (il y en a peut-�tre d'autres):
	  * faire une exception logicielle (SVC) : probl�me, on va utiliser SVC par la suite, il ne 
		  faudrait pas que un utilisateur r�utilise cet appel syst�me qui initialise le noyau. 2 solutions :
      * 1a) on change la table d'exception (via le vecteur de relocation du nvic)
      * 1b) lors des appels syst�mes, on n'autorise cet appel qu'� partir d'un thread privil�gi�  			
	  * 2) on utilise une exception qui ne sert pas et que l'on ne peut pas appeler en mode thread non
		  privil�gi�, par exmple Pend_SVC en activant l'exception par logiciel dans le NVIC
		
		Pour l'�tape 1, nous allons faire simple et utilis� un simple SVC. Pour les �tapes suivantes,
		� vous de voir. Personnellement je trouve plus "�l�gant" 1b puis 2 puis 1a.
		
	*/
	
	// appel d'une fonction en assembleur pour faire le SVC 
	// on pourrait le faire via du C ou via une instruction assembleur ins�r�e directement
	
	appelSvcInitNoyau();

	// on ne revient pas ici mais afin de parer � tout probl�me
	// vous pouvez imaginer ici un appel � une fonction panicKernel avec un syst�me d'erreur et de message
	
	while (true) ;

}
