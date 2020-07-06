//
// Created by Feed on 7/6/2020.
//

namespace ShaderSrc {
    inline constexpr const char* const msdf_vert =
#include "shader/msdf.vert"
        ;

    inline constexpr const char* const msdf_frag =
#include "shader/msdf.frag"
        ;

    inline constexpr const char* const pane_vert =
#include "shader/pane.vert"
        ;

    inline constexpr const char* const pane_frag =
#include "shader/pane.frag"
        ;
}