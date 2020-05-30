//
// Created by Feed on 5/29/2020.
//

#include "baskerville.h"

void Baskerville::handleCodepoint(Event::Codepoint codepoint) {
    std::cout << codepoint.value << std::endl;
}

void Baskerville::handleMacro(Event::Macro macro) {

}

void Baskerville::handleResize(Event::Resize value) {

}

void Baskerville::draw() {

}
