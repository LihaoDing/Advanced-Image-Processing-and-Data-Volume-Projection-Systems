/*
 * Group Name: Ukkonen
 * Members:
 * - Zeyu Zhao (@edsml-zz2123)
 * - Ark Saini (@acse-as12123)
 * - Lihao Ding (@acse-ld823)
 * - Geyu JI (@acse-gj23)
 * - Yanan Wang (@acse-yy3123)
 * - Chandrasekhar Gudipati (@edsml-cg1123)
 */

#include "Image.h"
#include "Volume.h"
#include <chrono>
#include <iostream>
#include <filesystem>


void Projection3D(int type, Volume volume, bool time) {
    std::string userInput;
    bool manual = false;
    int startIndex = 0;
    int endIndex = 0;
    // check if user wants to manually enter the start and end index
    while (true) {
        std::cout << "\nDo you want to manually enter the start and end index? (y/n)" << std::endl;
        std::cout << ">>>";
        std::getline(std::cin, userInput);
        if (userInput != "y" && userInput != "n") {
            std::cout << "\nInvalid input. Please try again." << std::endl;
            continue;
        } else {
            if (userInput == "y") {
                manual = true;
            }
            break;
        }
    }

    if (manual) {
        while (true) {
            std::cout << "\nPlease enter the start index." << std::endl;
            std::cout << ">>>";
            std::getline(std::cin, userInput);
            if (userInput.find_first_not_of("0123456789") != std::string::npos) {
                std::cout << "\nInvalid input. Please try again." << std::endl;
                continue;
            }
            if (std::stoi(userInput) <= 0) {
                std::cout << "\nInvalid input. Please try again." << std::endl;
                continue;
            }
            startIndex = std::stoi(userInput);
            break;
        }
        while (true) {
            std::cout << "\nPlease enter the end index." << std::endl;
            std::cout << ">>>";
            std::getline(std::cin, userInput);
            // check if the input is a positive number and it cannot be 0
            if (userInput.find_first_not_of("0123456789") != std::string::npos) {
                std::cout << "\nInvalid input. Please try again." << std::endl;
                continue;
            }
            if (std::stoi(userInput) <= 0) {
                std::cout << "\nInvalid input. Please try again." << std::endl;
                continue;
            }
            endIndex = std::stoi(userInput);
            break;
        }
    }
    
    if (type == 1) {
        std::cout << "\nPlease enter the output image path for the Maximum Intensity Projection." << std::endl;
    } else if (type == 2) {
        std::cout << "\nPlease enter the output image path for the Minimum Intensity Projection." << std::endl;
    } else if (type == 3) {
        std::cout << "\nPlease enter the output image path for the Average Intensity Projection." << std::endl;
    } else if (type == 4) {
        std::cout << "\nPlease enter the output image path for the Average Intensity Projection with Median." << std::endl;
    }
    std::cout << ">>>";
    std::getline(std::cin, userInput);
    bool success;
    auto starTime = std::chrono::high_resolution_clock::now();
    if (type == 1) {
        if (manual) {
            success = volume.MaxProjection(userInput, startIndex, endIndex);
        } else {
            success = volume.MaxProjection(userInput);
        }
    } else if (type == 2) {
        if (manual) {
            success = volume.MinProjection(userInput, startIndex, endIndex);
        } else {
            success = volume.MinProjection(userInput);
        }
    } else if (type == 3) {
        if (manual) {
            success = volume.AverageProjection(userInput, startIndex, endIndex);
        } else {
            success = volume.AverageProjection(userInput);
        }
    } else if (type == 4) {
        if (manual) {
            success = volume.AverageProjectionMedian(userInput, startIndex, endIndex);
        } else {
            success = volume.AverageProjectionMedian(userInput);
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
    if (time) {
        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
    }
    if (success) {
        std::cout << "\nProjection generated successfully." << std::endl;
    } else {
        std::cout << "\nProjection failed to generate. Try again!" << std::endl;
    }
}


void Slice3D(Volume volume, bool time) {
    std::string userInput;
    int sliceIndex = 0;
    while (true) {
        std::cout << "\nPlease enter the slice plane (YZ, XZ)." << std::endl;
        std::cout << ">>>";
        std::getline(std::cin, userInput);
        if (userInput != "YZ" && userInput != "XZ") {
            std::cout << "\nInvalid input. Please try again." << std::endl;
            continue;
        } else {
            break;
        }
    }
    SlicePlane plane;
    if (userInput == "YZ") {
        plane = SlicePlane::YZ;
    } else if (userInput == "XZ") {
        plane = SlicePlane::XZ;
    }

    while (true) {
        std::cout << "\nPlease enter the slice index." << std::endl;
        std::cout << ">>>";
        std::getline(std::cin, userInput);
        if (userInput.find_first_not_of("0123456789") != std::string::npos) {
            std::cout << "\nInvalid input. Please try again." << std::endl;
            continue;
        }
        if (std::stoi(userInput) <= 0) {
            std::cout << "\nInvalid input. Please try again." << std::endl;
            continue;
        }
        sliceIndex = std::stoi(userInput);
        break;
    }

    std::cout << "\nPlease enter the output image path for the slice." << std::endl;
    std::cout << ">>>";
    std::getline(std::cin, userInput);
    auto starTime = std::chrono::high_resolution_clock::now();
    bool success = volume.slice3DVolume(plane, sliceIndex, userInput);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
    if (time) {
        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
    }

    if (success) {
        std::cout << "\nSlice generated successfully." << std::endl;
    } else {
        std::cout << "\nSlice failed to generate. Try again!" << std::endl;
    }
}


int main() {
    Image image = Image();
    Volume volume = Volume();
    namespace fs = std::filesystem;
    int model = 0;
    std::string userInput;
    bool time = true; // check if user wants to see the time taken by the function

    while(true) {
        if (model == 0) {
            std::cout << "\nPlease enter the model you want to use" << std::endl;
            std::cout << "\n'3d' for 3D model" << std::endl;
            std::cout << "'2d' for 2D model" << std::endl;
            std::cout << ">>>";
            std::getline(std::cin, userInput);
            if (userInput == "3d") {
                model = 3;
            }
            else if (userInput == "2d") {
                model = 2;
            }
            else {
                std::cout << "\nInvalid input. Please try again." << std::endl;
                continue;
            }
        }

        if (model == 2 && image.getExist() == 0) {
            while (true) {
                std::cout << "\nPlease add a photo(path) to start the canvas." << std::endl;
                std::cout << ">>>";
                std::getline(std::cin, userInput);
                bool success = image.loadImage(userInput);
                if (success) {
                    std::cout << "\nImage loaded successfully." << std::endl;
                    break;
                } else {
                    std::cout << "\nImage failed to load. Try again!" << std::endl;
                }
            }
        }

        if (model == 2) {
            while (true) {
                std::cout << "\n============================MODEL2D MENU================================" << std::endl;
                std::cout << "|             For the current loaded Image:                            |" << std::endl;
                std::cout << "|             ENTER 'check' to look the data path & data info.         |" << std::endl;
                std::cout << "|             ENTER 'reload' to reload the image.                      |" << std::endl;
                std::cout << "|             ENTER 'save' to save the current image.                  |" << std::endl;
                std::cout << "|             ENTER '3d' to change to 3d model.                        |" << std::endl;
                std::cout << "|             ENTER 'exit' to quit the project.                        |" << std::endl;
                std::cout << "|----------------------------------------------------------------------|" << std::endl;
                std::cout << "|             ENTER 'gray' to apply grayscale.                         |" << std::endl;
                std::cout << "|             ENTER 'bright' to apply brightness.                      |" << std::endl;
                std::cout << "|             ENTER 'hist' to apply Histogram equalisation.            |" << std::endl;
                std::cout << "|             ENTER 'salt' to apply salt and pepper noise.             |" << std::endl;
                std::cout << "|             ENTER 'threshold' to apply threshold.                    |" << std::endl;
                std::cout << "|----------------------------------------------------------------------|" << std::endl;
                std::cout << "|             ENTER 'gaussian' to apply GaussianFilter.                |" << std::endl;
                std::cout << "|             ENTER 'median' to apply MedianFilter.                    |" << std::endl;
                std::cout << "|             ENTER 'box' to apply boxFilter.                          |" << std::endl;
                std::cout << "|----------------------------------------------------------------------|" << std::endl;
                std::cout << "|             ENTER 'sobel' to apply sobelDetection.                   |" << std::endl;
                std::cout << "|             ENTER 'prewitt' to apply prewittDetection.               |" << std::endl;
                std::cout << "|             ENTER 'scharr' to apply scharrDetection.                 |" << std::endl;
                std::cout << "|             ENTER 'roberts' to apply robertsCrossDetection.          |" << std::endl;
                std::cout << "========================================================================" << std::endl;
                std::cout << "\n>>>";
                std::getline(std::cin, userInput);
                if (userInput == "check") {
                    std::cout << "\nThe current data path is: " << image.getPath() << std::endl;
                    std::cout << "The size of the image is: " << image.getWidth() << " * " << image.getHeight() << " * " << image.getChannels() << std::endl;
                } else if (userInput == "reload") {
                    while (true) {
                        std::cout << "\nPlease add a photo(path) to start the canvas." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        bool success = image.loadImage(userInput);
                        if (success) {
                            std::cout << "\nImage loaded successfully." << std::endl;
                            break;
                        } else {
                            std::cout << "\nImage failed to load. Try again!" << std::endl;
                        }
                    }
                } else if (userInput == "3d") {
                    model = 3;
                    break;
                } else if (userInput == "exit") {
                    return 0;
                } else if (userInput == "gray") {
                    bool success = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.Grayscale();
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.Grayscale();
                    }
                    if (success) {
                        image.saveImage(userInput);
                        std::cout << "\nApply grayscale successfully." << std::endl;
                    } else {
                        std::cout << "\nGrayscale failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "save") {
                    std::cout << "\nPlease determine the output path for the current image." << std::endl;
                    std::cout << ">>>";
                    std::getline(std::cin, userInput);
                    int success = image.saveImage(userInput);
                    if (success) {
                        std::cout << "\nImage saved successfully." << std::endl;
                    } else {
                        std::cout << "\nImage failed to save. Try again!" << std::endl;
                    }
                } else if (userInput == "bright") {
                    while (true) {
                        std::cout << "\nPlease enter the brightness value." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        bool isValid = true;
                        for (size_t i = 0; i < userInput.size(); ++i) {
                            if (i == 0 && userInput[i] == '-') {
                                if (userInput.size() == 1) {
                                    isValid = false;
                                    break;
                                }
                                continue;
                            }
                            if (userInput[i] < '0' || userInput[i] > '9') {
                                isValid = false;
                                break;
                            }
                        }
                        if (!isValid) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        if (std::stoi(userInput) < -255 || std::stoi(userInput) > 255) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        break;
                    }
                    bool success = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.Brightness(std::stoi(userInput));
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.Brightness(std::stoi(userInput));
                    }
                    if (success) {
                        std::cout << "\nApply brightness successfully." << std::endl;
                    } else {
                        std::cout << "\nBrightness failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "hist") {
                    // check if user wants to use HSL
                    bool use_hsl = false;
                    while (true) {
                        std::cout << "\nPlease enter whether to use HSL or not (y/n)." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput != "y" && userInput != "n") {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        } else {
                            if (userInput == "y") {
                                use_hsl = true;
                            }
                            break;
                        }
                    }
                    bool success = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.HistogramEqualization(use_hsl);
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.HistogramEqualization(use_hsl);
                    }
                    if (success) {
                        std::cout << "\nApply histogram equalisation successfully." << std::endl;
                    } else {
                        std::cout << "\nHistogram equalisation failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "salt") {
                    while (true) {
                        std::cout << "\nPlease enter the percentage of salt and pepper noise." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput.find_first_not_of("0123456789") != std::string::npos) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        if (std::stoi(userInput) <= 0 || std::stoi(userInput) > 100) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        break;
                    }

                    bool success = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.SaltAndPepper(std::stoi(userInput));
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.SaltAndPepper(std::stoi(userInput));
                    }
                    if (success) {
                        std::cout << "\nApply salt and pepper noise successfully." << std::endl;
                    } else {
                        std::cout << "\nSalt and pepper noise failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "threshold") {
                    while (true) {
                        std::cout << "\nPlease enter the threshold value." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput.find_first_not_of("0123456789") != std::string::npos) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        if (std::stoi(userInput) <= 0 || std::stoi(userInput) > 255) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        break;
                    }
                    int threshold = std::stoi(userInput);
                    while (true) {
                        std::cout << "\nPlease enter whether to use HSL or not (y/n)." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput != "y" && userInput != "n") {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        } else {
                            break;
                        }
                    }
                    bool use_hsl;
                    if (userInput == "y") {
                        use_hsl = true;
                    } else {
                        use_hsl = false;
                    }
                    bool success = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.Threshold(threshold, use_hsl);
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.Threshold(threshold, use_hsl);
                    }
                    if (success) {
                        std::cout << "\nApply threshold successfully." << std::endl;
                    } else {
                        std::cout << "\nThreshold failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "gaussian") {
                    while (true) {
                        std::cout << "\nPlease enter the filter size(>>>3 for size 3*3 kernal)." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput.find_first_not_of("0123456789") != std::string::npos) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        if (std::stoi(userInput) <= 0) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        break;
                    }

                    int filterSize = std::stoi(userInput);
                    while (true) {
                        std::cout << "\nPlease enter the sigma value." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput.find_first_not_of("0123456789.") != std::string::npos) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        if (std::stod(userInput) <= 0) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        break;
                    }

                    bool success = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.GaussianFilter(filterSize, std::stod(userInput));
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.GaussianFilter(filterSize, std::stod(userInput));
                    }
                    if (success) {
                        std::cout << "\nApply Gaussian filter successfully." << std::endl;
                    } else {
                        std::cout << "\nGaussian filter failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "median") {
                    while (true) {
                        std::cout << "\nPlease enter the filter size(>>>3 for 3*3 size kernal)." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput.find_first_not_of("0123456789") != std::string::npos) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        if (std::stoi(userInput) <= 0) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        break;
                    }

                    bool success = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.MedianFilter(std::stoi(userInput));
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.MedianFilter(std::stoi(userInput));
                    }
                    if (success) {
                        std::cout << "\nApply Median filter successfully." << std::endl;
                    } else {
                        std::cout << "\nMedian filter failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "box") {
                    while (true) {
                        std::cout << "\nPlease enter the filter size(>>>3 for size 3*3 kernal)." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput.find_first_not_of("0123456789") != std::string::npos) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        if (std::stoi(userInput) <= 0) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        break;
                    }

                    bool success = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.boxFilter(std::stoi(userInput));
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.boxFilter(std::stoi(userInput));
                    }
                    if (success) {
                        std::cout << "\nApply Box filter successfully." << std::endl;
                    } else {
                        std::cout << "\nBox filter failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "sobel") {
                    bool success = false;
                    bool success_edge = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.Grayscale();
                        success_edge = image.sobelDetection();
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.Grayscale();
                        success_edge = image.sobelDetection();
                    }
                    if (success && success_edge) {
                        std::cout << "\nApply Sobel detection successfully." << std::endl;
                    } else {
                        std::cout << "\nSobel detection failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "prewitt") {
                    bool success = false;
                    bool success_edge = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.Grayscale();
                        success_edge = image.prewittDetection();
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.Grayscale();
                        success_edge = image.prewittDetection();
                    }
                    if (success) {
                        std::cout << "\nApply Prewitt detection successfully." << std::endl;
                    } else {
                        std::cout << "\nPrewitt detection failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "scharr") {
                    bool success = false;
                    bool success_edge = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.Grayscale();
                        success_edge = image.scharrDetection();
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.Grayscale();
                        success_edge = image.scharrDetection();
                    }
                    if (success) {
                        std::cout << "\nApply Scharr detection successfully." << std::endl;
                    } else {
                        std::cout << "\nScharr detection failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "roberts") {
                    bool success = false;
                    bool success_edge = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = image.Grayscale();
                        success_edge = image.robertsCrossDetection();
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = image.Grayscale();
                        success_edge = image.robertsCrossDetection();
                    }
                    if (success) {
                        std::cout << "\nApply Roberts Cross detection successfully." << std::endl;
                    } else {
                        std::cout << "\nRoberts Cross detection failed to generate. Try again!" << std::endl;
                    }
                } else {
                    std::cout << "\nInvalid input. Please try again." << std::endl;
                    continue;
                }
            }
            
        }

        if (model == 3 && volume.getExist() == 0) {
            while (true) {
                std::cout << "\nPlease add a folder path to start the canvas." << std::endl;
                std::cout << ">>>";
                std::getline(std::cin, userInput);
                bool success = volume.loadImages(userInput);
                if (success) {
                    std::cout << "\nFolder path loaded successfully." << std::endl;
                    break;
                } else {
                    std::cout << "\nFolder path failed to load. Try again!" << std::endl;
                }
            }
        }

        if (model == 3) {
            while (true) {
                std::cout << "\n============================MODEL3D MENU================================" << std::endl;
                std::cout << "|             For the current loaded Image:                            |" << std::endl;
                std::cout << "|             ENTER 'check' to look the data path & data info.         |" << std::endl;
                std::cout << "|             ENTER 'reload' to reload the image set.                  |" << std::endl;
                std::cout << "|             ENTER 'save' to save the current image set.              |" << std::endl;
                std::cout << "|             ENTER '2d' to change to 2d model.                        |" << std::endl;
                std::cout << "|             ENTER 'exit' to quit the project.                        |" << std::endl;
                std::cout << "|----------------------------------------------------------------------|" << std::endl;
                std::cout << "|             ENTER 'gaussian' to apply 3D GaussianFilter.             |" << std::endl;
                std::cout << "|             ENTER 'median' to apply 3D MedianFilter.                 |" << std::endl;
                std::cout << "|----------------------------------------------------------------------|" << std::endl;
                std::cout << "|             ENTER 'max' to apply MaxProjection.                      |" << std::endl;
                std::cout << "|             ENTER 'min' to apply MinProjection.                      |" << std::endl;
                std::cout << "|             ENTER 'average' to apply AverageProjection.              |" << std::endl;
                std::cout << "|             ENTER 'amedian' to apply AverageProjection with median.  |" << std::endl;
                std::cout << "|----------------------------------------------------------------------|" << std::endl;
                std::cout << "|             ENTER 'slice' to apply slicing operation.                |" << std::endl;
                std::cout << "========================================================================" << std::endl;
                std::cout << "\n>>>";
                std::getline(std::cin, userInput);
                if (userInput == "exit") {
                    return 0;
                } else if (userInput == "2d") {
                    model = 2;
                    break;
                } else if (userInput == "check") {
                    std::cout << "\nThe current data path is: " << volume.getFolderPath() << std::endl;
                    std::cout << "The size of the image set is: " << volume.getImages().size() << std::endl;
                    std::cout << "The size of single image is: " << volume.getWidth() << " * " << volume.getHeight() << " * " << volume.getChannels() << std::endl;
                } else if (userInput == "reload") {
                    while (true) {
                        std::cout << "\nPlease add a folder path to start the canvas." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        bool success = volume.loadImages(userInput);
                        if (success) {
                            std::cout << "\nFolder path loaded successfully." << std::endl;
                            break;
                        } else {
                            std::cout << "\nFolder path failed to load. Try again!" << std::endl;
                        }
                    }
                } else if (userInput == "save") {
                    std::cout << "\nPlease determine the output folder path for the current images." << std::endl;
                    std::cout << ">>>";
                    std::getline(std::cin, userInput);
                    bool success = volume.saveImages(userInput);
                    if (success) {
                        std::cout << "\nImages saved successfully." << std::endl;
                    } else {
                        std::cout << "\nImages failed to save. Try again!" << std::endl;
                    }
                } else if (userInput == "gaussian") {
                    while (true) {
                        std::cout << "\nPlease enter the filter size(>>>3 for 3*3*3 size kernal)." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput.find_first_not_of("0123456789") != std::string::npos) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        if (std::stoi(userInput) <= 0) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        break;
                    }

                    int filterSize = std::stoi(userInput);
                    while (true) {
                        std::cout << "\nPlease enter the sigma value." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput.find_first_not_of("0123456789.") != std::string::npos) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        if (std::stod(userInput) <= 0) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        break;
                    }

                    bool success = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = volume.applyGaussianFilter(filterSize, std::stod(userInput));
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = volume.applyGaussianFilter(filterSize, std::stod(userInput));
                    }
                    if (success) {
                        std::cout << "\nApply Gaussian filter successfully." << std::endl;
                    } else {
                        std::cout << "\nGaussian filter failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "median") {
                    while (true) {
                        std::cout << "\nPlease enter the filter size(>>>3 for 3*3*3 size kernal)." << std::endl;
                        std::cout << ">>>";
                        std::getline(std::cin, userInput);
                        if (userInput.find_first_not_of("0123456789") != std::string::npos) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        if (std::stoi(userInput) <= 0) {
                            std::cout << "\nInvalid input. Please try again." << std::endl;
                            continue;
                        }
                        break;
                    }

                    bool success = false;
                    if (time) {
                        auto starTime = std::chrono::high_resolution_clock::now();
                        success = volume.applyMedianFilter(std::stoi(userInput));
                        auto endTime = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - starTime);
                        std::cout << "\nExecution Time: " << duration.count() << " microseconds" << std::endl;
                    } else {
                        success = volume.applyMedianFilter(std::stoi(userInput));
                    }
                    if (success) {
                        std::cout << "\nApply Median filter successfully." << std::endl;
                    } else {
                        std::cout << "\nMedian filter failed to generate. Try again!" << std::endl;
                    }
                } else if (userInput == "max") {
                    Projection3D(1, volume, time);
                } else if (userInput == "min") {
                    Projection3D(2, volume, time);
                } else if (userInput == "average") {
                    Projection3D(3, volume, time);
                } else if (userInput == "amedian") {
                    Projection3D(4, volume, time);
                } else if (userInput == "slice") {
                    Slice3D(volume, time);
                } else {
                    std::cout << "\nInvalid input. Please try again." << std::endl;
                    continue;
                }
            }
        }
    }
    
    return 0;
}