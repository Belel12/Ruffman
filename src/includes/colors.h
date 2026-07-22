/*
Cabeçalho auxiliar que facilita a coloração 
e estilização do texto de saída no terminal.
Ex. sem macros definidas: "\033[31m Ola Mundo em Vermelho \033[0m"
Mesmo exemplo com as macros de auxílio: RED "Ola Mundo em Vermelho" RESET
*/
#ifndef COLORS_H

//Cores de texto
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

//Cores de background
#define BLACK_BG   "\033[40m"
#define RED_BG     "\033[41m"
#define GREEN_BG   "\033[42m"
#define YELLOW_BG  "\033[43m"
#define BLUE_BG    "\033[44m"
#define MAGENTA_BG "\033[45m"
#define CYAN_BG    "\033[46m"
#define WHITE_BG   "\033[47m"

//Estilos de texto extras
#define NEGRITO     "\033[1m"
#define FRACO       "\033[2m"
#define ITALICO     "\033[3m"
#define SUBLINHADO  "\033[4m"
#define PISCANDO    "\033[5m"
#define INVERTIDO   "\033[7m"
#define TACHADO     "\033[9m"

//Resetar as cores
#define RESET   "\033[0m"

#define COLORS_H
#endif