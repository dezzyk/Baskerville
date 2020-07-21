//
// Created by Feed on 7/20/2020.
//

#pragma once

#include "common.h"

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
