#pragma once

#include "common.h"

#include <optional>

// Exclusively for use by types in the Meta namespace, do not manually manipulate out of risk of DRAGONS
namespace _TypeIndexInternals {

    extern u32 _id_incrementor;
    struct ID {
        inline ID() : value(_id_incrementor) {
            _id_incrementor++;
        }
        const u32 value;
    };

    template<class T>
    u32 getId() {
        static const _TypeIndexInternals::ID id;
        return { id.value };
    }

}

struct TypeIndex {
    struct Empty {};
    template<class T>
    static TypeIndex make() {
        return { _TypeIndexInternals::getId<T>() };
    }
    u32 value = _TypeIndexInternals::getId<Empty>();
};

/*template<class T>
TypeIndex MakeTypeIndex() {
    return { Internals::getId<T>() };
};*/

inline bool operator==(const TypeIndex& lhs, const TypeIndex& rhs) { return (lhs.value == rhs.value); }
inline bool operator!=(const TypeIndex& lhs, const TypeIndex& rhs) { return !(lhs == rhs); }