#!/bin/bash
set -e

printf '1\nnkmpz\n' > pedidos_teste.txt
printf 'Ana\n3\nZeca\n8\n' > historico_teste.txt

{
    printf '1\n'
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

cp historico_teste.txt historico_original.txt
printf '3\n1\n3\n2\n0\n' | ./build/dado_burger pedidos_teste.txt historico_teste.txt > saida_historico.txt
cmp historico_teste.txt historico_original.txt
grep -q "Tempo do Quick Sort" saida_historico.txt
test "$(grep -c 'Ordenacao do historico' medicoes.log)" -eq 2

printf 'Teste de partida, cache, historico e Quick Sort concluido.\n'
