#include <histogramManager.h>

using namespace std;
using namespace cv;

/**
 * @brief histogramManager::extractHistogram
 * @param image
 * @param xml
 */
void histogramManager::extractHistogram(string image, string xml){
    Mat src_test, hsv_test;
    vector<Mat> hsv_planes;

    // Load image
    src_test = imread(image, CV_LOAD_IMAGE_COLOR);

    // Convert to HSV
    cvtColor(src_test, hsv_test, CV_BGR2HSV );

    // Extract HSV planes
    split(hsv_test, hsv_planes);

    // Bins to use
    int h_bins = 50; int s_bins = 50; int v_bins = 100;

    // Ranges
    float hrang[] = {0, 180};
    const float *h_ranges = { hrang };

    float srang[] = {0, 256};
    const float *s_ranges = { srang };

    float vrang[] = {0, 256};
    const float *v_ranges = { vrang };

    // Histograms
    Mat hist_h, hist_s, hist_v;

    // Calculate the histogram for the H image
    calcHist( &hsv_planes[0], 1, 0, Mat(), hist_h, 1, &h_bins, &h_ranges, true, false );
    normalize( hist_h, hist_h, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_planes[1], 1, 0, Mat(), hist_s, 1, &s_bins, &s_ranges, true, false );
    normalize( hist_s, hist_s, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_planes[2], 1, 0, Mat(), hist_v, 1, &v_bins, &v_ranges, true, false );
    normalize( hist_v, hist_v, 0, 1, NORM_MINMAX, -1, Mat() );

    // Store histograms on disc
    FileStorage fs(xml, FileStorage::WRITE);
    fs << "imageName" << image;
    fs << "hist_h" << hist_h;
    fs << "hist_s" << hist_s;
    fs << "hist_v" << hist_v;
    fs.release();
}

/**
 * @brief histogramManager::compareHistograms
 * @param hist1
 * @param hist2
 * @param method
 * @return
 */
double histogramManager::compareHistograms(hist_data hist1, hist_data hist2, int method){
    // Load two images with different environment settings
    int compare_method;

    if (method == 1)
      compare_method = CV_COMP_CORREL; // Correlation
    else if (method == 2)
      compare_method = CV_COMP_CHISQR;  // Chi-Square
    else if (method == 3)
      compare_method = CV_COMP_INTERSECT; // Intersection
    else if (method == 4)
      compare_method = CV_COMP_BHATTACHARYYA; // Bhattacharyya distance
    else {
      printf("ERROR: no valid value for compare_method\n");
      return -1;
    }

    // Histogram comparison
    double comphist_h = compareHist( hist1.h, hist2.h, compare_method);
    double comphist_s = compareHist( hist1.s, hist2.s, compare_method);
    double comphist_v = compareHist( hist1.v, hist2.v, compare_method);

    // Compare
    //cout << "Comparing " << hist1.fname << " and " << hist2.fname << endl;
    //cout << "Method " << method << ", Result " << comphist_h + comphist_s + comphist_v << endl;

    return comphist_h + comphist_s + comphist_v;
}

/**
 * @brief histogramManager::loadHistogram
 * @param h_path
 * @return
 */
hist_data histogramManager::loadHistogram(string h_path) {
    hist_data hist;

    // Read histogram
    FileStorage fs(h_path, FileStorage::READ);

    fs["imageName"] >> hist.fname;
    fs["hist_h"] >> hist.h;
    fs["hist_s"] >> hist.s;
    fs["hist_v"] >> hist.v;
    fs.release();

    return hist;

}
