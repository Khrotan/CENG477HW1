#include <iostream>
#include <fstream>
#include <Scene.h>
#include <Eigen/Dense>

using namespace Eigen;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Please provide scene and camera files" << std::endl;
        return 1;
    }

    MatrixXd m = MatrixXd::Random(3,3);
    cout << "m =" << endl << m << endl;
    m = m.inverse();
    cout << "m inverse" << endl << m << endl;

    ReadScene(argc, argv);

    for (const auto& camera : CurrentScene->Cameras())
    {
        Image img = camera.Render();

        ///////////////////////
        // BEWARE !
        // RUN YOUR TESTS HERE!
        ///////////////////////

        std::ofstream out(camera.OutputFile());
        out << img;
    }

    return 0;
}