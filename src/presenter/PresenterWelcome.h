//
// Created by Vinícius Ferrão on 02/03/22.
//

#ifndef CLOYSTERHPC_PRESENTERWELCOME_H
#define CLOYSTERHPC_PRESENTERWELCOME_H

#include "Presenter.h"
#include "../const.h"

class PresenterWelcome : public Presenter {
private:
    struct Messages {
        struct Welcome {
            static constexpr const char *message =
                "Welcome to the guided installer of " PRODUCT_NAME ".\n\n" PRODUCT_NAME " is a software program to "
                "guide the user to set up an HPC clustered environment. It asks for specific questions to get a basic "
                "HPC cluster up and running as quick as possible.\n\nFor more information about the software, feature "
                "requests or bug reports please check its repository at " PRODUCT_URL;
        };
    };

public:
    PresenterWelcome(std::unique_ptr<Cluster>& model,
                     std::unique_ptr<Newt>& view);
};

#endif //CLOYSTERHPC_PRESENTERWELCOME_H
