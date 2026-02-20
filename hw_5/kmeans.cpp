#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>
#include <algorithm>

using namespace std;

struct Point {
    double x, y;
    int cluster;

    Point(double xVal = 0.0, double yVal = 0.0) {
        x = xVal;
        y = yVal;
        cluster = -1;
    }
};

class KMeans {
private:
    vector<Point> points;
    vector<Point> centers;
    int k;
    double tolerance;
    int maxIterations;
    int iterations;
    mt19937 gen;

public:
    KMeans(int kValue = 3, double tol = 1e-4, int maxIters = 100) {
        k = kValue;
        tolerance = tol;
        maxIterations = maxIters;
        iterations = 0;
        random_device rd;
        gen.seed(rd());
    }

    void addPoint(double x, double y) {
        points.push_back(Point(x, y));
    }

    void generateSyntheticData(int numPoints) {
        random_device rd;
        mt19937 gen(rd());
        normal_distribution<> dis(0, 1);

        vector<pair<double, double>> clusterCenters = {
            {2, 2}, {8, 8}, {8, 2}
        };

        int pointsPerCluster = numPoints / k;
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < pointsPerCluster; j++) {
                double x = clusterCenters[i % clusterCenters.size()].first + dis(gen) * 0.5;
                double y = clusterCenters[i % clusterCenters.size()].second + dis(gen) * 0.5;
                addPoint(x, y);
            }
        }

        cout << "Generated " << points.size() << " points for K-Means\n";
    }

    double distance(const Point& p1, const Point& p2) {
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        return sqrt(dx * dx + dy * dy);
    }

    void initializeCenters() {
        centers.clear();
        int totalPoints = points.size();
        if (totalPoints < k) {
            return;
        }

        vector<int> used(totalPoints, 0);
        uniform_int_distribution<int> pick(0, totalPoints - 1);

        while ((int)centers.size() < k) {
            int idx = pick(gen);
            if (used[idx] == 0) {
                used[idx] = 1;
                centers.push_back(points[idx]);
            }
        }

        for (int i = 0; i < (int)centers.size(); i++) {
            centers[i].cluster = i;
        }
    }

    void assignPoints() {
        for (size_t i = 0; i < points.size(); i++) {
            double md = 1e18;
            int id = -1;

            for (int c = 0; c < k; c++) {
                double d = distance(points[i], centers[c]);
                if (d < md) {
                    md = d;
                    id = c;
                }
            }

            points[i].cluster = id;
        }
    }

    bool updateCenters() {
        vector<double> sumX(k, 0.0);
        vector<double> sumY(k, 0.0);
        vector<int> count(k, 0);

        for (size_t i = 0; i < points.size(); i++) {
            int c = points[i].cluster;
            sumX[c] += points[i].x;
            sumY[c] += points[i].y;
            count[c]++;
        }

        bool done = true;
        for (int c = 0; c < k; c++) {
            if (count[c] == 0) {
                continue;
            }

            Point newCenter(sumX[c] / count[c], sumY[c] / count[c]);
            newCenter.cluster = c;

            double move = distance(centers[c], newCenter);
            if (move > tolerance) {
                done = false;
            }

            centers[c] = newCenter;
        }

        return done;
    }

    double calculateInertia() {
        double total = 0.0;
        for (size_t i = 0; i < points.size(); i++) {
            int c = points[i].cluster;
            double d = distance(points[i], centers[c]);
            total += d * d;
        }
        return total;
    }

    void fit() {
        if (points.empty()) {
            cout << "No data points.\n";
            return;
        }
        if (k <= 0 || (int)points.size() < k) {
            cout << "Bad k value for current data.\n";
            return;
        }

        initializeCenters();
        bool done = false;

        for (int i = 0; i < maxIterations; i++) {
            assignPoints();
            done = updateCenters();
            iterations = i + 1;

            double inertia = calculateInertia();
            cout << "Iteration " << iterations << ", inertia: " << inertia << "\n";

            if (done) {
                cout << "Centers stabilized.\n";
                break;
            }
        }

        if (!done) {
            cout << "Reached max iterations.\n";
        }
    }

    void saveAsImage(const string& filename) {
        const int width = 600;
        const int height = 600;
        const int m = 35;

        vector<int> img(width * height * 3, 255);

        if (points.empty()) {
            return;
        }

        int col[3][3] = {
            {220, 40, 60},
            {40, 160, 70},
            {40, 90, 220}
        };

        for (size_t i = 0; i < points.size(); i++) {
            int px = m + (int)(points[i].x * 50.0);
            int py = height - (m + (int)(points[i].y * 50.0));

            int cid = points[i].cluster;
            int r = col[cid % 3][0];
            int g = col[cid % 3][1];
            int b = col[cid % 3][2];

            for (int dx = -2; dx <= 2; dx++) {
                for (int dy = -2; dy <= 2; dy++) {
                    int xx = px + dx;
                    int yy = py + dy;
                    if (xx < 0 || xx >= width || yy < 0 || yy >= height) {
                        continue;
                    }
                    int idx = (yy * width + xx) * 3;
                    img[idx] = r;
                    img[idx + 1] = g;
                    img[idx + 2] = b;
                }
            }
        }

        for (size_t i = 0; i < centers.size(); i++) {
            int px = m + (int)(centers[i].x * 50.0);
            int py = height - (m + (int)(centers[i].y * 50.0));

            for (int d = -7; d <= 7; d++) {
                int x1 = px + d;
                int y1 = py;
                if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
                    int idx1 = (y1 * width + x1) * 3;
                    img[idx1] = 0;
                    img[idx1 + 1] = 0;
                    img[idx1 + 2] = 0;
                }

                int x2 = px;
                int y2 = py + d;
                if (x2 >= 0 && x2 < width && y2 >= 0 && y2 < height) {
                    int idx2 = (y2 * width + x2) * 3;
                    img[idx2] = 0;
                    img[idx2 + 1] = 0;
                    img[idx2 + 2] = 0;
                }
            }
        }

        ofstream out(filename);
        out << "P3\n" << width << " " << height << "\n255\n";
        for (int i = 0; i < width * height; i++) {
            out << img[i * 3] << " " << img[i * 3 + 1] << " " << img[i * 3 + 2] << "\n";
        }

        out.close();
        cout << "Saved image to " << filename << "\n";
    }
};

int main() {
    KMeans kmeans(3, 1e-4, 100);
    kmeans.generateSyntheticData(300);
    kmeans.fit();
    kmeans.saveAsImage("output.ppm");

    return 0;
}
