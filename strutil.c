#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
#define _GNU_SOURCE 

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

char *substr(const char *str, size_t n){
    char* cadena = (char*) malloc((n+1) * sizeof(char));
    if(!cadena) return NULL;
    size_t i = 0;
    size_t largo = strlen(str);
   	while((i < n) && (i < largo)){
   		if(str[i] != '\0') cadena[i] = str[i];
   		i ++;
   	}
   	if(i < n) cadena[strlen(str)] = '\0';
    else if(i <= strlen(str)) cadena[n] = '\0';
    return cadena;
}

int contar_separador(const char* str, char sep){
	int i = 0;
	int cant_sep = 0;
	while(str[i] != '\0'){
		if(str[i] == sep){
			cant_sep ++;
		}
		i++;
	}
	return cant_sep;
}

char **split(const char *str, char sep){
	int pos = 0;
	char* dup;
	int valor_inicial_a = 0;                                                  
	int valor_inicial_b = 0;                                                
	int* desde = &valor_inicial_a;                                           
	int* hasta = &valor_inicial_b;                                           

	int cant_sep = contar_separador(str, sep);
	char** str_dinamico = (char**) malloc((cant_sep + 2)* sizeof(char*));
	if(!str_dinamico) return NULL;
	for (int i=0; i <= strlen(str); i ++){
		if((str[i] == sep) || (str[i] == '\0')){
			dup = strndup(str + (*desde), (*hasta) - (*desde));
			if(!dup){
				free(str_dinamico);
				return NULL;
			}
			(*desde) = (*hasta) + 1;
			str_dinamico[pos] = strdup(dup);
			free(dup);
			if(!str_dinamico[pos]){
				free(str_dinamico);
				return NULL;
			}
			pos ++;
		}
		(*hasta) ++;
	}
	str_dinamico[pos] = NULL;
	return str_dinamico;
}

char* join(char **strv, char sep){
    int cont = 0;
    int cant_sep = 0;
    int a_restar = 0;
    size_t largo = 0;
    size_t pos = 0;

    if (strv){ 
        for (cont = 0; strv[cont]; cont++) largo += strlen(strv[cont]);
    }
  	else {
  		char* resultante = (char*) malloc(sizeof(char));
  		resultante[0] = '\0';
  		return resultante;
  	}
    if (cont > 1) 	cant_sep = cont-1;
    if (sep == '\0') a_restar = cant_sep;
    char* resultante = (char*) malloc(sizeof(char)*(largo + cant_sep + 1 - a_restar));
    if (!resultante) return NULL;
    for (size_t i=0; strv[i]; i++) {
        for (size_t j=0; strv[i][j] != '\0'; j++) {
            resultante[pos] = strv[i][j];
            pos ++;
        }
        if ((cant_sep > 0) && (sep != '\0')) {
            resultante[pos] = sep;
            pos ++;
            cant_sep --;
        }
    }
    resultante[pos] = '\0';
    return resultante;
}

void free_strv(char *strv[]){
	for(int i = 0; strv[i]; i++) free(strv[i]);
	free(strv);
}
