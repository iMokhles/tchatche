#ifndef	_FUNCTION_H
#define	_FUNCTION_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <glib.h>


gboolean insert_pseudo_pipe(GHashTable * hash_tab, char * pseudo , char * pipe);
gboolean insert_pseudo_id(GHashTable * hash_tab, int * id ,char * pseudo  ) ; 
gboolean remove_user(GHashTable * hash_tab ,char const * key) ; 
gboolean is_in_hashTab(GHashTable * hash_tab ,char const * key) ;





#endif /*_FUNCTION_H*/
