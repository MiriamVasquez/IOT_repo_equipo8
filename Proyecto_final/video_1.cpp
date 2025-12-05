

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace cv;

std::string getTimestamp() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: No se pudo abrir la cámara" << std::endl;
        return -1;
    }

    Ptr<BackgroundSubtractorMOG2> pBackSub = createBackgroundSubtractorMOG2(100, 8, false);
    Mat frame, fgMask;

    std::ofstream logFile("movimiento.log", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de log" << std::endl;
        return -1;
    }

    std::cout << "Detectando movimiento... Presiona ESC para salir." << std::endl;

    int frameCount = 0;
    long ruidoPromedio = 0;
    int umbral = 0;
    int contadorQuieto = 0;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Tasa de aprendizaje alta para actualizar fondo rápido
        pBackSub->apply(frame, fgMask, 0.01);

        frameCount++;

        // Filtrar ruido
        threshold(fgMask, fgMask, 100, 255, THRESH_BINARY);

        int movimiento = countNonZero(fgMask);

        // Auto-calibración durante los primeros 50 frames
        if (frameCount <= 50) {
            ruidoPromedio += movimiento;
            if (waitKey(30) == 27) break;
            continue;
        }
        if (frameCount == 51) {
            umbral = (ruidoPromedio / 50) * 1; // 3 veces el ruido inicial
            std::cout << "Umbral calibrado: " << umbral << std::endl;
        }

        // Mostrar valores para depuración
        std::cout << "Movimiento actual: " << movimiento << " Umbral: " << umbral << std::endl;

        // Detectar movimiento
        if (movimiento > umbral) {
            std::cout << "Movimiento detectado! (" << movimiento << " píxeles)" << std::endl;
            logFile << getTimestamp() << " - Movimiento detectado (" << movimiento << " píxeles)" << std::endl;
            contadorQuieto = 0;
        } else {
            contadorQuieto++;
            if (contadorQuieto > 10) {
                std::cout << "Escena estable nuevamente." << std::endl;
                logFile << getTimestamp() << " - Escena estable" << std::endl;
                contadorQuieto = 0;
            }
        }

        if (waitKey(30) == 27) break;
    }

    cap.release();
    logFile.close();
    return 0;
}
