#include "util/color.h"

// 初始化颜色常量
const std::string Color::RESET = "\033[0m";

// 文本颜色
const std::string Color::BLACK = "\033[30m"; // 黑色
const std::string Color::RED = "\033[31m"; // 红色
const std::string Color::GREEN = "\033[32m"; // 绿色
const std::string Color::YELLOW = "\033[33m"; // 黄色
const std::string Color::BLUE = "\033[34m"; // 蓝色
const std::string Color::MAGENTA = "\033[35m"; // 品红色
const std::string Color::CYAN = "\033[36m"; // 青色
const std::string Color::WHITE = "\033[37m"; // 白色
const std::string Color::GRAY = "\033[90m";

// 亮色文本
const std::string Color::LIGHT_BLACK = "\033[90m"; // 亮黑色
const std::string Color::LIGHT_RED = "\033[91m"; // 亮红色
const std::string Color::LIGHT_GREEN = "\033[92m"; // 亮绿色
const std::string Color::LIGHT_YELLOW = "\033[93m"; // 亮黄色
const std::string Color::LIGHT_BLUE = "\033[94m"; // 亮蓝色
const std::string Color::LIGHT_MAGENTA = "\033[95m"; // 亮品红色
const std::string Color::LIGHT_CYAN = "\033[96m"; // 亮青色
const std::string Color::LIGHT_WHITE = "\033[97m"; // 亮白色

// 背景颜色
const std::string Color::BG_BLACK = "\033[40m"; // 背景黑色
const std::string Color::BG_RED = "\033[41m"; // 背景红色
const std::string Color::BG_GREEN = "\033[42m"; // 背景绿色
const std::string Color::BG_YELLOW = "\033[43m"; // 背景黄色
const std::string Color::BG_BLUE = "\033[44m"; // 背景蓝色
const std::string Color::BG_MAGENTA = "\033[45m"; // 背景品红色
const std::string Color::BG_CYAN = "\033[46m"; // 背景青色
const std::string Color::BG_WHITE = "\033[47m"; // 背景白色

// 文本样式
const std::string Color::BOLD = "\033[1m"; // 加粗
const std::string Color::UNDERLINE = "\033[4m"; // 下划线
const std::string Color::ITALIC = "\033[3m"; // 斜体

// 应用颜色到文本
std::string Color::colorize(const std::string& text, const std::string& colorCode)
{
    if (!supportsColor()) return text; // 如果终端不支持颜色，直接返回原文本

    return colorCode + text + RESET;
}

// 检查当前终端是否支持颜色输出
bool Color::supportsColor()
{
    // 简单实现：假设在Windows和Unix-like系统上都支持颜色
    // 更复杂的实现可以检查环境变量或终端类型
    return true;
}