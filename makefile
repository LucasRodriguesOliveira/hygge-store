CC = gcc
CFLAGS = -Wall -I include

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
EXEC = build/hygge

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build/*.o

# Comando para realizar backup dos arquivos de /data para /backup
backup:
	mkdir -p backup
	cp -r data/* backup/

# Comando para carregar os arquivos do backup para /data
load:
	mkdir -p data
	cp -r backup/* data/

# Comando para compilar, limpar os arquivos objeto e executar o programa
cnr:
	make && make clean && ./build/hygge

# Comando para remover todos os arquivos da pasta /data
clean_data:
	rm -rf data/*

.PHONY: all clean
