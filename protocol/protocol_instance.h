#ifndef protocol_instance_header
#define protocol_instance_header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "protocol_primitives.h"

/* ---------------------------------------------
			 FONCTIONS D'ENCODAGE
   --------------------------------------------- */

// ------------------- Connexion -------------------
protocol_message encodeConnexion(char* username, char* tube);

protocol_message encodeConnexionConfirmation(int id);

// ------------------- Echec -------------------
protocol_message encodeFail();

// ------------------- Deconnexion -------------------
protocol_message encodeDeconnexion(int id);

protocol_message encodeDeconnexionConfirmation(int id);

// ------------------- Message public -------------------
protocol_message encodePublicMessage(int id, char* message);

protocol_message encodePublicMessageFeedback(char* username, char* message);

/* ---------------------------------------------
			FONCTIONS DE DECODAGE
   --------------------------------------------- */

// ------------------- Connexion -------------------
char* get_connexion_username(protocol_data* dissection);
char* get_connexion_pipe(protocol_data* dissection);

int get_connexionConfirmation_id(protocol_data* dissection);

// ------------------- Deconnexion -------------------
int get_deconnexion_id(protocol_data* dissection);

int get_deconnexionConfirmation_id(protocol_data* dissection);

// ------------------- Message public -------------------
int get_publicMessage_id(protocol_data* dissection);
char* get_publicMessage_message(protocol_data* dissection);

char* get_publicMessageFeedback_username(protocol_data* dissection);
char* get_publicMessageFeedback_message(protocol_data* dissection);


#endif
