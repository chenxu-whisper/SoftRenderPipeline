#include <iostream>
#include "thread_pool.h"
#include "film.h"
#include "camera.h"
#include "sphere.h"

#define FILM_WIDTH 1080
#define FILM_HEIGHT 720
#define FILM_BACKGROUND_COLOR glm::vec3(0.0f, 0.0f, 0.0f)

int main()
{
    ThreadPool thread_pool;
    Film film(FILM_WIDTH, FILM_HEIGHT);
    Camera camera(film, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 90.0f);
    Sphere sphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.5f);


    thread_pool.parallelFor(film.getWidth(), film.getHeight(), [&](size_t x, size_t y)->void
    {
        Ray ray = camera.generateRay(glm::vec2(x, y), glm::vec2(0.5f, 0.5f)); // 生成射线
        std::optional<float> hit = sphere.intersect(ray); // 计算射线与球体的交点
        if (hit.has_value())
            film.setPixel(x, y, glm::vec3(1.0f, 0.5f, 0.0f)); // 计算像素颜色
    });

    thread_pool.wait();
    film.save("E:\\ByteDance\\Projects\\Code\\SoftRenderPipeline\\test.ppm");

    return 0;
}
