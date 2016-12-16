#ifndef protocol_primitives_header
#define protocol_primitives_header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef char* protocol_message;

#define MESSAGE_TYPE_LENGTH 4

typedef enum message_type {
	BCST_t,
	PRVT_t,
	BADD_t,
	OKOK_t,
	BYEE_t,
	HELO_t,
	LIST_t,
	SHUT_t,
	DEBG_t,
	FILE_t
} message_type;

typedef enum content_type {
	STRING_MESSAGE,
	INTEGER_MESSAGE
} content_type;

typedef union content_union {
	int integer;
	char* string;
} content_union;

typedef struct content_data {
	int is_string;
	content_union* data_union;
} content_data;

typedef struct data_element {
	content_data* resource;
	struct data_element* next;
} data_element;

typedef struct protocol_data {
	int total_length;
	message_type type;
	data_element* data;
} protocol_data;

/* ---------------------------------------------
		    MANIPULATION DE DONNEES
   --------------------------------------------- */

void freeProtocolContent(content_data* d);

void freeProtocolData(protocol_data* d);

void insertData(protocol_data* d, content_data* content);

/* ---------------------------------------------
			 FONCTIONS D'ENCODAGE
   --------------------------------------------- */

const char* encodeType(message_type type);

char* encodeNumber(int n, int length);

char* encodeString(char* string, int length);

protocol_data* initMessageHeader(message_type type);

protocol_message encodeProtocolData(protocol_data* d);

void addMessageString(protocol_data* d, char* string);

void addMessageNumber(protocol_data* d, int number);

/* ---------------------------------------------
			FONCTIONS DE DECODAGE
   --------------------------------------------- */

int char2int(char c);

int decodeNumber(char* number);

int decodeLength(protocol_message message);

// Retourn -1 si échec
message_type decodeType(protocol_message message);

// Permet de récupérer le code structure d'un protocol encodé sous forme d'une chaine de caractre
// Le codage est le suivant :
// I : Integer
// L : Long
// S : String
// Retourne NULL si échec
const char* getTypeStructure(message_type type);

void extractMessageContent(protocol_message message, protocol_data* data, const char* codeStructure);

int headerLength(protocol_message message);

protocol_data* dissectProtocol(protocol_message message);

#endif
