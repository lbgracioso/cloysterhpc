//
// Created by Vinícius Ferrão on 11/02/22.
//

#include "slurm.h"

SLURM::SLURM() {
    setKind(QueueSystem::Kind::SLURM);
}