#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "hashmap.h"
#define MAXCHAR100 101

//================================================
// Estructura utilizada para el programa
//================================================

typedef struct {
  char nombre[MAXCHAR100];  	
  char tipo[MAXCHAR100];
  char direccion[MAXCHAR100];
  char horario[MAXCHAR100];
  char descripcion[MAXCHAR100];
} puntosInteres;

typedef struct {
  char pasaporte[MAXCHAR100];
	char nombre[MAXCHAR100];
	char pais[MAXCHAR100];
  HashMap *mapaLugaresFav;
} turistas;

typedef struct {
  turistas datos[MAXCHAR100]; 
  int numTuristas; 
} ListaTuristas;

//================================================
// Función para extraer un campo específico de una línea de texto en formato CSV 
//================================================
const char *get_csv_field (char * tmp, int k) {
  int open_mark = 0;
  char* ret=(char*) malloc (100*sizeof(char));
  int ini_i=0, i=0;
  int j=0;
  while(tmp[i+1]!='\0'){

    if(tmp[i]== '\"'){
      open_mark = 1-open_mark;
      if(open_mark) ini_i = i+1;
        i++;
        continue;
      }

    if(open_mark || tmp[i]!= ','){
      if(k==j) ret[i-ini_i] = tmp[i];
        i++;
        continue;
      }

      if(tmp[i]== ','){
        if(k==j) {
          ret[i-ini_i] = 0;
          return ret;
        }
        j++; ini_i = i+1;
      }
      i++;
    }
    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }
    return NULL;
}
//================================================
// Función de lectura de carácteres. Ocupamos esta función para evitar saltos de línea al leer en nuestro programa.
//================================================
void leer100(char cadena[MAXCHAR100]){
  scanf("%100[^\n]s",cadena);
  int largo=strlen(cadena);
  if(largo>100){
    printf("No se ha podido registrar");
  }
  getchar();
}
//================================================
// El usuario ingresa los datos del lugar y la aplicación los almacena. (1)
//================================================
void registrarPtoInt(HashMap *map){
  getchar();
  char nombre[MAXCHAR100];
  printf("Ingrese nombre del lugar: ");
  leer100(nombre);

  puntosInteres *nuevoPunto = (puntosInteres *)malloc(sizeof(puntosInteres));

  if(nuevoPunto==NULL){
    printf("No se pudo asignar memoria para el nuevo punto de interés");
    return;
  }
  
  strcpy(nuevoPunto->nombre, nombre);
  
  printf("Ingrese tipo: ");
  leer100(nuevoPunto -> tipo);
  
  printf("Ingrese la dirección: ");
  leer100(nuevoPunto -> direccion);
  
  printf("Ingrese horario: ");
  leer100(nuevoPunto -> horario);

  printf("Ingrese descripcion: ");
  leer100(nuevoPunto -> descripcion);

  insertMap(map, nuevoPunto->nombre, nuevoPunto);
  printf("Punto de interés registrado con éxito!\n");
}

//================================================
// El usuario ingresa un nombre, se busca en el mapa y se muestran los datos del punto de interés. (2)
//================================================
void mostrarDatosPtoInt(HashMap *map){
  getchar();
  char nombreBuscado[MAXCHAR100];
  
  printf("Ingrese el nombre del lugar: ");
  leer100(nombreBuscado);

  Pair *result = searchMap(map, nombreBuscado);

  if (result != NULL) 
  {
    puntosInteres *punto = (puntosInteres*)result -> value;
    printf("Nombre del lugar: %s\n", punto -> nombre);
    printf("Tipo: %s\n", punto -> tipo);
    printf("Dirección: %s\n", punto -> direccion);
    printf("Horario: %s\n", punto -> horario);
    printf("Descripción: %s\n", punto -> descripcion);
  } 
  else
  {
    printf("No se encontró un punto de interés con el nombre \"%s\".\n", nombreBuscado);
  }
}

//================================================
// El usuario elimina un punto de interés en el mapa con el nombre del lugar como clave. (3)
//================================================
void eliminarPtoInt(HashMap *map,puntosInteres *punto){
  getchar();
  char nombreBuscado[MAXCHAR100];
  printf("Ingrese el nombre del lugar: ");
  leer100(nombreBuscado);
  
  Pair *result = searchMap(map, nombreBuscado);

  if(result !=NULL){
    eraseMap(map,nombreBuscado);
    printf("¡Se ha eliminado el punto de interés!\n");
  }
  else{
    printf("El nombre del lugar no se encontró en el mapa\n");
  }
  return;
}

//================================================
// El usuario registra un nuevo turista para el mapa. (4)
//================================================
void registrarTurista(HashMap *map){
  getchar();
  char pasaporte[MAXCHAR100];
  printf("Ingrese el pasaporte del turista: ");
  leer100(pasaporte);
  
  turistas *nuevaInfo = (turistas *)malloc(sizeof(turistas));

  if(nuevaInfo==NULL){
    printf("No se pudo asignar memoria para el nuevo turista");
    return;
  }

  strcpy(nuevaInfo->pasaporte, pasaporte);

  printf("Ingrese el nombre del turista: ");
  leer100(nuevaInfo->nombre);

  printf("Ingrese país del turista: ");
  leer100(nuevaInfo->pais);

  insertMap(map, nuevaInfo->pasaporte, nuevaInfo);
  printf("\nInformación de turista registrado con éxito!\n");
}

//================================================
// El usuario agrega el lugar favorito a turista con la clave pasaporte (5)
//================================================

void agregarLugarFavTurista(HashMap *map){
  getchar();
  char pasaporte[MAXCHAR100];
  char nombreLugarFav[MAXCHAR100];

  printf("Ingrese el pasaporte del turista: ");
  leer100(pasaporte);

  Pair *result = searchMap(map, pasaporte);

  if(result!=NULL){
    turistas *turista=(turistas *)result->value;
    if(turista->mapaLugaresFav==NULL){
      turista->mapaLugaresFav=createMap(100);  
    }

    printf("Ingrese el nombre del lugar favorito para turista: ");
    leer100(nombreLugarFav);
    insertMap(turista->mapaLugaresFav,pasaporte,nombreLugarFav);
  }
  else{
    printf("No se encontró turista con el pasaporte indicado\n");
    return;
  }
  printf("Lugar favorito registrado con éxito!\n");
}

//================================================
// El usuario decide ver los turistas registrados por país (6)
//================================================
void mostrarTuristasPorPais(HashMap *map){
  getchar();
  char paisIndicado[MAXCHAR100];
  printf("Ingrese un país para ver sus turistas: ");
  leer100(paisIndicado);
  int encontrado = 0;
  
  printf("\nTuristas de %s: \n\n",paisIndicado);
  //printf("===============================\n");

  Pair *pair=firstMap(map);
  while(pair!=NULL){
    turistas *turista=(turistas *)pair->value;
    printf("--------------------------------------\n");

    if(strcmp(turista->pais,paisIndicado)==0){
      printf("Pasaporte: %s \n",turista->pasaporte);
      printf("Nombre turista: %s \n",turista->nombre);
      printf("País: %s \n",turista->pais);
      }
      encontrado ++;
    }
    printf("--------------------------------------\n\n");
    pair=nextMap(map);

  if(encontrado == 0)
  {
    printf("No se encontraron turistas para el país indicado\n\n");
  }
  return;
}

//================================================
// El usuario decide ver los puntos de interés de un tipo (7)
//================================================
void mostrarPtoInteresPorTipo(HashMap *map){
  getchar();
  char tipoIndicado[MAXCHAR100];
  printf("Ingrese un tipo para ver los punto de interés relacionados: ");
  leer100(tipoIndicado);
  int encontrado = 0;

  printf("Puntos de interés de tipo %s:\n",tipoIndicado);

  Pair *pair = firstMap(map);
  while(pair != NULL){
    puntosInteres *punto=(puntosInteres *)pair->value;
    printf("--------------------------------------\n");
    if(strcmp(punto -> tipo, tipoIndicado)==0){
      printf("Nombre: %s\n",punto->nombre);
      printf("Tipo: %s\n",punto->tipo);
      printf("Direccion: %s\n",punto->direccion);
      printf("Horario: %s\n",punto->horario);
      printf("Descripción: %s\n",punto->descripcion);
      encontrado++;
    }
    pair=nextMap(map);
  }
  if(encontrado==0){
    printf("No se encontraron puntos de interés para el tipo indicado\n\n");
  }
  return;
}

//================================================
// El usuario decide IMPORTAR información desde un archivo CSV. Se crean 2 opciones: para puntos de interés y para turistas. (8, 1/2)
//================================================

void importarPuntosIntCSV(char * nombreArchivo, void *datos){
  FILE *archivo = fopen(nombreArchivo, "r");

  if (archivo == NULL)
  {
    printf("\nNo se pudo abrir el archivo\n");
    return;
  }

  char linea[MAXCHAR100 * 5]; //5 aspectos en cada archivo

  while (fgets(linea, sizeof(linea), archivo) != NULL)
  {
    puntosInteres *puntos = (puntosInteres*) malloc(sizeof(puntosInteres));
    if (puntos == NULL)
    {
      printf("Error. El archivo no se pudo leer.");
      return;
    }
    sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^\n]",
    puntos->nombre,
    puntos->tipo,
    puntos->direccion,
    puntos->horario,
    puntos->descripcion
    );
    
  }
  fclose(archivo);
  printf("¡Se ha importado los puntos de interes del archivo CSV!\n");
  
}

void importarTuristasCSV(char * nombreArchivo, HashMap *mapaTuristas)
{
  FILE *archivo = fopen(nombreArchivo, "r");
  
  if (archivo == NULL)
  {
    printf("\nNo se pudo abrir el archivo\n");
    return;
  }
  
  int numTuristas = 0;
  char linea[MAXCHAR100 * 4]; //5 aspectos en cada archivo

  while (fgets(linea, sizeof(linea), archivo) != NULL)
  {
    turistas *nuevoTurista = (turistas *)malloc(sizeof(turistas));

    if (nuevoTurista == NULL)
    {
      printf("Error. El archivo no se pudo leer.");
      return;
    }

    sscanf(linea, "%[^,],%[^,],%[^\n]",
    nuevoTurista->pasaporte,
    nuevoTurista->nombre,
    nuevoTurista->pais);

    nuevoTurista -> mapaLugaresFav = createMap(MAXCHAR100);
    
    insertMap(mapaTuristas, nuevoTurista->pasaporte, nuevoTurista);
    
  }

  fclose(archivo);
  printf("¡Se han importado los turistas del archivo CSV!\n");
}
//================================================
// El usuario decide EXPORTAR información desde un archivo CSV. Se crean 2 opciones: para puntos de interés y para turistas. (9, 1/2)
//================================================

void exportarPuntosIntCSV(char *nombreArchivo, void *datos){
  FILE *archivo = fopen(nombreArchivo, "w");
  if (archivo == NULL) 
  {
    printf("No se pudo abrir el archivo CSV para escritura.\n");
    return;
  }

  puntosInteres *puntos = (puntosInteres *)datos;

  for (int i = 0; i < 5; i++) 
  {
    fprintf(archivo, "%s,%s,%s,%s,%s\n", puntos[i].nombre, puntos[i].tipo, puntos[i].direccion, puntos[i].horario, puntos[i].descripcion);
  }


  fclose(archivo);

  printf("La información de los puntos de interés se han exportado correctamente!.\n\n");
}

void exportarTuristasCSV(char* nombreArchivo, ListaTuristas *listaTuristas)
{
  FILE *archivo = fopen(nombreArchivo, "w");
  if (archivo == NULL) 
  {
    printf("No se pudo abrir el archivo CSV para escritura.\n");
    return;
  }

  for (int i = 0; i < listaTuristas->numTuristas; i++) {
    fprintf(archivo, "%s,%s,%s\n", listaTuristas->datos[i].pasaporte, listaTuristas->datos[i].nombre, listaTuristas->datos[i].pais);
    
  }


  fclose(archivo);

  printf("La información de los turistas se han exportado correctamente!.\n\n");
}

//================================================
// La función sirve como mini menú para elegir que tipo de archivo CSV vamos a importar: puntos o turistas.
//================================================
int menuImportar()
{
  int opcion;
  printf("¿Qué desea importar?\n");
  printf("1.- Puntos de Interés\n");
  printf("2.- Turistas\n");
  printf("Ingrese opción: ");
  scanf("%d", &opcion);
  if (opcion == 1 || opcion == 2)
  {
    return opcion;
  }
  else
  {
    printf("Opción inválida.\n");
    return 0;
  }
}

//================================================
// La función sirve como un mini menú para elegir que tipo de archivo CSV vamos a exportar: puntos o turistas.
//================================================
int menuExportar()
{
  int opcion;
  printf("¿Qué desea exportar?\n");
  printf("1.- Puntos de Interés\n");
  printf("2.- Turistas\n");
  printf("Ingrese opción: ");
  scanf("%d", &opcion);
  if (opcion == 1 || opcion == 2)
  {
    return opcion;
  }
  else
  {
    printf("Opción inválida.\n");
    return 0;
  }
}
//================================================
// Ocupamos la función imprimir Menu para tener nuestro programa más ordenado en el main.
//================================================

void imprimirMenu() {
  printf("==================================================\n");
  printf("              ¡Información Turística!                  \n");
  printf("==================================================\n\n");
  printf("1. Registrar punto de interés\n");
  printf("2. Mostrar datos del punto de interés\n");
  printf("3. Eliminar punto de interés\n");
  printf("4. Registrar turista\n");
  printf("5. Agregar lugar favorito a turista\n");
  printf("6. Mostrar turistas por país\n");
  printf("7. Mostrar todos los puntos de interés de un tipo\n");
  printf("8. Importar puntos de interés y turistas desde archivo CSV\n");
  printf("9. Exportar puntos de interés y turistas a archivo CSV\n");
  printf("0. Exit\n\n");
  printf("Ingrese un dígito: ");
}

//================================================
// En nuestro main se crean los mapas, y se crea el menú para cada opción del programa.
//================================================

int main(){
  HashMap *mapaPuntosInteres = createMap(100);
  HashMap *mapaTuristas = createMap(100);

  ListaTuristas listaTuristas;
  listaTuristas.numTuristas = 0;
  
  int digito;
  puntosInteres punto;
  turistas info;
  int opcion;
  
  while(true){
    imprimirMenu();
    scanf("%d", &digito);
    if (digito == 0)
    {
      printf("\n¡Has finalizado tu sesión!\n");
      return 0;
    }
    else if(digito == 1)
    {
      registrarPtoInt(mapaPuntosInteres);
    }
    else if(digito == 2)
    {
      mostrarDatosPtoInt(mapaPuntosInteres);
    }
    
    else if(digito == 3)
    {
      eliminarPtoInt(mapaPuntosInteres,&punto);
    }
  
    else if(digito == 4)
    {
      registrarTurista(mapaTuristas);
    }

    else if(digito == 5)
    {
      agregarLugarFavTurista(mapaTuristas);
    }
    
    else if(digito == 6)
    {
      mostrarTuristasPorPais(mapaTuristas); //cualquier orden
    }
    else if(digito == 7)
    {
      mostrarPtoInteresPorTipo(mapaPuntosInteres);
    }
      
    else if(digito == 8)
    {
      char nombreArchivo[MAXCHAR100];
      int opcion = menuImportar();
      
      if (opcion == 1)
      {
        getchar();
        printf("Ingrese nombre del archivo de puntos de interés: ");
        leer100(nombreArchivo);
        importarPuntosIntCSV(nombreArchivo, mapaPuntosInteres);
      }
      else if (opcion == 2)
      {
        getchar();
        printf("Ingrese nombre del archivo de turistas: ");
        leer100(nombreArchivo);
        importarTuristasCSV(nombreArchivo, mapaTuristas);
      }

    }

    else if(digito == 9)
    {
      char nombreArchivo[MAXCHAR100];
      int opcion = menuExportar();

      if (opcion == 1)
      {
        getchar();
        printf("Ingrese nombre del archivo de puntos de interés: ");
        leer100(nombreArchivo);
        exportarPuntosIntCSV(nombreArchivo , mapaPuntosInteres);
      }
      else if (opcion == 2)
      {
        getchar();
        printf("Ingrese nombre del archivo de turistas: ");
        leer100(nombreArchivo);
        exportarTuristasCSV(nombreArchivo, &listaTuristas);
      }
      
    }

    else
    {
      printf("\nOpción inválida. Ingrese opción nuevamente\n");
    }
    
  }

  
}
