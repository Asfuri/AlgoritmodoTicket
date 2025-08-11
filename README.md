# Ticket Lock - Exclusão Mútua com Justiça

Este projeto implementa o algoritmo de Ticket Lock em C usando threads (`pthread`). O objetivo é garantir exclusão mútua, acesso justo e ordenado à seção crítica entre múltiplas threads.

## Descrição

O Ticket Lock é uma solução para o problema de exclusão mútua que utiliza um sistema de tickets para controlar o acesso à seção crítica. Cada thread recebe um ticket sequencial ao tentar entrar na seção crítica e só pode entrar quando seu ticket for o próximo a ser atendido.

### Propriedades Garantidas

- **Exclusão Mútua:** Apenas uma thread por vez acessa a seção crítica.
- **Acesso FIFO:** A ordem de atendimento segue a ordem dos tickets (primeiro que chega, primeiro que entra).
- **Ausência de Starvation:** Toda thread que pega um ticket será atendida, pois os tickets são incrementados sequencialmente.
- **Justiça (Fairness):** Todas as threads têm chances iguais de acessar a seção crítica.

## Estrutura do Projeto

- `main.c`: Código-fonte principal com implementação do Ticket Lock e testes com múltiplas threads.
- `README.md`: Este arquivo de documentação.

## Link do GitHub

[AlgoritmodoTicket](https://github.com/Asfuri/AlgoritmodoTicket)

## Como Compilar

```bash
gcc -pthread main.c -o ticketlock
```

## Como Executar

```bash
./ticketlock
```

## Saída Esperada

O programa exibe o acesso das threads à seção crítica, mostrando o ticket de cada uma, o valor do shared_resource e o tempo total de execução.
