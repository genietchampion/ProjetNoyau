#include "constC.h"

IMPORT void affiche(const char*);

/*------------------------------------------------------------------------------
	Description : fonction invoqu�e par d�faut en terminaison de processus
*/
EXPORT void detruitProcessusAppelant() {
  affiche("declenchera detruitProcessus(obtientPid())");
  while (true);				// evite de partir dans la nature � la fin
}
