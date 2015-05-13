#include <histogramManager.h>

using namespace std;
using namespace cv;

void histogramManager::extractHistogram(string image,string xml){
    Mat src_test, hsv_test;
    vector<Mat> hsv_planes;

    /// Load two images with different environment settings
    /*if( argc < 3 )
    { printf("** Error. Usage: ./histextract <image> <file.xml>\n");
      return -1;
    }*/

    // Load image
    src_test = imread(image, CV_LOAD_IMAGE_COLOR);

    /// Convert to HSV
    cvtColor(src_test, hsv_test, CV_BGR2HSV );

    // Extract HSV planes
    split(hsv_test, hsv_planes);

    /// Bins to use
    int h_bins = 50; int s_bins = 50; int v_bins = 100;

    // Ranges
    float hrang[] = {0, 180};
    const float *h_ranges = { hrang };

    float srang[] = {0, 256};
    const float *s_ranges = { srang };

    float vrang[] = {0, 256};
    const float *v_ranges = { vrang };

    /// Histograms
    Mat hist_h, hist_s, hist_v;

    /// Calculate the histogram for the H image
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

    //return 0;
  }

void histogramManager::compareHistograms(string hist1, string hist2, int method){
    /// Load two images with different environment settings

    int compare_method;

    //method = atoi(argv[3]);
    if (method == 1)
      compare_method = CV_COMP_CORREL; // Correlation
    else if (method == 2)
      compare_method = CV_COMP_CHISQR; // Chi-Square
    else if (method == 3)
      compare_method = CV_COMP_INTERSECT; // Intersection
    else if (method == 4)
      compare_method = CV_COMP_BHATTACHARYYA; // Bhattacharyya distance
    else {
      printf("ERROR: no valid value for compare_method\n");
      exit(1);
    }

    Mat hist_h1, hist_s1, hist_v1;
    Mat hist_h2, hist_s2, hist_v2;

    // Read histogram1
    FileStorage fs1(hist1, FileStorage::READ);

    string fname1;
    fs1["imageName"] >> fname1;
    fs1["hist_h"] >> hist_h1;
    fs1["hist_s"] >> hist_s1;
    fs1["hist_v"] >> hist_v1;

    fs1.release();

    // Read histogram2
    FileStorage fs2(hist2, FileStorage::READ);
    string fname2;
    fs2["imageName"] >> fname2;
    fs2["hist_h"] >> hist_h2;
    fs2["hist_s"] >> hist_s2;
    fs2["hist_v"] >> hist_v2;

    fs2.release();

    /// Histogram comparison
    double comphist_h = compareHist( hist_h1, hist_h2, compare_method);
    double comphist_s = compareHist( hist_s1, hist_s2, compare_method);
    double comphist_v = compareHist( hist_v1, hist_v2, compare_method);

    // Compare
    cout << "Comparing " << fname1 << " and " << fname2 << endl;
    cout << "Method " << method << ", Result " << comphist_h + comphist_s + comphist_v << endl;

    //return 0;
  }
