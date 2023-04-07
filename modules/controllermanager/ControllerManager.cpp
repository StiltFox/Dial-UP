#include "ControllerManager.hpp"

using namespace std;
using namespace StiltFox::RestApi_Framework;
using namespace StiltFox::UniversalLibrary;

HttpMessage ControllerManager::dispatchMessage(HttpMessage request)
{
    HttpMessage output(404);

    if (registry.contains(request.requestUri))
    {
        if (registry.at(request.requestUri).contains(request.httpMethod))
        {
            output = registry[request.requestUri][request.httpMethod](request);
        }
    }

    return output;
}

void ControllerManager::registerController(string path, HttpMessage::Method method, function<HttpMessage(HttpMessage)> controller)
{
    registry[path][method] = controller;
}