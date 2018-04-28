/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h> /* exit() */
#include <sys/types.h> /* define pid_t */
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h> /* fork() */

#define N_PROCESSOS 4

int eh_primo(unsigned long int );

int main() {
  pid_t pid[N_PROCESSOS];
  unsigned long int x[64];
  char c;
  int j;

  /* Definir flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Criar area de memoria compartilhada */
  int *num_primos;
  num_primos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

  j = 0;
  do{
    scanf("%li", &x[j]);
    c = getchar();
    j += 1;
  }while (c != '\n' && j < 64);

  *num_primos = 0;
  for(int i = 0; i < N_PROCESSOS; i++){
    pid[i] = fork();
    if (pid[i] == 0){
      for(int k = 0; i+k < j; k += 4){
        *num_primos += eh_primo(x[i+k]);
      }
      exit(0);
    }
  }
  
  for(int i = 0; i < N_PROCESSOS; i++){
    waitpid(pid[i], NULL, 0);
  }

  printf("%d\n", *num_primos);
  return 0;
}

int eh_primo(unsigned long int x){
  int response = 1;
  int i = 2;

  if (x < 2) response = 0;
  
  while(i <= x/2 && response == 1){
    if (x%i == 0)
      response = 0;
    i++;
  }
  
  return response;
}