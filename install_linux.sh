#compila o executavel
gcc -o bin/ruffman \
    src/main.c \
    src/ruffman.c \
    src/utils.c \
    src/operations.c \
    -Isrc/includes/

#copia o executavel para o path do usuário para que ele possa
#chamar  o programa de qualquer diretório
cp bin/ruffman ~/.local/bin/

#cria o arquivo de desinstalação
printf "rm -f ~/.local/bin/ruffman\n\
        rm -f ./bin/* \n\
        rm -f ./uninstall_linux.sh" > ./uninstall_linux.sh