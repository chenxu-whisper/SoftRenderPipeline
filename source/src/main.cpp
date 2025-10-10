#include <iostream>
#include "thread_pool.h"
#include "film.h"
#include "camera.h"
#include "Shape.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "model.h"
#include "scene.h"

#define FILM_WIDTH 1080
#define FILM_HEIGHT 720
#define FILM_BACKGROUND_COLOR glm::vec3(0.0f, 0.0f, 0.0f)

int main()
{
    ThreadPool thread_pool;
    std::atomic<int> pixel_count(0);
    Film film(FILM_WIDTH, FILM_HEIGHT);
    // Camera camera(film, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 60.0f);
    Camera camera(film, glm::vec3(0.0f, 0.4f, 0.8f), glm::vec3(0.0f, 0.0f, 0.0f), 60.0f);
    Sphere sphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.5f);
    Plane plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    Triangle triangle(glm::vec3(-0.5f, -0.5f, -1.0f), glm::vec3(0.5f, -0.5f, -1.0f), glm::vec3(0.0f, 0.5f, -1.0f));
    Model model(R"(E:\ByteDance\Projects\Code\SoftRenderPipeline\asset\model\simple_dragon.obj)");

    Scene scene;
    scene.addShape(&sphere, glm::vec3(0.3f, 0.1f, 0.0f), glm::vec3(0.3f), glm::vec3(0.0f, 0.0f, 0.0f));
    scene.addShape(&plane, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // scene.addShape(&triangle, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    scene.addShape(&model, glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.6f), glm::vec3(0.0f, 120.0f, 0.0f));

    thread_pool.parallelFor(film.getWidth(), film.getHeight(), [&](size_t x, size_t y)->void
    {
        Ray ray = camera.generateRay(glm::vec2(x, y), glm::vec2(0.5f, 0.5f)); // 生成射线

        // 计算射线与场景中的物体的交点
        std::optional<HitInfo> hit_info = scene.intersect(ray, 0.0f, 1000.0f);
        if (hit_info.has_value())
        {
            glm::vec3 light_pos(-1.0f, 2.0f, 1.0f);
            glm::vec3 light_dir = glm::normalize(light_pos - hit_info->position);
            glm::vec3 normal_dir = hit_info->normal; // 将法线映射到 [0, 1] 范围
            float diff = std::max(0.0f, glm::dot(normal_dir, light_dir));
            float ambient = 0.1f;
            float final_color = glm::clamp(diff + ambient, 0.0f, 1.0f);
            film.setPixel(x, y, glm::vec3(final_color));
        }

        // 计算射线与三角形的交点
        // std::optional<HitInfo> hit_triangle = triangle.intersect(ray);
        // if (hit_triangle.has_value())
        // {
        //     film.setPixel(x, y, glm::vec3(1.0f, 0.0f, 0.0f));
        // }

        // 计算射线与平面的交点
        // std::optional<HitInfo> hit_plane = plane.intersect(ray);
        // if (hit_plane.has_value())
        // {
        //     film.setPixel(x, y, glm::vec3(1.0f, 0.0f, 1.0f));
        // }

        // 计算射线与球体的交点
        // std::optional<HitInfo> hit_sphere = sphere.intersect(ray); // 计算射线与球体的交点
        // if (hit_sphere.has_value())
        // {
        //     // glm::vec3 normal = glm::normalize(hit_sphere->position - sphere.m_center);
        //     glm::vec3 light_pos(-1.0f, 1.0f, 1.0f);
        //     glm::vec3 light_dir = glm::normalize(light_pos - hit_sphere->position);
        //     float diff = std::max(0.0f, glm::dot(hit_sphere->normal, light_dir));
        //     float ambient = 0.1f;
        //     film.setPixel(x, y, glm::vec3(diff+ambient));
        //
        //     // film.setPixel(x, y, glm::vec3(1.0f, 1.0f, 0.0f));
        // }

        int n= ++pixel_count;
        if (n % film.getWidth() == 0)
            std::cout << static_cast<float>(n) / (film.getWidth() * film.getHeight()) << std::endl;
    });

    thread_pool.wait();
    film.save(R"(E:\ByteDance\Projects\Code\SoftRenderPipeline\test.ppm)");

    return 0;
}
