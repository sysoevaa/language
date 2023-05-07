//
// Created by А ну да on 07.05.2023.
//

#include "Executive.h"

void Executive::Jump() {
    _callStack.push(_pos);
    _pos = dynamic_cast<PolizJump*>(_cells[_pos])->pos;
}
