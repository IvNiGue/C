#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define G 6.6743e-11 //unidades: N m^2/kg^2

char eleccion;

typedef struct
{
    double comp_x;
    double comp_y;
}componente;

typedef struct
{
    double masa; //kg
    double radio; //metros
    componente pos; //metros
    componente vel; //velocidad m/s
    componente acl; //aceleracion m/s^2
}cuerpo;

double dist(componente p1, componente p2)
{
    return (sqrt( pow(p1.comp_x - p2.comp_x,2)+ pow(p1.comp_y - p2.comp_y,2)));
}

double calc_ang(double cx, double cy)
{
    if(cx > 0 && cy < 0)  return atan(cy/cx)+ 2.0*M_PI;
    if(cx > 0 && cy == 0)  return 0;
    if(cx > 0 && cy > 0)  return atan(cy/cx);
    if(cx == 0 && cy < 0)  return 3.0*M_PI_2;
    if(cx == 0 && cy == 0)  return 0;            //no debe ocurrir (es una colisión)
    if(cx == 0 && cy > 0)  return M_PI_2;
    if(cx < 0 && cy < 0)  return atan(cy/cx) + M_PI;
    if(cx < 0 && cy == 0)  return M_PI;
    if(cx < 0 && cy > 0)  return atan(cy/cx) + M_PI;

    printf("Algo salió mal\n");
    return 0;
}

int main()
{
    //archivos para el primer caso
    FILE* Archivo3;

    //archivos para el segundo caso
    FILE* Archivo;
    FILE* Archivo2;

    //archivos para el tercero caso
    FILE* Archivo4;
    FILE* Archivo5;
    FILE* Archivo6;

    double d_min = DBL_MAX;
    double f_pos_min, f_vel_min;

    int num_cuerpos=3; //numero que determina el numero de cuerpos que se va analizar
    cuerpo sistema_solar[num_cuerpos];

    //sistema_solar[0] ---> sol
    //sistema_solar[1] ---> tierra
    //sistema_solar[2] ---> luna

    printf("-----Interaccion Gravitacional----\n");
    printf("\n\n---------------------------\n\n\n");
    printf("----(Realizado por: Ivan Nieto Guerrero)----\n");
    printf("\n\n---------------------------\n\n\n");
    printf("Digite cualquier caracter para continuar\n");
    fflush(stdin);
    scanf("%c", &eleccion);

    do
    {
        system("cls");

        printf("Bienvenido, soy un programa destinado para la emulacion de una interaccion gravitacional \n\n");
        printf("(1. Si desea obtner las posiciones de la tierra en un año, digite A(Sol inmovil))\n");
        printf("(2. Si desea obtner las posiciones de la tierra y el sol en un año, digite S)\n");
        printf("(3. Si desea obtner las posiciones de la tierra, sol y la luna en un año, digite N)\n");
        printf("(4. Si desea salir del programa, digite S)\n\n");
        fflush(stdin);
        scanf("%c", &eleccion);

        switch(eleccion)
        {
        case 'A':
            {

                Archivo3 = fopen("Pos_tierra.txt", "wt");

                system("cls");

                printf("Usted desea que se realice el primer caso \n\n");

                for(double f_pos=0.90; f_pos<=1.10; f_pos+=0.01)
                {
                    for(double f_vel=0.90; f_vel<=1.10; f_vel+=0.01)
                    {
                        //cuerpo sol;
                        sistema_solar[0].masa = 1.989e30;
                        sistema_solar[0].radio = 696340000;
                        sistema_solar[0].pos.comp_x = 0;
                        sistema_solar[0].pos.comp_y = 0;
                        sistema_solar[0].vel.comp_x = 0;
                        sistema_solar[0].vel.comp_y = 0;
                        sistema_solar[0].acl.comp_x = 0;
                        sistema_solar[0].acl.comp_y = 0;

                        //cuerpo tierra;
                        sistema_solar[1].masa = 5.972e24;
                        sistema_solar[1].radio = 6370000;
                        sistema_solar[1].pos.comp_x = 1.4913e11;
                        sistema_solar[1].pos.comp_y = 0;
                        sistema_solar[1].vel.comp_x = 0;
                        sistema_solar[1].vel.comp_y = (2*M_PI*1.4913e11)/(365.25*24*60*60);

                        //cuerpo luna


                        sistema_solar[1].vel.comp_y *= f_vel;
                        sistema_solar[1].pos.comp_x *= f_pos;

                        //calcular la aceleracion resultante de la tierra

                        double year = 365.25*24*60*60;
                        double dt = year/10000;

                        componente pos_inicial = sistema_solar[1].pos;

                        for(double t = 0; t <= year; t+=dt)
                        {
                            if(dist(sistema_solar[1].pos, sistema_solar[0].pos) < (sistema_solar[1].radio+sistema_solar[0].radio))
                            {
                                printf("Colision detectada! \n");
                                break;
                            }

                            //fprintf(Archivo ,"%f \t %f \n", tierra.pos.comp_x, tierra.pos.comp_y);
                            //printf("%f \t %f \n", tierra.pos.comp_x, tierra.pos.comp_y);

                            double f_res = ((-G*sistema_solar[0].masa*sistema_solar[1].masa)/pow(dist(sistema_solar[1].pos,sistema_solar[0].pos),2));
                            double compx = sistema_solar[1].pos.comp_x - sistema_solar[0].pos.comp_x;
                            double compy = sistema_solar[1].pos.comp_y - sistema_solar[0].pos.comp_y;

                            double ang = calc_ang(compx, compy);

                            sistema_solar[1].acl.comp_x = (f_res * cos(ang))/ sistema_solar[1].masa;
                            sistema_solar[1].pos.comp_x = sistema_solar[1].pos.comp_x + (sistema_solar[1].vel.comp_x * dt) + (0.5 * sistema_solar[1].acl.comp_x * dt * dt);
                            sistema_solar[1].vel.comp_x = sistema_solar[1].vel.comp_x + (sistema_solar[1].acl.comp_x * dt);

                            sistema_solar[1].acl.comp_y = (f_res * sin(ang))/ sistema_solar[1].masa;
                            sistema_solar[1].pos.comp_y = sistema_solar[1].pos.comp_y + (sistema_solar[1].vel.comp_y * dt) + (0.5 * sistema_solar[1].acl.comp_y * dt * dt);
                            sistema_solar[1].vel.comp_y = sistema_solar[1].vel.comp_y + (sistema_solar[1].acl.comp_y * dt);

                        }

                        double distancia = dist(pos_inicial, sistema_solar[1].pos);

                        if(distancia<d_min)
                        {
                            d_min = distancia;
                            f_pos_min = f_pos;
                            f_vel_min = f_vel;
                            printf("%f, %f, %f\n", f_pos_min, f_vel_min, d_min);
                        }

                        }
                    }

                    //otra simulacion con el valor de los factores encontrados


                        //cuerpo tierra;
                        sistema_solar[1].masa = 5.972e24;
                        sistema_solar[1].radio = 6370000;
                        sistema_solar[1].pos.comp_x = 1.4913e11;
                        sistema_solar[1].pos.comp_y = 0;
                        sistema_solar[1].vel.comp_x = 0;
                        sistema_solar[1].vel.comp_y = (2*M_PI*1.4913e11)/(365.25*24*60*60);

                        sistema_solar[1].vel.comp_y *= f_vel_min;
                        sistema_solar[1].pos.comp_x *= f_pos_min;


                        //calcular la aceleracion resultante de la tierra

                        double year = 365.25*24*60*60;
                        double dt = year/100000;

                        int cont = 0;

                        for(double t = 0; t <= 5*year; t+=dt)
                        {
                            if(dist(sistema_solar[1].pos, sistema_solar[0].pos) < (sistema_solar[1].radio+sistema_solar[0].radio))
                            {
                                printf("Colision detectada! \n");
                                break;
                            }

                            //tierra
                            double f_res = ((-G*sistema_solar[0].masa*sistema_solar[1].masa)/pow(dist(sistema_solar[1].pos,sistema_solar[0].pos),2));
                            double compx = sistema_solar[1].pos.comp_x - sistema_solar[0].pos.comp_x;
                            double compy = sistema_solar[1].pos.comp_y - sistema_solar[0].pos.comp_y;

                            double ang = calc_ang(compx, compy);

                            sistema_solar[1].acl.comp_x = (f_res * cos(ang))/ sistema_solar[1].masa;
                            sistema_solar[1].pos.comp_x = sistema_solar[1].pos.comp_x + (sistema_solar[1].vel.comp_x * dt) + (0.5 * sistema_solar[1].acl.comp_x * dt * dt);
                            sistema_solar[1].vel.comp_x = sistema_solar[1].vel.comp_x + (sistema_solar[1].acl.comp_x * dt);

                            sistema_solar[1].acl.comp_y = (f_res * sin(ang))/ sistema_solar[1].masa;
                            sistema_solar[1].pos.comp_y = sistema_solar[1].pos.comp_y + (sistema_solar[1].vel.comp_y * dt) + (0.5 * sistema_solar[1].acl.comp_y * dt * dt);
                            sistema_solar[1].vel.comp_y = sistema_solar[1].vel.comp_y + (sistema_solar[1].acl.comp_y * dt);


                            if(cont%1000 == 0)
                            {

                                fprintf(Archivo3 ,"%f \t %f \n", sistema_solar[1].pos.comp_x, sistema_solar[1].pos.comp_y);
                                //fprintf(Archivo2 ,"%f \t %f \n", sistema_solar[0].pos.comp_x, sistema_solar[0].pos.comp_y);
                            }

                            cont++;


                }
                fclose(Archivo3);
                //fclose(Archivo4);

                printf("\nLas trayectorias ya se calcularon, y se guardaron en un archivo \n");
                printf("Si desea salir, digite S, caso contrario digite cualquier caracter \n");
                fflush(stdin);
                scanf("%c", &eleccion);

            }break;

            case 'S':
            {

                Archivo = fopen("Pos_tierra2.txt", "wt");
                Archivo2 = fopen("Pos_sol2.txt", "wt");

                system("cls");

                printf("Usted desea que se realice el segundo caso \n\n");

                for(double f_pos=0.90; f_pos<=1.10; f_pos+=0.01)
                {
                    for(double f_vel=0.90; f_vel<=1.10; f_vel+=0.01)
                    {
                        //cuerpo sol;
                        sistema_solar[0].masa = 1.989e30;
                        sistema_solar[0].radio = 696340000;
                        sistema_solar[0].pos.comp_x = 0;
                        sistema_solar[0].pos.comp_y = 0;
                        sistema_solar[0].vel.comp_x = 0;
                        sistema_solar[0].vel.comp_y = 0;
                        sistema_solar[0].acl.comp_x = 0;
                        sistema_solar[0].acl.comp_y = 0;

                        //cuerpo tierra;
                        sistema_solar[1].masa = 5.972e24;
                        sistema_solar[1].radio = 6370000;
                        sistema_solar[1].pos.comp_x = 1.4913e11;
                        sistema_solar[1].pos.comp_y = 0;
                        sistema_solar[1].vel.comp_x = 0;
                        sistema_solar[1].vel.comp_y = (2*M_PI*1.4913e11)/(365.25*24*60*60);

                        //cuerpo luna


                        sistema_solar[1].vel.comp_y *= f_vel;
                        sistema_solar[1].pos.comp_x *= f_pos;

                        //calcular la aceleracion resultante de la tierra

                        double year = 365.25*24*60*60;
                        double dt = year/10000;

                        componente pos_inicial = sistema_solar[1].pos;

                        for(double t = 0; t <= year; t+=dt)
                        {
                            if(dist(sistema_solar[1].pos, sistema_solar[0].pos) < (sistema_solar[1].radio+sistema_solar[0].radio))
                            {
                                printf("Colision detectada! \n");
                                break;
                            }

                            //fprintf(Archivo ,"%f \t %f \n", tierra.pos.comp_x, tierra.pos.comp_y);
                            //printf("%f \t %f \n", tierra.pos.comp_x, tierra.pos.comp_y);

                            double f_res = ((-G*sistema_solar[0].masa*sistema_solar[1].masa)/pow(dist(sistema_solar[1].pos,sistema_solar[0].pos),2));
                            double compx = sistema_solar[1].pos.comp_x - sistema_solar[0].pos.comp_x;
                            double compy = sistema_solar[1].pos.comp_y - sistema_solar[0].pos.comp_y;

                            double ang = calc_ang(compx, compy);

                            sistema_solar[1].acl.comp_x = (f_res * cos(ang))/ sistema_solar[1].masa;
                            sistema_solar[1].pos.comp_x = sistema_solar[1].pos.comp_x + (sistema_solar[1].vel.comp_x * dt) + (0.5 * sistema_solar[1].acl.comp_x * dt * dt);
                            sistema_solar[1].vel.comp_x = sistema_solar[1].vel.comp_x + (sistema_solar[1].acl.comp_x * dt);

                            sistema_solar[1].acl.comp_y = (f_res * sin(ang))/ sistema_solar[1].masa;
                            sistema_solar[1].pos.comp_y = sistema_solar[1].pos.comp_y + (sistema_solar[1].vel.comp_y * dt) + (0.5 * sistema_solar[1].acl.comp_y * dt * dt);
                            sistema_solar[1].vel.comp_y = sistema_solar[1].vel.comp_y + (sistema_solar[1].acl.comp_y * dt);

                        }

                        double distancia = dist(pos_inicial, sistema_solar[1].pos);

                        if(distancia<d_min)
                        {
                            d_min = distancia;
                            f_pos_min = f_pos;
                            f_vel_min = f_vel;
                            printf("%f, %f, %f\n", f_pos_min, f_vel_min, d_min);
                        }

                        }
                    }

                    //otra simulacion con el valor de los factores encontrados


                        //cuerpo tierra;
                        sistema_solar[1].masa = 5.972e24;
                        sistema_solar[1].radio = 6370000;
                        sistema_solar[1].pos.comp_x = 1.4913e11;
                        sistema_solar[1].pos.comp_y = 0;
                        sistema_solar[1].vel.comp_x = 0;
                        sistema_solar[1].vel.comp_y = (2*M_PI*1.4913e11)/(365.25*24*60*60);

                        sistema_solar[1].vel.comp_y *= f_vel_min;
                        sistema_solar[1].pos.comp_x *= f_pos_min;

                        //cuerpo sol;
                        sistema_solar[0].masa = 1.989e30;
                        sistema_solar[0].radio = 696340000;
                        sistema_solar[0].pos.comp_x = 0;
                        sistema_solar[0].pos.comp_y = 0;
                        sistema_solar[0].vel.comp_x = 0;
                        sistema_solar[0].vel.comp_y = -sistema_solar[1].masa*sistema_solar[1].vel.comp_y/sistema_solar[0].masa;
                        sistema_solar[0].acl.comp_x = 0;
                        sistema_solar[0].acl.comp_y = 0;


                        //calcular la aceleracion resultante de la tierra

                        double year = 365.25*24*60*60;
                        double dt = year/100000;

                        int cont = 0;

                        for(double t = 0; t <= 5*year; t+=dt)
                        {
                            if(dist(sistema_solar[1].pos, sistema_solar[0].pos) < (sistema_solar[1].radio+sistema_solar[0].radio))
                            {
                                printf("Colision detectada! \n");
                                break;
                            }

                            //tierra
                            double f_res = ((-G*sistema_solar[0].masa*sistema_solar[1].masa)/pow(dist(sistema_solar[1].pos,sistema_solar[0].pos),2));
                            double compx = sistema_solar[1].pos.comp_x - sistema_solar[0].pos.comp_x;
                            double compy = sistema_solar[1].pos.comp_y - sistema_solar[0].pos.comp_y;

                            double ang = calc_ang(compx, compy);

                            sistema_solar[1].acl.comp_x = (f_res * cos(ang))/ sistema_solar[1].masa;
                            sistema_solar[1].pos.comp_x = sistema_solar[1].pos.comp_x + (sistema_solar[1].vel.comp_x * dt) + (0.5 * sistema_solar[1].acl.comp_x * dt * dt);
                            sistema_solar[1].vel.comp_x = sistema_solar[1].vel.comp_x + (sistema_solar[1].acl.comp_x * dt);

                            sistema_solar[1].acl.comp_y = (f_res * sin(ang))/ sistema_solar[1].masa;
                            sistema_solar[1].pos.comp_y = sistema_solar[1].pos.comp_y + (sistema_solar[1].vel.comp_y * dt) + (0.5 * sistema_solar[1].acl.comp_y * dt * dt);
                            sistema_solar[1].vel.comp_y = sistema_solar[1].vel.comp_y + (sistema_solar[1].acl.comp_y * dt);

                            //sol
                            double f_res2 = ((G*sistema_solar[0].masa*sistema_solar[1].masa)/pow(dist(sistema_solar[1].pos,sistema_solar[0].pos),2));
                            double compx2 = sistema_solar[1].pos.comp_x - sistema_solar[0].pos.comp_x;
                            double compy2 = sistema_solar[1].pos.comp_y - sistema_solar[0].pos.comp_y;

                            double ang2 = calc_ang(compx2, compy2);

                            sistema_solar[0].acl.comp_x = (f_res2 * cos(ang2))/ sistema_solar[0].masa;
                            sistema_solar[0].pos.comp_x = sistema_solar[0].pos.comp_x + (sistema_solar[0].vel.comp_x * dt) + (0.5 * sistema_solar[0].acl.comp_x * dt * dt);
                            sistema_solar[0].vel.comp_x = sistema_solar[0].vel.comp_x + (sistema_solar[0].acl.comp_x * dt);

                            sistema_solar[0].acl.comp_y = (f_res2 * sin(ang2))/ sistema_solar[0].masa;
                            sistema_solar[0].pos.comp_y = sistema_solar[0].pos.comp_y + (sistema_solar[0].vel.comp_y * dt) + (0.5 * sistema_solar[0].acl.comp_y * dt * dt);
                            sistema_solar[0].vel.comp_y = sistema_solar[0].vel.comp_y + (sistema_solar[0].acl.comp_y * dt);

                            if(cont%1000 == 0)
                            {
                                //printf("contador: %d \n", cont);
                                //system("PAUSE");
                                fprintf(Archivo ,"%f \t %f \n", sistema_solar[1].pos.comp_x, sistema_solar[1].pos.comp_y);
                                fprintf(Archivo2 ,"%f \t %f \n", sistema_solar[0].pos.comp_x, sistema_solar[0].pos.comp_y);
                            }

                            cont++;


                }
                fclose(Archivo);
                fclose(Archivo2);

                printf("\nLas trayectorias ya se calcularon, y se guardaron en un archivo \n");
                printf("Si desea salir, digite S, caso contrario digite cualquier caracter \n");
                fflush(stdin);
                scanf("%c", &eleccion);

            }break;

            case 'N':
            {

                Archivo4 = fopen("Pos_tierra4.txt", "wt");
                Archivo5 = fopen("Pos_sol5.txt", "wt");
                Archivo6 = fopen("Pos_luna6.txt", "wt");

                system("cls");

                printf("Usted desea que se realice el tercer caso \n\n");

                for(double f_pos=0.90; f_pos<=1.10; f_pos+=0.01)
                {
                    for(double f_vel=0.90; f_vel<=1.10; f_vel+=0.01)
                    {
                        //cuerpo sol;
                        sistema_solar[0].masa = 1.989e30;
                        sistema_solar[0].radio = 696340000;
                        sistema_solar[0].pos.comp_x = 0;
                        sistema_solar[0].pos.comp_y = 0;
                        sistema_solar[0].vel.comp_x = 0;
                        sistema_solar[0].vel.comp_y = 0;
                        sistema_solar[0].acl.comp_x = 0;
                        sistema_solar[0].acl.comp_y = 0;

                        //cuerpo tierra;
                        sistema_solar[1].masa = 5.972e24;
                        sistema_solar[1].radio = 6370000;
                        sistema_solar[1].pos.comp_x = 1.4913e11;
                        sistema_solar[1].pos.comp_y = 0;
                        sistema_solar[1].vel.comp_x = 0;
                        sistema_solar[1].vel.comp_y = (2*M_PI*1.4913e11)/(365.25*24*60*60);


                        sistema_solar[1].vel.comp_y *= f_vel;
                        sistema_solar[1].pos.comp_x *= f_pos;

                        //calcular la aceleracion resultante de la tierra

                        double year = 365.25*24*60*60;
                        double dt = year/10000;

                        componente pos_inicial = sistema_solar[1].pos;

                        for(double t = 0; t <= year; t+=dt)
                        {
                            if(dist(sistema_solar[1].pos, sistema_solar[0].pos) < (sistema_solar[1].radio+sistema_solar[0].radio))
                            {
                                printf("Colision detectada! \n");
                                break;
                            }

                            double f_res = ((-G*sistema_solar[0].masa*sistema_solar[1].masa)/pow(dist(sistema_solar[1].pos,sistema_solar[0].pos),2));
                            double compx = sistema_solar[1].pos.comp_x - sistema_solar[0].pos.comp_x;
                            double compy = sistema_solar[1].pos.comp_y - sistema_solar[0].pos.comp_y;

                            double ang = calc_ang(compx, compy);

                            sistema_solar[1].acl.comp_x = (f_res * cos(ang))/ sistema_solar[1].masa;
                            sistema_solar[1].pos.comp_x = sistema_solar[1].pos.comp_x + (sistema_solar[1].vel.comp_x * dt) + (0.5 * sistema_solar[1].acl.comp_x * dt * dt);
                            sistema_solar[1].vel.comp_x = sistema_solar[1].vel.comp_x + (sistema_solar[1].acl.comp_x * dt);

                            sistema_solar[1].acl.comp_y = (f_res * sin(ang))/ sistema_solar[1].masa;
                            sistema_solar[1].pos.comp_y = sistema_solar[1].pos.comp_y + (sistema_solar[1].vel.comp_y * dt) + (0.5 * sistema_solar[1].acl.comp_y * dt * dt);
                            sistema_solar[1].vel.comp_y = sistema_solar[1].vel.comp_y + (sistema_solar[1].acl.comp_y * dt);

                        }

                        double distancia = dist(pos_inicial, sistema_solar[1].pos);

                        if(distancia<d_min)
                        {
                            d_min = distancia;
                            f_pos_min = f_pos;
                            f_vel_min = f_vel;
                            printf("%f, %f, %f\n", f_pos_min, f_vel_min, d_min);
                        }

                        }
                    }

                    //otra simulacion con el valor de los factores encontrados


                        //cuerpo tierra;
                        sistema_solar[1].masa = 5.972e24;
                        sistema_solar[1].radio = 6370000;
                        sistema_solar[1].pos.comp_x = 1.4913e11;
                        sistema_solar[1].pos.comp_y = 0;
                        sistema_solar[1].vel.comp_x = -(7.349e22*(2*M_PI*384400000)/(29.5*24*60*60))/sistema_solar[1].masa;
                        sistema_solar[1].vel.comp_y = (2*M_PI*1.4913e11)/(365.25*24*60*60);

                        sistema_solar[1].vel.comp_y *= f_vel_min;
                        sistema_solar[1].pos.comp_x *= f_pos_min;


                        //cuerpo luna
                        sistema_solar[2].masa = 7.349e22;
                        sistema_solar[2].radio = 1737400;
                        sistema_solar[2].pos.comp_x = 1.4913e11 + 384400000;
                        sistema_solar[2].pos.comp_y = 0;
                        sistema_solar[2].vel.comp_x = (2*M_PI*384400000)/(29.5*24*60*60);
                        sistema_solar[2].vel.comp_y = -(2*M_PI*1.4913e11)/(365.25*24*60*60);
                        sistema_solar[2].acl.comp_x = 0;
                        sistema_solar[2].acl.comp_y = 0;

                        //cuerpo sol;
                        sistema_solar[0].masa = 1.989e30;
                        sistema_solar[0].radio = 696340000;
                        sistema_solar[0].pos.comp_x = 0;
                        sistema_solar[0].pos.comp_y = 0;
                        sistema_solar[0].vel.comp_x = -sistema_solar[2].masa*sistema_solar[2].vel.comp_x/sistema_solar[0].masa;
                        sistema_solar[0].vel.comp_y = -sistema_solar[1].masa*sistema_solar[1].vel.comp_y/sistema_solar[0].masa;
                        sistema_solar[0].acl.comp_x = 0;
                        sistema_solar[0].acl.comp_y = 0;



                        //calcular la aceleracion resultante de la tierra

                        //variables de apoyo
                        double year = 365.25*24*60*60;
                        double dt = year/100000;
                        double f_aux[num_cuerpos-1], angul[num_cuerpos-1];
                        double compx, compy;
                        double sum_acl1, sum_acl2, consatnte = G;
                        int cordz;

                        sum_acl1 = 0;
                        sum_acl2 = 0;
                        cordz=0;

                        int cont = 0;

                        for(double t = 0; t <= 5*year; t+=dt)
                        {
                            if(dist(sistema_solar[1].pos, sistema_solar[0].pos) < (sistema_solar[1].radio+sistema_solar[0].radio))
                            {
                                printf("Colision detectada! \n");
                                break;
                            }

                            if(dist(sistema_solar[2].pos, sistema_solar[1].pos) < (sistema_solar[2].radio+sistema_solar[1].radio))
                            {
                                printf("Colision detectada! \n");
                                break;
                            }

                            //calculo de fuerzas resultantes


                            for(int cordx=0; cordx<num_cuerpos; cordx++)
                            {
                                for(int cordy=0; cordy<num_cuerpos; cordy++)
                                {
                                    if(cordy!=cordx)
                                    {

                                        //las reglas son otras de las cosas que se tienen que modificar para que sea 100% funcional
                                        //se sigue un tipo de patron para irse complementando, es el unico detalle que tiene el codigo
                                        //pd: el detalle de las reglas esta en si la consatnte es positiva o negativa
                                        if(cordx==1)
                                        {
                                            consatnte = consatnte*-1;
                                        }
                                        if(cordx==2 && cordy==0)
                                        {
                                            consatnte = consatnte*-1;
                                        }

                                        f_aux[cordz]= ((consatnte*sistema_solar[cordx].masa*sistema_solar[cordy].masa)/pow(dist(sistema_solar[cordy].pos,sistema_solar[cordx].pos),2));

                                        if(dist(sistema_solar[cordx].pos,sistema_solar[0].pos) >= dist(sistema_solar[cordy].pos,sistema_solar[0].pos))
                                        {
                                            compx = sistema_solar[cordx].pos.comp_x - sistema_solar[cordy].pos.comp_x;
                                            compy = sistema_solar[cordx].pos.comp_y - sistema_solar[cordy].pos.comp_y;
                                        }else if(dist(sistema_solar[cordx].pos,sistema_solar[0].pos) < dist(sistema_solar[cordy].pos,sistema_solar[0].pos))
                                        {
                                            compx = sistema_solar[cordy].pos.comp_x - sistema_solar[cordx].pos.comp_x;
                                            compy = sistema_solar[cordy].pos.comp_y - sistema_solar[cordx].pos.comp_y;
                                        }
                                        //fin de las reglas


                                        angul[cordz] = calc_ang(compx, compy);

                                        sum_acl1 = sum_acl1 + ((f_aux[cordz] * cos(angul[cordz]))/sistema_solar[cordx].masa);
                                        sum_acl2 = sum_acl2 + ((f_aux[cordz] * sin(angul[cordz]))/sistema_solar[cordx].masa);

                                        cordz++;
                                        consatnte = fabs(consatnte);
                                    }
                                }
                                cordz=0;
                                sistema_solar[cordx].acl.comp_x = sum_acl1;
                                sistema_solar[cordx].acl.comp_y = sum_acl2;
                                sum_acl1=0;
                                sum_acl2=0;
                            }

                            //nuevas velocidades y posiciones
                            for(int planeta=0; planeta<num_cuerpos; planeta++)
                            {
                                sistema_solar[planeta].pos.comp_x = sistema_solar[planeta].pos.comp_x + (sistema_solar[planeta].vel.comp_x * dt) + (0.5 * sistema_solar[planeta].acl.comp_x * dt * dt);
                                sistema_solar[planeta].vel.comp_x = sistema_solar[planeta].vel.comp_x + (sistema_solar[planeta].acl.comp_x * dt);

                                sistema_solar[planeta].pos.comp_y = sistema_solar[planeta].pos.comp_y + (sistema_solar[planeta].vel.comp_y * dt) + (0.5 * sistema_solar[planeta].acl.comp_y * dt * dt);
                                sistema_solar[planeta].vel.comp_y = sistema_solar[planeta].vel.comp_y + (sistema_solar[planeta].acl.comp_y * dt);
                            }


                            if(cont%1000 == 0)
                            {
                                fprintf(Archivo4 ,"%f \t %f \n", sistema_solar[1].pos.comp_x, sistema_solar[1].pos.comp_y);
                                fprintf(Archivo5 ,"%f \t %f \n", sistema_solar[0].pos.comp_x, sistema_solar[0].pos.comp_y);
                                fprintf(Archivo6 ,"%f \t %f \n", sistema_solar[2].pos.comp_x, sistema_solar[2].pos.comp_y);

                            }

                            cont++;


                }
                fclose(Archivo4);
                fclose(Archivo5);
                fclose(Archivo6);

                printf("\nLas trayectorias ya se calcularon, y se guardaron en un archivo \n");
                printf("Si desea salir, digite S, caso contrario digite cualquier caracter \n");
                fflush(stdin);
                scanf("%c", &eleccion);

            }break;
        }
    }

    while (eleccion!='S');



    return 0;
}
