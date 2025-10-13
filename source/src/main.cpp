#include <iostream>
#include <random>
#include "thread_pool.h"
#include "film.h"
#include "camera.h"
#include "frame.h"
#include "Shape.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "model.h"
#include "scene.h"

#define FILM_WIDTH 1080
#define FILM_HEIGHT 720

int main()
{
    ThreadPool thread_pool;
    std::atomic<int> pixel_count(0);
    Film film(FILM_WIDTH, FILM_HEIGHT);
    // Camera camera(film, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 60.0f);
    Camera camera(film, glm::vec3(0.0f, 0.5f, 0.6f), glm::vec3(0.0f, 0.2f, 0.0f), 60.0f);
    Sphere sphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.5f);
    Plane plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    Triangle triangle(glm::vec3(-0.5f, -0.5f, -1.0f), glm::vec3(0.5f, -0.5f, -1.0f), glm::vec3(0.0f, 0.5f, -1.0f));
    Model model(R"(../../asset/model/simple_dragon.obj)");

    Scene scene;
    scene.addShape(sphere,
        {glm::vec3(0.8f, 1.0f, 0.4f), true, glm::vec3(0.2f, 0.1f, 0.3f)},
               glm::vec3(0.3f, 0.1f, 0.0f), glm::vec3(0.3f), glm::vec3(0.0f, 0.0f, 0.0f));
    scene.addShape(plane,
        {glm::vec3(0.4f, 0.8f, 1.0f), false, glm::vec3(0.3f, 0.2f, 0.1f)},
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // scene.addShape(&triangle, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    scene.addShape(model,
        {glm::vec3(1.0f, 0.4f, 0.6f), false, glm::vec3(0.1f, 0.2f, 0.3f)},
            glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.6f), glm::vec3(0.0f, 120.0f, 0.0f));

    std::mt19937 gen(23451334); // 随机数生成器
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f); // 随机数分布
    const int spp = 128; // 采样次数

    thread_pool.parallelFor(film.getWidth(), film.getHeight(), [&](size_t x, size_t y)->void
    {
        // spp: 采样次数
        for (int i = 1; i < spp; ++i)
        {
            Ray ray = camera.generateRay(glm::vec2(x, y), glm::abs(glm::vec2(dis(gen), dis(gen)))); // 生成射线
            glm::vec3 beta = { 1.0f, 1.0f, 1.0f };
            glm::vec3 final_color = { 0.0f, 0.0f, 0.0f };

            // 光线追踪：https://raytracing.github.io/books/RayTracingInOneWeekend.html
            while (true)
            {
                std::optional<HitInfo> hit_info = scene.intersect(ray, 0.0f, 1000.0f);
                if (hit_info.has_value())
                {
                    final_color += beta * hit_info->material->m_emissive;
                    beta *= hit_info->material->m_albedo;

                    ray.m_origin = hit_info->position + hit_info->normal * 0.001f; // 避免与物体本身相交
                    Frame frame(hit_info->normal); // 计算反射方向
                    glm::vec3 light_direction;
                    if (hit_info->material->m_is_specular)
                    {
                        glm::vec3 view_dir = frame.world_to_screen(-ray.m_direction);
                        light_direction = glm::vec3(-view_dir.x, view_dir.y, -view_dir.z);
                    }
                    else
                    {
                        do{ light_direction = glm::vec3(dis(gen), dis(gen), dis(gen));
                        } while (glm::length(light_direction) > 1.0f);

                        // 半球采样
                        if (light_direction.y < 0.0f)
                            light_direction.y = -light_direction.y;
                    }
                    ray.m_direction = frame.world_to_screen(light_direction);
                }
                else { break; }
            }
            film.setPixel(x, y, final_color);
        }


        // std::optional<HitInfo> hit_info = scene.intersect(ray, 0.0f, 1000.0f);
        // if (hit_info.has_value())
        // {
        //     glm::vec3 light_pos(-1.0f, 2.0f, 1.0f);
        //     glm::vec3 light_dir = glm::normalize(light_pos - hit_info->position);
        //     glm::vec3 normal_dir = hit_info->normal;
        //     glm::vec3 diff = std::max(0.0f, glm::dot(normal_dir, light_dir)) * hit_info->material->m_albedo;
        //     glm::vec3 emissive = hit_info->material->m_emissive;
        //     glm::vec3 ambient = glm::vec3(0.1f);
        //     final_color = glm::clamp(diff + emissive + ambient, 0.0f, 1.0f);
        //     film.setPixel(x, y, final_color);
        // }

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
    film.save(R"(../../test.ppm)");

    return 0;
}
