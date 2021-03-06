//
// Created by Feed on 6/28/2020.
//

#pragma once

#include "common.h"

namespace Draw {

    struct RedrawFlag {
        RedrawFlag() {}
        RedrawFlag(const RedrawFlag& other) {
            if(!val) {
                val = other.val;
            }
        }
        RedrawFlag(RedrawFlag&& other) noexcept {
            if(!val) {
                val = other.val;
            }
        }
        RedrawFlag(b32 v) {
            if(!val) {
                val = v;
            }
        }
        RedrawFlag& operator=(const RedrawFlag& other) {
            if(!val) {
                val = other.val;
            }
            return *this;
        }
        RedrawFlag& operator=(RedrawFlag&& other) {
            if(!val) {
                val = other.val;
            }
            return *this;
        }
        RedrawFlag& operator=(b32 v) {
            if(!val) {
                val = v;
            }
            return *this;
        }
        operator b32() const{
            return val;
        }
    private:
        b32 val = false;
    };

}