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
#include "../protocol/protocol_instance.h"

#define OCTET_4 4 
#define OCTET_8 8 


void _protocol_hello(){


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
  
  gboolean r ; 
  GHashTable * hash_tab_pseudo_id =  g_hash_table_new (g_str_hash, g_str_equal);
  GHashTable * hash_tab_id_pipe =  g_hash_table_new (g_str_hash, g_str_equal);
  
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
  char * msg ;
  char size_msg [5] ;
  
  
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
    printf("lus = %s \n",msg);
    
    sleep(1);
    
    protocol_data * dissection = dissectProtocol(msg);
    printf("pseudo %s \n",get_connexion_pseudo(dissection));
    printf("pipe %s \n",get_connexion_pipe(dissection));
    
  }
  return  0 ;
}


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
