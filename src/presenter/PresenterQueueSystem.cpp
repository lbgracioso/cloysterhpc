//
// Created by Vinícius Ferrão on 02/03/22.
//

#include "PresenterQueueSystem.h"

PresenterQueueSystem::PresenterQueueSystem(
        std::unique_ptr<Cluster>& model,
        std::unique_ptr<Newt>& view)
        : Presenter(model, view) {

    m_model->setQueueSystem(
            magic_enum::enum_cast<QueueSystem::Kind>(
                    m_view->listMenu(Messages::title,
                                     Messages::question,
                                     magic_enum::enum_names<QueueSystem::Kind>(),
                                     Messages::help)).value());

    // TODO: Placeholder data
    auto fieldsSLURM = std::to_array<
            std::pair<std::string, std::string>>({
                    {Messages::SLURM::partition, "execution"}
            });

    auto listPBS = std::to_array<std::string_view>(
            // TODO: Should be an enum class inside PBS
            {"Shared", "Scatter"}
    );

    if (auto& queue = m_model->getQueueSystem()) {
        switch (queue.value()->getKind()) {
            case QueueSystem::Kind::None: {
                __builtin_unreachable();
            }

            case QueueSystem::Kind::SLURM: {
                fieldsSLURM = m_view->fieldMenu(Messages::SLURM::title,
                                                Messages::SLURM::question,
                                                fieldsSLURM,
                                                Messages::SLURM::help);

                const auto &slurm = dynamic_cast<SLURM*>(queue.value().get());
                slurm->setDefaultQueue(fieldsSLURM[0].second);
                LOG_INFO("Set SLURM default queue: {}", slurm->getDefaultQueue());

                break;
            }

            case QueueSystem::Kind::PBS: {
                const auto &execution = m_view->listMenu(
                        Messages::PBS::title,
                        Messages::PBS::question,
                        listPBS,
                        Messages::PBS::help);

                const auto &pbs = dynamic_cast<PBS*>(queue.value().get());
                pbs->setExecutionPlace(
                        magic_enum::enum_cast<PBS::ExecutionPlace>(execution).value());
                LOG_INFO("Set PBS Execution Place: {}",
                         magic_enum::enum_name<PBS::ExecutionPlace>(pbs->getExecutionPlace()));

                break;
            }
        }
    }
}