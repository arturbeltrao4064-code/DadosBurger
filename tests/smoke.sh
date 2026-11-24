#!/bin/bash
set -e

printf '1\nnkmpz\n' > pedidos_teste.txt
: > historico_teste.txt

{
    printf '1\\n'
    for i in {1..10}; do
        printf 'I\nn\nA\nn\nA\nk\nA\nm\nA\np\nA\nz\n'
    done
    printf 'Teste\n0\n'
} | ./build/dado_burger pedidos_teste.txt historico_teste.txt > saida_teste.txt

grep -q "VITORIA" saida_teste.txt
grep -q "Hash - hits: 9, misses: 1" saida_teste.txt
grep -q "Teste" historico_teste.txt
grep -q "10" historico_teste.txt
grep -q "RELATORIO DE DESEMPENHO" medicoes.log

printf 'Teste de partida concluido com sucesso.\n'
