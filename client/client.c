#include "client.h"

// Colors for text
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char* network_path = "../server/Server.pipe";
//char* my_pipe = "";
int id;
int *pipes;

void run()
{
	pipes = (int*) malloc (sizeof(int) * 2);
	connexion();
	// boucle programme

// 	char* input;
// 	while(strcmpcmp((input = get_user_input()), "/quit") != 0)
// 	{

// 	}
}

void connexion()
{
	//mode_t t = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	if(access (network_path, F_OK) == -1){
		printf("can't access server");
		exit(-1);
	}

	pipes[0] = open(network_path, O_WRONLY);

	//size_t size = 16;
	char pseudo[50];
	char tube[50];
	do
	{
		printf("Veuillez choisir un pseudo : ");
		scanf("%s", pseudo);
		printf("Veuillez choisir un tube : ");
		scanf("%s", tube);

	}while(connexion_approval(pseudo, tube) == -1);

	printf("connexion approuvée");
}

//FIXME : envoyer le msg encodé + récup id + chemin de tube perso et l'ouvrir en lecture.
int connexion_approval(char* pseudo, char* tube)
{

	protocol_message connexion = encodeConnexion(pseudo, tube); // protocol_message = char* 
	send_message(connexion);

	char my_pipe[50];
	strcpy(my_pipe, "../server/");
	strcat(my_pipe, tube);

	pipes[1] = open(my_pipe, O_RDONLY);
	if(pipes[1] == -1)
	{
		printf("problème ouverture du pipe");
		exit(-1);
	}
	char* message = read_message();
	printf("message : %s\n%d\n", message, strlen(message));
	protocol_data* dissection = dissectProtocol(message, TCHATCHE_CLIENT);
	

	if(dissection->type == OKOK_t)
	{
		id = get_connexionConfirmation_id(dissection);
		free(message);
		freeProtocolData(dissection);
		return 1;
	}
	
	free(message);
	freeProtocolData(dissection);
	return -1;
}

char* get_user_input()
{
	char c;
	int size = 0;
	char* result = (char*) calloc (1, sizeof(char));
	getchar();
	printf(">");
	while((c = getchar()) != '\n')
	{	
		*(result + (size++)) = c;
		result = (char*) realloc(result, sizeof(char) * size+1);
	}

	*(result+size) = '\0';

	printf("message de l'utilisateur : %s\n", result);
	return result;

}


void deconnexion()
{
	// char* msg = encode_deconnexion(id);
	// write(pipes[0], msg, strlen(msg) + sizeof(char));

	close(pipes[0]);
	close(pipes[1]);
	free(pipes);
}

char* read_message()
{

	// char c;
	// int tmp = 1;	
	char* tmp_size = (char*) calloc (5, sizeof(char));
	read(pipes[1], tmp_size, sizeof(char)*4);
	tmp_size[4] = '\0';
	// while(1) //number for ascii
	// {

	//   	read(pipes[1], &c, sizeof(char));
	//     if(c < 48 || c > 57) // c is not a number, we got the full length
	//     	break;
	//     tmp++;
	//     tmp_size = (char*) realloc (tmp_size, tmp);
	//     tmp_size[tmp-1] = c;

	// }
	printf("tmp_size : %s\n", tmp_size);
	int msg_length = atoi(tmp_size);
	printf("msg_length : %d\n", msg_length);
	char* body = (char*) calloc (msg_length, sizeof(char));

	read(pipes[1], body, msg_length);

	char* result = (char*) calloc (strlen(body) + msg_length, sizeof(char));
	strcpy(result, tmp_size);
	strcat(result, body);

	free(body);
	free(tmp_size);

	return result;


}

void send_message(protocol_message msg)
{
	
	int i = write(pipes[0], msg, strlen(msg) * sizeof(char));
	if(i == -1)
	{
		printf("problème écriture : \nprotocole %s\n", msg);
		exit(-1);
	}
}

void print_message(const char* msg)
{
	printf("%s", parsing(msg));
}

// Coloriage du texte, gestion des pseudo pour les messages privé, etc.
char* parsing(const char* msg)
{
	return NULL;
}

int main()
{
	run();
	return 0;
}