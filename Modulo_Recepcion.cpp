#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>

struct fecha
{
	int dia;
	int mes;
	int anio;
};

struct Usuario
{
	char usuario[10];
	char contra[10];
	char ApeNom[60];
};

struct Medicos
{
    char ApeyNom[60];
    int matriculamedico;
    int dni;
    char telefono[25];
};

struct paciente
{
	char ApeyNom[60];
	char domicilio[60];
	int dni_paciente;
	char localidad[60];
	fecha fecha_nacimiento;
	float peso;
	char telefono[25];
};

struct Turnos
{
	int matriculamedico;
	fecha fecha_turno;
	int dni_paciente;
	char detalles_atencion[380];
 	int mostrado; //cambié a variable int
};

int menuprincipal();
void iniciosesion(FILE *usuario1,Usuario user,bool &verificacion);
void regpaciente(FILE *pacie1,paciente pacie2,bool &verificacion);
void regturnos(FILE *turno1,Turnos tur,FILE *med,Medicos meder,bool &verificacion);
void listaten(FILE *turno1,Turnos tur,FILE *med,Medicos meder,bool &verificacion);

main()
{
	setlocale(LC_ALL,"spanish");
	FILE *pacie;
	FILE *turno;
	FILE *med;
	FILE *usuario;
	paciente pacie2;
	Turnos tur;
	Medicos meder;
	Usuario user;
	bool verificacion=false;
	
	int op;
	
	do
	{
		op=menuprincipal();
		switch(op)
		{
			case 1:
				system("cls");
				iniciosesion(usuario,user,verificacion);
				system("pause");
				system("cls");
				break;
			case 2:
				system("cls");
				regpaciente(pacie,pacie2,verificacion);
				system("pause");
				system("cls");
				break;
			case 3:
				system("cls");
				regturnos(turno,tur,med,meder,verificacion);
				system("pause");
				system("cls");
				break;
			case 4:
				system("cls");
				listaten(turno,tur,med,meder,verificacion);
				system("pause");
				system("cls");
				break;
			case 5:
				system("cls");
				printf("\nSaliendo del aplicacion,");
				system("pause");
				system("cls");
				break;
			default:
				system("cls");
				printf("\nNo ingreso una opcion valida, intente nuevamente.");
				printf("\n\n");
				system("pause");
				system("cls");
				break;
		}
	}while(op!=5);
	
}


int menuprincipal()
{
	int op;
	
	printf("\nMenu del Recepcionista.");
	printf("\n===============================");
	printf("\n1.- Iniciar sesion.");
	printf("\n2.- Registrar paciente.");
	printf("\n3.- Registrar turno.");
	printf("\n4.- Listado de atenciones por Profesional y Fecha.");
	printf("\n\n");
	printf("\n5.- Cerrar la aplicacion");
	printf("\n\n");
	printf("\nIngrese una opcion: ");
	scanf("%d",&op);
	return op;
}

void iniciosesion(FILE *usuario1,Usuario user,bool &verificacion)
{
	char ingusuario[10];
	char ingcontra[10];
	int musuario=0;
	int mcontra=0;
	usuario1=fopen("Recepcionistas.dat","rb");
	
	printf("\nIngrese el nombre de usuario: ");
	_flushall();
	gets(ingusuario);
	printf("\nIngrese la contrasenia: ");
	_flushall();
	gets(ingcontra);
	
	fread(&user,sizeof(Usuario),1,usuario1);
	while(!feof(usuario1) and mcontra==0 and musuario==0)
	{
		if((strcmp(ingusuario,user.usuario)!=0) or (strcmp(ingcontra,user.contra)!=0)) //Cambio a uso de función strcmp()
		{
			fread(&user,sizeof(Usuario),1,usuario1);
		}
		else
		{
			mcontra=1;
			musuario=1;
		}
	}
	
	if(mcontra==0 or musuario==0)
	{
		printf("\nLos datos ingresados son incorrectos");
	}
	else
	{
		printf("\nInicio de sesion correcto.");
		verificacion=true;
	}
	
	fclose(usuario1);
}

void regpaciente(FILE *pacie1,paciente pacie2,bool &verificacion)
{
	if(verificacion==false)
	{
		printf("\nNo inicio sesion,volviendo al menu....");
	}
	else
	{
		pacie1=fopen("Clientes.dat","ab");
		
		printf("\nIngrese el nombre del paciente: ");
		_flushall();
		gets(pacie2.ApeyNom);
		printf("\nIngrese el domicilio del paciente: ");
		gets(pacie2.domicilio);
		printf("\nIngrese el DNI del paciente: ");
		scanf("%d",&pacie2.dni_paciente);
		printf("\nIngrese la localidad: ");
		_flushall();
		gets(pacie2.localidad);
		printf("\nIngrese la fecha de nacimiento del paciente.");
		printf("\nIngrese el dia: ");
		scanf("%d",&pacie2.fecha_nacimiento.dia);
		printf("\nIngrese el mes: ");
		scanf("%d",&pacie2.fecha_nacimiento.mes);
		printf("\nIngrese el anio: ");
		scanf("%d",&pacie2.fecha_nacimiento.anio);
		printf("\nIngrese el peso del paciente en KG: ");
		scanf("%f",&pacie2.peso); 
		printf("\nIngrese el telefono del paciente: ");
		_flushall();
		gets(pacie2.telefono);
		
		fwrite(&pacie2,sizeof(paciente),1,pacie1);
		fclose(pacie1);
	}
	
}

void regturnos(FILE *turno1,Turnos tur,FILE *med,Medicos meder,bool &verificacion)
{
	int b=0;
	int buscar;
	
	if(verificacion==false)
	{
		printf("\nNo inicio sesion,volviendo al menu....");
	}
	else
	{
		med=fopen("Profesionales.dat","rb");
		
		if(med==NULL)
		{
			system("cls");
			printf("\nERROR,No se encontro el archivo Medicoss.Contacte con el soporte");
			exit(1);
		}
		
		printf("Ingrese la matricula del Medicos: ");
		scanf("%d",&buscar);
		fread(&meder,sizeof(Medicos),1,med);
		while(!feof(med))
		{
			if(buscar==meder.matriculamedico)
			{
				b=1;
				
				break;
			}
			else
			{
				fread(&meder,sizeof(Medicos),1,med);
			}
		}

		fclose(med);

		if(b==0)
		{
			printf("\nMatricula de Medicos no encontrada.");
		}
		else
		{
			turno1=fopen("Turnos.dat","ab");

			tur.matriculamedico=meder.matriculamedico;
			printf("Ingrese el DNI del paciente: ");
			scanf("%d",&tur.dni_paciente);
			printf("\nIngrese la fecha del turno.");
			printf("\nIngrese el dia: ");
			scanf("%d",&tur.fecha_turno.dia);
			printf("\nIngrese el mes: ");
			scanf("%d",&tur.fecha_turno.mes);
			printf("\nIngrese el anio: ");
			scanf("%d",&tur.fecha_turno.anio);
			strcpy(tur.detalles_atencion,"vacio");
			tur.mostrado=0; //cambie a = 0
			printf("\n======Turno registrado======.");
			fwrite(&tur,sizeof(Turnos),1,turno1);

			fclose(turno1);
		}
	}
	
	
}

void listaten(FILE *turno1,Turnos tur,FILE *med,Medicos meder,bool &verificacion)
{
	if(verificacion==false)
	{
		printf("\nNo inicio sesion,volviendo al menu....");
	}
	else
	{
		turno1=fopen("Turnos.dat","rb");
		med=fopen("Profesionales.dat","rb");
		
		if(turno1==NULL or med==NULL)
		{
			printf("\nError al intentar abrir los archivos, contacte con el operador del sistema...");
			printf("\n\n");
			system("pause");
			exit(1);
		}
		
		fread(&tur,sizeof(Turnos),1,turno1);
	    fread(&meder,sizeof(Medicos),1,med);
		
		printf("\nListado de atenciones");
		printf("\nMedicos");
		printf("\t\tFecha del turno");
		printf("\n===========================================");
		
		while(!feof(turno1))
		{
			printf("\n\n");
			if(tur.matriculamedico==meder.matriculamedico)
			{
				printf("%s",meder.ApeyNom);
				printf("\t\t%d/%d/%d",tur.fecha_turno.dia,tur.fecha_turno.mes,tur.fecha_turno.anio);
			}
			fread(&tur,sizeof(Turnos),1,turno1);
	    	fread(&meder,sizeof(Medicos),1,med);		
		}
		fclose(turno1);
		fclose(med);
	}
}
