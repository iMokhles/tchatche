/* ___________________________________________
		  PROTOCOLE DE COMMUNICATION
   ___________________________________________

Format du protocole :
- On represente la taille du messages avec 4 ou 8 caracteres
- Le type avec 4 caracteres
- Le reste des donnees
  + Soit une taille de chaine suivie de la chaine concernee
  + Un entier (4 ou 8 caracteres)

Le header correspond a la taille + le type
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "protocol_primitives.h"

/* ---------------------------------------------
			 FONCTIONS D'ENCODAGE
   --------------------------------------------- */

// ------------------- Connexion -------------------
protocol_message encodeConnexion(char* username, char* tube) {
	protocol_data* header = initMessageHeader(HELO_t);
	addMessageString(header, username);
	addMessageString(header, tube);
	return encodeProtocolData(header);
}

protocol_message encodeConnexionConfirmation(int id) {
	protocol_data* header = initMessageHeader(OKOK_t);
	addMessageNumber(header, id);
	return encodeProtocolData(header);
}

// ------------------- Echec -------------------
protocol_message encodeFail() {
	protocol_data* header = initMessageHeader(BADD_t);
	return encodeProtocolData(header);
}

// ------------------- Deconnexion -------------------
protocol_message encodeDeconnexion(int id) {
	protocol_data* header = initMessageHeader(BYEE_t);
	addMessageNumber(header, id);
	return encodeProtocolData(header);
}

protocol_message encodeDeconnexionConfirmation(int id) {
	return encodeDeconnexion(id);
}

// ------------------- Message public -------------------
protocol_message encodePublicMessage(int id, char* message) {
	protocol_data* header = initMessageHeader(BCST_t);
	addMessageNumber(header, id);
	addMessageString(header, message);
	return encodeProtocolData(header);
}

protocol_message encodePublicMessageFeedback(char* username, char* message) {
	protocol_data* header = initMessageHeader(BCST_t);
	addMessageString(header, username);
	addMessageString(header, message);
	return encodeProtocolData(header);
}

/* ---------------------------------------------
			FONCTIONS DE DECODAGE
   --------------------------------------------- */

// ------------------- Connexion -------------------
char* get_connexion_username(protocol_data* dissection) {
	char* username = get_nth_dissection(dissection, 0)->string;
	return username;
}

char* get_connexion_pipe(protocol_data* dissection) {
	char* pipe = get_nth_dissection(dissection, 1)->string;
	return pipe;
}

int get_connexionConfirmation_id(protocol_data* dissection) {
	int id = get_nth_dissection(dissection, 0)->integer;
	return id;
}

// ------------------- Deconnexion -------------------
int get_deconnexion_id(protocol_data* dissection) {
	int id = get_nth_dissection(dissection, 0)->integer;
	return id;
}

int get_deconnexionConfirmation_id(protocol_data* dissection) {
	int id = get_nth_dissection(dissection, 0)->integer;
	return id;
}

// ------------------- Message public -------------------
int get_publicMessage_id(protocol_data* dissection) {
	int id = get_nth_dissection(dissection, 0)->integer;
	return id;
}

char* get_publicMessage_message(protocol_data* dissection) {
	char* message = get_nth_dissection(dissection, 1)->string;
	return message;
}

char* get_publicMessageFeedback_username(protocol_data* dissection) {
	char* username = get_nth_dissection(dissection, 0)->string;
	return username;
}

char* get_publicMessageFeedback_message(protocol_data* dissection) {
	char* message = get_nth_dissection(dissection, 1)->string;
	return message;
}