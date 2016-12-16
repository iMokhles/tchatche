#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "protocol_primitives.h"
#include "protocol_instance.h"

int main() {
  // ENCODAGE
  int i;
	protocol_message connexion = encodeConnexion("engboris", "s1");
	for (i = 0; i < decodeLength(connexion); i++) {
		printf("%c", connexion[i]);
	}
	printf("\n");
	protocol_message confirmation = encodeConnexionConfirmation(15);
	for (i = 0; i < decodeLength(confirmation); i++) {
		printf("%c", confirmation[i]);
	}
	printf("\n");
	protocol_message fail = encodeFail();
	for (i = 0; i < decodeLength(fail); i++) {
		printf("%c", fail[i]);
		}
	printf("\n");
	protocol_message deconnexion = encodeDeconnexion(15);
	for (i = 0; i < decodeLength(deconnexion); i++) {
		printf("%c", deconnexion[i]);
	}
	printf("\n");
	printf("Type : %s\n", encodeType(decodeType(deconnexion)));

  // DECODAGE
  protocol_message connexion2 = encodeConnexion("engboris", "s1");
  protocol_data* dissection = dissectProtocol(connexion2);
  printf("Pseudo : %s\n", get_connexion_pseudo(dissection));
  printf("Pipe : %s\n", get_connexion_pipe(dissection));

  protocol_message connexion3 = encodeConnexion("chelbibachir", "s2");
  protocol_data* dissection2 = dissectProtocol(connexion3);
  printf("Pseudo : %s\n", get_connexion_pseudo(dissection2));
  printf("Pipe : %s\n", get_connexion_pipe(dissection2));

  protocol_message deconnexion2 = encodeDeconnexion(15);
  protocol_data* dissection3 = dissectProtocol(deconnexion2);
  printf("ID : %d\n", get_deconnexion_id(dissection3));

  protocol_message connexion4 = encodeConnexion("kostiachardonnet", "s3");
  protocol_data* dissection4 = dissectProtocol(connexion4);
  printf("Pseudo : %s\n", get_connexion_pseudo(dissection4));
  printf("Pipe : %s\n", get_connexion_pipe(dissection4));

	return 0;
}
