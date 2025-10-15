#include "./renderer/base_renderer.h"
#include "./thread/thread_pool.h"
#include "./util/progress.h"
#include "./util/profile.h"

void BaseRenderer::render(size_t spp, const std::filesystem::path &output_dir)
{
    PROFILE("Render " + std::to_string(spp) + "spp " + output_dir.string());

    size_t current_spp = 0; // 当前采样次数
    size_t increase = 1; // 每次增加的采样次数

    Film& film = m_camera.getFilm(); //
    Progress progress(film.getWidth() * film.getHeight() * spp); // 进度条

    // 渲染每个像素
    while (current_spp < spp)
    {
        thread_pool.parallelFor(film.getWidth(), film.getHeight(), [&](size_t x, size_t y)
        {
            for (int i = 0; i < increase; ++i)
                film.setPixel(x, y, renderPixel(glm::ivec2(x, y)));

            progress.update(increase); // 更新进度条
        });

        thread_pool.wait();

        current_spp += increase; // 增加采样次数
        increase = std::min<size_t>(current_spp, 32); // 每次增加的采样次数不能超过总采样次数

        film.save(output_dir); // 保存渲染结果
    }
}
