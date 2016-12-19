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

#define MAX_BIS(x,y) ((x)>(y)?(x):(y))
#define OCTET_4 4 
#define OCTET_8 8 

int creat_tube(char *pipe){
  if(pipe == NULL){
    return -1 ;
  }
  if(access(pipe,F_OK) == -1){
    int r = mkfifo(pipe,0666);
    if(r == -1){
      printf("error mkfifo \n");
      exit(24);
    }
  }
  return 0 ;
}

void f(int sig) {
  printf("oups SIGPIPE!!!\n");
  printf("client deconnecter !!!\n");
}

int id = 0 ;
char * path_server = "Server.pipe" ;

int main(int arg , char * argv []){
  struct sigaction action;
  action.sa_handler = &f;
  sigemptyset(&(action.sa_mask));
  action.sa_flags = 0;
  sigaction(SIGPIPE,&action,NULL);
  

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
  
  char size_msg [5] ;
  size_t l;
  fd_set enslec;
  
  while(1){
    int lus = read(fd,size_msg,4*sizeof(char));
    size_msg[4] = '\0';
    if(lus == -1 ){
      perror("read ");
      continue ; 
    }
    
    int size = atoi(size_msg) ;
    char * msg_bis = malloc(size * sizeof(char) - 4);
    
    lus = read(fd,msg_bis,size);
    char * msg = malloc(size * sizeof(char));
    strcpy(msg,size_msg);
    strcat(msg,msg_bis);
    protocol_data* dissection = dissectProtocol(msg,TCHATCHE_SERVER);
    printf("Pseudo : %s\n", get_connexion_username(dissection));
    printf("Pipe : %s\n", get_connexion_pipe(dissection));
    int r = creat_tube(get_connexion_pipe(dissection));
    int fd_c  = open(get_connexion_pipe(dissection),O_WRONLY); 
    int id = 9 ;
    char * msg_c = encodeConnexionConfirmation(id);
    write(fd_c,msg_c,strlen(msg));
    //freeProtocolData(dissection);
    
    sleep(3);
    
    
    
  }
  return  0 ;
}


/* gboolean r ; 
   GHashTable * hash_tab_pseudo_id =  g_hash_table_new (g_str_hash, g_str_equal);
   GHashTable * hash_tab_id_pipe =  g_hash_table_new (g_str_hash, g_str_equal); */
  


// protocol_data * dissection = dissectProtocol(msg);
//printf("pseudo %s \n",get_connexion_pseudo(dissection));
//printf("pipe %s \n",get_connexion_pipe(dissection));

/*
  char * pseudo ="bachir";
  char * id = "kira" ;
  
  r = insert_user(hash_tab,pseudo,id);
  if(r==FALSE){
  printf("false") ;
  exit(1); 
  }
  
  r = is_in_hashTab(hash_tab,pseudo);
  printf("is in hashtab = %d \n",r);

  r = remove_user(hash_tab,pseudo);
  printf("remove = %d \n",r);

  r = is_in_hashTab(hash_tab,pseudo);
  printf("is in hashtab = %d \n",r);


*/
