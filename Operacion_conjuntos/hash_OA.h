#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //cstdint
#include <assert.h>

//para checar los bytes
#define YES 1
#define NO 0

#define VALID 1
#define NOTVALID 0
#define DELETED 2

#define MAX_SIZE 0.5
#define MIN_SIZE 0.05

//tamaños de la tabla hash
const uint32_t Hash_size[] = {11, 127, 131, 257, 521, 1031, 2053, 4099, 8209, 16411, 32771, 65537, 131101, 262147, 524309, 1048583, 2097169, 4194319, 8388617, 16777259, 33554467, 67108879, 134217757, 268435459, 536870923, 1073741827,2147483659};

const size_t sub_size[] = {7, 61, 127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749,65521, 131071, 262139, 524287, 1048573, 2097143, 4194301, 8388593, 16777213, 33554393, 67108859, 134217689, 268435399, 536870909, 1073741789, 2147483647};

//estructura de cada record o informacion a ingresar
typedef struct 
{
    void *bytes; //void es un tipo de varible global, que no le importa la infomacion (tipo) que llegue a ser
    size_t len;
    
}record;

typedef struct 
{
    int C1;
    int C2;
    int C3;
    
}business; //es una estructura de banderas de las emprsas/conjunto

//elemento, contiene su informacion, estatus y su respectiva llave
typedef struct
{
    record rec;
    char status;
    uint32_t key;
    business conjuntos; //guarda a que conjuntos pertenece el elemento
} hash_item;

typedef struct 
{
    hash_item *items; //arreglo lineal de hash items
    size_t n; //cantidad de elementos
    size_t n_del; //cantidad de elementos borrados
    size_t index_size; //posicion del hash size
    size_t size; //tamaño de la tabla
}HTable_OP;

//funcion adler 
const uint32_t MOD_ADLER = 65521;

uint32_t adler32(unsigned char *data, size_t len) 
{
    uint32_t a = 1, b = 0;
    size_t index;

    // Procesar cada byte de los datos en orden
    for (index = 0; index < len; ++index)
    {
        a = (a + data[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}

//creacion de la tabla
HTable_OP* newHTableCap_OP(size_t index){ 
    HTable_OP *HT = (HTable_OP*)malloc(sizeof(HTable_OP)*1);
    if(HT == NULL){
        fprintf(stderr, "no se pudo alojar memoria para la tabla");
        exit(1);
    }
    HT->size = Hash_size[index];
    HT->index_size = index;
    HT->n = 0;
    HT->n_del = 0;
    HT->items = (hash_item*)calloc(HT->size, sizeof(hash_item)); //malloc no inicializa la memoria, solo la reserva
    // la tabla ya esta incializada por calloc

    if(HT->items == NULL){
        fprintf(stderr, "no se pudo alojar memoria para la tabla");
        exit(1);
    }
    return HT;
}

HTable_OP *new_htable_OP(){
    return newHTableCap_OP(1);
}

//liberar memoria
void freehashitem(hash_item *item){
    if(item->rec.bytes != NULL){
        //printf("entro a borrar bytes \n");
        assert(item->rec.bytes != NULL);
        free(item->rec.bytes);
    }

    //free(item);
}

void freeHTable_OP(HTable_OP *HT){
    for(size_t i=0; i<HT->size; i++){
        if((HT->n + HT->n_del) != 0){
            freehashitem(&(HT->items[i]));
        }

    }

    assert(HT->items != NULL);
    free(HT->items);
    assert(HT != NULL);
    free(HT);

}

//funciones hash para moverse por la tabla
static inline size_t hashFunction_linear(uint32_t key, size_t hashSize, size_t collision){ //lo que hace el static inline es manter fijos los valores y que el compilador no este sobreescribiendo 
    return (key + collision) % hashSize;
}

static inline size_t hashFunction_cuadratic(uint32_t key, size_t hashSize, size_t collision){ //lo que hace el static inline es manter fijos los valores y que el compilador no este sobreescribiendo 
    return (key + (collision*collision)) % hashSize;
}

// double hash
static inline size_t sub_hash(uint32_t key, size_t subsize){
    return subsize - (key % subsize);
}

static inline size_t hashFunction_double(uint32_t key, size_t hashSize, size_t subsizex, size_t collision){ //lo que hace el static inline es manter fijos los valores y que el compilador no este sobreescribiendo 
    return (key + (collision * sub_hash(key, subsizex))) % hashSize;
}

//funcion para checar los bytes
int checkMatchBytes(record *A, record *B){
    if(A->len != B->len){
        return NO;
    }
    
    unsigned char *pA = (unsigned char*)A->bytes;
    unsigned char *pB = (unsigned char*)B->bytes;

    for(size_t i=0; i<A->len; i++){
        if(pA[i] != pB[i]){ 
            return NO;
        }

    }
    return YES;
}

//funcion para imprimir (general)

void printItem_OA(hash_item *item){
    
    if(item->status == VALID){
        char *str = (char*)item->rec.bytes;
        for(size_t i=0; i<item->rec.len; i++){
            printf("%c", str[i]);
        }
        printf("[%u]   ", item->key); //[%u] , item->key
    }
    
}

void printhash_OA(HTable_OP *HT){
    printf("------- Cantidad de elementos en la tabla: %ld -----\n\n", HT->n);
    for(size_t i=0; i<HT->size; i++){
        printf("%ld:  ", i);
        
        printItem_OA(&(HT->items[i]));

        printf("\n");
    }    

 }

 //funciones para el double hash

 hash_item *HTfindkey_OA_dh(HTable_OP **HT, uint32_t key){
    size_t collision = 0;
    int mark = 0;
    
    //while que va a buscar el item 
    while ((mark == 0) && (collision != (*HT)->size))
    {
        size_t index = hashFunction_double(key, (*HT)->size, sub_size[(*HT)->index_size],collision);

        hash_item current = (*HT)->items[index];

        if(current.status == NOTVALID){
            mark++;
            return NULL; //regresa null porque el elemento aun no es insertado
        }else if(current.status != NOTVALID && current.key == key){
            mark++;
            return &((*HT)->items[index]);
        }

        collision++;
    }
    return NULL;
}

hash_item *HTfindRecord_OA_dh(HTable_OP **HT, record *rec){
    uint32_t key = adler32((unsigned char*)rec->bytes,rec->len);
    hash_item *item = HTfindkey_OA_dh(HT, key);

    if(item == NULL){
        return NULL;
    }

    if(checkMatchBytes(rec, &(item->rec)) == YES){
        return item;
    }

    //en caso de que no coincidieran los bytes, se pone a buscar a ver si haya el item

    int mark=0;
    size_t collision=0;

    while ((mark == 0) && (collision != (*HT)->size))
    {
        size_t index = hashFunction_double(key, (*HT)->size, sub_size[(*HT)->index_size],collision);

        hash_item current = (*HT)->items[index];

        if(current.status == NOTVALID){
            mark++;
            return NULL; //regresa null porque el elemento aun no es insertado
        }else if(current.status != NOTVALID && current.key == key && (checkMatchBytes(rec, &(current.rec)) == YES)){
            mark++;
            return &((*HT)->items[index]);
        }

        collision++;
    }


}

//funcion para insertar ln
hash_item* HTinsertRecord_OA_dh(HTable_OP **HT, record *rec){
    hash_item *item =HTfindRecord_OA_dh(HT, rec);
    if(item != NULL){
        if(item->status == DELETED){
            item->status = VALID;
            
            (*HT)->n++;
            (*HT)->n_del--;
        }
        return item;
    }

    //el item no estaba previamente en la tabla, asi que toca insertarlo
    (*HT)->n++;

    uint32_t key = adler32((unsigned char*)rec->bytes,rec->len);
    size_t collision = 0;
    int mark = 0;
    
    //while que va a buscar el item 
    while ((mark == 0) && (collision != (*HT)->size))
    {
        size_t index = hashFunction_double(key, (*HT)->size, sub_size[(*HT)->index_size],collision);

        hash_item current = (*HT)->items[index];

        if(current.status == NOTVALID){
            mark++;

            current.key = key;
            current.status = VALID;
            current.rec.bytes = malloc(rec->len);
            current.conjuntos.C1 = 0;
            current.conjuntos.C2 = 0;
            current.conjuntos.C3 = 0;

            if(current.rec.bytes == NULL){
                fprintf(stderr, "No se pudo rervar memoria para el elemento \n"); 
                return NULL;
            }

            memcpy(current.rec.bytes,rec->bytes, rec->len);
            current.rec.len = rec->len;

            (*HT)->items[index] = current;

            return &((*HT)->items[index]); 
        }else if(current.status == DELETED){
            mark++;

            (*HT)->n_del--;

            current.status = VALID;
            current.key = key;

            if(current.rec.len < rec->len){
                current.rec.bytes = realloc(current.rec.bytes, rec->len);

                if(current.rec.bytes == NULL){
                    fprintf(stderr, "No se pudo rervar memoria para el elemento \n"); 
                    return NULL;
                }
            }
            current.rec.len = rec->len;
            memcpy(current.rec.bytes, rec->bytes, rec->len);

            (*HT)->items[index] = current;

            return &((*HT)->items[index]);
        }

        collision++;
    }


}

//funcion para borrar (ln)
void HTdeleteRecord_OA_dh(HTable_OP **HT, record *rec){
    hash_item *item = HTfindRecord_OA_dh(HT, rec);
    if(item == NULL){
        return;
    }

    item->status = DELETED;
    (*HT)->n--;
    (*HT)->n_del++;

}

//funcion rehash lineal
void reHash_OA_dh (HTable_OP **HT){
    float numerador = (*HT)->n + (*HT)->n_del;
    float denominador = (*HT)->size;
    float busy_memory = numerador/denominador;

    if(busy_memory >= MAX_SIZE){
        size_t newindex = (*HT)->index_size + 1;

        if(newindex > 26){
            return; //maximo de memoria
        }

        //creacion de la nueva tabla
        HTable_OP *newTable = newHTableCap_OP(newindex);
        
        for(size_t i=0; i<(*HT)->size; i++){
            hash_item *item = &((*HT)->items[i]);

            if(item->status == VALID){
                hash_item *dir = HTinsertRecord_OA_dh(&newTable, &(item->rec));
                dir->conjuntos = item->conjuntos;
            }

        }

        //liberar tabla antigua
        freeHTable_OP(*HT);

        //igualar el nuevo puntero
        *HT = newTable;

    }else if(busy_memory <= MIN_SIZE){
        size_t newindex = (*HT)->index_size - 1;

        if(newindex == -1){
            return; //minimo de memoria
        }

        //creacion de la nueva tabla
        HTable_OP *newTable = newHTableCap_OP(newindex);
        
        for(size_t i=0; i<(*HT)->size; i++){
            hash_item *item = &((*HT)->items[i]);

            if(item->status == VALID){
                hash_item *dir = HTinsertRecord_OA_dh(&newTable, &(item->rec));
                dir->conjuntos = item->conjuntos;
            }

        }

        //liberar tabla antigua
        freeHTable_OP(*HT);

        //igualar el nuevo puntero
        *HT = newTable;
    }
}

//funciones con crecimineto dinamico para ln
void OA_insertreHash_dh(HTable_OP **HT, record *rec){
    HTinsertRecord_OA_dh(HT, rec);

    reHash_OA_dh(HT);
}

void OA_deletereHash_dh(HTable_OP **HT, record *rec){
    HTdeleteRecord_OA_dh(HT, rec);

    reHash_OA_dh(HT);

}

// ---------------- FUNCIONES CON CONJUNTOS

//arreglo de cadenas
typedef struct {
    char str[100];
}array_str;


//IMPRIMIR

void printItem_OA_2(hash_item *item){
    
    if(item->status == VALID){
        char *str = (char*)item->rec.bytes;
        for(size_t i=0; i<item->rec.len; i++){
            printf("%c", str[i]);
        }
        printf("   "); //[%u] , item->key
    }
    
}

void print_c (HTable_OP *HT, int n_conj){
    printf("Conjunto [ %d ]: { ", n_conj);
    for(size_t i=0; i<HT->size; i++){
        
        if(n_conj == 1){
            if(HT->items[i].conjuntos.C1 == 1){
                printItem_OA_2(&(HT->items[i]));
            }
        }else if(n_conj == 2){
            if(HT->items[i].conjuntos.C2 == 1){
                printItem_OA_2(&(HT->items[i]));
            }
        }else{
            if(HT->items[i].conjuntos.C3 == 1){
                printItem_OA_2(&(HT->items[i]));
            }
        }

    }
    printf(" } \n"); 
}

hash_item * OA_insertreHash_dh2(HTable_OP **HT, record *rec){

    reHash_OA_dh(HT);

    return HTinsertRecord_OA_dh(HT, rec);
}

// UNION
record aux; //variable auxiliar para ponder cada elemento
void union_c (HTable_OP **HT, array_str **elems, int n_elem, int c_n){ //tabla hash, arreglo de strings, cantidad de elementos a insertar, n_conjunto
    
    for(int i=0; i<n_elem; i++){
        //record aux; //variable auxiliar para ponder cada elemento
        printf("%d \n", i);
        aux.bytes = elems[i]->str;
        printf("yes %d %d\n", i, strlen(elems[i]->str));
        aux.len = strlen(elems[i]->str);
        printf("yes %d \n", i);

        printf("Voy a intentar: %s nelem %d \n", aux.bytes, aux.len);
        system("pause");

        hash_item *dir_insert = OA_insertreHash_dh2(HT, &aux);
        free(aux.bytes);
        aux.bytes = NULL;

        printf("pase el insert: %p \n", dir_insert);
        /*if(c_n == 1){ //conjunto a marcar
            dir_insert->conjuntos.C1 = 1;
        }else if(c_n == 2){
            dir_insert->conjuntos.C2 = 1;
        }else if(c_n == 3){
            dir_insert->conjuntos.C3 = 1;
        }*/

        printf("-------- \n");
        
    }

    printf("Termine de insertar los elementos e la tabla \n");

    printhash_OA(*HT);

    print_c(*HT, c_n);

}

