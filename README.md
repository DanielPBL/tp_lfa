#TP LFA

Autor: Daniel de Paula Braga Lopes

Matrícula: 201412040361

Para compilar o programa basta executar o comando "make".

Execução ./build/gerador \*.afd [-c]

Obs.: O parâmetro opicional '-c' serve para gerar AFDs completos (estado de erro
e' é adicionado para as transições inválidas).

Junto com os arquivos do programa existe uma pasta chamada "tests" que contém
um shell script para executar entradas de teste. Este executa o programa e gera os
PDFs para os .dot's resultantes.

Execução: ./run.sh \*.afd [-c]

Um conjunto de entradas de teste estão disponíveis dentro da pasta "tests"
