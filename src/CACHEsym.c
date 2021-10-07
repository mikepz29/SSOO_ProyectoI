#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define LIMITE 6


//Estructura de la cache
typedef struct T_LINEA_CACHE{
	short int etq;
	short int datos[8];
}T_LINEA_CACHE;


//Inicio las funciones que voy a utilizar en el programa
void inicioCache(T_LINEA_CACHE* T_LINEA_CACHE);
void aperturaFicheroRAM(T_LINEA_CACHE* T_LINEA_CACHE);
int lecturaFicheroMemorias(int direccion_binario);
void asignacion();


//Funcion que asigna la palbra, linea y etiqueta a cada direccion de memoria del fichero 'accesos_memoria.txt'
void asignacion(){	

	int direccion;
	int palabra, linea, etiqueta;
	int bloque;
	
	//Lo que ocupa cada mascara(campo)
	palabra = direccion & 0b111;
	linea = (direccion & 0b11)>>3;
	etiqueta = (direccion & 0b11111)>>5;
	
	bloque = etiqueta + linea;
}


//Funcion para inicializar la cache (ETQ y Datos iguales a FF y 0)
void inicioCache(T_LINEA_CACHE* T_LINEA_CACHE){
	
	int a, i;
	printf("Inicio de la cache:");
	sleep(2);
	for(a=4; a>0; a--){
		T_LINEA_CACHE[a].etq = 0XFF;
		printf("\nETQ: %X || Datos:", T_LINEA_CACHE[a].etq);
		
		for(i=8; i>0; i--){
			T_LINEA_CACHE[a].datos[i] = 0;
			printf(" %02X", T_LINEA_CACHE[a].datos[i]);		
		}
	}	
}


//Funcion que abre y lee el fichero 'RAM.bin'
void aperturaFicheroRAM(T_LINEA_CACHE* T_LINEA_CACHE){
	
	FILE *fichero_ram = fopen("RAM.bin", "r"); 		//Apertura del fichero para lectura
	printf("\n\nLeyendo el fichero 'RAM.bin'...\n");
	sleep(2);	
	
	if (fichero_ram == NULL) { 						//Comprobacion del fichero abierto 
		printf( "\n\tNo se puede abrir el fichero 'RAM.bin'. Saliendo...\n" );
		exit( -1 );
	}
	
	unsigned char ram[1024]; 						//Declaro el array en el que guardaré el contenido del fichero RAM.bin
	ram[1024] = getc(fichero_ram);	
	//printf("Contenido del fichero:\n");
	while (feof(fichero_ram) == 0) { 				//Comprobación de fin de fichero. Si esta en 0 (no ha acabado), sigue leyendo 
		//printf( "%c", ram[1024]);			    	//Imprime el contenido del fichero
		ram[1024] = getc(fichero_ram); 			    //ram = contenido del fichero
	}	
	
	if (fclose(fichero_ram)!= 0){ 					//cierre de fichero
		printf( "\n\n\tProblemas al cerrar el fichero 'RAM.bin'. Saliendo...\n" );
		exit( -1 );
	}
	else{
		printf("\tFichero 'RAM.bin' leido correctamente.\n");
		sleep(2);
	}	
}


//Funcion que lee el fichero 'accesos_memira.txt' 
int lecturaFicheroMemorias(int direccion_binario){
	
	FILE *fichero_direcciones = fopen("accesos_memoria.txt", "r");
	char direccion_memoria[4];
	while(fgets(direccion_memoria, 6, (FILE*) fichero_direcciones)){
		//printf("%s --> Binario: ", direccion_memoria);
		direccion_binario = strtol(direccion_memoria, NULL, 16);
		//printf ("%d\n", direccion_binario);
	}
}



int main(int argc, char* argv[]){
	
	//IINICIALIZACION DE VARIABLES
	T_LINEA_CACHE T_LINEA_CACHE[4];
	int tiempo_global = 0, num_fallos = 0;
	int direccion;
	int palabra, linea, etiqueta;
	int bloque;
	int direccion_binario;
	int dato;
	
	
	//FUNCION DE INICIO DE LA CACHE
	inicioCache(T_LINEA_CACHE);
	

	//FUNCION QUE ABRE Y LEE EL FICHERO 'RAM.bin'
	aperturaFicheroRAM(T_LINEA_CACHE);
	
	
	//APERURA DEL FICHERO 'accesos_memoria.txt'
	FILE *fichero_direcciones = fopen("accesos_memoria.txt", "r");  //Apertura del fichero para lectura
	printf( "\n\nLeyendo el fichero 'accesos_memoria.txt'...\n" );
	sleep(2);
	
	if (fichero_direcciones == NULL) { 								//Comprobacion del fichero abierto 
		printf( "\n\tNo se puede abrir el fichero 'accesos_memoria.txt'. Saliendo...\n" );
		exit( -1 );
	}
	
	//Para leer el fichero linea a linea y que pase las direcciones a binario
	lecturaFicheroMemorias(direccion_binario);
	
	if (fclose(fichero_direcciones)!= 0){ 							 //cierre de fichero
		printf("\n\n\tProblemas al cerrar el fichero 'accesos_memoria.txt'. Saliendo...\n");
		exit( -1 );
	}
	else{
		printf("\tFichero 'accesos_memoria.txt' leido correctamente.\n");
		sleep(2);
	}


	direccion = lecturaFicheroMemorias(direccion_binario); //Para que la direccion sean las lineas que se leen en el fichero 'accesos_memoria.txt'


	//FUNCION que asigna la palabrra, linea y etiqueta
	asignacion();
	
	
	//MENU CUANDO HAY FALLO o ACIERTO
	printf("\n\nComparando etiqueta de la cache y etiqueta de la direccion de memoria...\n");
	sleep(2);
	
	if(T_LINEA_CACHE[0].etq == etiqueta){
		printf("\tT: %d | Acierto de CACHE | ADDR %04X | ETQ %X | Linea %02X | Palabra %02X | DATO %02X\n", tiempo_global, direccion, etiqueta, linea, palabra, dato);
	}
	else{
		tiempo_global =+ 10;
		num_fallos++;
		printf("\tT: %d | Fallo de CACHE %d | ADDR %04X | ETQ %X | Linea %02X | Palabra %02X | bloque %02X\n", tiempo_global, num_fallos, direccion, etiqueta, linea, palabra, bloque);
	}
		
	return 0;
		
}

