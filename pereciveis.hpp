#ifndef PERECIVEIS_HPP
#define PERECIVEIS_HPP

struct ingrediente{
    int turno_validade;
};

struct heap{
    char cod_ingrediente;
    ingrediente *vetor;
    int tempo_preparo;
    int validade;
};

#endif