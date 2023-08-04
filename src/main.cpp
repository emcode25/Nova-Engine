#include <Eigen/Core>
#include <flecs.h>
#include <iostream>

namespace PGE
{
    struct Transform
    {
        Eigen::Vector3f position;
        Eigen::Vector3f rotation;
        Eigen::Vector3f scale;
    };
}

int main(void)
{
    flecs::world ecs;

    flecs::entity a = ecs.entity(); //Creates a new entity
    
    a.add<PGE::Transform>();
    a.set<PGE::Transform>({{1.0f, 2.0f, 3.0f}, {0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}});

    const PGE::Transform *t = a.get<PGE::Transform>();

    std::cout << "Pos: " << t->position.transpose() << std::endl;
    std::cout << "Rot: " << t->rotation.transpose() << std::endl;
    std::cout << "Sca: " << t->scale.transpose() << std::endl;

    a.remove<PGE::Transform>();
    a.destruct();

    return 0;
}