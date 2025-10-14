#include <iostream>
#include "./camera/film.h"
#include "./camera/camera.h"
#include "./util/frame.h"
#include "./shape/Shape.h"
#include "./shape/sphere.h"
#include "./shape/plane.h"
#include "./shape/model.h"
#include "./shape/scene.h"
#include "./renderer/normal_renderer.h"
#include "./renderer/raytracing_renderer.h"


#define FILM_WIDTH 108
#define FILM_HEIGHT 72
#define SPP 128

int main()
{
    Film film(FILM_WIDTH, FILM_HEIGHT);
    // Camera camera(film, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 60.0f);
    Camera camera(film, glm::vec3(0.0f, 0.5f, 0.6f), glm::vec3(0.0f, 0.2f, 0.0f), 60.0f);
    Sphere sphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.5f);
    Plane plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    Model model(R"(../../asset/model/simple_dragon.obj)");

    Scene scene;
    scene.addShape(sphere,
        {glm::vec3(0.8f, 1.0f, 0.4f), true, glm::vec3(0.2f, 0.1f, 0.3f)},
               glm::vec3(0.3f, 0.1f, 0.0f), glm::vec3(0.3f), glm::vec3(0.0f, 0.0f, 0.0f));
    scene.addShape(plane,
        {glm::vec3(0.4f, 0.8f, 1.0f), false, glm::vec3(0.3f, 0.2f, 0.1f)},
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    scene.addShape(model,
        {glm::vec3(1.0f, 0.4f, 0.6f), false, glm::vec3(0.1f, 0.2f, 0.3f)},
            glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.6f), glm::vec3(0.0f, 120.0f, 0.0f));

    NormalRenderer normal_renderer(camera, scene);
    normal_renderer.render(SPP, R"(../../normal.ppm)");

    film.clear();

    RaytracingRenderer raytracing_renderer(camera, scene);
    raytracing_renderer.render(SPP, R"(../../raytracing.ppm)");

    return 0;
}
