#ifndef HISTOGRAMMANAGER_H
#define HISTOGRAMMANAGER_H


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <string>

class histogramManager  {
public:
void extractHistogram(std::string image,std::string xml);
void compareHistograms(std::string hist1, std::string hist2, int method);

};
#endif // HISTOGRAMMANAGER_H
