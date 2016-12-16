#ifndef protocol_instance_header
#define protocol_instance_header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* ---------------------------------------------
			 FONCTIONS D'ENCODAGE
   --------------------------------------------- */

// ------------------- Connexion -------------------
protocol_message encodeConnexion(char* pseudo, char* tube);

protocol_message encodeConnexionConfirmation(int id);

// ------------------- Echec -------------------
protocol_message encodeFail();

// ------------------- Deconnexion -------------------
protocol_message encodeDeconnexion(int id);

protocol_message encodeDeconnexionConfirmation(int id);

/* ---------------------------------------------
			FONCTIONS DE DECODAGE
   --------------------------------------------- */

// ------------------- Connexion -------------------
char* get_connexion_pseudo(protocol_data* message);

char* get_connexion_pipe(protocol_data* message);

int get_connexionConfirmation_id(protocol_data* message);

// ------------------- Deconnexion -------------------
int get_deconnexion_id(protocol_data* message);

int get_deconnexionConfirmation_id(protocol_data* message);

#endif
