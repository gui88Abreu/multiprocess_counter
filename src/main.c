/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
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
  pid_t pid[N_PROCESSOS];
  unsigned long int numbers[N_MAX];
  char c;
  int j;

  /* Definir flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Criar area de memoria compartilhada */
  int *prime_numbers_amount;
  prime_numbers_amount = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

  /* Ler no maximo N_MAX numeros inteiros sem sinal seguidos de um \n*/
  j = 0;
  do{
    scanf("%li", &numbers[j]);
    c = getchar();
    j += 1;
  }while (c != '\n' && j < N_MAX);

  /* Contar quantos numeros primos estao armazenados no vetor prime_number de entradas em N_PROCESSOS processos paralelos*/
  *prime_numbers_amount = 0;
  for(int i = 0; i < N_PROCESSOS; i++){
    pid[i] = fork();
    if (pid[i] == 0){
      
      for(int k = 0; i+k < j; k += N_PROCESSOS){
        *prime_numbers_amount += is_prime(numbers[i+k]);
      }
      
      exit(EXIT_SUCCESS);
    }
  }
  
  /* Esperar pelo fim dos N_PROCESSOS processos iniciados*/
  for(int i = 0; i < N_PROCESSOS; i++){
    waitpid(pid[i], NULL, 0);
  }

  printf("%d\n", *prime_numbers_amount);
  return 0;
}

/* Determinar se o numero dado eh primo*/
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