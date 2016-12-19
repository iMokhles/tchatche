#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "protocol_primitives.h"
#include "protocol_instance.h"

int main() {
	// ============ ENCODAGE ============
	int i;
	// Connexion
	protocol_message connexion = encodeConnexion("engboris", "s1");
	for (i = 0; i < decodeLength(connexion); i++) {
		printf("%c", connexion[i]);
	}
	printf("\n");
	// Connexion confirmation
	protocol_message confirmation = encodeConnexionConfirmation(15);
	for (i = 0; i < decodeLength(confirmation); i++) {
		printf("%c", confirmation[i]);
	}
	printf("\n");
	// Fail
	protocol_message fail = encodeFail();
	for (i = 0; i < decodeLength(fail); i++) {
		printf("%c", fail[i]);
	}
	printf("\n");
	// Message public
	protocol_message messagePublic = encodePublicMessageFeedback("engboris", "salut les amis ca va ? ^^ moi oui ca va tres bien");
	for (i = 0; i < decodeLength(messagePublic); i++) {
		printf("%c", messagePublic[i]);
	}
	printf("\n");

	// ============ DECODAGE ============
	// Connexion
	protocol_data* dissectionConnexion = dissectProtocol(connexion, TCHATCHE_SERVER);
	printf("Pseudo : %s\n", get_connexion_username(dissectionConnexion));
	printf("Pipe : %s\n", get_connexion_pipe(dissectionConnexion));
	// Message public
	protocol_data* dissectionMessagePublic = dissectProtocol(messagePublic, TCHATCHE_CLIENT);
	printf("[%s] %s\n", get_publicMessageFeedback_username(dissectionMessagePublic), get_publicMessageFeedback_message(dissectionMessagePublic));
	// Confirmation
	protocol_data* dissectionOKOK = dissectProtocol(confirmation, TCHATCHE_CLIENT);
	printf("ID : %d\n", get_connexionConfirmation_id(dissectionOKOK));
	return 0;
}