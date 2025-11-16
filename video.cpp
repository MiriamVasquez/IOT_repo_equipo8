#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
    VideoCapture cap(0); // Cámara por defecto
    if (!cap.isOpened()) {
        std::cerr << "Error: No se pudo abrir la cámara" << std::endl;
        return -1;
    }

    int width = 640;
    int height = 480;
    cap.set(CAP_PROP_FRAME_WIDTH, width);
    cap.set(CAP_PROP_FRAME_HEIGHT, height);

    // Configurar VideoWriter (AVI con XVID)
    VideoWriter writer("output.avi",
                       VideoWriter::fourcc('M','J','P','G'),
                       20.0,
                       Size(width, height));

    if (!writer.isOpened()) {
        std::cerr << "Error: No se pudo abrir el archivo de salida" << std::endl;
        return -1;
    }

    Mat frame;
    std::cout << "Grabando video... Presiona Ctrl+C para detener." << std::endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        writer.write(frame);
    }

    cap.release();
    writer.release();
    std::cout << "Video guardado en output.avi" << std::endl;
    return 0;
}
