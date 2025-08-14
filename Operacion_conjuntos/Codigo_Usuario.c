#include <stdio.h>
#include <stdlib.h>
#include "hash_OA.h"

//varibles para el usuario
char opcion[100] = "";
char elem[100] = "";

int main(){

    printf("\n --------------------Operaciones con conjuntos------------------------  \n");
    printf(" --------------------Presentado por: Ivan Nieto-----------------------  \n\n");

    HTable_OP *table = new_htable_OP();
    record rec;

    //system("pause");
    
    printf("Si desea insertar un elemento a un conjunto digite 'insert' \n");
    printf("Si desea buscar un elemento a un conjunto digite 'find' \n");
    printf("Si desea borrar un elemento a un conjunto digite 'delete' \n");
    printf("Si desea imprmir los conjuntos digite 'print' \n");
    printf("Si desea salir digite 'E' \n");

    scanf("%s", opcion);

    do{
        if(strcmp("insert", opcion) == 0){
            int num_conj, num_elem;

            system("cls");
            printf("Selecciono insertar \n");

            printf("A que empresa desea ingresar los productos? \n");
            printf("Empresa 1 (1) \n");
            printf("Empresa 2 (2) \n");
            printf("Empresa 3 (3) \n");
            scanf("%d", &num_conj);
            printf("\n");

            printf("Cuantos elementos va a insertar? \n");
            scanf("%d", &(num_elem));
            printf("\n");

            array_str *arr_elm = (array_str*)malloc(sizeof(array_str)*num_elem);

            for(int i=0; i<num_elem; i++){
                printf("Inserte el %d elemento: \n", i+1);
                scanf("%s", arr_elm[i].str);
            }
            printf("\n");

            for(int i=0; i<num_elem; i++){
                rec.bytes = arr_elm[i].str;
                rec.len = strlen(arr_elm[i].str);

                hash_item *dir= OA_insertreHash_dh2(&table, &rec);

                if(num_conj == 1){
                    dir->conjuntos.C1 = 1;
                }else if(num_conj == 2){
                    dir->conjuntos.C2 = 1;
                }else{
                    dir->conjuntos.C3 = 1;
                }
            }

            print_c(table, num_conj);

            printf("\n ---- \n");

            //printhash_OA(table);

            system("pause");

            free(arr_elm);


        }else if(strcmp("find", opcion) == 0){
            int num_elem;
            system("cls");
            printf("Selecciono buscar \n");

            printf("Cuantos elementos va a buscar? \n");
            scanf("%d", &(num_elem));
            printf("\n");

            array_str *arr_elm = (array_str*)malloc(sizeof(array_str)*num_elem);

            for(int i=0; i<num_elem; i++){
                printf("Ingrese el %d elemento: \n", i+1);
                scanf("%s", arr_elm[i].str);
            }
            printf("\n");

            //contadores para sumar banderas
            float cnt1 = 0, cnt2 = 0, cnt3 = 0;
            for(int i=0; i<num_elem; i++){
                
                rec.bytes = arr_elm[i].str;
                rec.len = strlen(arr_elm[i].str);

                hash_item *dir = HTfindRecord_OA_dh(&table, &rec);

                cnt1 += dir->conjuntos.C1;
                cnt2 += dir->conjuntos.C2;
                cnt3 += dir->conjuntos.C3;

                printf("El elemento %s esta en: ", arr_elm[i].str);
                if(dir->conjuntos.C1 == 1){
                    printf(" c1 ");
                }
                if(dir->conjuntos.C2 == 1){
                    printf(" c2 ");
                }
                if(dir->conjuntos.C3 == 1){
                    printf(" c3 ");
                }
                printf(" \n\n");

            }

            cnt1 /= num_elem;
            cnt2 /= num_elem;
            cnt3 /= num_elem;

            if(cnt1 == 1){
                printf("El conjunto 1 contiene todos los elementos \n");
            }
            if(cnt2 == 1){
                printf("El conjunto 2 contiene todos los elementos \n");
            }
            if(cnt3 == 1){
                printf("El conjunto 3 contiene todos los elementos \n");
            }

            free(arr_elm);

        }else if(strcmp("delete", opcion) == 0){
            int num_elem, num_conj;
            system("cls");
            printf("Selecciono borrar \n");

            printf("A que empresa desea quitar los productos? \n");
            printf("Empresa 1 (1) \n");
            printf("Empresa 2 (2) \n");
            printf("Empresa 3 (3) \n");
            scanf("%d", &num_conj);
            printf("\n");

            printf("Cuantos elementos va a borrar? \n");
            scanf("%d", &(num_elem));
            printf("\n");

            array_str *arr_elm = (array_str*)malloc(sizeof(array_str)*num_elem);

            for(int i=0; i<num_elem; i++){
                printf("Ingrese el %d elemento: \n", i+1);
                scanf("%s", arr_elm[i].str);
            }
            printf("\n");

            printf("Antiguo conjunto: \n");
            print_c(table, num_conj);
            printf("\n");

            for(int i=0; i<num_elem; i++){
                
                rec.bytes = arr_elm[i].str;
                rec.len = strlen(arr_elm[i].str);

                hash_item *dir = HTfindRecord_OA_dh(&table, &rec);

                if(num_conj == 1){
                    dir->conjuntos.C1 = 0;
                }else if(num_conj == 2){
                    dir->conjuntos.C2 = 0;
                }else{
                    dir->conjuntos.C3 = 0;
                }

            }

            printf("Nuevo conjunto \n");
            print_c(table, num_conj);

            free(arr_elm);

        }else if(strcmp("print", opcion) == 0){
            system("cls");
            printf("Los conjuntos son: \n\n");

            print_c(table, 1);
            print_c(table, 2);
            print_c(table, 3);
        }

        printf("\n");
        printf("Si desea insertar un elemento a un conjunto digite 'insert' \n");
        printf("Si desea buscar un elemento a un conjunto digite 'find' \n");
        printf("Si desea borrar un elemento a un conjunto digite 'delete' \n");
        printf("Si desea imprmir los conjuntos digite 'print' \n");
        printf("Si desea salir digite 'E' \n");

        scanf("%s", opcion);

    }while (strcmp("E", opcion) != 0);
    
    freeHTable_OP(table);

    printf("\n ------------------------------ FIN --------------------------------  \n");
    
    return 0;
}