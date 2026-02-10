#ifndef FRUTA_H
#define FRUTA_H

#include <string>

enum class TipoFruta {
    MELANCIA,
    UVA,
    MORANGO
};

inline std::string tipoFrutaParaString(TipoFruta tipo) {
    switch (tipo) {
        case TipoFruta::MELANCIA: return "Melancia";
        case TipoFruta::UVA: return "Uva";
        case TipoFruta::MORANGO: return "Morango";
        default: return "Desconhecido";
    }
}

#endif
