# RUFFMAN
Este projeto consiste em um programa compactador/descompactador de arquivos usando o algoritmo de Ruffman através de CLI via terminal

## Dependências prévias
* O usuário deve ter o compilador GCC instalado e disponível no PATH para que o instalador compile o programa

## Instalação
### Linux
Ao executar o arquivo `install_linux.sh`, o programa será compilado para a pasta bin e o executável será copiado para o path padrão do usuário local para que o comando possa ser executado de qualquer diretório dentro do terminal, e um arquivo de desinstalação `uninstall_linux.sh` executável será criado.
### Windows
Ao executar o arquivo `install_windows.ps1`, o programa será compilado para a pasta bin e o caminho adicionado ao PATH do usuário, e um arquivo de desinstalação `uninstall_windows.ps1` executável será criado. No caso do instalador do windows, é provável que a execução de scripts esteja bloqueada pelo sistema por segurança. Você pode executar `powershell -ExecutionPolicy Bypass -File .\install_windows.ps1` para executar mesmo assim ou compile e adicione ao PATH manualmente conforme desejar.

## Como usar o programa
Após a instalação, você pode verificar o uso do programa com o comando `ruffman help`.

### Observações
* O comando passado para huffman é case insensitive, por exemplo:
```shell
ruffman help
ruffman HELP
ruffman UnZiP
ruffman Zip
```

todos são comandos válidos

* Atenção: o path de entrada e de o de saída não são case insensitive.

* Após a compactação de um arquivo, a taxa de compactação será exibida em porcentagem, calculada com a fórmula:
> (1.0 - tamanho_arquivo(arquivo_de_saída) / tamanho_arquivo(arquivo_de_saida)) * 100.0;

(caso a taxa seja negativa, quer dizer que o arquivo compactado ficou maior devido ao overhead da árvore de huffman e meta dados, o que não é algo difícil de ocorrer dado o fato que este é um algoritmo simples de compactação que terá melhor performance principalmente em arquivos de texto)