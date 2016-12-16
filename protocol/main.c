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
  char* message = "0026HELO0008engboris0002s1";
  protocol_data* dissection = dissectProtocol(message);
  printf("Pseudo : %s\n", get_connexion_pseudo(dissection));
  printf("Pipe : %s\n", get_connexion_pipe(dissection));

	return 0;
}
