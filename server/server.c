#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "function.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/select.h>
#include "protocol_instance.h"
#include "protocol_primitives.h"

#define OCTET_4 4 
#define OCTET_8 8 

char * path_server = "Server.pipe" ;
/*insertion id -> pseudo
  pseudo -> pipe
*/

int creat_tube(char *pipe){
  if(pipe == NULL){
    return -1 ;
  }
  if(access(pipe,F_OK) == -1){
    int r = mkfifo(pipe,0666);
    if(r == -1){
      printf("error mkfifo \n");
      return -1 ;
    }
  }
  return 0 ;
}

int hello(GHashTable * hash_pseudo_id ,GHashTable * hash_id_pipe ,char * pseudo, char * pipe,int * id){
  char * msg_c ;
  gboolean  bool ;
  int fd = open(pipe,O_WRONLY);
  if(fd == -1 ){
    perror("open");
    return -1 ; 
  }
  if((is_in_hashTab(hash_id_pipe,pseudo)) == TRUE){ /* si le client exciste deja */
    msg_c = encodeFail();
    write(fd,msg_c,strlen(msg_c));
    return -2 ; 
  }
  creat_tube(pipe);
  *id = *id + 1 ; 
  bool = insert_pseudo_id(hash_pseudo_id ,id , pseudo);
  if(bool == FALSE){
    return -1  ; 
  }
  bool = insert_pseudo_pipe(hash_id_pipe,pseudo ,pipe);
  if(bool == FALSE){
    return -1  ; 
  }
  
  msg_c = encodeConnexionConfirmation(*id);
  printf("msg envoyer %s \n",msg_c);
  write(fd,msg_c,strlen(msg_c));
  return 0 ; 
}



void f(int sig) {
  printf("oups SIGPIPE!!!\n");
  printf("client deconnecter !!!\n");
}


int main(int arg , char * argv []){
  struct sigaction action;
  action.sa_handler = &f;
  sigemptyset(&(action.sa_mask));
  action.sa_flags = 0;
  sigaction(SIGPIPE,&action,NULL);
  
  char size_msg [4] ;
  GHashTable * hash_tab_pseudo_id =  g_hash_table_new (g_int_equal,g_str_hash);
  GHashTable * hash_tab_id_pipe =  g_hash_table_new (g_str_hash, g_str_equal); 
  int id = 0 ; 
  if(access(path_server,F_OK) == -1){
    int r = mkfifo(path_server,0666);
    if(r == -1){
      printf("error mkfifo \n");
      exit(24);
    }
  }
  
  int fd  = open(path_server,O_RDONLY);
  printf("ouverture pipe server \n");
  if(fd == -1){
    fprintf(stderr,"error fd \n");
    exit(4);
  }
  
  while(1){
    int lus = read(fd,size_msg,4*sizeof(char));
    size_msg[4] = '\0';
    if(lus == -1 ){
      perror("read ");
      continue ; 
    }
    int size = atoi(size_msg) ;
    char * msg_bis = malloc((size * sizeof(char)) - 3);
    lus = read(fd,msg_bis,size);
    char * msg = malloc(size * sizeof(char));
    msg[size-1] ='\0';
    strcpy(msg,size_msg);
    strcat(msg,msg_bis);
    protocol_data* dissection = dissectProtocol(msg,TCHATCHE_SERVER);
    if(dissection->type == HELO_t){
      char *pseudo =  get_connexion_username(dissection) ; 
      char * pipe  =  get_connexion_pipe(dissection) ; 
      hello(hash_tab_pseudo_id,hash_tab_id_pipe,pseudo,pipe,&id) ; 
    }else if(dissection->type == BYEE_t ){
      printf("deconnexion");
    }
    
    sleep(3);
    
  }
  return  0 ;
}

