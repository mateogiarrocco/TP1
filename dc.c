#define _GNU_SOURCE
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strutil.h"
#include <math.h>

#define SUMA "+"
#define RESTA "-"
#define MUL "*"
#define DIV "/"
#define POT "^"
#define RAIZ "sqrt"
#define LOG "log"
#define TERNARIO "?"

//2 1 log sqrt imprime dos veces ERROR

bool leer_linea(FILE* archivo, char** linea, size_t *capacidad){
	if(!linea) return false;
	size_t leidos = getline(linea, capacidad, archivo);
	if(leidos == -1) return false;
	return true;
}

long int* ternario(char* op, long int a, long int b, long int c){
	long int* result = malloc(sizeof(long int));
  	if(!result)	return NULL;
	if(strcmp(op, TERNARIO) == 0) *result = a ? b : c;
	else{
		fprintf(stdout, "ERROR\n");
		free(result);
		return NULL;
	}
	return result;
}

long int* raiz(char* op, int a){
	long int* result = malloc(sizeof(long int));
	if(!result) return NULL;
	if (a < 0){
		fprintf(stdout, "ERROR\n");
		free(result);
		return NULL;
	}
	if(strcmp(op, RAIZ) == 0){
		double aux = sqrt(a);
		*result = (long int)aux;
	}
	else{
		fprintf(stdout, "ERROR\n");
		free(result);
		return NULL;
	}
	return result;
}

long int logaritmo(long int base, long int exponente){
    if(base == 1) return 0;
	return (long int)(log((double)exponente) / log((double)base));
}

long int* operar_dos_argumentos(char* op, long int a, long int b) { //alternativo: que sea bool y pasar el result por parametro
    long int* result = malloc(sizeof(long int));
  	if(!result)	return NULL;
    if (strcmp(op,SUMA) == 0) *result = (a + b);
    else if (strcmp(op,RESTA) == 0) *result = b - a;
    else if (strcmp(op,MUL) == 0) *result = a * b;
    else if (strcmp(op,DIV) == 0){
    	if(a == 0){
    		fprintf(stdout, "ERROR\n");
    		free(result);
    		return NULL;
    	}
    	*result = b / a;
    }
    else if(strcmp(op,POT) == 0){
    	if(b < 0){
    		fprintf(stdout, "ERROR\n");
    		free(result);
    		return NULL;
    	}
    	double aux = pow((double)a, (double)b);
    	*result = (long)aux;
    }
    else if(strcmp(op,LOG) == 0){
    	if((a < 2) || (b < 1)){
			fprintf(stdout, "ERROR\n");
    		free(result);
  			return NULL;
		}
    	*result = logaritmo(a,b);
	}
    else{
    	fprintf(stdout, "ERROR\n");
    	free(result);
    	return NULL;
    }
    return result;
}

long int* convertir_a_numero(char* cadena, char* resto){
	long int* numero = (long int*) malloc(sizeof(long int));
	if(!numero){
		return NULL;
	}
	*numero = strtol(cadena, &resto, 10);
	return numero;
}

bool operar_con_mas_de_un_operando(long int* numero, long int* primer_num, long int* segundo_num, long int* tercer_num, int* desapilados,
	bool* esta_en_heap, int cant_argumentos, long int** resultado, char** cadena_dinamica, int i, bool resultado_valido, pila_t* pila){

	if(*desapilados == 0) *primer_num = *numero;
	else if(*desapilados == 1) *segundo_num = *numero;
	else if(*desapilados == 2) *tercer_num = *numero;
	if(*esta_en_heap){
		free(numero);
	}
	if(*desapilados < 0) *desapilados = 0; 
	if((*desapilados +1) == cant_argumentos){
		if(cant_argumentos == 2) *resultado = operar_dos_argumentos(cadena_dinamica[i], *primer_num, *segundo_num);
		if(cant_argumentos == 3) *resultado = ternario(cadena_dinamica[i], *tercer_num, *segundo_num, *primer_num);
		if(!*resultado){
			resultado_valido = false;
			return false;
		}
		if(cadena_dinamica[i+1]){
			pila_apilar(pila, *resultado);
			*esta_en_heap = false;
		}
	}
	(*desapilados) ++;
	return true;
}

void dc(){
	char* linea = NULL;
	size_t capacidad = 0;
	long int* resultado = 0;
	int cant_argumentos;
	char* resto = NULL;
	long int primer_num = ' ';
	long int segundo_num = ' ';
	long int tercer_num = ' ';

	while (leer_linea(stdin, &linea, &capacidad)){
		bool resultado_valido = true;
		int desapilados = 0;
		int cant_operadores = 0;
		if((linea[0] == '\n') || (!linea)){
			fprintf(stdout, "ERROR\n");
			break;
		}
		linea[strlen(linea)-1] = '\0';
		pila_t* pila = pila_crear();
		if(!pila) return;
		char** cadena_dinamica = split(linea,' ');
		if(!cadena_dinamica){
			fprintf(stdout, "ERROR\n");
			break;
		}
		else{
			for(int i = 0; cadena_dinamica[i]; i ++){
				if(strcmp(cadena_dinamica[i], "") == 0){
					continue;
				}
				long int* numero = convertir_a_numero(cadena_dinamica[i], resto);
				bool esta_en_heap = true;
				if ((strcmp(cadena_dinamica[i],"0") == 0) || (*numero != 0)){
					if(strcmp(cadena_dinamica[i],"0") == 0) *numero = atoi("0");
					pila_apilar(pila, numero);
					esta_en_heap = true;
				}
				else {
					free(numero);
					if(strcmp(cadena_dinamica[i], RAIZ) == 0) cant_argumentos = 1;
					else if(strcmp(cadena_dinamica[i], TERNARIO) == 0) cant_argumentos = 3;
					else cant_argumentos = 2;
					cant_operadores ++;
					for(int j = 0; j < cant_argumentos; j ++){
						if(pila_esta_vacia(pila)){
							fprintf(stdout, "ERROR\n");
							resultado_valido = false;
							break;
						}
						numero = (long int*)pila_desapilar(pila);
						if(cant_argumentos == 1){
							resultado = raiz(cadena_dinamica[i], *(int*)numero);
							if(!resultado) {
								resultado_valido = false;
								free(numero);
								break;
							}
							if(cadena_dinamica[i+1]){
								pila_apilar(pila, resultado);
								esta_en_heap = false;
							}
							free(numero);
						}
						else{
							resultado_valido = operar_con_mas_de_un_operando(numero, &primer_num, &segundo_num, &tercer_num, &desapilados,
								&esta_en_heap, cant_argumentos, &resultado, cadena_dinamica, i, resultado_valido, pila);
						}
					}
					int factor_comparacion;
					if(cant_argumentos == 3) factor_comparacion = 2;
					else factor_comparacion = 1;
					if(cant_operadores + factor_comparacion < cant_argumentos){
						fprintf(stdout, "ERROR\n");
						free(resultado);
						resultado_valido = false;
						break;
					}
					desapilados = 0;
				}
			}
		}
		if(resultado_valido){
			if(pila_esta_vacia(pila)){
				fprintf(stdout, "%ld\n", *resultado);
				free(resultado);
			}
			else{
				fprintf(stdout, "ERROR\n");
				while(!pila_esta_vacia(pila)){
					int* a_borrar = (int*)pila_desapilar(pila);
					free(a_borrar);
				}
			}
		}
		else{
			while(!pila_esta_vacia(pila)){
				int* a_borrar2 = (int*)pila_desapilar(pila);
				free(a_borrar2);
			}
		}
		free_strv(cadena_dinamica);
		pila_destruir(pila);
	}
	free(linea);
}

int main(){
	dc();
	return 0;
}