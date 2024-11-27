# Compilador e flags
CC = gcc
CFLAGS = -Wall -I include -g

# Definindo os arquivos fonte, objetos e o executável
SRC = $(wildcard src/**/*.c) $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
EXEC = build/hygge

# Alvo principal
all: $(EXEC)

# Regra para compilar o executável a partir dos objetos
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para compilar cada arquivo .c em um arquivo .o correspondente
build/%.o: src/%.c | build
	mkdir -p $(dir $@)  # Cria o diretório build/subdirs se necessário
	$(CC) $(CFLAGS) -c $< -o $@

# Alvo para criar o diretório build, caso não exista
build:
	mkdir -p build

# Limpeza dos arquivos objeto
clean:
	rm -rf build/*.o build/**/*.o

# Comando para backup
backup:
	mkdir -p backup
	cp -r data/* backup/

# Comando para restaurar do backup
load:
	mkdir -p data
	cp -r backup/* data/

# Comando para limpar /data
clean_data:
	rm -rf data/*

# Comando para compilar, limpar e executar
cnr:
	make && make clean && ./build/hygge

.PHONY: all clean backup load clean_data cnr
