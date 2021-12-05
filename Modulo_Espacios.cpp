#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<conio.h>
#include<string.h>

//Estructuras:
struct fecha
{
    int dia;
    int mes;
    int anio;
};

struct Usuarios
{
    char user[10];
    char contra[33];
    char ApeyNom[60];
};

struct Medico
{
    char ApeyNom[60];
    int matriculaMed;
    int dni;
    char telefono[25];
};

struct Paciente
{
    char ApeyNom[60];
    char domicilio[60];
    int  dniPaciente;
    char localidad[60];
    fecha fecha_nacimiento;
    float peso;
    char telefono[25];
};

struct Turnos
{
    int matriculaMed;
    fecha fecha_turno;
    int dniPaciente;
    char detalles_atencion[380];
    int mostrado;
};

//Protipos de funciones:
void end();
void cargar_registros(Usuarios reg_usuarios[50],Medico reg_meds[50],Paciente reg_Pacientes[50],Turnos reg_turnos[50],int &num_usuarios,int &numMeds,int &num_Pacientes,int &num_turnos);
bool inicio_sesion(Usuarios reg_usuarios[50],Medico reg_meds[50],int num_usuarios,int numMeds,int &buscar_matricula);
void listado(Paciente reg_Pacientes[50],Turnos reg_turnos[50],int num_Pacientes,int num_turnos,int buscar_matricula);
void mostrar_datos_Paciente(Paciente reg_Pacientes[50],int num_Pacientes,int buscarDniPacien,int buscar_anio);
void registar_evolucion(Turnos reg_turnos[50],int num_turnos,int matricula);
void guardar_archivo(Turnos reg_turnos[50],int num_turnos);

main()
{
	Usuarios reg_usuarios[50];
    Medico reg_meds[50];
    Paciente reg_Pacientes[50];
    Turnos reg_turnos[50];

    int num_usuarios = 0,numMeds = 0,num_Pacientes = 0,num_turnos = 0,buscar_matricula = 0;
    int opcion;
    bool inicio = false;
    
    cargar_registros(reg_usuarios,reg_meds,reg_Pacientes,reg_turnos,num_usuarios,numMeds,num_Pacientes,num_turnos);

    do
    {
        system("cls");
        printf("Modulo Espacios\n");
        printf("==============================\n");
        printf("1.- Iniciar Sesion.\n");
        printf("2.- Visulizar Lista de Espera de Turnos (Informe).\n");
        printf("3.- Registrar Evolucion del tratamiento.");
        printf("\n\n");
        printf("4.- Cerrar la aplicacion.");
        printf("\n\n");
        printf("Ingrese una opcion: ");
        scanf("%d",&opcion);

        switch (opcion)
        {
            case 1:
                inicio = inicio_sesion(reg_usuarios,reg_meds,num_usuarios,numMeds,buscar_matricula);
                printf("\n\n");
                system("pause");
                break;
            
            case 2:
                if (inicio)
                {
                    listado(reg_Pacientes,reg_turnos,num_Pacientes,num_turnos,buscar_matricula);
                }
                else
                {
                    printf("\nNo inicio sesion, volvera al menu.");
                }
                printf("\n\n");
                system("pause");
                break;
            
            case 3:
                if (inicio)
                {
                    registar_evolucion(reg_turnos,num_turnos,buscar_matricula);
                }
                else
                {
                    printf("\nNo inicio sesion, volvera al menu.");
                }
                printf("\n\n");
                system("pause");
                break;

            case 4:
                guardar_archivo(reg_turnos,num_turnos);
                printf("\nGracias por utilizar el programa.");
                printf("\n\n");
                system("pause");
                break;
            
            default:
                printf("\nIngreso una opcion incorrecta, volvera al menu.");
                printf("\n\n");
                system("pause");
                break;
        }
    } while (opcion != 4);
}

void cargar_registros(Usuarios reg_usuarios[50],Medico reg_meds[50],Paciente reg_Pacientes[50],Turnos reg_turnos[50],int &num_usuarios,int &numMeds,int &num_Pacientes,int &num_turnos)
{
    FILE *arch;
    int i = 0;
    
    //Archivo de medicos:
    arch=fopen("Profesionales.dat","rb");

    if (arch==NULL)
    {
        printf("\nEl archivo 'Profesionales.dat' no fue creado o se elimino, contacte con soporte.");
    }
    else
    {
        fread(&reg_meds[i],sizeof(Medico),1,arch);
        while (!feof(arch))
        {
            i++;
            fread(&reg_meds[i],sizeof(Medico),1,arch);
        }
        numMeds = i;
        i = 0;
        fclose(arch);
    }

    //Archivo de usuarios:
    arch=fopen("Usuarios.dat","rb");
    
    if (arch==NULL)
    {
        printf("\nEl archivo 'Usuarios.dat' no fue creado o se elimino, contacte con soporte.");
    }
    else
    {
        fread(&reg_usuarios[i],sizeof(Usuarios),1,arch);
        while (!feof(arch))
        {
            i++;
            fread(&reg_usuarios[i],sizeof(Usuarios),1,arch); 
        }
        num_usuarios = i;
        i = 0;
        fclose(arch);
    }
    
    //Archivo de Pacientes:
    arch=fopen("Pacientes.dat","rb");

    if (arch==NULL)
    {
        printf("\nEl archivo 'Pacientes.dat' no fue creado o se elimino, contacte con soporte.");
    }
    else
    {
        fread(&reg_Pacientes[i],sizeof(Paciente),1,arch);
        while (!feof(arch))
        {
            i++;
            fread(&reg_Pacientes[i],sizeof(Paciente),1,arch); 
        }
        num_Pacientes = i;
        i = 0;
        fclose(arch);
    }
    
    //Archivo de turnos:
    arch=fopen("Turnos.dat","rb");

    if (arch==NULL)
    {
        printf("\nEl archivo 'Turnos.dat' no fue creado o se elimino, contacte con soporte.");
    }
    else
    {
        fread(&reg_turnos[i],sizeof(Turnos),1,arch);
        while (!feof(arch))
        {
            i++;
            fread(&reg_turnos[i],sizeof(Turnos),1,arch); 
        }
        num_turnos = i;
        i = 0;
        fclose(arch);
    }
}

bool inicio_sesion(Usuarios reg_usuarios[50],Medico reg_meds[50],int num_usuarios,int numMeds,int &buscar_matricula)
{
    char buscar_contra[10],buscar_ApeyNom[60];
    bool esta = false;
    
    printf("\nIngrese su numero de matricula: ");
    scanf("%d",&buscar_matricula);

    /*printf("\nIngrese su contraseña: ");
    _flushall();
    gets(buscar_contra);
    system("cls");*/

    for (int i = 0; i < numMeds; i++)
    {
        if (buscar_matricula == reg_meds[i].matriculaMed)
        {
            strcpy(buscar_ApeyNom,reg_meds[i].ApeyNom);
            esta = true;
            i = numMeds + 1;
        }
    }
    
    /*if (esta)
    {
        esta = true;

        for (int i = 0; i < num_usuarios; i++)
        {
            if (strcmp(buscar_ApeyNom,reg_usuarios[i].ApeyNom) == 0)
            {
                /*if (strcmp(buscar_contra,reg_usuarios[i].contra) == 0)
                {
                    esta = true;
                    i = num_usuarios + 1;
                }
            }
        }
        
        if (esta)
        {
            printf("\nInicio sesion con exito.");
            return true;
        }
        else
        {
            printf("\nEl numero de matricula, o contraseña, es incorrecta, volvera al menu.");
            return false;
        }
    }
    else
    {
        printf("\nEl numero de matricula, o contraseña, es incorrecta, volvera al menu.");
        return false;
    }
    */
}

void listado(Paciente reg_Pacientes[50],Turnos reg_turnos[50],int num_Pacientes,int num_turnos,int buscar_matricula)
{
    fecha buscar;
    bool esta = false;
    int dni;

    printf("\nIngrese la fecha: ");
    printf("\nDia: ");
    scanf("%d",&buscar.dia);
    printf("\nMes: ");
    scanf("%d",&buscar.mes);
    printf("\nAnio: ");
    scanf("%d",&buscar.anio);

    for (int i = 0; i < num_turnos; i++)
    {
        if ((buscar_matricula == reg_turnos[i].matriculaMed) and (reg_turnos[i].mostrado==0))
        {
            if ((buscar.dia == reg_turnos[i].fecha_turno.dia) and (buscar.mes == reg_turnos[i].fecha_turno.mes) and (buscar.anio == reg_turnos[i].fecha_turno.anio))
            {                
                esta = true;
                
                for (int k = 0; k < num_Pacientes; k++)
                {
                    if (reg_turnos[i].dniPaciente == reg_Pacientes[k].dniPaciente)
                    {
                        printf("\n\nApellido y Nombre (Paciente): ");
                        puts(reg_Pacientes[k].ApeyNom);
                        printf("DNI: %d",reg_Pacientes[k].dniPaciente);
                        printf("\nLocalidad: ");
                        puts(reg_Pacientes[k].localidad);
                        printf("Edad: %d",(buscar.anio-reg_Pacientes[k].fecha_nacimiento.anio));
                        printf("\nPeso: %.2f",reg_Pacientes[k].peso);
                    }
                }
            }   
        }   
    }

    if (!esta)
    {
       printf("\nNo se encontraron turno para la fecha ingresada.");
    }
}

void registar_evolucion(Turnos reg_turnos[50],int num_turnos,int matricula)
{
    fecha buscar;
    int buscarDniPacien;
    bool esta = false;

    printf("\nIngrese la fecha del turno: ");
    printf("\nDia: ");
    scanf("%d",&buscar.dia);
    printf("\nMes: ");
    scanf("%d",&buscar.mes);
    printf("\nAnio: ");
    scanf("%d",&buscar.anio);

    printf("\nIngrese el DNI: ");
    scanf("%d",&buscarDniPacien);
    
    for (int i = 0; i < num_turnos; i++)
    {
        if ((matricula == reg_turnos[i].matriculaMed) and (reg_turnos[i].mostrado==0))
        {
            if ((buscar.dia == reg_turnos[i].fecha_turno.dia) and (buscar.mes == reg_turnos[i].fecha_turno.mes) and (buscar.anio == reg_turnos[i].fecha_turno.anio))
            {
                if (buscarDniPacien == reg_turnos[i].dniPaciente)
                {
                    esta = true;
                    reg_turnos[i].mostrado = 1;
                    system("cls");
                    printf("\nIngrese los detalles de la atencion (Maximo 380 caracteres, incluidos espacios):\n");
                    printf("\t-> ");
                    _flushall();
                    gets(reg_turnos[i].detalles_atencion);
                    
                    i = num_turnos + 1;
                }
            }   
        }   
    }

    if (!esta)
    {
        printf("\nEl turno ingresado no se encuentra, o ya se le realizo una devolicion.");
    }
    
}

void guardar_archivo(Turnos reg_turnos[50],int num_turnos)
{
    FILE *arch;
    arch =  fopen("Turnos.dat","w+b");

    for (int i = 0; i < num_turnos; i++)
    {
        fwrite(&reg_turnos[i],sizeof(Turnos),1,arch);
    }
}