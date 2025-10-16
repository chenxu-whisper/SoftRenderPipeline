#ifndef COLOR_H
#define COLOR_H

#include <string>

#define LOG_SUCCESS(str) (Color::GREEN + str + Color::RESET)
#define LOG_INFO(str) (Color::BLUE + str + Color::RESET)
#define LOG_ERROR(str) (Color::RED + str + Color::RESET)
#define LOG_WARNING(str) (Color::YELLOW + str + Color::RESET)
#define LOG_DEBUG(str) (Color::MAGENTA + str + Color::RESET)

// 用于终端输出颜色的工具类
class Color {
public:
    // 重置颜色到默认值
    static const std::string RESET;

    // 文本颜色
    static const std::string BLACK; // 黑色
    static const std::string RED; // 红色
    static const std::string GREEN; // 绿色
    static const std::string YELLOW; // 黄色
    static const std::string BLUE; // 蓝色
    static const std::string MAGENTA; // 品红色
    static const std::string CYAN; // 青色
    static const std::string WHITE; // 白色
    static const std::string GRAY; // 灰色

    // 亮色文本
    static const std::string LIGHT_BLACK; // 亮黑色
    static const std::string LIGHT_RED; // 亮红色
    static const std::string LIGHT_GREEN; // 亮绿色
    static const std::string LIGHT_YELLOW; // 亮黄色
    static const std::string LIGHT_BLUE; // 亮蓝色
    static const std::string LIGHT_MAGENTA; // 亮品红色
    static const std::string LIGHT_CYAN; // 亮青色
    static const std::string LIGHT_WHITE; // 亮白色

    // 背景颜色
    static const std::string BG_BLACK; // 背景黑色
    static const std::string BG_RED; // 背景红色
    static const std::string BG_GREEN; // 背景绿色
    static const std::string BG_YELLOW; // 背景黄色
    static const std::string BG_BLUE; // 背景蓝色
    static const std::string BG_MAGENTA; // 背景品红色
    static const std::string BG_CYAN; // 背景青色
    static const std::string BG_WHITE; // 背景白色

    // 文本样式
    static const std::string BOLD; // 加粗
    static const std::string UNDERLINE; // 下划线
    static const std::string ITALIC; // 斜体

    // 应用颜色到文本
    static std::string colorize(const std::string& text, const std::string& colorCode);

    // 检查当前终端是否支持颜色输出
    static bool supportsColor();
};

#endif // COLOR_H
