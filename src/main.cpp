
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {

    // Verifica se os dois arquivos foram informados
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0]
                  << " <pedidos.txt> <historico.txt>\n";
        return 1;
    }

    // Abre o arquivo de pedidos
    std::ifstream arquivoPedidos(argv[1]);

    if (!arquivoPedidos.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de pedidos: "
                  << argv[1] << '\n';
        return 1;
    }

    // Abre o arquivo de histórico
    std::ifstream arquivoHistorico(argv[2]);

    if (!arquivoHistorico.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de histórico: "
                  << argv[2] << '\n';
        return 1;
    }

    std::cout << "Arquivos abertos com sucesso!\n";

    // O carregamento dos dados será integrado na próxima etapa.

    return 0;
}
