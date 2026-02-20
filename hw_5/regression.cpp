#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>

using namespace std;

struct DataPoint {
    double x, y;
    double predicted;

    DataPoint(double xVal = 0.0, double yVal = 0.0) {
        x = xVal;
        y = yVal;
        predicted = 0.0;
    }
};

class LinearRegression {
private:
    vector<DataPoint> data;
    double slope;
    double intercept;
    double rSquared;

public:
    LinearRegression() {
        slope = 0.0;
        intercept = 0.0;
        rSquared = 0.0;
    }

    void addPoint(double x, double y) {
        data.push_back(DataPoint(x, y));
    }

    void generateSyntheticData(int numPoints = 120, double trueSlope = 2.5,
                               double trueIntercept = 1.0, double noise = 0.5) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> xDis(0, 10);
        normal_distribution<> noiseDis(0, noise);

        for (int i = 0; i < numPoints; i++) {
            double x = xDis(gen);
            double y = trueSlope * x + trueIntercept + noiseDis(gen);
            addPoint(x, y);
        }

        cout << "Generated " << numPoints << " synthetic data points.\n";
        cout << "True model: y = " << trueSlope << "x + " << trueIntercept << "\n";
        cout << "Noise level: " << noise << "\n\n";
    }

    double predict(double x) {
        return slope * x + intercept;
    }

    void calculateMetrics() {
        if (data.empty()) {
            rSquared = 0.0;
            return;
        }

        double yMean = 0.0;
        for (size_t i = 0; i < data.size(); i++) {
            yMean += data[i].y;
        }
        yMean /= data.size();

        double sse = 0.0;
        double sst = 0.0;

        for (size_t i = 0; i < data.size(); i++) {
            data[i].predicted = predict(data[i].x);
            double err = data[i].y - data[i].predicted;
            sse += err * err;

            double dev = data[i].y - yMean;
            sst += dev * dev;
        }

        rSquared = 1.0 - (sse / sst);
    }

    double calculateMSE() {
        if (data.empty()) {
            return 0.0;
        }

        double total = 0.0;
        for (size_t i = 0; i < data.size(); i++) {
            double err = data[i].y - data[i].predicted;
            total += err * err;
        }
        return total / data.size();
    }

    double calculateMAE() {
        if (data.empty()) {
            return 0.0;
        }

        double total = 0.0;
        for (size_t i = 0; i < data.size(); i++) {
            total += fabs(data[i].y - data[i].predicted);
        }
        return total / data.size();
    }

    void fit() {
        if (data.empty()) {
            cout << "No training data.\n";
            return;
        }

        double xMean = 0.0;
        double yMean = 0.0;

        for (size_t i = 0; i < data.size(); i++) {
            xMean += data[i].x;
            yMean += data[i].y;
        }

        xMean /= data.size();
        yMean /= data.size();

        double up = 0.0;
        double down = 0.0;

        for (size_t i = 0; i < data.size(); i++) {
            up += (data[i].x - xMean) * (data[i].y - yMean);
            down += (data[i].x - xMean) * (data[i].x - xMean);
        }

        slope = up / down;
        intercept = yMean - slope * xMean;

        calculateMetrics();

        cout << "Model: y = " << slope << "x + " << intercept << "\n";
        cout << "R^2: " << rSquared << "\n";
        cout << "MSE: " << calculateMSE() << "\n";
        cout << "MAE: " << calculateMAE() << "\n";
    }

    void saveAsImage(const string& filename) {
        const int w = 600;
        const int h = 600;
        const int p = 40;

        vector<int> img(w * h * 3, 255);

        if (data.empty()) {
            return;
        }

        double minX = data[0].x;
        double maxX = data[0].x;
        double minY = data[0].y;
        double maxY = data[0].y;

        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].x < minX) minX = data[i].x;
            if (data[i].x > maxX) maxX = data[i].x;
            if (data[i].y < minY) minY = data[i].y;
            if (data[i].y > maxY) maxY = data[i].y;
            if (data[i].predicted < minY) minY = data[i].predicted;
            if (data[i].predicted > maxY) maxY = data[i].predicted;
        }

        for (size_t i = 0; i < data.size(); i++) {
            int px = p + (int)((data[i].x - minX) / (maxX - minX) * (w - 2 * p));
            int py = h - (p + (int)((data[i].y - minY) / (maxY - minY) * (h - 2 * p)));

            for (int dx = -2; dx <= 2; dx++) {
                for (int dy = -2; dy <= 2; dy++) {
                    int xx = px + dx;
                    int yy = py + dy;
                    if (xx < 0 || xx >= w || yy < 0 || yy >= h) {
                        continue;
                    }
                    int pos = (yy * w + xx) * 3;
                    img[pos] = 30;
                    img[pos + 1] = 90;
                    img[pos + 2] = 220;
                }
            }
        }

        for (int x = p; x < w - p; x++) {
            double xv = minX + (double)(x - p) / (w - 2 * p) * (maxX - minX);
            double yv = predict(xv);
            int y = h - (p + (int)((yv - minY) / (maxY - minY) * (h - 2 * p)));
            if (y < 1 || y >= h - 1) {
                continue;
            }
            int pos1 = (y * w + x) * 3;
            int pos2 = ((y - 1) * w + x) * 3;
            int pos3 = ((y + 1) * w + x) * 3;
            img[pos1] = 220; img[pos1 + 1] = 20; img[pos1 + 2] = 60;
            img[pos2] = 220; img[pos2 + 1] = 20; img[pos2 + 2] = 60;
            img[pos3] = 220; img[pos3 + 1] = 20; img[pos3 + 2] = 60;
        }

        ofstream out(filename);
        out << "P3\n" << w << " " << h << "\n255\n";
        for (int i = 0; i < w * h; i++) {
            out << img[i * 3] << " " << img[i * 3 + 1] << " " << img[i * 3 + 2] << "\n";
        }

        out.close();
        cout << "Saved image to " << filename << "\n";
    }
};

int main() {
    LinearRegression lr;
    lr.generateSyntheticData();
    lr.fit();
    lr.saveAsImage("regression.ppm");

    return 0;
}
