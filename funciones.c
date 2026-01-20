#include "funciones.h"
#include <stdio.h>
#include <string.h>

#include <ctype.h>

// FUNCION SALTO DE LINEA
void SaltodeLinea(char *cadena, int n) {
    fgets(cadena, n, stdin);
    int len = strlen(cadena) - 1;
    if(cadena[len] == '\n') {
        cadena[len] = '\0';
    }
}

// VERIFICAR SI STRING ES VACIO O SOLO ESPACIOS
int esStringVacio(const char* str) {
    int i = 0;
    while(str[i] != '\0') {
        if(!isspace(str[i])) { //estudiar
            return 0;
        }
        i++;
    }
    return 1;
}

// VALIDAR DIA DE LA SEMANA
int validarDiaSemana(const char* prompt, char* dia) {
    const char* dias_validos[] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"};
    char dia_ingresado[20];
    int i, j, valido = 0;
    
    do {
        printf("%s (lunes-domingo): ", prompt);
        SaltodeLinea(dia_ingresado, 20);
        
        // Convertir a minusculas
        for(j = 0; dia_ingresado[j]; j++) {
            dia_ingresado[j] = tolower(dia_ingresado[j]);
        }
        
        // Buscar dia valido
        for(i = 0; i < 7; i++) {
            if(strcmp(dia_ingresado, dias_validos[i]) == 0) {
                strcpy(dia, dias_validos[i]);
                valido = 1;
                return i;  // Retorna posicion del dia (0=lunes, 6=domingo)
            }
        }
        
        if(!valido) {
            printf("Error: Dia invalido. Ingrese un dia de la semana valido.\n");
        }
    } while(!valido);
    
    return -1;
}

// OBTENER SIGUIENTE DIA DE LA SEMANA
char* obtenerSiguienteDia(int dia_actual) {
    const char* dias[] = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo"};
    int siguiente = (dia_actual + 1) % 7;
    static char resultado[20];
    strcpy(resultado, dias[siguiente]);
    return resultado;
}

// FUNCION MENU PRINCIPAL
int menu() {
    int opciones;
    printf("\n========================================================================\n");
    printf("   SISTEMA INTEGRAL DE GESTION Y PREDICCION DE CONTAMINACION DEL AIRE\n");
    printf("========================================================================\n");
    printf("1)  Gestionar zonas (Agregar/Modificar/Eliminar)\n");
    printf("2)  Ingresar datos de contaminacion actual\n");
    printf("3)  Monitorear niveles de contaminacion actual\n");
    printf("4)  Calcular promedios historicos (30 dias)\n"); //cambiemos esto- a que?, pongamoos Calcular Registros en los ultimos 30 dias, para no poner "historicos"
    printf("5)  Predecir niveles futuros (24 horas)\n");  //mueve vos verga xdddddddddddd
    printf("6)  Generar alertas preventivas\n");
    printf("7)  Generar recomendaciones de mitigacion\n");
    printf("8)  Comparar niveles entre zonas\n");
    printf("9)  Ver historial completo de registros\n");
    printf("10) Exportar reporte completo\n");
    printf("11) Salir\n");
    printf("========================================================================\n");
    printf("Opcion: ");
    scanf("%d", &opciones);
    fflush(stdin);
    return opciones;
}

// VALIDAR FLOAT CON OPCION DE OMITIR - SOLO NUMEROS
float validarFloat(const char* prompt, float min, float max) {
    float valor;
    int valido;
    char linea[50];
    int i, tiene_punto = 0;
    
    do {
        printf("%s (o presione 0 para omitir): ", prompt);
        if(fgets(linea, sizeof(linea), stdin) != NULL) {
            valido = 1;
            
            // Verificar que solo contenga digitos y un punto decimal
            for(i = 0; linea[i] != '\0' && linea[i] != '\n'; i++) {
                if(linea[i] == '.') {
                    if(tiene_punto) {
                        valido = 0;  // Mas de un punto
                        break;
                    }
                    tiene_punto = 1;
                } else if(linea[i] == '-' && i == 0) {
                    // Permitir signo negativo al inicio
                    continue;
                } else if(!isdigit(linea[i])) {
                    valido = 0;  // Caracter no numerico
                    break;
                }
            }
            
            if(valido && sscanf(linea, "%f", &valor) == 1) { //sisas
                if(valor < min || valor > max) {
                    printf("Error: El valor debe estar entre %.2f y %.2f.\n", min, max);
                    valido = 0;
                }
            } else {
                if(valido) {
                    printf("Error: Debe ingresar solo numeros. No se aceptan caracteres especiales ni espacios.\n");
                }
                valido = 0;
            }
            tiene_punto = 0;  // Resetear para siguiente iteracion
        } else {
            valido = 0;
        }
    } while(!valido);
    return valor;
}

// VALIDAR ENTERO
int validarEntero(const char* prompt, int min, int max) { //estudiar
    int valor;
    int valido;
    do {
        printf("%s", prompt);
        valido = (scanf("%d", &valor) == 1);
        fflush(stdin);
        if(!valido) {
            printf("Error: Debe ingresar un numero entero valido.\n");
        } else if(valor < min || valor > max) {
            printf("Error: El valor debe estar entre %d y %d.\n", min, max);
            valido = 0;
        }
    } while(!valido);
    return valor;
}

// VALIDAR TEXTO SOLO LETRAS
void validarTextoSoloLetras(const char* prompt, char* destino, int max_len) {  //estudiar
    int valido;
    int j;
    do {
        printf("%s", prompt);
        SaltodeLinea(destino, max_len);
        valido = 1;
        
        if(esStringVacio(destino)) {
            valido = 0;
            printf("Error: El texto no puede estar vacio o contener solo espacios.\n");
        } else {
            for(j = 0; destino[j] != '\0'; j++) {
                if(!((destino[j] >= 'A' && destino[j] <= 'Z') || 
                     (destino[j] >= 'a' && destino[j] <= 'z') ||
                     destino[j] == ' ')) {
                    valido = 0;
                    printf("Error: Solo se permiten letras y espacios.\n");
                    break;
                }
            }
        }
    } while(!valido);
}

// VERIFICAR SI ZONA EXISTE
int verificarZonaExiste(const char* nombre, DatosZona *zonas, int numZonas) { 
    int i;
    for(i = 0; i < numZonas; i++) {
        if(strcmp(zonas[i].nombre, nombre) == 0) {
            return 1;
        }
    }
    return 0;
}

// AGREGAR ZONA
void agregarZona(DatosZona *zonas, int *numZonas) {    //estudiar
    DatosZona nuevaZona;
    FILE *archivo;
    int i;
    
    printf("\n--- Agregar Nueva Zona ---\n");
    
    if(*numZonas >= MAX_ZONAS) {
        printf("Error: Se ha alcanzado el limite maximo de zonas.\n");
        return;
    }
    
    validarTextoSoloLetras("Ingrese el nombre de la zona: ", nuevaZona.nombre, 50);
    
    if(verificarZonaExiste(nuevaZona.nombre, zonas, *numZonas)) {
        printf("Error: Ya existe una zona con ese nombre.\n");
        return;
    }
    
    nuevaZona.id = *numZonas + 1;
    nuevaZona.co2 = 0.0;
    nuevaZona.so2 = 0.0;
    nuevaZona.no2 = 0.0;
    nuevaZona.pm25 = 0.0;
    nuevaZona.temperatura = 0.0;
    nuevaZona.velocidad_viento = 0.0;
    nuevaZona.humedad = 0.0;
    
    zonas[*numZonas] = nuevaZona;
    (*numZonas)++;
    
    archivo = fopen("zonas.dat", "wb");  //estudiar
    if(archivo != NULL) {
        fwrite(numZonas, sizeof(int), 1, archivo);
        for(i = 0; i < *numZonas; i++) {
            fwrite(&zonas[i], sizeof(DatosZona), 1, archivo);
        }
        fclose(archivo);
        printf("\n*** Zona agregada exitosamente ***\n");
        printf("ID: %d | Nombre: %s\n", nuevaZona.id, nuevaZona.nombre);
    } else {
        printf("Error al guardar la zona.\n");
    }
}

// MOSTRAR ZONAS REGISTRADAS (DETALLADO)
void mostrarZonasRegistradas() {
    DatosZona zonas[MAX_ZONAS];
    int numZonas = 0;
    FILE *archivo;
    int i;
    
    printf("\n--- Zonas Registradas ---\n");
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas en el sistema.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    if(numZonas == 0) {
        printf("No hay zonas registradas.\n");
        fclose(archivo);
        return;
    }
    
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    for(i = 0; i < numZonas; i++) {
        printf("\n[Zona #%d]\n", zonas[i].id);
        printf("  Nombre: %s\n", zonas[i].nombre);
        printf("  Ultima actualizacion:\n");
        printf("    CO2: %.2f ppm | SO2: %.2f ug/m3\n", zonas[i].co2, zonas[i].so2);
        printf("    NO2: %.2f ug/m3 | PM2.5: %.2f ug/m3\n", zonas[i].no2, zonas[i].pm25);
        printf("------------------------------------------------------------------------\n");
    }
}

// MOSTRAR MENU COMPACTO DE ZONAS
void mostrarMenuZonas() {
    DatosZona zonas[MAX_ZONAS];
    int numZonas = 0;
    FILE *archivo;
    int i;
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    if(numZonas == 0) {
        printf("No hay zonas registradas.\n");
        fclose(archivo);
        return;
    }
    
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    printf("\n--- Seleccionar Zona ---\n");
    printf("========================================================================\n");
    for(i = 0; i < numZonas; i++) {
        printf("%d) %s (ID: %d)\n", i + 1, zonas[i].nombre, zonas[i].id);
    }
    printf("========================================================================\n");
}

// GESTIONAR ZONAS
void gestionarZonas(DatosZona *zonas, int *numZonas) {
    int opcion, retorno;
    
    do {
        printf("\n--- Gestion de Zonas ---\n");
        printf("1) Agregar zona\n");
        printf("2) Modificar zona\n");
        printf("3) Eliminar zona\n");
        printf("4) Ver zonas registradas\n");
        printf("5) Volver al menu principal\n");
        opcion = validarEntero("Opcion: ", 1, 5);
        
        switch(opcion) {
            case 1:
                agregarZona(zonas, numZonas);
                break;
            case 2:
                retorno = modificarZona(zonas, *numZonas);
                if(retorno == 1) {
                    opcion = 5;
                }
                break;
            case 3:
                retorno = eliminarZona(zonas, numZonas);
                if(retorno == 1) {
                    opcion = 5;
                }
                break;
            case 4:
                mostrarZonasRegistradas();
                break;
        }
    } while(opcion != 5);
}

// MODIFICAR ZONA
int modificarZona(DatosZona *zonas, int numZonas) {  //estudiar
    FILE *archivo;
    int i, idZona, encontrado = 0;
    char nuevoNombre[50];
    char confirmacion;
    
    if(numZonas == 0) {
        printf("No hay zonas para modificar.\n");
        return 0;
    }
    
    mostrarMenuZonas();
    idZona = validarEntero("Selecciona el numero de la zona: ", 1, numZonas);
    
    for(i = 0; i < numZonas; i++) {
        if(zonas[i].id == idZona) {
            encontrado = 1;
            
            printf("\n========================================================================\n");
            printf("Zona seleccionada: %s (ID: %d)\n", zonas[i].nombre, zonas[i].id);
            printf("========================================================================\n");
            printf("Esta seguro que desea modificar esta zona? (s/n): ");
            scanf("%c", &confirmacion);
            fflush(stdin);
            
            if(confirmacion == 's' || confirmacion == 'S') {
                printf("\nZona actual: %s\n", zonas[i].nombre);
                validarTextoSoloLetras("Ingrese el nuevo nombre: ", nuevoNombre, 50);
                strcpy(zonas[i].nombre, nuevoNombre);
                
                archivo = fopen("zonas.dat", "wb");
                if(archivo != NULL) {
                    fwrite(&numZonas, sizeof(int), 1, archivo);
                    for(i = 0; i < numZonas; i++) {
                        fwrite(&zonas[i], sizeof(DatosZona), 1, archivo); 
                    }
                    fclose(archivo);
                    printf("\n*** Zona modificada exitosamente ***\n");
                    return 0;
                }
            } else {
                printf("\n*** Modificacion cancelada - Volviendo al menu principal ***\n");
                return 1;
            }
            break;
        }
    }
    
    if(!encontrado) {
        printf("Zona no encontrada.\n");
    }
    return 0;
}

// ELIMINAR ZONA
int eliminarZona(DatosZona *zonas, int *numZonas) {  
    FILE *archivo;
    int i, j, opcionZona, encontrado = 0;
    char confirmacion;
    
    if(*numZonas == 0) {
        printf("No hay zonas para eliminar.\n");
        return 0;
    }
    
    mostrarMenuZonas();
    opcionZona = validarEntero("Selecciona el numero de la zona: ", 1, *numZonas);
    int idZona = opcionZona;  // El ID es la posición
    
    // Buscar por posición directa (ID es la posición + 1)
    if(idZona > 0 && idZona <= *numZonas) {
        i = idZona - 1;  // Convertir ID a índice del array
        encontrado = 1;
        
        printf("\n========================================================================\n");
        printf("Zona seleccionada: %s (ID: %d)\n", zonas[i].nombre, zonas[i].id);
        printf("========================================================================\n");
        printf("Esta seguro que desea eliminar esta zona? (s/n): ");
        scanf("%c", &confirmacion);
        fflush(stdin);
        
        if(confirmacion == 's' || confirmacion == 'S') {
            printf("\nEliminando zona: %s\n", zonas[i].nombre);
            
            // Desplazar zonas hacia atras
            for(j = i; j < *numZonas - 1; j++) {
                zonas[j] = zonas[j + 1];
            }
            (*numZonas)--;
            
            // REASIGNAR IDS SECUENCIALMENTE
            for(j = 0; j < *numZonas; j++) { //e
                zonas[j].id = j + 1;
            }
            
            archivo = fopen("zonas.dat", "wb");
            if(archivo != NULL) {
                fwrite(numZonas, sizeof(int), 1, archivo);
                for(j = 0; j < *numZonas; j++) {
                    fwrite(&zonas[j], sizeof(DatosZona), 1, archivo);
                }
                fclose(archivo);
                printf("\n*** Zona eliminada exitosamente ***\n");
                return 0;
            }
        } else {
            printf("\n*** Eliminacion cancelada - Volviendo al menu principal ***\n");
            return 1;
        }
    }
    
    if(!encontrado) {
        printf("Zona no encontrada.\n");
    }
    return 0;
}

// INGRESAR DATOS DE CONTAMINACION
void ingresarDatosContaminacion(DatosZona *zonas, int numZonas) {
    FILE *archivo;
    RegistroDiario registro;
    int i, dia, mes, anio, dia_semana;
    char dia_nombre[20];
    
    printf("\n--- Ingresar Datos de Contaminacion Actual ---\n");
    
    if(numZonas == 0) {
        printf("Error: Debe registrar al menos una zona primero.\n");
        return;
    }
    
    printf("\nIngrese la fecha del registro:\n");
    dia = validarEntero("Dia (1-31): ", 1, 31);
    mes = validarEntero("Mes (1-12): ", 1, 12);
    anio = validarEntero("Anio (2020-2026): ", 2020, 2026);
    dia_semana = validarDiaSemana("Dia de la semana", dia_nombre);
    
    sprintf(registro.fecha, "%s, %02d/%02d/%d", dia_nombre, dia, mes, anio);
    registro.dia = dia;
    registro.mes = mes;
    registro.anio = anio;
    
    mostrarZonasRegistradas();
    
    int idZona = validarEntero("\nSeleccione el ID de la zona: ", 1, numZonas);
    int encontrado = 0;
    
    for(i = 0; i < numZonas; i++) {
        if(zonas[i].id == idZona) {
            encontrado = 1;
            printf("\n--- Datos para: %s ---\n", zonas[i].nombre);
            printf("(Presione 0 para omitir un dato)\n\n");
            
            zonas[i].co2 = validarFloat("CO2 (ppm, 0-2000)", 0.0, 2000.0);
            zonas[i].so2 = validarFloat("SO2 (ug/m3, 0-500)", 0.0, 500.0);
            zonas[i].no2 = validarFloat("NO2 (ug/m3, 0-500)", 0.0, 500.0);
            zonas[i].pm25 = validarFloat("PM2.5 (ug/m3, 0-500)", 0.0, 500.0);
            zonas[i].temperatura = validarFloat("Temperatura (C, -20 a 50)", -20.0, 50.0);
            zonas[i].velocidad_viento = validarFloat("Velocidad del viento (km/h, 0-200)", 0.0, 200.0);
            zonas[i].humedad = validarFloat("Humedad (%, 0-100)", 0.0, 100.0);
            
            registro.zona = zonas[i];
            
            archivo = fopen("historial.dat", "ab");
            if(archivo != NULL) {
                fwrite(&registro, sizeof(RegistroDiario), 1, archivo);
                fclose(archivo);
            }
            
            archivo = fopen("zonas.dat", "wb");
            if(archivo != NULL) {
                fwrite(&numZonas, sizeof(int), 1, archivo);
                for(i = 0; i < numZonas; i++) {
                    fwrite(&zonas[i], sizeof(DatosZona), 1, archivo);
                }
                fclose(archivo);
                printf("\n*** Datos guardados exitosamente en: %s ***\n", registro.fecha);
            } else {
                printf("Error al guardar los datos.\n");
            }
            break;
        }
    }
    
    if(!encontrado) {
        printf("Zona no encontrada.\n");
    }
}

// MONITOREAR CONTAMINACION ACTUAL
void monitorearContaminacionActual() {
    DatosZona zonas[MAX_ZONAS];
    int numZonas = 0;
    FILE *archivo;
    int i;
    
    printf("\n--- Monitoreo de Contaminacion Actual ---\n");
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay datos disponibles.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    if(numZonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    // Mostrar menú de zonas
    printf("\n--- Seleccionar Zona a Monitorear ---\n");
    printf("========================================================================\n");
    for(i = 0; i < numZonas; i++) {
        printf("%d) %s (ID: %d)\n", i + 1, zonas[i].nombre, zonas[i].id);
    }
    printf("========================================================================\n");
    
    int opcionZona = validarEntero("Selecciona el numero de la zona: ", 1, numZonas);
    int indiceZona = opcionZona - 1;
    int zonaSeleccionadaID = zonas[indiceZona].id;
    
    printf("\nLimites OMS: CO2=%.0f ppm | SO2=%.0f | NO2=%.0f | PM2.5=%.0f ug/m3\n",
           LIMITE_CO2, LIMITE_SO2, LIMITE_NO2, LIMITE_PM25);
    printf("========================================================================\n");
    printf("\n[%s] - Historial de Registros\n", zonas[indiceZona].nombre);
    printf("========================================================================\n");
    
    // Leer historial para mostrar todos los registros de esa zona
    FILE *archivoHistorial = fopen("historial.dat", "rb");
    if(archivoHistorial == NULL) {
        printf("No hay historial disponible.\n");
        return;
    }
    
    RegistroDiario registro;
    int registrosEncontrados = 0;
    
    while(fread(&registro, sizeof(RegistroDiario), 1, archivoHistorial)) {
        if(registro.zona.id == zonaSeleccionadaID) {
            registrosEncontrados++;
            printf("\nFecha: %s\n", registro.fecha);
            printf("------------------------------------------------------------------------\n");
            printf("  CO2:   %.2f ppm", registro.zona.co2);
            if(registro.zona.co2 > LIMITE_CO2) {
                printf(" [EXCEDE: +%.1f%%]", ((registro.zona.co2 - LIMITE_CO2) / LIMITE_CO2) * 100);
            } else {
                printf(" [OK]");
            }
            printf("\n");
            
            printf("  SO2:   %.2f ug/m3", registro.zona.so2);
            if(registro.zona.so2 > LIMITE_SO2) {
                printf(" [EXCEDE: +%.1f%%]", ((registro.zona.so2 - LIMITE_SO2) / LIMITE_SO2) * 100);
            } else {
                printf(" [OK]");
            }
            printf("\n");
            
            printf("  NO2:   %.2f ug/m3", registro.zona.no2);
            if(registro.zona.no2 > LIMITE_NO2) {
                printf(" [EXCEDE: +%.1f%%]", ((registro.zona.no2 - LIMITE_NO2) / LIMITE_NO2) * 100);
            } else {
                printf(" [OK]");
            }
            printf("\n");
            
            printf("  PM2.5: %.2f ug/m3", registro.zona.pm25);
            if(registro.zona.pm25 > LIMITE_PM25) {
                printf(" [EXCEDE: +%.1f%%]", ((registro.zona.pm25 - LIMITE_PM25) / LIMITE_PM25) * 100);
            } else {
                printf(" [OK]");
            }
            printf("\n");
            
            printf("\n  Condiciones climaticas:\n");
            printf("    Temperatura: %.2f C | Viento: %.2f km/h | Humedad: %.2f%%\n",
                   registro.zona.temperatura, registro.zona.velocidad_viento, registro.zona.humedad);
        }
    }
    
    fclose(archivoHistorial);
    
    if(registrosEncontrados == 0) {
        printf("No hay registros para esta zona en el historial.\n");
    }
    
    printf("\n========================================================================\n");
}

// CALCULAR PROMEDIOS HISTORICOS
void calcularPromediosHistoricos() {
    FILE *archivo;
    RegistroDiario registros[1000];
    PromedioHistorico promedios[MAX_ZONAS];
    DatosZona zonas[MAX_ZONAS];
    int totalRegistros = 0;
    int i, j, numPromedios = 0, numZonas = 0;
    
    printf("\n--- Promedios Historicos (Ultimos 30 Dias) ---\n");
    
    // Cargar zonas
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }
    fread(&numZonas, sizeof(int), 1, archivo);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    if(numZonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    // Mostrar menú de zonas
    printf("\n--- Seleccionar Zona para Ver Promedios ---\n");
    printf("========================================================================\n");
    for(i = 0; i < numZonas; i++) {
        printf("%d) %s (ID: %d)\n", i + 1, zonas[i].nombre, zonas[i].id);
    }
    printf("========================================================================\n");
    
    int opcionZona = validarEntero("Selecciona el numero de la zona: ", 1, numZonas);
    int indiceZona = opcionZona - 1;
    char zonaSeleccionada[50];
    strcpy(zonaSeleccionada, zonas[indiceZona].nombre);
    
    archivo = fopen("historial.dat", "rb");
    if(archivo == NULL) {
        printf("No hay historial disponible.\n");
        return;
    }
    
    while(fread(&registros[totalRegistros], sizeof(RegistroDiario), 1, archivo) == 1 && 
          totalRegistros < 1000) {
        totalRegistros++;
    }
    fclose(archivo);
    
    if(totalRegistros == 0) {
        printf("No hay registros historicos.\n");
        return;
    }
    
    // Calcular promedios solo para la zona seleccionada
    float suma_co2 = 0, suma_so2 = 0, suma_no2 = 0, suma_pm25 = 0;
    int dias_analizados = 0;
    
    for(i = 0; i < totalRegistros && i < DIAS_HISTORICOS; i++) {
        if(strcmp(registros[i].zona.nombre, zonaSeleccionada) == 0) {
            suma_co2 += registros[i].zona.co2;
            suma_so2 += registros[i].zona.so2;
            suma_no2 += registros[i].zona.no2;
            suma_pm25 += registros[i].zona.pm25;
            dias_analizados++;
        }
    }
    
    printf("\nDatos analizados: %d registros para %s\n", dias_analizados, zonaSeleccionada);
    printf("========================================================================\n");
    
    if(dias_analizados > 0) {
        float prom_co2 = suma_co2 / dias_analizados;
        float prom_so2 = suma_so2 / dias_analizados;
        float prom_no2 = suma_no2 / dias_analizados;
        float prom_pm25 = suma_pm25 / dias_analizados;
        
        int excede_co2 = (prom_co2 > LIMITE_CO2);
        int excede_so2 = (prom_so2 > LIMITE_SO2);
        int excede_no2 = (prom_no2 > LIMITE_NO2);
        int excede_pm25 = (prom_pm25 > LIMITE_PM25);
        
        printf("\n[%s] - %d dias\n", zonaSeleccionada, dias_analizados);
        printf("------------------------------------------------------------------------\n");
        printf("  CO2:   %.2f ppm   [Limite: %.0f] %s\n", 
               prom_co2, LIMITE_CO2, 
               excede_co2 ? "[EXCEDE, En los ultimos 30 dias se ha excedido el limite de CO2]" : "[CO2 ESTABLE]");
        printf("  SO2:   %.2f ug/m3 [Limite: %.0f] %s\n", 
               prom_so2, LIMITE_SO2,
               excede_so2 ? "[EXCEDE, En los ultimos 30 dias se ha excedido el limite de SO2]" : "[SO2 ESTABLE]");
        printf("  NO2:   %.2f ug/m3 [Limite: %.0f] %s\n", 
               prom_no2, LIMITE_NO2,
               excede_no2 ? "[EXCEDE, En los ultimos 30 dias se ha excedido el limite de NO2]" : "[NO2 ESTABLE]");
        printf("  PM2.5: %.2f ug/m3 [Limite: %.0f] %s\n", 
               prom_pm25, LIMITE_PM25,
               excede_pm25 ? "[EXCEDE, En los ultimos 30 dias se ha excedido el limite de PM2.5]" : "[PM2.5 ESTABLE]");
    } else {
        printf("No hay registros para esta zona en el historial.\n");
    }
    printf("\n========================================================================\n");
}

// PREDECIR NIVELES FUTUROS (COMPLETA)
void predecirNivelesFuturos() {
    FILE *archivo;
    RegistroDiario registros[100];
    DatosZona zonas[MAX_ZONAS];
    int totalRegistros = 0;
    int numZonas = 0;
    int i, j, k;
    
    printf("\n--- Prediccion de Niveles Futuros (24 Horas) ---\n");
    
    // Cargar zonas
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }
    fread(&numZonas, sizeof(int), 1, archivo);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    if(numZonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    // Cargar historial
    archivo = fopen("historial.dat", "rb");
    if(archivo == NULL) {
        printf("No hay historial para realizar predicciones.\n");
        return;
    }
    
    while(fread(&registros[totalRegistros], sizeof(RegistroDiario), 1, archivo) == 1 && 
          totalRegistros < 100) {
        totalRegistros++;
    }
    fclose(archivo);
    
    if(totalRegistros == 0) {
        printf("No hay registros para obtener el dia actual.\n");
        return;
    }
    
    // Mostrar menú de zonas
    printf("\n--- Seleccionar Zona para Prediccion ---\n");
    printf("========================================================================\n");
    for(i = 0; i < numZonas; i++) {
        printf("%d) %s (ID: %d)\n", i + 1, zonas[i].nombre, zonas[i].id);
    }
    printf("========================================================================\n");
    
    int opcionZona = validarEntero("Selecciona el numero de la zona: ", 1, numZonas);
    int indiceZona = opcionZona - 1;
    char zonaSeleccionada[50];
    strcpy(zonaSeleccionada, zonas[indiceZona].nombre);
    
    printf("\nAlgoritmo: Promedio ponderado con factores climaticos\n");
    printf("Registros analizados: %d\n", totalRegistros);
    printf("========================================================================\n\n");
    
    float sum_co2 = 0, sum_so2 = 0, sum_no2 = 0, sum_pm25 = 0;
    float peso_total = 0;
    int registros_zona = 0;
    int dia_actual_zona = -1;
    char dia_nombre_siguiente_zona[20];
    
    // OBTENER ULTIMO REGISTRO DE ESTA ZONA ESPECIFICA
    for(j = totalRegistros - 1; j >= 0; j--) {
        if(strcmp(registros[j].zona.nombre, zonaSeleccionada) == 0) {
            // Extraer el dia de la semana del ultimo registro de esta zona
            char dia_ultimo_zona[20];
            sscanf(registros[j].fecha, "%[^,]", dia_ultimo_zona);
            
            // Convertir a minusculas
            int k;
            for(k = 0; dia_ultimo_zona[k]; k++) {
                dia_ultimo_zona[k] = tolower(dia_ultimo_zona[k]);
            }
            
            // Encontrar posicion del dia
            const char* dias_validos[] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"};
            const char* dias_nombres[] = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo"};
            
            for(k = 0; k < 7; k++) {
                if(strcmp(dia_ultimo_zona, dias_validos[k]) == 0) {
                    dia_actual_zona = k;
                    break;
                }
            }
            
            if(dia_actual_zona != -1) {
                int dia_siguiente_zona = (dia_actual_zona + 1) % 7;
                strcpy(dia_nombre_siguiente_zona, dias_nombres[dia_siguiente_zona]);
            }
            break;
        }
    }
    
    // Si no hay registros para esta zona, mostrar error
    if(dia_actual_zona == -1) {
        printf("No hay registros disponibles para esta zona.\n");
        return;
    }
    
    for(j = totalRegistros - 1; j >= 0 && registros_zona < 10; j--) {
        if(strcmp(registros[j].zona.nombre, zonaSeleccionada) == 0) {
            float peso = (float)(10 - registros_zona) / 10.0;
            peso_total += peso;
            
            sum_co2 += registros[j].zona.co2 * peso;
            sum_so2 += registros[j].zona.so2 * peso;
            sum_no2 += registros[j].zona.no2 * peso;
            sum_pm25 += registros[j].zona.pm25 * peso;
            registros_zona++;
        }
    }
    
    if(registros_zona > 0 && peso_total > 0) {
        float co2_pred = sum_co2 / peso_total;
        float so2_pred = sum_so2 / peso_total;
        float no2_pred = sum_no2 / peso_total;
        float pm25_pred = sum_pm25 / peso_total;
        
        int excede_count = 0;
        if(co2_pred > LIMITE_CO2) excede_count++;
        if(so2_pred > LIMITE_SO2) excede_count++;
        if(no2_pred > LIMITE_NO2) excede_count++;
        if(pm25_pred > LIMITE_PM25) excede_count++;
        
        printf("--- Resultados de Prediccion ---\n\n");
        printf("[%s]\n", zonaSeleccionada);
        printf("------------------------------------------------------------------------\n");
        
        // Mostrar el ultimo dia registrado para esta zona
        for(j = totalRegistros - 1; j >= 0; j--) {
            if(strcmp(registros[j].zona.nombre, zonaSeleccionada) == 0) {
                char dia_temp[20];
                sscanf(registros[j].fecha, "%[^,]", dia_temp);
                printf("  El ultimo dia registrado fue: %s\n", dia_temp);
                break;
            }
        }
        printf("\n");
        
        printf("  CO2 predicho:   %.2f ppm   [Limite: %.0f] %s\n",
               co2_pred, LIMITE_CO2,
               co2_pred > LIMITE_CO2 ? "[ALERTA, Se prevee aumento prolongado de CO2]" : "[Se prevee CO2 MODERADO-ESTABLE]");
        printf("  SO2 predicho:   %.2f ug/m3 [Limite: %.0f] %s\n",
               so2_pred, LIMITE_SO2,
               so2_pred > LIMITE_SO2 ? "[ALERTA, Se prevee aumento prolongado de SO2]" : "[Se prevee SO2 MODERADO-ESTABLE]");
        printf("  NO2 predicho:   %.2f ug/m3 [Limite: %.0f] %s\n",
               no2_pred, LIMITE_NO2,
               no2_pred > LIMITE_NO2 ? "[ALERTA, Se prevee aumento prolongado de NO2]" : "[Se prevee NO2 MODERADO-ESTABLE]");
        printf("  PM2.5 predicho: %.2f ug/m3 [Limite: %.0f] %s\n",
               pm25_pred, LIMITE_PM25,
               pm25_pred > LIMITE_PM25 ? "[ALERTA, Se prevee aumento prolongado de PM2.5]" : "[Se prevee PM2.5 MODERADO-ESTABLE]");
        printf("  Nivel de alerta: %d contaminante(s) excede limite\n", excede_count);
        printf("  Prediccion para: Manana (%s)\n\n", dia_nombre_siguiente_zona);
    }
    printf("========================================================================\n");
}


// GENERAR ALERTAS PREVENTIVAS
void generarAlertasPreventivas() {
    DatosZona zonas[MAX_ZONAS];
    RegistroDiario registros[1000];
    FILE *archivo;
    int numZonas = 0;
    int i, j, k, alertas_count = 0, totalRegistros = 0;
    char dia_nombre[20];
    int dia_actual_index;
    
    printf("\n--- Generar Alertas Preventivas ---\n");
    
    dia_actual_index = validarDiaSemana("Ingrese el dia actual", dia_nombre);
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    if(numZonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    // Cargar historial
    archivo = fopen("historial.dat", "rb");
    if(archivo != NULL) {
        while(fread(&registros[totalRegistros], sizeof(RegistroDiario), 1, archivo) == 1 && 
              totalRegistros < 1000) {
            totalRegistros++;
        }
        fclose(archivo);
    }
    
    if(totalRegistros == 0) {
        printf("No hay registros en el historial para hacer predicciones.\n");
        return;
    }
    
    // Mostrar menu de zonas
    printf("\n--- Seleccionar Zona para Alertas ---\n");
    printf("========================================================================\n");
    for(i = 0; i < numZonas; i++) {
        printf("%d) %s (ID: %d)\n", i + 1, zonas[i].nombre, zonas[i].id);
    }
    printf("========================================================================\n");
    
    int opcionZona = validarEntero("Selecciona el numero de la zona: ", 1, numZonas);
    int indiceZona = opcionZona - 1;
    
    // Calcular el siguiente dia
    int dia_siguiente_index = (dia_actual_index + 1) % 7;
    const char* dias_nombres[] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"};
    const char* dias_nombres_caps[] = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo"};
    char dia_siguiente[20];
    strcpy(dia_siguiente, dias_nombres[dia_siguiente_index]);
    
    printf("\n========================================================================\n");
    printf("Prediccion de alertas para manana (%s)...\n", dias_nombres_caps[dia_siguiente_index]);
    printf("Zona: %s\n", zonas[indiceZona].nombre);
    printf("========================================================================\n\n");
    
    // Calcular prediccion con promedio ponderado de los ultimos 10 registros
    float sum_co2 = 0, sum_so2 = 0, sum_no2 = 0, sum_pm25 = 0;
    float peso_total = 0;
    int registros_zona = 0;
    
    for(j = totalRegistros - 1; j >= 0 && registros_zona < 10; j--) {
        if(strcmp(registros[j].zona.nombre, zonas[indiceZona].nombre) == 0) {
            float peso = (float)(10 - registros_zona) / 10.0;
            peso_total += peso;
            
            sum_co2 += registros[j].zona.co2 * peso;
            sum_so2 += registros[j].zona.so2 * peso;
            sum_no2 += registros[j].zona.no2 * peso;
            sum_pm25 += registros[j].zona.pm25 * peso;
            registros_zona++;
        }
    }
    
    // Calcular valores predichos
    float co2_pred = (registros_zona > 0 && peso_total > 0) ? sum_co2 / peso_total : 0;
    float so2_pred = (registros_zona > 0 && peso_total > 0) ? sum_so2 / peso_total : 0;
    float no2_pred = (registros_zona > 0 && peso_total > 0) ? sum_no2 / peso_total : 0;
    float pm25_pred = (registros_zona > 0 && peso_total > 0) ? sum_pm25 / peso_total : 0;
    
    int alerta = 0;
    printf("[%s] - Prediccion para manana\n", zonas[indiceZona].nombre);
    printf("------------------------------------------------------------------------\n");
    
    // Mostrar todos los valores predichos
    printf("  CO2 predicho:   %.2f ppm   [Limite: %.0f]", co2_pred, LIMITE_CO2);
    if(co2_pred > LIMITE_CO2) {
        printf(" [EXCEDE LIMITE]\n");
        alertas_count++;
    } else if(co2_pred > LIMITE_CO2 * 0.8) {
        printf(" [ALERTA PREVENTIVA]\n");
        alertas_count++;
    } else {
        printf(" [OK]\n");
    }
    
    printf("  SO2 predicho:   %.2f ug/m3 [Limite: %.0f]", so2_pred, LIMITE_SO2);
    if(so2_pred > LIMITE_SO2) {
        printf(" [EXCEDE LIMITE]\n");
        alertas_count++;
    } else if(so2_pred > LIMITE_SO2 * 0.8) {
        printf(" [ALERTA PREVENTIVA]\n");
        alertas_count++;
    } else {
        printf(" [OK]\n");
    }
    
    printf("  NO2 predicho:   %.2f ug/m3 [Limite: %.0f]", no2_pred, LIMITE_NO2);
    if(no2_pred > LIMITE_NO2) {
        printf(" [EXCEDE LIMITE]\n");
        alertas_count++;
    } else if(no2_pred > LIMITE_NO2 * 0.8) {
        printf(" [ALERTA PREVENTIVA]\n");
        alertas_count++;
    } else {
        printf(" [OK]\n");
    }
    
    printf("  PM2.5 predicho: %.2f ug/m3 [Limite: %.0f]", pm25_pred, LIMITE_PM25);
    if(pm25_pred > LIMITE_PM25) {
        printf(" [EXCEDE LIMITE]\n");
        alertas_count++;
    } else if(pm25_pred > LIMITE_PM25 * 0.8) {
        printf(" [ALERTA PREVENTIVA]\n");
        alertas_count++;
    } else {
        printf(" [OK]\n");
    }
    printf("\n");
    
    printf("========================================================================\n");
    printf("Total de alertas predichas para manana (%s): %d\n", dias_nombres_caps[dia_siguiente_index], alertas_count);
    printf("Se recomienda monitoreo continuo para mantener la calidad del aire.\n");
    printf("========================================================================\n");
}

// GENERAR RECOMENDACIONES
void generarRecomendaciones() {
    DatosZona zonas[MAX_ZONAS];
    FILE *archivo;
    int numZonas = 0;
    int i;
    
    printf("\n--- Generar Recomendaciones de Mitigacion ---\n");
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    if(numZonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    // Mostrar menu de zonas
    printf("\n--- Seleccionar Zona ---\n");
    printf("========================================================================\n");
    for(i = 0; i < numZonas; i++) {
        printf("%d) %s (ID: %d)\n", i + 1, zonas[i].nombre, zonas[i].id);
    }
    printf("========================================================================\n");
    
    int opcionZona = validarEntero("Selecciona el numero de la zona: ", 1, numZonas);
    int indiceZona = opcionZona - 1;
    
    printf("\n========================================================================\n");
    printf("\n[%s]\n", zonas[indiceZona].nombre);
    printf("------------------------------------------------------------------------\n");
    
    int hay_recomendaciones = 0;
    
    if(zonas[indiceZona].co2 > LIMITE_CO2) {
        printf("  -> Aumentar areas verdes y arboles (capturan CO2)\n");
        printf("  -> Reducir trafico vehicular en horas pico\n");
        printf("  -> Promover uso de transporte publico\n");
        hay_recomendaciones = 1;
    }
    
    if(zonas[indiceZona].so2 > LIMITE_SO2) {
        printf("  -> Controlar emisiones de plantas industriales\n");
        printf("  -> Reducir combustibles fosiles\n");
        printf("  -> Implementar filtros en chimeneas\n");
        hay_recomendaciones = 1;
    }
    
    if(zonas[indiceZona].no2 > LIMITE_NO2) {
        printf("  -> Regular emision de gases de escape\n");
        printf("  -> Usar combustibles limpios\n");
        printf("  -> Mejorar mantenimiento de vehiculos\n");
        hay_recomendaciones = 1;
    }
    
    if(zonas[indiceZona].pm25 > LIMITE_PM25) {
        printf("  -> Riego de areas para controlar polvo\n");
        printf("  -> Limitar actividades de construccion\n");
        printf("  -> Mantener pavimentacion de calles\n");
        hay_recomendaciones = 1;
    }
    
    if(!hay_recomendaciones) {
        printf("  -> Mantener las buenas practicas ambientales\n");
        printf("  -> Continuar monitoreo regular\n");
    }
    printf("\n========================================================================\n");
}

// COMPARAR ZONAS
void compararZonas() {
    DatosZona zonas[MAX_ZONAS];
    FILE *archivo;
    int numZonas = 0;
    int i, zona1, zona2;
    
    printf("\n--- Comparar Niveles entre Zonas ---\n");
    
    archivo = fopen("zonas.dat", "rb");
    if(archivo == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo);
    if(numZonas < 2) {
        printf("Se necesita al menos 2 zonas para comparar.\n");
        fclose(archivo);
        return;
    }
    
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo);
    }
    fclose(archivo);
    
    mostrarZonasRegistradas();    
    zona1 = validarEntero("\nSeleccione primera zona (ID): ", 1, numZonas);
    zona2 = validarEntero("Seleccione segunda zona (ID): ", 1, numZonas);
    
    if(zona1 == zona2) {
        printf("Error: Debe seleccionar zonas diferentes.\n");
        return;
    }
    
    printf("\n========================================================================\n");
    printf("COMPARACION: %s vs %s\n", zonas[zona1-1].nombre, zonas[zona2-1].nombre);
    printf("========================================================================\n\n");
    
    printf("%-20s | %-15s | %-15s | Diferencia\n", "Parametro", zonas[zona1-1].nombre, zonas[zona2-1].nombre);
    printf("------------------------------------------------------------------------\n");
    
    printf("CO2 (ppm)    | %.2f        | %.2f        | %.2f\n",
           zonas[zona1-1].co2, zonas[zona2-1].co2,
           zonas[zona1-1].co2 - zonas[zona2-1].co2);
    
    printf("SO2 (ug/m3)  | %.2f        | %.2f        | %.2f\n",
           zonas[zona1-1].so2, zonas[zona2-1].so2,
           zonas[zona1-1].so2 - zonas[zona2-1].so2);
    
    printf("NO2 (ug/m3)  | %.2f        | %.2f        | %.2f\n",
           zonas[zona1-1].no2, zonas[zona2-1].no2,
           zonas[zona1-1].no2 - zonas[zona2-1].no2);
    
    printf("PM2.5(ug/m3) | %.2f        | %.2f        | %.2f\n",
           zonas[zona1-1].pm25, zonas[zona2-1].pm25,
           zonas[zona1-1].pm25 - zonas[zona2-1].pm25);
    
    printf("\nZona mas contaminada: %s\n",
           (zonas[zona1-1].co2 + zonas[zona1-1].so2 + zonas[zona1-1].no2 + zonas[zona1-1].pm25) >
           (zonas[zona2-1].co2 + zonas[zona2-1].so2 + zonas[zona2-1].no2 + zonas[zona2-1].pm25) ?
           zonas[zona1-1].nombre : zonas[zona2-1].nombre);
    
    printf("========================================================================\n");
}

// VER HISTORIAL COMPLETO
void verHistorialCompleto() {
    FILE *archivo;
    RegistroDiario registro;
    int contador = 0;
    
    printf("\n--- Historial Completo de Registros ---\n");
    
    archivo = fopen("historial.dat", "rb");
    if(archivo == NULL) {
        printf("No hay historial disponible.\n");
        return;
    }
    
    printf("\n========================================================================\n");
    
    while(fread(&registro, sizeof(RegistroDiario), 1, archivo) == 1) {
        contador++;
        printf("\n[Registro #%d]\n", contador);
        printf("Fecha: %s | Zona: %s\n", registro.fecha, registro.zona.nombre);
        printf("------------------------------------------------------------------------\n");
        printf("  CO2:   %.2f ppm\n", registro.zona.co2);
        printf("  SO2:   %.2f ug/m3\n", registro.zona.so2);
        printf("  NO2:   %.2f ug/m3\n", registro.zona.no2);
        printf("  PM2.5: %.2f ug/m3\n", registro.zona.pm25);
        printf("  Temperatura: %.2f C | Viento: %.2f km/h | Humedad: %.2f%%\n",
               registro.zona.temperatura, registro.zona.velocidad_viento, registro.zona.humedad);
    }
    
    fclose(archivo);
    printf("\n========================================================================\n");
    printf("Total de registros: %d\n", contador);
    printf("========================================================================\n");
}

// EXPORTAR REPORTE COMPLETO
void exportarReporte() {
    FILE *archivo_salida, *archivo_entrada;
    DatosZona zonas[MAX_ZONAS];
    RegistroDiario registro;
    int numZonas = 0, i;
    
    printf("\n--- Exportar Reporte Completo ---\n");
    
    archivo_entrada = fopen("zonas.dat", "rb");
    if(archivo_entrada == NULL) {
        printf("No hay datos para exportar.\n");
        return;
    }
    
    fread(&numZonas, sizeof(int), 1, archivo_entrada);
    for(i = 0; i < numZonas; i++) {
        fread(&zonas[i], sizeof(DatosZona), 1, archivo_entrada);
    }
    fclose(archivo_entrada);
    
    archivo_salida = fopen("reporte_completo.txt", "w");
    if(archivo_salida == NULL) {
        printf("Error al crear el archivo de reporte.\n");
        return;
    }
    
    fprintf(archivo_salida, "========================================================================\n");
    fprintf(archivo_salida, "   REPORTE COMPLETO - SISTEMA DE GESTION DE CONTAMINACION DEL AIRE\n");
    fprintf(archivo_salida, "========================================================================\n\n");
    
    fprintf(archivo_salida, "--- ZONAS REGISTRADAS ---\n\n");
    for(i = 0; i < numZonas; i++) {
        fprintf(archivo_salida, "Zona ID %d: %s\n", zonas[i].id, zonas[i].nombre);
        fprintf(archivo_salida, "  CO2: %.2f ppm | SO2: %.2f ug/m3\n", zonas[i].co2, zonas[i].so2);
        fprintf(archivo_salida, "  NO2: %.2f ug/m3 | PM2.5: %.2f ug/m3\n\n", zonas[i].no2, zonas[i].pm25);
    }
    
    fprintf(archivo_salida, "\n--- HISTORIAL DE REGISTROS ---\n\n");
    archivo_entrada = fopen("historial.dat", "rb");
    if(archivo_entrada != NULL) {
        while(fread(&registro, sizeof(RegistroDiario), 1, archivo_entrada) == 1) {
            fprintf(archivo_salida, "Fecha: %s | Zona: %s\n", registro.fecha, registro.zona.nombre);
            fprintf(archivo_salida, "CO2: %.2f | SO2: %.2f | NO2: %.2f | PM2.5: %.2f\n\n",
                   registro.zona.co2, registro.zona.so2, registro.zona.no2, registro.zona.pm25);
        }
        fclose(archivo_entrada);
    }
    
    fprintf(archivo_salida, "\n========================================================================\n");
    fprintf(archivo_salida, "Limites OMS: CO2=%.0f | SO2=%.0f | NO2=%.0f | PM2.5=%.0f\n",
           LIMITE_CO2, LIMITE_SO2, LIMITE_NO2, LIMITE_PM25);
    fprintf(archivo_salida, "========================================================================\n");
    
    fclose(archivo_salida);
    printf("\n*** Reporte exportado exitosamente en 'reporte_completo.txt' ***\n");
}

