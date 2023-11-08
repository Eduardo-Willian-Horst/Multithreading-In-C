#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


    sem_t semaphore;
    int clientes_atendidos_sem_espera = 0;
    int clientes_nao_atendidos = 0;
    int atendidos_tatuador = 0;
    int atendidos_bodypiercer = 0;
    int puffs_tatuagem_ocupados = 0;
    int puffs_piercirg_ocupados = 0;
    int clientes_maximos = 0;
    int pufs_tatoo = 0;
    int pufs_piercing = 0;
    float prob_tatoo = 0;
    int temp_min_atendimento = 0;
    int temp_max_atendimento = 0;
    bool tatoo_atendendo = false;
    bool piercing_atendendo = false;

void *bodypiercer(void *qualquercoisa);
void *cliente(void *qualquercoisa);
void *tatuador(void *qualquercoisa);


int main(int argc, char *argv[]) {

    //Inicia o random
    srand(time(NULL));

    //Inicia o semaforo
    sem_init(&semaphore, 0, 1);

    //Verificacao de erro de uso
    if( argc != 7){
        printf("Uso incorreto. Tente: %s CLI PT PBP PAT MINATEN MAXATEN \n", argv[0]);
        return 1;
    }
    if(atoi(argv[1]) < 10 || atoi(argv[1]) > 100){
        printf("Uso incorreto! Número de clientes no dia deve ser entre 10 e 100.");
        return 1;
    }
    if(atoi(argv[2]) < 0 || atoi(argv[2]) > 10){
        printf("Uso incorreto! O número de puffs deve ser entre 0 e 10");
        return 1;
    }
    if(atoi(argv[3]) < 0 || atoi(argv[3]) > 10){
        printf("Uso incorreto! O número de puffs deve ser entre 0 e 10");
        return 1;
    }
    
    if(atoi(argv[5]) < 0 || atoi(argv[5]) > 100){
        printf("Uso incorreto! Tempo mínimo de atendimento de um profissional deve ser superior a 100");
        return 1;
    }
    if(atoi(argv[6]) < atoi(argv[5]) || atoi(argv[6]) > 500){
        printf("Uso incorreto! Tempo máximo de atendimento de um profissional deve ser superior a minima e inferior a 500");
        return 1;
    }
    srand(time(NULL));
    
    

    clientes_maximos = atoi(argv[1]);
    pufs_tatoo = atoi(argv[2]);
    pufs_piercing = atoi(argv[3]);
    prob_tatoo = atof(argv[4]);
    temp_min_atendimento = atoi(argv[5]);
    temp_max_atendimento = atoi(argv[6]);

    if(prob_tatoo < 0 || prob_tatoo > 0.9f){
        printf("Uso incorreto! Probabilidade de cliente para tatoo deve ser entre 0 e 0.99");
        return 1;
    }
    pthread_t client_id, tatto_id, piercing_id;


    pthread_create(&client_id, NULL, cliente, NULL);
    pthread_create(&tatto_id, NULL, tatuador, NULL);
    pthread_create(&piercing_id, NULL, bodypiercer, NULL);




    pthread_join(client_id, NULL);
    pthread_join(tatto_id, NULL);
    pthread_join(piercing_id, NULL);

    int clientes_totais_do_dia = atendidos_tatuador + atendidos_bodypiercer;

    printf("------------------------------------------------------------------\n");
    printf("Foram atendidos um total de %d pessoas.\n", clientes_totais_do_dia);
    printf("O tatuador atendeu um total de %d pessoas.\n", atendidos_tatuador);
    printf("O bodypircin atendeu um total de %d pessoas.\n", atendidos_bodypiercer);
    printf("Deixaram de ser atendidos um total de %d pessoas.\n", clientes_nao_atendidos);
    printf("Foram atendidos na hora um total de %d pessoas.\n", clientes_atendidos_sem_espera);




    return 0;
}


bool terminaExpediente(){
    if((atendidos_tatuador + atendidos_bodypiercer) >= clientes_maximos){
        return true; //aqui o expediente termina
    }else{
        return false; //aqui o expediente continua
    }
}



void *tatuador(void *qualquercoisa) {

    while( !terminaExpediente() || puffs_tatuagem_ocupados > 0 ){
        if(puffs_tatuagem_ocupados > 0){

            printf("O tatuador começou um atendimento. \n");
            sem_wait(&semaphore);//inicia a sessão crítica
            tatoo_atendendo = true;
            puffs_tatuagem_ocupados--;
            atendidos_tatuador++;
            sem_post(&semaphore);//termina a sessão crítica
            
            int tempo_atendimento = rand() % (temp_max_atendimento - temp_min_atendimento + 1) + temp_min_atendimento;
            usleep(tempo_atendimento);
            printf("O tatuador terminou o atendimento. \n");

            sem_wait(&semaphore);//inicia a sessão crítica
            tatoo_atendendo = false;
            sem_post(&semaphore);//termina a sessão crítica
        }
    }
    return NULL;
}

void *bodypiercer(void *qualquercoisa) {
    while( !terminaExpediente() || puffs_piercirg_ocupados > 0 ){
        if(puffs_piercirg_ocupados > 0){

            printf("O BodyPiercing começou um atendimento. \n");
            sem_wait(&semaphore);//inicia a sessão crítica
            piercing_atendendo = true;
            puffs_piercirg_ocupados--;
            atendidos_bodypiercer++;
            sem_post(&semaphore);//termina a sessão crítica
            
            int tempo_atendimento = rand() % (temp_max_atendimento - temp_min_atendimento + 1) + temp_min_atendimento;
            usleep(tempo_atendimento);
            printf("O BodyPiercing terminou o atendimento. \n");


            sem_wait(&semaphore);//inicia a sessão crítica
            piercing_atendendo = false;
            sem_post(&semaphore);//termina a sessão crítica
            
        }
    }
    return NULL;
}

void *cliente(void *qualquercoisa) {

    while(!terminaExpediente()){
        int chegada_de_cliente = rand() % (10 - 1 + 1) + 1;
        
        for( int i = 0; i < chegada_de_cliente; i++ ){
            float tipo_atendimento = (float) rand() / (float) RAND_MAX;
            if(tipo_atendimento > prob_tatoo){ //Caso caia aqui ele fara um piercing)
                if(piercing_atendendo){//aqui ele estara atendendo
                    if(puffs_piercirg_ocupados == pufs_piercing){
                        printf("Um cliente de Piercing chegou e foi embora pois não foi atendido e não podia sentar (não há pufs disponíveis).\n");
                        sem_wait(&semaphore);//inicia a sessão crítica
                        clientes_nao_atendidos++;
                        sem_post(&semaphore);//termina a sessão crítica
                    }else{
                        printf("Chegou e foi sentar num puf de espera.\n");
                        sem_wait(&semaphore);//inicia a sessão crítica
                        puffs_piercirg_ocupados++;
                        sem_post(&semaphore);//termina a sessão crítica
                    }
                }else{
                    printf("Chegou e foi atendido imediatamente.\n");
                    sem_wait(&semaphore);//inicia a sessão crítica
                    clientes_atendidos_sem_espera++;
                    puffs_piercirg_ocupados++;
                    sem_post(&semaphore);//termina a sessão crítica
                }
            }else{
                if(tatoo_atendendo){//aqui ele estara atendendo
                    if(puffs_tatuagem_ocupados == pufs_tatoo){
                        printf("Um cliente de Tatto chegou e foi embora pois não foi atendido e não podia sentar (não há pufs disponíveis).\n");
                        sem_wait(&semaphore);//inicia a sessão crítica
                        clientes_nao_atendidos++;
                        sem_post(&semaphore);//termina a sessão crítica
                    }else{
                        printf("Chegou e foi sentar num puf de espera.\n");
                        sem_wait(&semaphore);//inicia a sessão crítica
                        puffs_tatuagem_ocupados++;
                        sem_post(&semaphore);//termina a sessão crítica
                    }
                }else{
                    printf("Chegou e foi atendido imediatamente.\n");
                    sem_wait(&semaphore);//inicia a sessão crítica
                    clientes_atendidos_sem_espera++;
                    puffs_tatuagem_ocupados++;
                    sem_post(&semaphore);//termina a sessão crítica
                }
            }    
        }
        usleep(100);
    }
    return NULL;
}

