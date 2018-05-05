/*
 * Esta tarefa consiste em receber um texto na entrada e contar quantos números primos existem nela
 * utilizando multiprocessos.
 */

#include <stdio.h>
#include <stdlib.h> /* exit() */
#include <sys/types.h> /* define pid_t */
#include <sys/wait.h> /* wait process*/
#include <sys/mman.h> /* memory map*/
#include <unistd.h> /* fork() */

#define N_PROCESSOS 4
#define N_MAX 64

int is_prime(unsigned long int );

int main() {
  pid_t pid[N_PROCESSOS]; /* vetor de threads*/
  unsigned long int numbers[N_MAX]; /* armazena numeros a serem analisados*/
  char c; /*auxilia na leitura*/
  int j; /* armazena a contagem de numeros lidos*/

  /* Defini flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Cria area de memoria compartilhada */
  int *prime_numbers_amount, *result;
  prime_numbers_amount = (int*) mmap(NULL, 4*sizeof(int), protection, visibility, 0, 0);
  result = (int*) mmap(NULL, 4*sizeof(int), protection, visibility, 0, 0);

  /* Le no maximo N_MAX numeros inteiros sem sinal seguidos de um \n*/
  j = 0;
  do{
    scanf("%li", &numbers[j]);
    c = getchar();
    j += 1;
  }while (c != '\n' && j < N_MAX);

  /* Conta quantos numeros primos estao armazenados no vetor numbers de entradas em N_PROCESSOS processos paralelos*/
  for(int i = 0; i < N_PROCESSOS; i++){
    pid[i] = fork();
    if (pid[i] == 0){
      prime_numbers_amount[i] = 0;
      for(int k = 0; i+k < j; k += N_PROCESSOS){
        prime_numbers_amount[i] += is_prime(numbers[i+k]);
      }
      
      exit(EXIT_SUCCESS);
    }
  }
  
  *result = 0;
  /* Espera pelo fim dos N_PROCESSOS processos iniciados*/
  for(int i = 0; i < N_PROCESSOS; i++){
    waitpid(pid[i], NULL, 0);
    *result += prime_numbers_amount[i];
  }

  printf("%d\n", *result);
  return 0;
}

/* Determina se o numero dado eh primo*/
/* return 1 caso seja e 0 caso contrario*/
int is_prime(unsigned long int number){
  char response = 1;
  unsigned long int i = 2;

  if (number < 2) response = 0;
  
  while(i <= number/2 && response == 1){
    if (number%i == 0)
      response = 0;
    i++;
  }
  
  return (int)response;
}