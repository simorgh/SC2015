#ifndef HISTOGRAMMANAGER_H
#define HISTOGRAMMANAGER_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <string>

struct hist_data {
    std::string fname;
    cv::Mat h;
    cv::Mat s;
    cv::Mat v;
};

class histogramManager  {
public:
    std::vector <hist_data> *histograms;

    void extractHistogram(std::string image,std::string xml);
    double compareHistograms(hist_data hist1, hist_data hist2, int method);
    hist_data loadHistogram(std::string h_path);
};
#endif // HISTOGRAMMANAGER_H
