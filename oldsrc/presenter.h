#ifndef PRESENTER_H
#define PRESENTER_H

#include <string>
#include <memory>

#include "cluster.h"
#include "headnode.h"
#include "terminalui.h"
#include "viewSubscriber.h"
#include "viewTerminalUI.h"

class Presenter : public viewSubscriber {
public:
    Presenter(std::unique_ptr<viewTerminalUI> view,
              std::unique_ptr<Cluster> model);
    ~Presenter();

    void notifyEvent() override;

    /* TODO: Move to top to better organize */
private:
    std::unique_ptr<Cluster> m_model;
    std::unique_ptr<viewTerminalUI> m_view;


    //explicit Presenter (Cluster&);
    //~Presenter ();
};

#endif /* PRESENTER_H */
