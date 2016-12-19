#include"function.h" 
/*la cle -> c'est le pseudo
  la valeur -> c'est l'identifiant
  gboolean -> (TRUE,FALSE)
  TRUE -> !FALSE
  FALSE -> 0 
*/

gboolean insert_pseudo_pipe(GHashTable * hash_tab, char * pseudo , char * pipe){
  
  char * key = strdup(pseudo);
  char * val = strdup(pipe);
  if(key == NULL || val == NULL){
    return FALSE ; 
  }
  gboolean r = g_hash_table_insert (hash_tab,key,val) ;
  return r ; 
   
}

gboolean insert_pseudo_id(GHashTable * hash_tab, int * id ,char * pseudo  ){
  
  int * key = malloc(sizeof(int));
	*key  = *id ;	
  if(key == NULL ){
    return FALSE ; 
  }
  gboolean r = g_hash_table_insert (hash_tab,key,pseudo) ;
  return r ; 
   
}


gboolean is_in_hashTab(GHashTable * hash_tab ,char const * key){
  gboolean r ; 
  r =  g_hash_table_contains (hash_tab,key);
  return r ; 
}

gboolean remove_user(GHashTable * hash_tab ,char const * key){

  gboolean r ; 
  r = g_hash_table_remove (hash_tab, key);
  return r ; 
}
