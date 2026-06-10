void drawAsciiArt(const std::string& filename, int start_y, int start_x) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        mvprintw(start_y, start_x, "Failed to load ASCII art");
        return;
    }

    int y = start_y;

    while (std::getline(file, line)) {
        mvprintw(y++, start_x, "%s", line.c_str());
    }
}
