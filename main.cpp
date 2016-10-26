#include <iostream>
#include <fstream>
#include "Scene.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please provide scene and camera files" << std::endl;
        return 1;
    }

    ReadScene(argc, argv);

/*    for (const auto& camera : CurrentScene->Cameras())
    {
        Image img = camera.Render();

        ///////////////////////
        // BEWARE !
        // RUN YOUR TESTS HERE!
        ///////////////////////

        std::ofstream out(camera.OutputFile());
        out << img;
    }*/

    Vector3 deneme1, deneme2;

    deneme1._data[0] = 2;
    deneme1._data[1] = 3;
    deneme1._data[2] = 4;

    deneme2._data[0] = 5;
    deneme2._data[1] = 6;
    deneme2._data[2] = 7;

    cout << (deneme1 + deneme2)._data[0] << endl;

    return 0;
}