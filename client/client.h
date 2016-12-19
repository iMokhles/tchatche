#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>
#include "protocol_primitives.h"
#include "protocol_instance.h"

void run();
void connexion();
int connexion_approval(char* pseudo);
void deconnexion();
char* read_message();
void send_message();
void print_message(const char* msg);
char* parsing(const char* msg);
