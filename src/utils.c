#include "utils.h"

#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

//retorna uma nova string em lower_case
char* to_lower(const char* string){
    if(string == NULL){
        return NULL;
    }
    char* new_string = calloc(strlen(string)+1,sizeof(char));
    for(size_t i = 0; i < strlen(string); i++){
        new_string[i] = tolower(string[i]);
    }
    return new_string; 
}

struct carregamento_options {
    char* message;
    atomic_uchar* flag_concluido;
};

//flag é obrigatória
LoadingOpt* new_LoadingOptions(char* message, atomic_uchar* flag_concluido){
    if(flag_concluido == NULL){
        return NULL;
    }
    LoadingOpt* newLO = (LoadingOpt*) malloc(sizeof(LoadingOpt));
    if(newLO == NULL){
        puts("ERRO AO CRIAR LoadingOpt");
        return NULL;
    }
    newLO->flag_concluido = flag_concluido;
    newLO->message= message;
    return newLO;
}

void free_LoadingOpt(LoadingOpt* ptr){
    if(!ptr) return;
    free(ptr->message);
    free(ptr);
}

void* tela_carregamento(void* load_options){
    if(load_options == NULL){
        puts("Erro ao iniciar tela de carregamento: Null load_options");
        return NULL;
    }
    LoadingOpt* options = (LoadingOpt*) load_options;
    char* loading_carrossel[] = {
        "|          |",
        "|>         |",
        "|-->       |",
        "|  -->     |",
        "|     -->  |",
        "|        --|",
    };

    int loading_carrosel_size = sizeof(loading_carrossel) / sizeof(char*);
    int stage = 0;
    while(!atomic_load(options->flag_concluido)){
        printf("\r%s %s",loading_carrossel[stage++ % loading_carrosel_size],options->message);
        fflush(stdout);
        sleep(1);
    }
    puts("\33[2K\r");
    return NULL;
}

//retorna o tamanho do arquivo em bytes
long tamanho_arquivo(FILE* arquivo){
    if(!arquivo){
        return 0;
    }
    long current = ftell(arquivo);
    fseek(arquivo,0,SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo,current,SEEK_SET);
    return tamanho;
}

//retorna quantos bytes ainda tem até o fim do arquivo
long bytes_til_EOF(FILE* file){
    if(!file) return 0;
    long pos_atual = ftell(file);
    fseek(file,0,SEEK_END);
    long restante = ftell(file) - pos_atual;
    fseek(file,pos_atual,SEEK_SET);
    return restante;
}

//retorna 1 caso o arquivo exista
int arquivo_existe(const char* path){
    if(!path) return 0;
    FILE* f = fopen(path,"r");
    int existe = (f == NULL)? 0 : 1;
    fclose(f);
    return existe;
}

//retorna um ponteiro para o nome do arquivo, ignorando diretorios do path
char* get_file_name(char* path){
    if(!path) return NULL;
    char* nome_isolado = strrchr(path,'/');
    return (nome_isolado)? nome_isolado+1 : path;
}

//retorna uma nova string alocada com o nome do arquivo com a extensão alterada
//a extensão deve ser inserida sem o '.' antes
char* change_file_extension(char* file_name, char* new_extension){
    if(! file_name){
        return NULL;
    }
    char* novo_nome = (char*) calloc(
        (strlen(file_name)+strlen(new_extension)+1) , sizeof(char)
    );
    strcpy(novo_nome,file_name);
    char* extensao = strrchr(novo_nome,'.');
    if(extensao){
        if(new_extension){
            extensao++;
            strcpy(extensao,new_extension);
        }
        else{
            *extensao = '\0';
        }
    }
    else{
        if(new_extension){
            strcpy(&novo_nome[strlen(novo_nome)],new_extension);
        }
    }

    return novo_nome;
    
}

//retorna o separador de diretório usado pelo sistema operacional do usuário
char get_os_path_separator(){
    #if defined(__WIN32) || defined(__WIN64)
        return '\\';
    #else
        return '/';
    #endif
}

//concatena a string do nome do arquivo à string do path
char* join_file_to_path(char* file_name, char* path){
    if(!file_name || !path){
        return NULL;
    }

    char* joined_string = (char*) malloc(
        (strlen(file_name)+strlen(path)+1) * sizeof(char)
    );

    strcpy(joined_string,path);
    
    char* last_character = &joined_string[strlen(path)-1];
    if(*last_character == get_os_path_separator()){
        last_character++;
        strcpy(last_character,file_name);
    }
    else{
        char separador = get_os_path_separator();
        *(++last_character) = separador;
        strcpy(++last_character,file_name);
    }
    return joined_string;
}

int is_directory(char* path){
    if(!path) return 0;

    #if defined(_WIN32) || defined(_WIN64)
        #define STAT _stat
        #define STAT_STRUCT struct _stat
    #else
        #define STAT stat
        #define STAT_STRUCT struct stat
    #endif

    STAT_STRUCT st;

    if (STAT(path, &st) != 0) {
        return 0;
    }

    #if defined(_WIN32) || defined(_WIN64)
        return (st.st_mode & _S_IFDIR) != 0;
    #else
        return S_ISDIR(st.st_mode);
    #endif
}
