CC = gcc
CFLAGS = -Wall -Wextra -std=c99
FONTES = banker.c leitura.c memoria.c simulador.c
CABECALHOS = banker.h leitura.h memoria.h simulador.h

banker: $(FONTES) $(CABECALHOS)
	$(CC) $(CFLAGS) -o banker $(FONTES)

clean:
	rm -f banker

.PHONY: clean