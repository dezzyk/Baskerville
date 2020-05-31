#pragma once

#include "common.h"

#include <optional>

namespace Meta {

    // Exclusively for use by types in the Meta namespace, do not manually manipulate out of risk of DRAGONS
    namespace Internals {

        extern u32 _id_incrementor;
        struct ID {
            inline ID() : value(_id_incrementor) {
                _id_incrementor++;
            }
            const u32 value;
        };

        template<class T>
        u32 getId() {
            static const Internals::ID id;
            return { id.value };
        }

    }

    struct Empty {};

    struct Type {
        u32 value = Internals::getId<Empty>();
    };

    template<class T>
    Type MakeType() {
        return { Internals::getId<T>() };
    };

    inline bool operator==(const Type& lhs, const Type& rhs) { return (lhs.value == rhs.value); }
    inline bool operator!=(const Type& lhs, const Type& rhs) { return !(lhs == rhs); }

}