# Dado Burger

Projeto em C++ para o trabalho prático final de Estruturas de Dados (INF01203).

## Compilar

O projeto usa CMake e C++17. No Linux:

```bash
cmake -S . -B build
cmake --build build
```

## Executar

Passe os arquivos de pedidos e histórico como argumentos:

```bash
./build/dado_burger dados/pedidos.txt dados/historico.txt
```

O programa não fixa os nomes dos arquivos. O primeiro argumento é o arquivo de pedidos e o segundo é o histórico. Se o arquivo de histórico ainda não existir, ele será criado vazio.

## Menu e comandos

- **1 — Novo jogo:** inicia uma partida com 10 clientes.
- **2 — Como jogar:** mostra as regras e os símbolos dos ingredientes.
- **3 — Histórico:** permite ordenar por nome ou por pontuação decrescente.
- **0 — Sair:** encerra o programa.

Durante a partida:

- **I:** preparar um ingrediente.
- **A:** adicionar um ingrediente ao hambúrguer.
- **R:** remover o ingrediente do topo.
- **E:** consultar o estoque, sem consumir turno.
- **P:** consultar o pedido atual, sem consumir turno.
- **S:** encerrar a partida voluntariamente.

Cada ação I, A ou R consome um turno. O cliente começa com 15 turnos de paciência. A partida termina quando todos os 10 clientes são atendidos ou quando a paciência do cliente atual chega a zero.

## Estruturas utilizadas

| Estrutura | Papel no jogo |
|---|---|
| AVL | Armazenar os pedidos e permitir busca pelo código |
| Lista contígua dinâmica | Manter o histórico de partidas |
| Tabela hash com encadeamento separado | Guardar receitas consultadas durante a partida |
| Lista encadeada | Armazenar os ingredientes de cada receita |
| Pilha encadeada | Montar o hambúrguer na ordem LIFO |
| Fila encadeada | Atender clientes na ordem de chegada |
| Heap mínimo | Controlar o vencimento de carne, ovo e bacon |
| Quick Sort | Ordenar uma cópia do histórico para exibição |

O código não utiliza contêineres prontos da STL para substituir as estruturas exigidas.

## Parâmetros adotados

- Clientes por partida: 10.
- Paciência inicial: 15 turnos por cliente.
- Validade de carne, ovo frito e bacon: 3 turnos.
- Tabela hash: 101 posições.
- Função hash: divisão, h(k) = k mod 101.
- Colisões: encadeamento separado.
- Ordenação do histórico: Quick Sort.

## Medições

O arquivo `medicoes.log` é criado ou sobrescrito a cada execução.

O programa registra:
- Nós inseridos, altura, rotações e tempo de carregamento da AVL.
- Quantidade de registros carregados no histórico.
- Comparações, número de buscas e tempo acumulado das buscas na AVL.
- Hits, misses, taxa de acerto, fator de carga e colisões da tabela hash.
- Tempo do Quick Sort sempre que o histórico é acessado.

A ordenação é feita sobre uma cópia do histórico, preservando a ordem dos registros no arquivo.

## Testes

O workflow do GitHub Actions compila o projeto, executa um teste automatizado de partida com 10 clientes, verifica cache e Quick Sort e testa o carregamento dos arquivos de desenvolvimento.

