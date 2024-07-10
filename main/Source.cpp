#include "../includes.h"

using json = nlohmann::json;

struct Player {
    double x, y;
};

std::vector<Player> players;

DWORD GetPIDByWindowName(const std::string& windowName) {
    HWND hwnd = FindWindowA(NULL, windowName.c_str());
    if (!hwnd) {
        std::cerr << "Window not found!" << std::endl;
        return 0;
    }

    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
}

uintptr_t GetModuleBaseAddress(DWORD pid, const std::string& moduleName) {
    uintptr_t modBaseAddr = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnapshot, &modEntry)) {
            do {
                if (strcmp(moduleName.c_str(), modEntry.szModule) == 0) {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &modEntry));
        }
    }
    CloseHandle(hSnapshot);
    return modBaseAddr;
}

json positions_to_json() {
    json positions_json;
    json players_json = json::array();

    for (const auto& player : players) {
        json player_json;
        player_json["x"] = player.x;
        player_json["y"] = player.y;
        players_json.push_back(player_json);
    }

    positions_json["players"] = players_json;
    return positions_json;
}

std::string read_file(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
static bool opened = false;
int main() {
    const std::string windowName = "Rust";
    const std::string moduleName = "GameAssembly.dll";

    DWORD pid = GetPIDByWindowName(windowName);
    if (pid == 0) {
        std::cerr << "Failed to get PID." << std::endl;
        return 1;
    }
    std::cout << "PID: " << pid << std::endl;

    uintptr_t modBaseAddr = GetModuleBaseAddress(pid, moduleName);
    if (modBaseAddr == 0) {
        std::cerr << "Failed to get module base address." << std::endl;
        return 1;
    }
    std::cout << "Module Base Address: " << std::hex << modBaseAddr << std::endl;

    if (pid != 0 && modBaseAddr != 0 && opened == false) {
        system("start http://localhost:8080"); opened = true;
    }

    crow::SimpleApp app;

    CROW_ROUTE(app, "/update_positions").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400);
        }

        players.clear();
        for (const auto& player_json : body["players"]) {
            players.push_back({ player_json["x"].d(), player_json["y"].d() });
        }

        return crow::response(200);
        });

    CROW_ROUTE(app, "/get_positions").methods(crow::HTTPMethod::GET)([]() {
        return crow::response(positions_to_json().dump());
        });

    CROW_ROUTE(app, "/")([]() {
        return crow::response(read_file("radar.html"));
        });

    CROW_ROUTE(app, "/styles.css")([]() {
        crow::response res(read_file("styles.css"));
        res.set_header("Content-Type", "text/css");
        return res;
        });

    app.port(8080).multithreaded().run();
}
