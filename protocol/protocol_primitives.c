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

Si on oublie de mettre a jour les fonctions lors de la creation
d'un nouveau type, on est prevenu par une erreur lors de l'execution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "protocol_primitives.h"

#define TYPE_LENGTH 4

/* ---------------------------------------------
		    MANIPULATION DE DONNEES
   --------------------------------------------- */

char* stringWithoutLength(char* string) {
	int length = strlen(string);
	int offset = 0;
	int i = 0;
	while (isdigit(string[i])) {
		offset++;
		i++;
	}
	char* buffer = (char*)malloc(sizeof(char)*(length-offset+1));
	buffer = strcpy(buffer, string+offset);
	free(string);
	return buffer;
}

void freeProtocolContent(content_data* d) {
	if (d != NULL) {
		if (d->is_string == 1 && d->data_union->string != NULL) {
			free(d->data_union->string);
		}
		free(d->data_union);
	}
}

void freeProtocolData(protocol_data* d) {
	data_element* e = d->data;
	while (e != NULL) {
	        freeProtocolContent(e->resource);
		data_element* tmp = e;
		e = e->next;
		free(tmp);
	}
	free(d);
}

void insertData(protocol_data* d, content_data* content) {
	data_element* es = (data_element*)malloc(sizeof(data_element));
	es->resource = content;
	es->next = NULL;

	data_element* e = d->data;
	if (e != NULL) {
		while (e->next != NULL)
			e = e->next;
		e->next = es;
	}
	else {
		d->data = es;
	}
}

/* ---------------------------------------------
			 FONCTIONS D'ENCODAGE
   --------------------------------------------- */

const char* encodeType(message_type type) {
	switch (type) {
		case BCST_t: return "BCST"; break;
		case PRVT_t: return "PRVT"; break;
		case BADD_t: return "BADD"; break;
		case OKOK_t: return "OKOK"; break;
		case BYEE_t: return "BYEE"; break;
		case HELO_t: return "HELO"; break;
		case LIST_t: return "LIST"; break;
		case SHUT_t: return "SHUT"; break;
		case DEBG_t: return "DEBG"; break;
		case FILE_t: return "FILE"; break;
		default:
			printf("messagetype_to_string : unknown or unimplemented type");
			exit(0);
			break;
	}
}

char* encodeNumber(long int n, int length) {
	char* buffer = (char*)malloc(sizeof(char)*length+1);
	int i;
	int mod_factor = 10, div_factor = 1;
	for (i = length-1; i >= 0; i--) {
		buffer[i] = (char)('0'+((int)(n%mod_factor/div_factor)));
		mod_factor *= 10;
		div_factor *= 10;
	}
	return buffer;
}

char* encodeString(char* string, long int length) {
	char* buffer;
	char* numberEncoding;
	if (length <= 9999) {
		buffer = (char*)calloc(sizeof(char), length+4+1);
		numberEncoding = encodeNumber(length, 4);
		buffer[length+4] = '\0';
	}
	else if (length <= 99999999) {
		buffer = (char*)calloc(sizeof(char), length+8+1);
		numberEncoding = encodeNumber(length, 8);
		buffer[length+8] = '\0';
	}
	else {
		printf("encodeString : length to large to encode");
		exit(0);
	}
	buffer = strcat(buffer, numberEncoding);
	buffer = strcat(buffer, string);
	free(numberEncoding);
	return buffer;
}

protocol_data* initMessageHeader(message_type type) {
	protocol_data* p = (protocol_data*)malloc(sizeof(protocol_data));
	p->total_length = 8;
	p->type = type;
	p->data = NULL;
	return p;
}

protocol_message encodeProtocolData(protocol_data* d) {
	// Composition de l'entête du message
	int length_nbchar = 0;
	char* numberEncoding;
	if (d->total_length <= 9999)
		length_nbchar = 4;
	else if (d->total_length <= 99999999)
		length_nbchar = 8;
	else {
		printf("encodeProtocolData : length to large to encode");
		exit(0);
	}
	char* buffer_message = (char*)calloc(sizeof(char), length_nbchar+TYPE_LENGTH+1);
	if (length_nbchar == 4) {
		numberEncoding = encodeNumber(d->total_length, 4);
		buffer_message = strcat(buffer_message, numberEncoding);
	}
	else {
		numberEncoding = encodeNumber(d->total_length, 8);
		buffer_message = strcat(buffer_message, numberEncoding);
	}
	buffer_message = strcat(buffer_message, encodeType(d->type));
	buffer_message[length_nbchar+TYPE_LENGTH] = '\0';

	// Composition des données du message dans une structure
	data_element* e = d->data;
	while (e != NULL) {
		if (e->resource->is_string == 1) {
			long int string_length = strlen(e->resource->data_union->string);
			buffer_message = (char*)realloc(buffer_message, sizeof(char)*(length_nbchar+TYPE_LENGTH+string_length));
			buffer_message = strcat(buffer_message, e->resource->data_union->string);
		}
		else {
			long int string_length;
			if (e->resource->data_union->integer <= 9999)
				string_length = 4;
			else if (e->resource->data_union->integer <= 99999999)
				string_length = 8;
			else {
				printf("encodeProtocolData : depassement de limite d'entier.");
				exit(0);
			}
			char* numberEncoding = encodeNumber(e->resource->data_union->integer, string_length);
			buffer_message = (char*)realloc(buffer_message, sizeof(char)*(length_nbchar+TYPE_LENGTH+string_length));
			buffer_message = strcat(buffer_message, numberEncoding);
			free(numberEncoding);
		}
		e = e->next;
	}

	freeProtocolData(d);

	return buffer_message;
}

void addMessageString(protocol_data* d, char* string) {
	content_data* cd = (content_data*)malloc(sizeof(content_data));
	content_union* cu = (content_union*)malloc(sizeof(content_union));
	// Encodage de la chaine
	long int string_length = strlen(string);
	int stringEncodingLength = string_length;
	if (stringEncodingLength <= 9999)
		stringEncodingLength += 4;
	else if (stringEncodingLength <= 99999999)
		string_length += 8;
	else {
		printf("addMessageString : depassement de limite d'entier.");
		exit(0);
	}
	d->total_length += stringEncodingLength;

	// Assemblage
	char* buffer = (char*)calloc(sizeof(char), string_length);
	buffer = strcat(buffer, string);

	cu->string = buffer;
	cd->is_string = 1;
	cd->data_union = cu;
	insertData(d, cd);
}

void addMessageNumber(protocol_data* d, long int number) {
  if (number <= 9999)
  	d->total_length += 4;
  else if (number <= 99999999)
  	d->total_length += 8;
  else {
  	printf("addMessageNumber : depassement de limite d'entier.");
	  exit(0);
  }
	content_data* cd = (content_data*)malloc(sizeof(content_data));
	content_union* cu = (content_union*)malloc(sizeof(content_union));
	cu->integer = number;
	cd->is_string = 0;
	cd->data_union = cu;
	insertData(d, cd);
}

/* ---------------------------------------------
			FONCTIONS DE DECODAGE
   --------------------------------------------- */

int char2int(char c) {
	return (int)(c-'0');
}

// (1) On suppose qu'on ne peut pas avoir de chiffres dans le type
// (2) On suppose que le type ne contient que des lettres majuscules
long int decodeNumber(char* message) {
	int allocation = 1;
	char* buffer = (char*)malloc(allocation*sizeof(char));
	int i = 0;
	while (isdigit(message[i])) {
		buffer[i] = message[i];
		allocation++;
		buffer = realloc(buffer, allocation*sizeof(char));
		i++;
	}
	return atoi(buffer);
}

long int decodeLength(protocol_message message) {
	return decodeNumber(message);
}

const char* getTypeStructure(message_type type) {
	switch (type) {
		case BADD_t: return ""; break;
		case BYEE_t: return "I"; break;
		case HELO_t: return "SS"; break;
		default:
			return NULL;
			break;
	}
}

message_type decodeType(protocol_message message) {
	int resultat = -1;
	int i = 0;
	while (isdigit(message[i]))
		i++;
	char* buffer = (char*)malloc(sizeof(char)*4);
	sprintf(buffer, "%c%c%c%c", message[i], message[i+1], message[i+2], message[i+3]);
	     if (strcmp("BCST", buffer) == 0) { resultat = BCST_t; }
	else if (strcmp("PRVT", buffer) == 0) { resultat = PRVT_t; }
	else if (strcmp("BADD", buffer) == 0) { resultat = BADD_t; }
	else if (strcmp("OKOK", buffer) == 0) { resultat = OKOK_t; }
	else if (strcmp("BYEE", buffer) == 0) { resultat = BYEE_t; }
	else if (strcmp("HELO", buffer) == 0) { resultat = HELO_t; }
	else if (strcmp("LIST", buffer) == 0) { resultat = LIST_t; }
	else if (strcmp("SHUT", buffer) == 0) { resultat = SHUT_t; }
	else if (strcmp("DEBG", buffer) == 0) { resultat = DEBG_t; }
	else if (strcmp("FILE", buffer) == 0) { resultat = FILE_t; }
	free(buffer);
	return resultat;
}

void extractMessageContent(protocol_message message, protocol_data* data, const char* codeStructure) {
	/*long int messageTotalLength = data->total_length;
	int codeStructureLength = strlen(codeStructure);
	char current_token;
	int i;
	for (i = 0; i < codeStructureLength; i++) {
			current_token = codeStructure[i];

	}*/
	// TEMPORAIRE EN ATTENDANT UN VRAI DECODAGE
  int header_length = headerLength(message);
	if (strcmp(codeStructure, "I") == 0) {
		addMessageNumber(data, atoi(message+header_length));
	}
	else if (strcmp(codeStructure, "SS") == 0) {
		int i;
		char size1[4];
		size1[0] = message[header_length];
	  size1[1] = message[header_length+1];
		size1[2] = message[header_length+2];
		size1[3] = message[header_length+3];
		int sizeString1 = atoi(size1);
		char* string1 = malloc(sizeString1+1);
		for (i = 0; i < sizeString1; i++) {
			string1[i] = message[header_length+4+i];
		}
		string1[sizeString1] = '\0';
		char size2[4];
		size2[0] = message[header_length+4+sizeString1];
	  size2[1] = message[header_length+4+sizeString1+1];
		size2[2] = message[header_length+4+sizeString1+2];
		size2[3] = message[header_length+4+sizeString1+3];
		int sizeString2 = atoi(size2);
		char* string2 = malloc(sizeString2+1);
		string2[sizeString2] = '\0';
		for (i = 0; i < sizeString2; i++) {
			string2[i] = message[header_length+4+sizeString1+4+i];
		}
		addMessageString(data, string1);
		addMessageString(data, string2);
	}
}

int headerLength(protocol_message message) {
  int l = 0;
	while (isdigit(message[l]))
	  l++;
	return l+4;
}

protocol_data* dissectProtocol(protocol_message message) {
	protocol_data* protocolData = (protocol_data*)malloc(sizeof(protocol_data));
	protocolData->total_length = decodeLength(message);
	protocolData->type = decodeType(message);
	const char* codeStructure = getTypeStructure(protocolData->type);
	extractMessageContent(message, protocolData, codeStructure);
	return protocolData;
}
