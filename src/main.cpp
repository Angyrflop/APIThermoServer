// #include <crow.h>
#include <iostream>
// #include <nlohmann/json.hpp>
#include <arpa/inet.h>
#include <vector>
#include <algorithm>
#include <string>

// using json = nlohmann::json;

int main()
{

    // crow::SimpleApp app;

    // const float temperature{25.6f};
    // const float humidity{55.9f};
    // const bool isLightOn{true};

    // json j;
    // j["temperature"] = temperature;
    // j["humidity"] = humidity;
    // j["light"] = isLightOn;

    // CROW_ROUTE(app, "/")([]()
    // {
    //     auto page = crow::mustache::load_text("index.html");
    //     return page;
    // });

    // CROW_ROUTE(app, "/style.css")([](const crow::request&, crow::response& res)
    // {
    //     res.set_static_file_info("style.css");
    //     res.end();
    // });

    // CROW_ROUTE(app, "/script.js")([](const crow::request&, crow::response& res)
    // {
    //     res.set_static_file_info("script.js");
    //     res.end();
    // });

    // CROW_ROUTE(app, "/getReading/kitchen").methods(crow::HTTPMethod::GET)([j](){
    //     return j.dump(0);
    // });

    // CROW_ROUTE(app, "/getReading/bathroom").methods(crow::HTTPMethod::GET)([j](){
    //     return j.dump(0);
    // });

    // CROW_ROUTE(app, "/getReading/bedroom1").methods(crow::HTTPMethod::GET)([j](){
    //     return j.dump(0);
    // });

    // CROW_ROUTE(app, "/getReading/bedroom2").methods(crow::HTTPMethod::GET)([j](){
    //     return j.dump(0);
    // });

    // app.port(2048).multithreaded().run();
    return 0;
}