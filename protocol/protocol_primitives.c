/* ___________________________________________
		  PROTOCOLE DE COMMUNICATION
   ___________________________________________

Format du protocole :
- On represente la taille du messages avec 4 caracteres
- Le type avec 4 caracteres
- Le reste des donnees
  + Soit une taille de chaine suivie de la chaine concernee
  + Un entier simple(4) ou long(8)

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

/* ---------------------------------------------
		    MANIPULATION DE DONNEES
   --------------------------------------------- */

void freeProtocolContent(content_data* d) {
	if (d != NULL) {
		if (d->is_string == 1 && d->data_union->string != NULL) {
			free(d->data_union->string);
		}
		free(d->data_union);
	}
	free(d);
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
	data_element* new = (data_element*)malloc(sizeof(data_element));
	new->resource = content;
	new->next = NULL;

	data_element* it = d->data;
	if (it != NULL) {
		while (it->next != NULL)
			it = it->next;
		it->next = new;
	}
	else {
		d->data = new;
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

char* encodeNumber(int n, int length) {
	char* buffer = (char*)malloc((length+1)*sizeof(char));
	int i;
	int mod_factor = 10, div_factor = 1;
	for (i = length-1; i >= 0; i--) {
		buffer[i] = (char)('0'+((int)(n%mod_factor/div_factor)));
		mod_factor *= 10;
		div_factor *= 10;
	}
	buffer[length] = '\0';
	return buffer;
}

char* encodeString(char* string, int length) {
	char* buffer = (char*)malloc((4+length+1)*sizeof(char));
	char* numberEncoding = encodeNumber(length, 4);
	strcpy(buffer, numberEncoding);
	strcat(buffer, string);
	free(numberEncoding);
	buffer[4+length] = '\0';
	return buffer;
}

protocol_data* initMessageHeader(message_type type) {
	protocol_data* p = (protocol_data*)malloc(sizeof(protocol_data));
	p->total_length = 4+MESSAGE_TYPE_LENGTH;
	p->type = type;
	p->data = NULL;
	return p;
}

protocol_message encodeProtocolData(protocol_data* d) {
	// Composition de l'entête du message
	const int header_length = 4+MESSAGE_TYPE_LENGTH;
	int allocation = header_length+1;
	char* buffer_message = (char*)malloc(allocation*sizeof(char));

	char* numberEncoding = encodeNumber(d->total_length, 4);
	buffer_message = strcpy(buffer_message, numberEncoding);
	buffer_message = strcat(buffer_message, encodeType(d->type));

	// Composition des données du message dans une structure
	data_element* e = d->data;
	while (e != NULL) {
		// Cas des chaînes
		if (e->resource->is_string == 1) {
			int string_length = strlen(e->resource->data_union->string);
			char* stringEncoding = encodeString(e->resource->data_union->string, string_length);
			allocation += (header_length+4+string_length+1);
			buffer_message = (char*)realloc(buffer_message, sizeof(char)*allocation);
			buffer_message = strcat(buffer_message, stringEncoding);
			free(stringEncoding);
		}
		// Cas des nombres
		else {
			int string_length;
			if (e->resource->data_union->integer <= 9999)
  				string_length = 4;
  			else if (e->resource->data_union->integer <= 99999999)
  				string_length = 8;
  			else {
  				printf("addMessageNumber : depassement de limite d'entier.");
	  			exit(0);
  			}
			char* numberEncoding = encodeNumber(e->resource->data_union->integer, string_length);
			allocation += (header_length+string_length+1);
			buffer_message = (char*)realloc(buffer_message, sizeof(char)*allocation);
			strcat(buffer_message, numberEncoding);
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
	int string_length = strlen(string);
	int stringEncodingLength = 4+string_length;
	d->total_length += stringEncodingLength;

	// Ajout
	char* buffer = (char*)malloc((string_length+1)*sizeof(char));
	strcpy(buffer, string);
	buffer[string_length] = '\0';

	cu->string = buffer;
	cd->is_string = 1;
	cd->data_union = cu;
	insertData(d, cd);
}

void addMessageNumber(protocol_data* d, int number) {
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
int decodeNumber(char* message) {
	int allocation = 1;
	int value;
	char* buffer = (char*)malloc(allocation*sizeof(char));
	int i = 0;
	while (isdigit(message[i])) {
		buffer[i] = message[i];
		allocation++;
		buffer = realloc(buffer, allocation*sizeof(char));
		i++;
	}
	value = atoi(buffer);
	free(buffer);
	return value;
}

int decodeLength(protocol_message message) {
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
	char* buffer = (char*)malloc(sizeof(char)*MESSAGE_TYPE_LENGTH);
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
	int codeStructureLength = strlen(codeStructure);
	char current_token;
	int current_position = headerLength(message);
	int i;
	for (i = 0; i < codeStructureLength; i++) {
			current_token = codeStructure[i];
			// Décodage d'un nombre
			if (current_token == 'I' || current_token == 'L') {
				int number_length;
				if (current_token == 'I')
					number_length = 4;
				else if (current_token == 'L')
					number_length = 8;
				char buffer[number_length];
				int j;
				for (j = 0; j < number_length; j++) {
					buffer[j] = message[current_position];
					current_position++;
				}
				addMessageNumber(data, atoi(buffer));
			}
			// Décodage d'une chaîne
			else if (current_token == 'S') {
				char size_length = 4;
				char buffer[size_length];
				int j;
				for (j = 0; j < size_length; j++) {
					buffer[j] = message[current_position];
					current_position++;
				}
				int size_value = atoi(buffer);
				char* string = (char*)malloc((size_value+1)*sizeof(char));
				for (j = 0; j < size_value; j++) {
					string[j] = message[current_position];
					current_position++;
				}
				string[size_value] = '\0';
				addMessageString(data, string);
			}
	}
}

int headerLength(protocol_message message) {
  int l = 0;
	while (isdigit(message[l]))
	  l++;
	return l+MESSAGE_TYPE_LENGTH;
}

protocol_data* dissectProtocol(protocol_message message) {
	protocol_data* protocolData = (protocol_data*)malloc(sizeof(protocol_data));
	protocolData->total_length = decodeLength(message);
	protocolData->type = decodeType(message);
	protocolData->data = NULL;
	const char* codeStructure = getTypeStructure(protocolData->type);
	extractMessageContent(message, protocolData, codeStructure);
	return protocolData;
}

content_union* get_nth_dissection(protocol_data* dissection, int nth) {
	data_element* it = dissection->data;
	int i;
	for (i = 0; i < nth; i++)
		it = it->next;
	return it->resource->data_union;
}
