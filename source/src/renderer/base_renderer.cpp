#include "./renderer/base_renderer.h"
#include "./thread/thread_pool.h"
#include "./util/progress.h"

void BaseRenderer::render(size_t spp, const std::filesystem::path &output_dir)
{
    size_t current_spp = 0;
    size_t increase = 1;

    Film& film = m_camera.getFilm();
    Progress progress(film.getWidth() * film.getHeight() * spp);

    while (current_spp < spp)
    {
        thread_pool.parallelFor(film.getWidth(), film.getHeight(), [&](size_t x, size_t y)
        {
            for (int i = 0; i < increase; ++i)
            {
                film.setPixel(x, y, renderPixel(glm::ivec2(x, y)));
            }

            progress.update(increase);
        });

        thread_pool.wait();

        current_spp += increase;
        increase = std::min<size_t>(current_spp, 32);

        film.save(output_dir);
    }
}
